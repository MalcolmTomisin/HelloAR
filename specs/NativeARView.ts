import {
  ViewProps,
  HostComponent,
  CodegenTypes,
  codegenNativeComponent,
  codegenNativeCommands,
} from 'react-native';

// 1. Define the events
// These names must match what you emit from Kotlin (e.g., "topError")
export type OnErrorEvent = Readonly<{
  code: string; // 'CAMERA_UNAVAILABLE', 'SESSION_PAUSED', etc.
  message: string;
}>;

export type OnTrackingStateEvent = Readonly<{
  state: 'tracking' | 'paused' | 'stopped';
  reason?: string; // 'excessive_motion', 'insufficient_light'
}>;

export type OnPlaneDetectedEvent = Readonly<{
  id: string;
  type: 'horizontal_up' | 'horizontal_down' | 'vertical';
  center: Readonly<{
    x: CodegenTypes.Float;
    y: CodegenTypes.Float;
    z: CodegenTypes.Float;
  }>;
  extent: Readonly<{ x: CodegenTypes.Float; z: CodegenTypes.Float }>; // Width/Height
}>;

// 1. Session Configuration (Requires Restart)
// These define "What kind of AR experience is this?"
export type ARSessionConfig = Readonly<{
  // 'world' = standard rear camera 6DOF
  // 'face' = front camera face mesh
  sessionType?: CodegenTypes.WithDefault<'world' | 'face', 'world'>;

  // Depth API: Critical for occlusion (hiding virtual objects behind real ones)
  // 'disabled': Fastest
  // 'automatic': ARCore handles occlusion automatically (e.g. hand in front of cube)
  // 'raw': You want raw depth data (Advanced, usually too heavy for JS bridge)
  depthMode?: CodegenTypes.WithDefault<
    'disabled' | 'automatic' | 'raw' | 'geospatial',
    'disabled'
  >;

  // Cloud Anchors allow sharing state between phones
  cloudAnchorMode?: CodegenTypes.WithDefault<
    'disabled' | 'enabled',
    'disabled'
  >;

  instantPlacementMode?: CodegenTypes.WithDefault<
    'disabled' | 'enabled',
    'disabled'
  >;
}>;

export type ARCameraConfig = Readonly<{
  // 1. Facing Direction (The most basic filter)
  facing?: CodegenTypes.WithDefault<'front' | 'back', 'back'>;

  // 2. Target FPS (Frames Per Second)
  // ARCore usually supports 30 or 60.
  // '60': Prioritize smoothness. '30': Prioritize battery/processing.
  targetFps?: CodegenTypes.WithDefault<'30' | '60', '30'>;

  // 3. Target Resolution (Height)
  // Instead of exact pixels (which vary by device), use buckets.
  // 'low' (SD), 'medium' (HD/720p), 'high' (1080p+)
  // Note: Higher res = slower CV processing.
  targetResolution?: CodegenTypes.WithDefault<'low' | 'medium' | 'high', 'medium'>;
  
  // 4. Depth Sensor Usage
  // Some phones have a dedicated Time-of-Flight (ToF) sensor.
  // forcing 'require-tof' might result in finding NO camera on cheap phones.
  depthSensorUsage?: CodegenTypes.WithDefault<'require-tof' | 'do-not-use' | 'any', 'any'>;
}>;

// 2. Run Configuration (Dynamic)
// These define "How should the experience look/behave right now?"
export type ARRunConfig = Readonly<{
  // Lighting: 'ambientIntensity' is cheap, 'environmentalHDR' is realistic
  lightEstimationMode?: CodegenTypes.WithDefault<
    'disabled' | 'ambientIntensity' | 'environmentalHDR',
    'ambientIntensity'
  >;

  // Plane Detection: What are we looking for?
  planeDetection?: CodegenTypes.WithDefault<
    'horizontal' | 'vertical' | 'both' | 'none',
    'horizontal'
  >;

  // Auto-focus usually best, but 'fixed' is needed for some CV tasks
  focusMode?: CodegenTypes.WithDefault<'auto' | 'fixed', 'auto'>;
}>;

// 3. Debug Options (Dynamic)
// Vital for developers to understand why tracking fails
export type DebugConfig = Readonly<{
  showPlanes?: boolean; // Draw the polylines detected
  showPointCloud?: boolean; // Draw the yellow dots
  showWorldOrigin?: boolean; // Draw XYZ axes at (0,0,0)
  showDepthMap?: boolean; // Overlay the depth buffer (heatmap)
}>;

// 2. Define the props
export interface NativeProps extends ViewProps {
  // Configuration
  planeDetection?: boolean;
  lightEstimation?: boolean;
  sessionConfig?: ARSessionConfig;
  runConfig?: ARRunConfig;
  debugConfig?: DebugConfig;

  // Lifecycle (Manual pause if needed)
  paused?: boolean;

  // Events
  onARCoreError?: CodegenTypes.DirectEventHandler<OnErrorEvent>;
  onTrackingStateChange?: CodegenTypes.DirectEventHandler<OnTrackingStateEvent>;
  onPlaneDetected?: CodegenTypes.DirectEventHandler<OnPlaneDetectedEvent>;
  onPlaneUpdated?: CodegenTypes.DirectEventHandler<OnPlaneDetectedEvent>;
}

const NativeARView = codegenNativeComponent<NativeProps>(
  'ARView',
) as HostComponent<NativeProps>;

// --- Commands ---

interface NativeCommands {
  // Capture the screen to a file
  takeSnapshot: (
    viewRef: React.ComponentRef<typeof NativeARView>,
    saveToDisk: boolean,
  ) => Promise<string>; // Returns file path

  // Reset tracking (e.g., if world is drifting)
  resetSession: (viewRef: React.ComponentRef<typeof NativeARView>) => void;

  // Convert 2D screen tap to 3D world ray
  hitTest: (
    viewRef: React.ComponentRef<typeof NativeARView>,
    x: CodegenTypes.Float,
    y: CodegenTypes.Float,
  ) => Promise<
    Readonly<{
      x: CodegenTypes.Float;
      y: CodegenTypes.Float;
      z: CodegenTypes.Float;
      hasHit: boolean;
    }>
  >;
}

export const Commands = codegenNativeCommands<NativeCommands>({
  supportedCommands: ['takeSnapshot', 'resetSession', 'hitTest'],
});

// 3. Export the component
// The name 'ARView' MUST match the name in your ViewManager later.
export default NativeARView;
