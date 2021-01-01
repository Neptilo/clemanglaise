# Clemanglaise

This program creates vocabulary tests in different languages using words from an online or a local database.

## How to use?

### Usual method, working on Linux, Mac or Windows

  * First, install Qt **with the Open Source licence**.
    * Download and run [the online installer](https://www.qt.io/download-thank-you) 
    * At *Select Components* step, expand *Qt 5.14.2* and select the relevant components for the architectures on which you wish to deploy the app.

      **E.g:** for Windows, tick *MinGW 7.3.0 64-bit*. 
    * To deploy on Android, expand *Qt 5.15.2* and check *Android*. (The *5.14.2* scripts contain bugs.)
  * Open the project **clemanglaise.pro** with QtCreator.
  * On Linux, run `sudo apt install libgl1-mesa-dev` to install `libGL`.
  * If you intend to deploy the app on Android, there is some more preparative work to do. Please follow [these steps](doc/android.md) (only for Linux users) and come back here when you're done.
  * Compile the project (hotkey: `CTRL+B`). (This will also run `qmake` before compiling.)
  * Since OpenSSL is not included in Qt and that I didn't want to include it in the repository, you'll need to download it yourself:
    * To build on Windows:
      * Download and extract [this zip](https://bintray.com/vszakats/generic/download_file?file_path=openssl-1.1.1d-win64-mingw.zip)(1).
      * In the extracted folder, copy `libcrypto-1_1-x64.dll` and `libssl-1_1-x64.dll` to the folder where Clemanglaise was built (in the same folder as `clemanglaise.exe`).

      Even though these libs are not in the exact version we should be using (it should be 1.1.1d), it works and I think that's good enough for now.
    * On Linux, if you have some version of OpenSSL 1.1.x installed on your system, it should work without having to do anything more.
    * On Mac, I haven't tested (let me know if you do), but I hope it's the same as on Linux.
  * Run the project (hotkey: `CTRL+R`) and enjoy!

If you have been granted the **administrator** rights to the project's database, you have to set a password argument to run the application.

  * To do it, go to **Projects mode > Run Settings**
  * Then set Arguments to `-p <pwd>` or `--password <pwd>`, replacing `<pwd>` with the password.
  * If you want to open help wizard set Arguments to `-h` or `-?` or `--help`	

### For command-line users

To compile it:

  * Create a `bin_clemanglaise` directory located in the same parent directory as clemanglaise.
  * Check that the location of the `qmake` is in your `$PATH` variable.
  * Don't regenerate the project file, use the one provided in the repository.
  * Go into `bin_clemanglaise` directory
  * Type `qmake <path>` in the command line, replacing `<path>` with the path to `clemanglaise.pro`.
  * Type `make` in the command line.

Add the necessary OpenSSL libs to your build folder: follow the step about OpenSSL in the previous section describing the *Usual method*.

To run the application, type:
* `./clemanglaise` in the command line if you are using Linux,
* `clemanglaise.exe` if you are using Windows,
* `open clemanglaise.app` if you are using the Mac.

To run the application with **administrator** rights to the database, type `<cmd> -p <pwd>`, replacing `<cmd>` with the command mentioned above, and `<pwd>` with the password.

To show help wizard, type `<cmd> -h`, replacing `<cmd>` with the command mentioned above.

Enjoy!

(1) To find OpenSSL libraries of a specific version, go [here](https://wiki.openssl.org/index.php/Binaries).
