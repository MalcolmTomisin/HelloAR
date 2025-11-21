import {TurboModule, TurboModuleRegistry} from 'react-native';

export interface Spec extends TurboModule {
  readonly initARCore: () => boolean;
}

export default TurboModuleRegistry.getEnforcing<Spec>(
  'NativeARModule',
);