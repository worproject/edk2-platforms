# Overview
Build all advanced features for testing build and integration.

Please refer to individual feature packages for details on intended uses.

# High-Level Theory of Operation
Enable all features and build a reasonable default configuration.  This is not expected to produce binaries that are directly usable in a product as features may require board or silicon specific configuration and libraries.

## Firmware Volumes
Produces:
* FvAdvancedPreMemory
* FvAdvancedUncompressed
* FvAdvanced

## Build
Supported build targets
* VS2019
* CLANGPDB
* GCC5

## Windows Example:
With current working directory at O:\

git clone https://github.com/tianocore/edk2.git

git clone https://github.com/tianocore/edk2-platforms.git

set workspace=O:\

set EDK_TOOLS_PATH=O:\Edk2\BaseTools

set packages_path=O:\edk2;O:\edk2-non-osi;O:\edk2-platforms\Platform\Intel;O:\edk2-platforms\Silicon\Intel;O:\edk2-platforms\Platform\Qemu;O:\edk2-platforms\Silicon\Qemu;O:\edk2-platforms\Features\Intel;O:\edk2-platforms\Features\Intel\Debugging;O:\edk2-platforms\Features\Intel\Network;O:\edk2-platforms\Features\Intel\OutOfBandManagement;O:\edk2-platforms\Features\Intel\PowerManagement;O:\edk2-platforms\Features\Intel\SystemInformation;O:\edk2-platforms\Features\Intel\UserInterface

cd \edk2

edksetup.bat Rebuild

build -a IA32 -a X64 -t CLANGPDB -b NOOPT -p AdvancedFeaturePkg\AdvancedFeaturePkg.dsc
