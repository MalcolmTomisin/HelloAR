# ARView Native Bindings Checklist (ARCore)

This checklist is derived from the contract in `specs/ARViewNativeComponent.ts`.

Use it to verify the binding is complete end-to-end:
JS/TS → Android ViewManager/View → `HelloAppSystem` JNI → C++ `HelloARApplicationWrapper` → C++ `HelloARApplication` / ARCore.

---

## 0) Naming + registration

- [x] The JS component name is **exactly** `ARView`.
- [x] Android `ViewManager.getName()` returns `ARView`.
- [x] The view manager is registered in the app (manual `ReactPackage` or codegen/Fabric).
- [x] Event registration names match the native event constants (direct events).

---

## 1) Props checklist (NativeProps)

For each prop below:
- Implement a `@ReactProp(name = "...")` setter in the Android manager.
- Call a corresponding method on the view (e.g. `view.setXxx(...)`).
- Forward to C++ through `HelloAppSystem` (JNI external) with flattened primitives.
- Apply changes safely across lifecycle (view created, surface created, session running).

### Feature toggles

- [ ] `planeDetection?: boolean`
  - [x] Manager prop wiring
  - [x] Kotlin view setter forwards to JNI
  - [ ] Native maps to ARCore plane finding mode (or disables it)

- [ ] `lightEstimation?: boolean`
  - [x] Manager prop wiring
  - [x] Kotlin view setter forwards to JNI
  - [ ] Native maps to ARCore light estimation mode (or disables it)

### Session configuration (flattened)

- [ ] `sessionType: 'world' | 'face'` (default: `world`)
  - [x] Manager prop wiring
  - [x] JNI setter exists: `setSessionType(reactTag, sessionType)`
  - [ ] Native validates support (e.g. face requires supported APIs) and reports `onARCoreError` if unsupported

- [ ] `depthMode: 'disabled' | 'automatic' | 'raw' | 'geospatial'` (default: `disabled`)
  - [x] Manager prop wiring
  - [x] JNI setter exists
  - [ ] Native maps to `ArConfig_setDepthMode` and guards unsupported modes

- [ ] `cloudAnchorMode: 'disabled' | 'enabled'` (default: `disabled`)
  - [x] Manager prop wiring
  - [x] JNI setter exists
  - [ ] Native gates feature availability and emits `onARCoreError` when required services are missing

- [ ] `instantPlacementMode: 'disabled' | 'enabled'` (default: `disabled`)
  - [x] Manager prop wiring
  - [x] JNI setter exists
  - [ ] Native maps to `ArConfig_setInstantPlacementMode`

### Run configuration (flattened)

- [ ] `lightEstimationMode: 'disabled' | 'ambientIntensity' | 'environmentalHDR'` (default: `ambientIntensity`)
  - [x] Manager prop wiring
  - [x] JNI setter exists
  - [ ] Native maps to `ArConfig_setLightEstimationMode` and guards HDR support

- [ ] `planeDetectionMode: 'horizontal' | 'vertical' | 'both' | 'none'` (default: `horizontal`)
  - [x] Manager prop wiring
  - [x] JNI setter exists
  - [ ] Native maps to `ArConfig_setPlaneFindingMode`

- [ ] `focusMode: 'auto' | 'fixed'` (default: `auto`)
  - [x] Manager prop wiring
  - [x] JNI setter exists
  - [ ] Native maps to `ArConfig_setFocusMode`

### Camera configuration (flattened)

> Note: camera config props are wired through `ARViewNative` / `ARViewNativeWrapper`
> (per-view JNI hybrid) rather than through `HelloAppSystem`. The C++ implementation
> in `ARViewNativeWrapper::applyCameraConfigIfPossible()` is functional and maps to
> the ARCore `ArCameraConfigFilter` API.

- [x] `cameraFacing: 'back' | 'front'` (default: `back`)
  - [x] Kotlin view setter forwards to JNI (queued on GL thread)
  - [x] JNI setter exists (`ARViewNativeWrapper::setCameraFacing`)
  - [x] Native maps to `ArCameraConfigFilter_setFacingDirection`

- [x] `cameraTargetFps: '30' | '60'` (default: `30`)
  - [x] Kotlin view setter forwards to JNI (queued on GL thread)
  - [x] JNI setter exists (`ARViewNativeWrapper::setCameraTargetFps`)
  - [x] Native maps to `ArCameraConfigFilter_setTargetFps`

- [x] `cameraDepthSensorUsage: 'doNotUse' | 'useIfAvailable' | 'requireAndUse'` (default: `useIfAvailable`)
  - [x] Kotlin view setter forwards to JNI (queued on GL thread)
  - [x] JNI setter exists (`ARViewNativeWrapper::setCameraDepthSensorUsage`)
  - [x] Native maps to `ArCameraConfigFilter_setDepthSensorUsage`

> **Known issue:** `ARViewManager` does not declare `@ReactProp` setters for
> `cameraFacing`, `cameraTargetFps`, or `cameraDepthSensorUsage`, so these props
> are not reachable from React. The Kotlin `ARView` has the setter methods, and the
> JNI + C++ path is fully implemented — only the manager wiring is missing.

### Debug flags

- [ ] `debugShowPlanes?: boolean`
  - [x] Manager prop wiring
  - [x] JNI setter exists
  - [ ] Native toggles debug rendering for planes

- [ ] `debugShowPointCloud?: boolean`
  - [x] Manager prop wiring
  - [x] JNI setter exists
  - [ ] Native toggles point cloud visualization

