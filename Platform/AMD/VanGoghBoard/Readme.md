This document introduces how AMD create a EDK II based sample platform BIOS for AMD Chachani-based reference board.
Customer can reference this document to study EDK2 BIOS integration.

# How to build

## The below steps are verified on Microsoft Windows 10 64-bit.
1.Install latest Microsoft Visual Studio 2017 Professional version(15.9.40 or newer) in the build machine ,make sure
  that Desktop development with C++ was selected when installing. And selected MSVC & Windows 10 SDK tool when installing.
2.Install Python 3.9.x (python-3.9.13-amd64.exe), make sure path is "C:\Python39".
3.Install NASM (nasm-2.15.05-installer-x64.exe), and make sure path is "C:\Nasm". (http://www.nasm.us/)
4.Download and extract iasl-win-20130117.zip from https://acpica.org/sites/acpica/files/iasl-win-20130117.zip,
  and copy iasl.exe to C:\ASL.
5.Download and install ActivePerl-5.24.3.2404-MSWin32-x64-404865.exe, copy folders Perl64\bin and Perl64\lib to correct path
  at Buildrom.bat.
6.Download and install OpenSSL https://github.com/openssl/openssl/archive/OpenSSL_1_0_1e.zip. And make sure OPENSSL_PATH set
  the correct path at GenCapsule.bat.

## Linux build environment preparation

### Common Environment
Just like Windows environment, you need to install several common tools: IASL & NASM. You may refer to your distribution's
instructions. E.g., apt install iasl nasm in Ubuntu, or pacman -S iasl nasm in Archlinux/SteamOS.Some distributions lack of
developer packages by default. E.g., on Ubuntu, you may install uuid-dev explicitly by apt install uuid-dev.Python3 is
built-in for most Linux distributions. You may install it manually if your distribution lacks of it.

### GCC Environment
GCC is built-in for most Linux distributions. Use gcc --version to check its version and ensure its major version > 5.
Also, make sure binutils is installed. Use ld --version to check its version. If gcc & binutils are not installed,
you may refer to your distribution's instructions.

### Clang Environment
For license reason, Clang is NOT included in most distributions. You may install manually with your distribution's instructions.
E.g., apt install llvm clang lld in Ubuntu, or pacman -S llvm clang lld in Archlinux/SteamOS.
Use clang --version to check Clang's version and ensure its major version > 9.
Use lld-link --version to check LLD's version and ensure its major version > 9.

## Obtaining source code
1. Create a new folder (directory) on your local development machine for use as your workspace. This example
   uses `/work/git/tianocore`, modify as appropriate for your needs.
   ```
   $ export WORKSPACE=/work/git/tianocore
   $ mkdir -p $WORKSPACE
   $ cd $WORKSPACE
   ```

2. Into that folder, clone:
   1. [edk2](https://github.com/tianocore/edk2)
   2. [edk2-platforms](https://github.com/tianocore/edk2-platforms)
   3. [edk2-non-osi](https://github.com/tianocore/edk2-non-osi)
   ```
   $ git clone https://github.com/tianocore/edk2.git
   $ git checkout <branch name> (Please follow ReleaseNote.txt to checkout the specified version)
   $ git submodule update --init
   ...
   $ git clone https://github.com/tianocore/edk2-platforms.git
   $ git submodule update --init
   ...
   $ git clone https://github.com/tianocore/edk2-non-osi.git
   $ git checkout <branch name> (Please follow ReleaseNote.txt to checkout the specified version)
   ```
## Manual building

### Windows building
Copy GoZ_ChachaniInt.bat to $WORKSPACE and run it , then execute command “buildrom.bat” to generate final BIOS binary 'ChachaniIntUDK.FD'.

### Linux building
Copy build.sh to $WORKSPACE and run it to generate final BIOS binary 'ChachaniIntUDK.FD'.
