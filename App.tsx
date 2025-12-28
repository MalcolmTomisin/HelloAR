/**
 * Sample React Native App
 * https://github.com/facebook/react-native
 *
 * @format
 */

import React, { useEffect, useRef } from 'react';
import { NewAppScreen } from '@react-native/new-app-screen';
import {
  StatusBar,
  StyleSheet,
  useColorScheme,
  View,
  Pressable,
  Platform,
  PermissionsAndroid
} from 'react-native';
import {
  SafeAreaProvider,
  useSafeAreaInsets,
} from 'react-native-safe-area-context';
import NativeARModule from './specs/NativeARModule';
import ARViewNativeComponent, { Commands } from './specs/ARViewNativeComponent';

function App() {
  const isDarkMode = useColorScheme() === 'dark';

  return (
    <SafeAreaProvider>
      <StatusBar barStyle={isDarkMode ? 'light-content' : 'dark-content'} />
      <AppContent />
    </SafeAreaProvider>
  );
}

function AppContent() {
  const safeAreaInsets = useSafeAreaInsets();
  const arViewRef = useRef<React.ComponentRef<typeof ARViewNativeComponent>>(null);

  useEffect(() => {
    async function requestCameraPermission() {
      if (Platform.OS !== 'android') {
        return;
      }

      try {
        const granted = await PermissionsAndroid.request(
          PermissionsAndroid.PERMISSIONS.CAMERA,
          {
            title: 'Camera Permission',
            message: 'This app needs camera access to use AR features.',
            buttonNeutral: 'Ask Me Later',
            buttonNegative: 'Cancel',
            buttonPositive: 'OK',
          },
        );

        if (granted === PermissionsAndroid.RESULTS.GRANTED) {
          console.log('Camera permission granted');

          if (arViewRef.current != null) {
            Commands.cameraPermissionGranted(arViewRef.current);
          }
        } else {
          console.log('Camera permission denied');
        }
      } catch (err) {
        console.warn(err);
      }
    }

    requestCameraPermission();
  }, []);

  return (
    <View style={styles.container}>
      
        <Pressable onPress={() => {
          const res = NativeARModule.initARCore(Platform.OS)
          console.log('Native module response ====> ', res)
        }} style={{ height: 150, backgroundColor: 'lightblue', justifyContent: 'center', alignItems: 'center' }}/>
      
      <NewAppScreen
        templateFileName="App.tsx"
        safeAreaInsets={safeAreaInsets}
      />
      <ARViewNativeComponent
        ref={arViewRef}
        onARCoreError={(event) => {
          console.log('ARCore Error:', event.nativeEvent);
        }}
        style={{ flex: 1 }}
      />
    </View>
  );
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
  },
});

export default App;
