import type * as React from 'react';
import {
  codegenNativeCommands,
  codegenNativeComponent,
  type CodegenTypes,
  type HostComponent,
  type ViewProps,
} from 'react-native';

// --- Events ---

export type OnErrorEvent = Readonly<{
  code: string;
  message: string;
}>;

export type OnTrackingStateEvent = Readonly<{
  state: 'tracking' | 'paused' | 'stopped';
  reason?: string;
}>;

export type OnPlaneDetectedEvent = Readonly<{
  id: string;
  type: 'horizontal_up' | 'horizontal_down' | 'vertical';
  center: Readonly<{
    x: CodegenTypes.Float;
    y: CodegenTypes.Float;
    z: CodegenTypes.Float;
  }>;
  extent: Readonly<{x: CodegenTypes.Float; z: CodegenTypes.Float}>;
}>;

// --- Flattened config enums ---

export type ARSessionType = 'world' | 'face';
export type ARDepthMode = 'disabled' | 'automatic' | 'raw' | 'geospatial';
export type ARCloudAnchorMode = 'disabled' | 'enabled';
export type ARInstantPlacementMode = 'disabled' | 'enabled';

export type ARLightEstimationMode =
  | 'disabled'
  | 'ambientIntensity'
  | 'environmentalHDR';

export type ARPlaneDetectionMode = 'horizontal' | 'vertical' | 'both' | 'none';
export type ARFocusMode = 'auto' | 'fixed';

// --- Props ---

export interface NativeProps extends ViewProps {
  planeDetection?: boolean;
  lightEstimation?: boolean;

  // Flattened Session Configuration
  sessionType?: CodegenTypes.WithDefault<ARSessionType, 'world'>;
  depthMode?: CodegenTypes.WithDefault<ARDepthMode, 'disabled'>;
  cloudAnchorMode?: CodegenTypes.WithDefault<ARCloudAnchorMode, 'disabled'>;
  instantPlacementMode?: CodegenTypes.WithDefault<ARInstantPlacementMode, 'disabled'>;

  // Flattened Run Configuration
  lightEstimationMode?: CodegenTypes.WithDefault<
    ARLightEstimationMode,
    'ambientIntensity'
  >;
  planeDetectionMode?: CodegenTypes.WithDefault<
    ARPlaneDetectionMode,
    'horizontal'
  >;
  focusMode?: CodegenTypes.WithDefault<ARFocusMode, 'auto'>;

  // Flattened Debug Flags
  debugShowPlanes?: boolean;
  debugShowPointCloud?: boolean;
  debugShowWorldOrigin?: boolean;
  debugShowDepthMap?: boolean;
  paused?: boolean;

  onARCoreError?: CodegenTypes.DirectEventHandler<OnErrorEvent>;
  onTrackingStateChange?: CodegenTypes.DirectEventHandler<OnTrackingStateEvent>;
  onPlaneDetected?: CodegenTypes.DirectEventHandler<OnPlaneDetectedEvent>;
  onPlaneUpdated?: CodegenTypes.DirectEventHandler<OnPlaneDetectedEvent>;
}

export const ARViewNativeComponent = codegenNativeComponent<NativeProps>(
  'ARView',
  {interfaceOnly: true},
) as HostComponent<NativeProps>;

// --- Commands ---

interface NativeCommands {
  takeSnapshot: (
    viewRef: React.ComponentRef<typeof ARViewNativeComponent>,
    saveToDisk: boolean,
  ) => Promise<string>;

  resetSession: (viewRef: React.ComponentRef<typeof ARViewNativeComponent>) => void;

  destroySession: (viewRef: React.ComponentRef<typeof ARViewNativeComponent>) => void;

  cameraPermissionGranted: (
    viewRef: React.ComponentRef<typeof ARViewNativeComponent>,
  ) => void;

  hitTest: (
    viewRef: React.ComponentRef<typeof ARViewNativeComponent>,
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
  supportedCommands: [
    'takeSnapshot',
    'resetSession',
    'destroySession',
    'cameraPermissionGranted',
    'hitTest',
  ],
});

export default ARViewNativeComponent;
