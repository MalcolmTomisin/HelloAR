package com.helloar

import android.content.Context
import android.opengl.GLSurfaceView
import android.os.Handler
import android.os.Looper
import android.view.GestureDetector
import android.view.MotionEvent
import android.view.ViewGroup
import android.widget.FrameLayout
import com.facebook.react.bridge.Arguments
import com.facebook.react.bridge.ReactContext
import com.facebook.react.bridge.WritableMap
import com.facebook.react.uimanager.UIManagerHelper
import com.facebook.react.uimanager.events.Event
import com.facebook.react.uimanager.events.EventDispatcher
import kotlin.math.max


class ARView(context: Context) : FrameLayout(context), GLSurfaceView.Renderer {

    private val mainHandler = Handler(Looper.getMainLooper())

    private val glSurfaceView: GLSurfaceView = GLSurfaceView(context).apply {
        layoutParams = LayoutParams(
            ViewGroup.LayoutParams.MATCH_PARENT,
            ViewGroup.LayoutParams.MATCH_PARENT,
        )
        setEGLContextClientVersion(2)
        setEGLConfigChooser(8, 8, 8, 8, 16, 0)
        preserveEGLContextOnPause = true
    }

    private val nativeView: ARViewNative = ARViewNative(this)

    private val gestureDetector: GestureDetector = GestureDetector(
        context,
        object : GestureDetector.SimpleOnGestureListener() {
            override fun onDown(e: MotionEvent): Boolean {
                // Must return true to continue receiving events.
                return true
            }

            override fun onSingleTapUp(e: MotionEvent): Boolean {
                val x = e.x
                val y = e.y

                // Run on the OpenGL (GLSurfaceView) thread.
                glSurfaceView.queueEvent {
                    nativeView.onGestureTap(x, y)
                }
                return true
            }
        },
    )


    init {
        layoutParams = LayoutParams(
            ViewGroup.LayoutParams.MATCH_PARENT,
            ViewGroup.LayoutParams.MATCH_PARENT,
        )

        addView(glSurfaceView)
        glSurfaceView.setRenderer(this)

        glSurfaceView.setEGLConfigChooser(8, 8, 8, 8, 16, 0)

        glSurfaceView.renderMode = GLSurfaceView.RENDERMODE_CONTINUOUSLY

        glSurfaceView.setWillNotDraw(false)

        glSurfaceView.setOnTouchListener { _, event ->
            // Feed gesture recognizer; we only care about single-tap-up.
            gestureDetector.onTouchEvent(event)
            true
        }
    }

    override fun onAttachedToWindow() {
        super.onAttachedToWindow()

        // Mark this view as active and ensure the session exists while the Activity is alive.
        HelloAppSystem.instance.onARViewMounted()

        val activity = CurrentActivityTracker.getCurrentActivity()
        if (activity != null) {
            HelloAppSystem.instance.onResume(context.applicationContext, activity)
        }
    }

    override fun onDetachedFromWindow() {
        // Detach view resources only; session is owned globally and must persist across navigation.
        nativeView.onSurfaceDestroyed()

        // Mark this view as inactive; global session pauses when no active views remain.
        HelloAppSystem.instance.onARViewUnmounted()
        super.onDetachedFromWindow()
    }

    override fun onSurfaceCreated(
        gl: javax.microedition.khronos.opengles.GL10?,
        config: javax.microedition.khronos.egl.EGLConfig?,
    ) {
        // PASS THE SURFACE TO C++ IMMEDIATELY
        nativeView.onSurfaceCreated(glSurfaceView.holder.surface)
        nativeView.onGlSurfaceCreated()
    }

    override fun onSurfaceChanged(
        gl: javax.microedition.khronos.opengles.GL10?,
        width: Int,
        height: Int,
    ) {
        nativeView.onSurfaceChanged(width, height)
        nativeView.onGlSurfaceChanged(width, height)
    }

    override fun onDrawFrame(gl: javax.microedition.khronos.opengles.GL10?) {
        nativeView.onGlDrawFrame()
    }
    
    // Props from React are passed directly to C++ via JNI
    fun setPaused(paused: Boolean) {
        nativeView.setPaused(paused)
    }

    fun setPlaneDetectionEnabled(enabled: Boolean?) {
        if (enabled != null) {
            nativeView.setPlaneDetectionEnabled(enabled)
        }
    }

    fun setLightEstimationEnabled(enabled: Boolean?) {
        if (enabled != null) {
            nativeView.setLightEstimationEnabled(enabled)
        }
    }

    fun setSessionType(sessionType: String?) {
        if (!sessionType.isNullOrEmpty()) {
            nativeView.setSessionType(sessionType)
        }
    }

