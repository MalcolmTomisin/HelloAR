package com.helloar

import java.util.Collections
import java.util.WeakHashMap

object ARViewRegistry {
  private val views: MutableSet<ARView> =
      Collections.newSetFromMap(WeakHashMap<ARView, Boolean>())

  @Synchronized
  fun register(view: ARView) {
    views.add(view)
  }

  @Synchronized
  fun unregister(view: ARView) {
    views.remove(view)
  }

  @Synchronized
  fun hasActiveViews(): Boolean {
    return views.isNotEmpty()
  }

  fun emitCameraPermissionDenied() {
    val snapshot: List<ARView> = synchronized(this) { views.toList() }

    snapshot.forEach { view ->
      view.post {
        view.emitARCoreError(
            "camera_permission_denied",
            "Camera permission is required to start AR.",
        )
      }
    }
  }
}
