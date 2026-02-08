package com.helloar

import android.content.Context
import androidx.lifecycle.DefaultLifecycleObserver
import androidx.lifecycle.LifecycleOwner

class AppLifecycleListener(
    private val appContext: Context,
) : DefaultLifecycleObserver {

    override fun onStart(owner: LifecycleOwner) {
        super.onStart(owner)
    }

    override fun onStop(owner: LifecycleOwner) {
        super.onStop(owner)
    }

    override fun onResume(owner: LifecycleOwner) {
        super.onResume(owner)

        val activity = CurrentActivityTracker.getCurrentActivity() ?: return
        HelloAppSystem.instance.ensureInitialized(appContext.applicationContext)
        HelloAppSystem.instance.onResume(appContext, activity)
    }

    override fun onPause(owner: LifecycleOwner) {
        super.onPause(owner)
        HelloAppSystem.instance.onPause()
    }
}