- [ ] `debugShowWorldOrigin?: boolean`
  - [x] Manager prop wiring
  - [x] JNI setter exists
  - [ ] Native toggles axis/world origin visualization

- [ ] `debugShowDepthMap?: boolean`
  - [x] Manager prop wiring
  - [x] JNI setter exists
  - [ ] Native toggles depth visualization

### Lifecycle control

- [ ] `paused?: boolean`
  - [x] Manager prop wiring
  - [x] JNI setter exists
  - [ ] Native pauses frame processing / session update loop without tearing down resources

---

## 2) Events checklist (DirectEventHandler)

Each event requires:
- Android view manager `getExportedCustomDirectEventTypeConstants()` mapping `topXxx` → JS registration name.
- Kotlin-side emit helper (from view or system), with payload matching the TS type.
- Native C++ triggers the Kotlin emit helper at the correct times.

- [ ] `onARCoreError({ code, message })`
  - [x] Emit on: ARCore install failure, permission issues, unsupported config, session failures
  - [x] Payload fields exactly: `code`, `message`
  - [x] Event mapping registered in `getExportedCustomDirectEventTypeConstants`
  - [x] Kotlin emit helper exists (`ARView.emitARCoreError`)
  - [ ] Native C++ triggers emit at all relevant failure points (currently only camera-permission-denied via `ARViewRegistry`)

- [ ] `onTrackingStateChange({ state, reason? })`
  - [x] Event mapping registered
  - [x] Kotlin emit helper exists (`ARView.emitTrackingStateChange`)
  - [ ] Emit on tracking state transitions (not yet triggered from C++)
  - [ ] Allowed `state`: `tracking | paused | stopped`
  - [ ] Optional `reason` only when present

- [ ] `onPlaneDetected({ id, type, center, extent })`
  - [x] Event mapping registered
  - [x] Kotlin emit helper exists (`ARView.emitPlaneDetected`)
  - [x] Payload shape matches TS type (`id`, `type`, `center{x,y,z}`, `extent{x,z}`)
  - [ ] Emit when plane first observed (not yet triggered from C++)

- [ ] `onPlaneUpdated({ id, type, center, extent })`
  - [x] Event mapping registered
  - [x] Kotlin emit helper exists (`ARView.emitPlaneUpdated`)
  - [x] Payload shape matches `onPlaneDetected`
  - [ ] Emit when existing plane geometry/type changes (not yet triggered from C++)

---

## 3) Commands checklist (NativeCommands)

Each command requires:
- A command handler in the Android manager.
- If async: resolve/reject back to JS predictably.
- Threading: run ARCore/GL-sensitive work on the GL thread (`GLSurfaceView.queueEvent`) if needed.

- [ ] `takeSnapshot(saveToDisk: boolean) => Promise<string>`
  - [ ] Command registered in `getCommandsMap`
  - [ ] Returns a stable URI or file path string
  - [ ] If `saveToDisk=false`, define and document where the snapshot lives (cache) and cleanup policy
  - [ ] Rejects with a meaningful error on failure

- [ ] `resetSession() => void`
  - [ ] Command registered in `getCommandsMap`
  - [ ] Clears anchors/planes as desired
  - [ ] Re-applies current config props after reset

- [x] `destroySession() => void`
  - [x] Command registered in `getCommandsMap`
  - [x] Calls `HelloAppSystem.destroySession()` → `HelloARApplication::DestroySession()` → `ArSessionManager::Pause()` + `Destroy()`
  - [x] Safe to call multiple times (guarded by null check in `ArSessionManager`)

- [x] `cameraPermissionGranted() => void`
  - [x] Command registered in `getCommandsMap`
  - [x] Triggers `HelloAppSystem.onResume()` which creates/resumes the session if conditions are met
  - [x] No-op if already running (guarded by `ArSessionManager::IsInitialized()`)

- [ ] `hitTest(x: number, y: number) => Promise<{ x, y, z, hasHit }>`
  - [ ] Command registered in `getCommandsMap`
  - [ ] Treats `x,y` as view-local pixel coordinates (document if otherwise)
  - [ ] On no hit: returns `{ hasHit: false, x: 0, y: 0, z: 0 }` (or another consistent convention)
  - [ ] On hit: returns world coords in meters

---

## 4) Input (tap) integration

Even though tap isn't defined as a JS event in the spec, it's part of the native surface/view behavior.

- [x] `onGestureTap(reactTag, x, y)` is exposed on `HelloAppSystem`.
- [x] `ARView` routes tap detection via `GestureDetector.onSingleTapUp`.
- [x] Tap callback is queued onto the GL thread using `GLSurfaceView.queueEvent { ... }`.
- [ ] Native uses tap `(x, y)` to drive hit tests / interaction (C++ stub is no-op).

---

## 5) Cross-cutting requirements

- [x] **No complex parsing** in Kotlin/C++: props are flattened strings/bools.
- [x] **Threading is explicit**: UI thread for props/layout; GL thread for rendering + ARCore frame update.
- [x] **Lifecycle safe**: view attach/detach and activity pause/resume are handled without crashes/leaks.
- [ ] **Error paths**: any recoverable failure emits `onARCoreError` (currently only camera-permission-denied; ARCore session/config errors not yet wired).

---

## 6) Suggested verification steps

- [ ] Render `ARView` and confirm surface + GL callbacks occur.
- [ ] Toggle every prop from JS at runtime; verify native receives updates (log or debug state).
- [ ] Force tracking state changes (covering `tracking/paused/stopped`) and confirm events.
- [ ] Create a plane-rich scene; confirm plane detected/updated events.
- [ ] Trigger `hitTest(x,y)` via command and via native tap path; confirm consistent results.
