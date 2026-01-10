# ARView Native Bindings Checklist (ARCore)

This checklist is derived from the contract in `specs/ARViewNativeComponent.ts`.

Use it to verify the binding is complete end-to-end:
JS/TS → Android ViewManager/View → `HelloAppSystem` JNI → C++ `HelloARApplicationWrapper` → C++ `HelloARApplication` / ARCore.

---

## 0) Naming + registration

- [ ] The JS component name is **exactly** `ARView`.
- [ ] Android `ViewManager.getName()` returns `ARView`.
- [ ] The view manager is registered in the app (manual `ReactPackage` or codegen/Fabric).
- [ ] Event registration names match the native event constants (direct events).

---

## 1) Props checklist (NativeProps)

For each prop below:
- Implement a `@ReactProp(name = "...")` setter in the Android manager.
- Call a corresponding method on the view (e.g. `view.setXxx(...)`).
- Forward to C++ through `HelloAppSystem` (JNI external) with flattened primitives.
- Apply changes safely across lifecycle (view created, surface created, session running).

### Feature toggles

- [ ] `planeDetection?: boolean`
  - [ ] Manager prop wiring
  - [ ] Kotlin view setter forwards to JNI
  - [ ] Native maps to ARCore plane finding mode (or disables it)

- [ ] `lightEstimation?: boolean`
  - [ ] Manager prop wiring
  - [ ] Kotlin view setter forwards to JNI
  - [ ] Native maps to ARCore light estimation mode (or disables it)

### Session configuration (flattened)

- [ ] `sessionType: 'world' | 'face'` (default: `world`)
  - [ ] Manager prop wiring
  - [ ] JNI setter exists: `setSessionType(reactTag, sessionType)`
  - [ ] Native validates support (e.g. face requires supported APIs) and reports `onARCoreError` if unsupported

- [ ] `depthMode: 'disabled' | 'automatic' | 'raw' | 'geospatial'` (default: `disabled`)
  - [ ] Manager prop wiring
  - [ ] JNI setter exists
  - [ ] Native maps to `ArConfig_setDepthMode` and guards unsupported modes

- [ ] `cloudAnchorMode: 'disabled' | 'enabled'` (default: `disabled`)
  - [ ] Manager prop wiring
  - [ ] JNI setter exists
  - [ ] Native gates feature availability and emits `onARCoreError` when required services are missing

- [ ] `instantPlacementMode: 'disabled' | 'enabled'` (default: `disabled`)
  - [ ] Manager prop wiring
  - [ ] JNI setter exists
  - [ ] Native maps to `ArConfig_setInstantPlacementMode`

### Run configuration (flattened)

- [ ] `lightEstimationMode: 'disabled' | 'ambientIntensity' | 'environmentalHDR'` (default: `ambientIntensity`)
  - [ ] Manager prop wiring
  - [ ] JNI setter exists
  - [ ] Native maps to `ArConfig_setLightEstimationMode` and guards HDR support

- [ ] `planeDetectionMode: 'horizontal' | 'vertical' | 'both' | 'none'` (default: `horizontal`)
  - [ ] Manager prop wiring
  - [ ] JNI setter exists
  - [ ] Native maps to `ArConfig_setPlaneFindingMode`

- [ ] `focusMode: 'auto' | 'fixed'` (default: `auto`)
  - [ ] Manager prop wiring
  - [ ] JNI setter exists
  - [ ] Native maps to `ArConfig_setFocusMode`

### Debug flags

- [ ] `debugShowPlanes?: boolean`
  - [ ] Manager prop wiring
  - [ ] JNI setter exists
  - [ ] Native toggles debug rendering for planes

- [ ] `debugShowPointCloud?: boolean`
  - [ ] Manager prop wiring
  - [ ] JNI setter exists
  - [ ] Native toggles point cloud visualization

- [ ] `debugShowWorldOrigin?: boolean`
  - [ ] Manager prop wiring
  - [ ] JNI setter exists
  - [ ] Native toggles axis/world origin visualization

- [ ] `debugShowDepthMap?: boolean`
  - [ ] Manager prop wiring
  - [ ] JNI setter exists
  - [ ] Native toggles depth visualization

