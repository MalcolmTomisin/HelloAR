import {TurboModule, TurboModuleRegistry} from 'react-native';

// Ensure the ARView component spec is reachable during RN codegen.
import './ARViewNativeComponent';

export interface Spec extends TurboModule {
  readonly initARCore: (rnPlatform: string) => boolean;
  readonly isDepthModeSupported: () => boolean;
  readonly isGeospatialModeSupported: () => boolean;
}

export default TurboModuleRegistry.getEnforcing<Spec>(
  'NativeARModule',
);