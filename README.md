# Clemanglaise

This program creates vocabulary tests in different languages using words from an online or a local database.

## How to use?

### Usual method, working on Linux, Mac or Windows

  * First Qt must be installed on your computer. 

    * [Qt installer for MAC 64 bits](http://download.qt-project.org/official_releases/online_installers/qt-mac-opensource-1.4.0-x86_64-online.dmg "download link for Mac") 
    * [Qt installer for Linux 64 bits](http://download.qt-project.org/official_releases/online_installers/qt-linux-opensource-1.4.0-x86_64-online.run "download link for Linux") 
    * [Qt installer for Windows](http://download.qt-project.org/official_releases/online_installers/qt-windows-opensource-1.4.0-x86-online.exe "Qt installer for Windows")

  * Open the project **clemanglaise.pro** with QtCreator.
  * Compile with qmake in the compilation tab.
  * Compile the project (hotkey: `CTRL+B`).
  * Run the project (hotkey: `CTRL+R`) and enjoy!

If you are an **administrator** after opening the project, you have to set a password argument
to run the application.

  * To do it, go to **Projects mode > Run Settings**
  * Then set Arguments to `-p PASSWORD` or `--password PASSWORD`
  * If you want to open help wizard set Arguments to `-h` or `-?` or `--help`	

### For command-line users

To compile it:

  * Check that the location of the `qmake` is in your `$PATH` variable.
  * Don't regenerate the project file, use the one provided in the repository.
  * Type `qmake` in the command line.
  * Type `make` in the command line.

To run the application, type `./clemanglaise` in the command line if you are using Linux, or `open clemanglaise.app` if you are using the Mac.

To run the application as an **administrator** type `./clemanglaise -p` and type the
password.

To show help wizard type `./clemanglaise -h|-?|--help`

Enjoy!