### Lifecycle control

- [ ] `paused?: boolean`
  - [ ] Manager prop wiring
  - [ ] JNI setter exists
  - [ ] Native pauses frame processing / session update loop without tearing down resources

---

## 2) Events checklist (DirectEventHandler)

Each event requires:
- Android view manager `getExportedCustomDirectEventTypeConstants()` mapping `topXxx` → JS registration name.
- Kotlin-side emit helper (from view or system), with payload matching the TS type.
- Native C++ triggers the Kotlin emit helper at the correct times.

- [ ] `onARCoreError({ code, message })`
  - [ ] Emit on: ARCore install failure, permission issues, unsupported config, session failures
  - [ ] Payload fields exactly: `code`, `message`

- [ ] `onTrackingStateChange({ state, reason? })`
  - [ ] Emit on tracking state transitions
  - [ ] Allowed `state`: `tracking | paused | stopped`
  - [ ] Optional `reason` only when present

- [ ] `onPlaneDetected({ id, type, center, extent })`
  - [ ] Emit when plane first observed
  - [ ] `type` matches: `horizontal_up | horizontal_down | vertical`
  - [ ] `center` fields: `x,y,z` (meters)
  - [ ] `extent` fields: `x,z` (meters)

- [ ] `onPlaneUpdated({ id, type, center, extent })`
  - [ ] Emit when existing plane geometry/type changes
  - [ ] Payload shape matches `onPlaneDetected`

---

## 3) Commands checklist (NativeCommands)

Each command requires:
- A command handler in the Android manager.
- If async: resolve/reject back to JS predictably.
- Threading: run ARCore/GL-sensitive work on the GL thread (`GLSurfaceView.queueEvent`) if needed.

- [ ] `takeSnapshot(saveToDisk: boolean) => Promise<string>`
  - [ ] Returns a stable URI or file path string
  - [ ] If `saveToDisk=false`, define and document where the snapshot lives (cache) and cleanup policy
  - [ ] Rejects with a meaningful error on failure

- [ ] `resetSession() => void`
  - [ ] Clears anchors/planes as desired
  - [ ] Re-applies current config props after reset

- [ ] `destroySession() => void`
  - [ ] Fully releases ARCore session/resources
  - [ ] Safe to call multiple times

- [ ] `cameraPermissionGranted() => void`
  - [ ] Used to continue session startup after runtime permission granted
  - [ ] No-op if already running

- [ ] `hitTest(x: number, y: number) => Promise<{ x, y, z, hasHit }>`
  - [ ] Treats `x,y` as view-local pixel coordinates (document if otherwise)
  - [ ] On no hit: returns `{ hasHit: false, x: 0, y: 0, z: 0 }` (or another consistent convention)
  - [ ] On hit: returns world coords in meters

---

## 4) Input (tap) integration

Even though tap isn’t defined as a JS event in the spec, it’s part of the native surface/view behavior.

- [ ] `onGestureTap(reactTag, x, y)` is exposed on `HelloAppSystem`.
- [ ] `ARView` routes tap detection via `GestureDetector.onSingleTapUp`.
- [ ] Tap callback is queued onto the GL thread using `GLSurfaceView.queueEvent { ... }`.
- [ ] Native uses tap `(x, y)` to drive hit tests / interaction.

---

## 5) Cross-cutting requirements

- [ ] **No complex parsing** in Kotlin/C++: props are flattened strings/bools.
- [ ] **Threading is explicit**: UI thread for props/layout; GL thread for rendering + ARCore frame update.
- [ ] **Lifecycle safe**: view attach/detach and activity pause/resume are handled without crashes/leaks.
- [ ] **Error paths**: any recoverable failure emits `onARCoreError`.

---

## 6) Suggested verification steps

- [ ] Render `ARView` and confirm surface + GL callbacks occur.
- [ ] Toggle every prop from JS at runtime; verify native receives updates (log or debug state).
- [ ] Force tracking state changes (covering `tracking/paused/stopped`) and confirm events.
- [ ] Create a plane-rich scene; confirm plane detected/updated events.
- [ ] Trigger `hitTest(x,y)` via command and via native tap path; confirm consistent results.