    fun setDepthMode(depthMode: String?) {
        if (!depthMode.isNullOrEmpty()) {
            nativeView.setDepthMode(depthMode)
        }
    }

    fun setCloudAnchorMode(mode: String?) {
        if (!mode.isNullOrEmpty()) {
            nativeView.setCloudAnchorMode(mode)
        }
    }

    fun setInstantPlacementMode(mode: String?) {
        if (!mode.isNullOrEmpty()) {
            nativeView.setInstantPlacementMode(mode)
        }
    }

    fun setLightEstimationMode(mode: String?) {
        if (!mode.isNullOrEmpty()) {
            nativeView.setLightEstimationMode(mode)
        }
    }

    fun setPlaneDetectionMode(mode: String?) {
        if (!mode.isNullOrEmpty()) {
            nativeView.setPlaneDetectionMode(mode)
        }
    }

    fun setFocusMode(mode: String?) {
        if (!mode.isNullOrEmpty()) {
            nativeView.setFocusMode(mode)
        }
    }

    fun setDebugShowPlanes(enabled: Boolean?) {
        if (enabled != null) {
            nativeView.setDebugShowPlanes(enabled)
        }
    }

    fun setDebugShowPointCloud(enabled: Boolean?) {
        if (enabled != null) {
            nativeView.setDebugShowPointCloud(enabled)
        }
    }

    fun setDebugShowWorldOrigin(enabled: Boolean?) {
        if (enabled != null) {
            nativeView.setDebugShowWorldOrigin(enabled)
        }
    }

    fun setDebugShowDepthMap(enabled: Boolean?) {
        if (enabled != null) {
            nativeView.setDebugShowDepthMap(enabled)
        }
    }

    // Called by JNI -> Java -> Kotlin to emit RN direct events.
    fun emitARCoreError(code: String, message: String) {
        val payload = Arguments.createMap().apply {
            putString("code", code)
            putString("message", message)
        }
        dispatchDirectEvent("topARCoreError", payload)
    }

    fun emitTrackingStateChange(state: String, reason: String?) {
        val payload = Arguments.createMap().apply {
            putString("state", state)
            if (!reason.isNullOrEmpty()) {
                putString("reason", reason)
            }
        }
        dispatchDirectEvent("topTrackingStateChange", payload)
    }

    fun emitPlaneDetected(
        planeId: String,
        type: String,
        cx: Float,
        cy: Float,
        cz: Float,
        ex: Float,
        ez: Float,
    ) {
        val payload = Arguments.createMap().apply {
            putString("id", planeId)
            putString("type", type)
            putMap(
                "center",
                Arguments.createMap().apply {
                    putDouble("x", cx.toDouble())
                    putDouble("y", cy.toDouble())
                    putDouble("z", cz.toDouble())
                },
            )
            putMap(
                "extent",
                Arguments.createMap().apply {
                    putDouble("x", max(0f, ex).toDouble())
                    putDouble("z", max(0f, ez).toDouble())
                },
            )
        }
        dispatchDirectEvent("topPlaneDetected", payload)
    }

    fun emitPlaneUpdated(
        planeId: String,
        type: String,
        cx: Float,
        cy: Float,
        cz: Float,
        ex: Float,
        ez: Float,
    ) {
        val payload = Arguments.createMap().apply {
            putString("id", planeId)
            putString("type", type)
            putMap(
                "center",
                Arguments.createMap().apply {
                    putDouble("x", cx.toDouble())
                    putDouble("y", cy.toDouble())
                    putDouble("z", cz.toDouble())
                },
            )
            putMap(
                "extent",
                Arguments.createMap().apply {
                    putDouble("x", max(0f, ex).toDouble())
                    putDouble("z", max(0f, ez).toDouble())
                },
            )
        }
        dispatchDirectEvent("topPlaneUpdated", payload)
    }

    private fun dispatchDirectEvent(topEventName: String, payload: WritableMap) {
        val reactContext = context as? ReactContext ?: return
        val dispatcher: EventDispatcher = UIManagerHelper.getEventDispatcherForReactTag(reactContext, id) ?: return
        val viewId = id
        val surfaceId = UIManagerHelper.getSurfaceId(this)

        class DirectEvent(
            surfaceId: Int,
            viewTag: Int,
            private val topEventName: String,
            private val eventData: WritableMap,
        ) : Event<DirectEvent>(surfaceId, viewTag) {
            override fun getEventName(): String = topEventName
            override fun getEventData(): WritableMap = eventData
        }

        val runnable = Runnable {
            dispatcher.dispatchEvent(DirectEvent(surfaceId, viewId, topEventName, payload))
        }

        if (Looper.myLooper() == Looper.getMainLooper()) {
            runnable.run()
        } else {
            mainHandler.post(runnable)
        }
    }

}