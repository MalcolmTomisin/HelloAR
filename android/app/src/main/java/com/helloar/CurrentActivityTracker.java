package com.helloar;

import android.app.Activity;
import android.app.Application;
import android.os.Bundle;
import java.lang.ref.WeakReference;

public class CurrentActivityTracker implements Application.ActivityLifecycleCallbacks {
    // Use WeakReference to avoid memory leaks if the activity is destroyed
    private static WeakReference<Activity> mCurrentActivity = new WeakReference<>(null);

    // Register this in your Application.onCreate()
    public static void register(Application app) {
        app.registerActivityLifecycleCallbacks(new CurrentActivityTracker());
    }

    public static Activity getCurrentActivity() {
        return mCurrentActivity.get();
    }

    @Override
    public void onActivityResumed(Activity activity) {
        // When an activity comes to foreground, store it
        mCurrentActivity = new WeakReference<>(activity);
    }

    @Override
    public void onActivityPaused(Activity activity) {
        // Optional: clear it if you only want "Resumed" activities
        // mCurrentActivity = new WeakReference<>(null); 
    }

    // ... Implement other interface methods as empty ...
    @Override public void onActivityCreated(Activity a, Bundle b) {}
    @Override public void onActivityStarted(Activity a) {}
    @Override public void onActivityStopped(Activity a) {}
    @Override public void onActivitySaveInstanceState(Activity a, Bundle b) {}
    @Override public void onActivityDestroyed(Activity a) {}
}
