# Clemanglaise

This program creates vocabulary tests in different languages using words from an online or a local database.

## Build & run

### Usual method, working on Linux, Mac or Windows

  * First, install Qt **with the Open Source licence**.
    * Download and run [the online installer](https://www.qt.io/download-qt-installer-oss) 
    * At *Select Components* step, expand *Qt 6.5.3* and select the relevant components for the architectures on which you wish to deploy the app.

      **E.g:** for Windows, tick *MinGW 11.2.0 64-bit* both under *Qt 6.5.3* and under *Build Tools*. 
    * To deploy on Android, expand *Qt 5.15.2* and check *Android*. (The *5.14.2* scripts contain bugs.)
    * To build in WebAssembly, check the corresponding option.
  * Open the project **clemanglaise.pro** with QtCreator.
  * On Linux, run `sudo apt install libgl1-mesa-dev` to install `libGL`.
  * If you intend to deploy the app on Android, there is some more preparative work to do. Please follow [these steps](doc/android.md) (only for Linux users) and come back here when you're done.
  * If you are building for WebAssembly, better do it on a Unix system (required by makefile).
    * Type:
      * `git clone https://github.com/emscripten-core/emsdk.git`
      * `cd emsdk`
      * `./emsdk install 1.39.7` That version is the one WebAssembly was built with in Qt 5.15.2.
      * `./emsdk activate --embedded 1.39.7`
      * `source ./emsdk_env.sh`
      * `sudo apt install python3`
      * `sudo apt install python-is-python3` (not sure if this one is needed)
      * `sudo apt install python3-distutils`
    * Configure QtCreator using [this guide](https://doc.qt.io/qtcreator/creator-setup-webassembly.html)
    
      Other useful link [here](https://doc.qt.io/qt-5/wasm.html)
  * Compile the project (hotkey: `CTRL+B`). (This will also run `qmake` before compiling.)
  * Configure the run target: Since the project uses a subdirs structure, you need to set up Qt Creator to run the main application:
    * Go to **Projects** (left sidebar) → **Run**
    * In the "Run configuration" dropdown, select "main". This will be the default target when you run the project.
  * Since OpenSSL is not included in Qt and that I didn't want to include it in the repository, you'll need to download it yourself:
    * To build on Windows:
      * Download and extract [this zip](https://bintray.com/vszakats/generic/download_file?file_path=openssl-1.1.1d-win64-mingw.zip)(1).
      * In the extracted folder, copy `libcrypto-1_1-x64.dll` and `libssl-1_1-x64.dll` to the folder where Clemanglaise was built (in the same folder as `clemanglaise.exe`).

      Even though these libs are not in the exact version we should be using (it should be 1.1.1d), it works and I think that's good enough for now.
    * On Linux, if you have some version of OpenSSL 1.1.x installed on your system, it should work without having to do anything more.
    * On Mac, I haven't tested (let me know if you do), but I hope it's the same as on Linux.
  * Run the project (hotkey: `CTRL+R`) and enjoy!

#### Running unit tests

To run the unit tests:

  * Build the project (`CTRL+B`) - this builds both the application and tests
  * Expand the project tree and locate the **tests** subproject
  * Right-click **tests** and select **Run** (or set it as the active target and press `CTRL+R`)

The test output will be displayed in the Application Output panel.

#### Administrator access

If you have been granted the **administrator** rights to the project's database, you have to set a password argument to run the application:

  * Go to **Projects mode > Run Settings**
  * Set Arguments to `-p <pwd>` or `--password <pwd>`, replacing `<pwd>` with the password
  * To open the help wizard, set Arguments to `-h` or `-?` or `--help`	

### For command-line users

To compile it:

  * Create a `build` directory located in the same parent directory as clemanglaise.
  * Check that the location of the `qmake` is in your `$PATH` variable.
  * Don't regenerate the project file, use the one provided in the repository.
  * Go into `build` directory
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

## Building the installer

Here are the steps to build a Windows installer:
* Download and install Inno Setup from either of these servers:
  * [US](https://jrsoftware.org/download.php/is.exe?site=1)
  * [Netherlands](https://jrsoftware.org/download.php/is.exe?site=2)
* Go to the location where `clemanglaise.exe` is built and use **windeployqt** to gather the required Qt libraries: 
`windeployqt.exe --compiler-runtime clemanglaise.exe`. This copies the necessary DLLs into your application folder.
* Compile and create the installer:
  * Open **Inno Setup Compiler**.
  * Click *File > Open* and select the clemanglaise.iss file located in the project root.
  * Click *Compile* or press F9.
  * The installer (setup_clemanglaise.exe) will be created in the "Output" directory.
* Code signing:
  * Make sure the `signtool` command is in your `$PATH` variable. If not, add `C:\Program Files (x86)\Windows Kits\10\bin\{version}\x64` to your path, replacing `{version}` with the latest available version.
  * In a Windows cmd console, go to the location where the installer was built, and run `signtool sign /a /tr http://timestamp.digicert.com /td SHA256 /fd SHA256 "setup_clemanglaise.exe"`.

(1) To find OpenSSL libraries of a specific version, go [here](https://wiki.openssl.org/index.php/Binaries).
