*Adapted from [Getting Started with Qt for Android](https://doc.qt.io/qt-5/android-getting-started.html)*

Here are instructions to build the project for Android, from Linux (Debian).

I first tried with Windows but it proved 100 times more difficult to install JDK v8 than on Linux.

## Installing the prerequisites

### Install SDK
Download and install [the SDK](https://developer.android.com/studio#downloads).

The SDK will be installed when first launching Android Studio. Take note of the installation path, which will be shown during the process. By default, it should be in ` ~/Android/Sdk`

Because Qt doesn't seem able to keep up with Android Studio updates, there are always some compatibility issues to fix. Here, we need to download an older version of the SDK tools:

Download and extract [this zip](https://dl.google.com/android/repository/tools_r25.2.5-linux.zip).

Then, move the `tools` folder it contains into the SDK folder.

### Install NDK
Download [the NDK](https://developer.android.com/ndk/downloads)
and extract it in a place where you can find it again later. E.g. I put it under `~/Android` where the SDK is also installed.

The path must not contain spaces.

### Install JDK
Run in command-line: `sudo apt install openjdk-8-jdk`

### Updating the Android SDK
Start Android Studio. In the bottom right corner, click on *Configure > SDK Manager*.

Then, in the *SDK Platforms* tab, tick *Android 7.0*. If you would like to try for earlier versions, please do, and let me know how it goes. At least I can say it failed to work on Android 5.0.

In the *SDK Tools* tab, tick anything that has the status "Update Available".

Finally, click *OK*.

### Configuring Your Development Host
Run `sudo apt install libstdc++6:i386 libgcc1:i386 zlib1g:i386 libncurses5:i386`

If you want to run the emulator, the following dependencies are also necessary:

`sudo apt install libsdl1.2debian:i386`

#### Qt Creator IDE
Launch Qt Creator and select *Tools > Options > Devices > Android* to add the JDK, Android NDK and SDK paths.

**E.g:**
* **JDK location:** `/usr/lib/jvm/java-8-openjdk-amd64`
* **Android SDK location:** `~/Android/Sdk`
* **Android NDK location:** `~/Android/android-ndk-r21`

Now in the Projects tab, you should see an Android kit.

Configure it: in the "Build Android APK" section, select "android-24" (this is the API version corresponding to Android 7.0) as the Android build SDK (don't forget to do it for all build configurations).

#### Connecting a device

This part should be more straightforward if you've already done development on Android. Activate developer mode on your device, plug it, and when you run the app from Qt Creator, it should ask you what device you want to use.

If you're unsure about how to do it, [here](https://doc.qt.io/qtcreator/creator-developing-android.html)'s a good help.