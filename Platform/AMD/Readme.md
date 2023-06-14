# AMD EDK2 Platform

This is AMD folder that provides the edk2 modules to support AMD edk2 firmware
solution for the server, client (e.g., Notebook) and S3 (Strategic Silicon Solutions)
platforms. The board packages under this folder are the firmware reference code for
booting certain AMD platforms. The definition of sub-folders is described in below sections.

## Term and Definitions

* **AMD Platform** (platform in short)

  AMD platform refers to a platform that supports the particular AMD SoC (processor), such as AMD EPYC Milan and Genoa processors.

* **AMD Board** (board in short)

  AMD board is a generic terminology refers to a board that is designed based on a
  specific AMD SoC architecture (also referred as AMD platform). More than one boards
  are possibly designed to support an AMD platform with different configuration, such as 1-processor socket or 2-processor sockets board.

* **AMD edk2 Platform Package** (platform package in short)
  
  The folder has the AMD edk2 platform common modules.

* **AMD edk2 Board Package** (board package in short)

  The folder has the edk2 meta files to build the necessary edk2 firmware modules
  and generate the binary to run on a board.

## Package Definition

* **AmdPlatformPkg**

  AMD platform edk2 package under this folder provides the common edk2 modules those
  are leverage by platforms. Usually those modules have no dependencies with
  particular platforms. The module under this scope can provides a common implementation
  for all platforms, or it may just provide a framework but the differences of implementation could be configured through the PCDs declared in AmdPlatformPkg.dec, or
  the board level library provided in the \<Board name\>Pkg.

* **AmdMinBoardPkg**

  This package provides the common edk2 modules those can be leverage across AMD boards those use MinPlatformPkg framework.

* **\<SoC name\>Board**

  This is the folder named by SoC and accommodate one or multiple board packages those
  are designed base on the same SoC platform. <SoC name>Board folder may contain edk2
  package meta files directly or the sub-folders named by \<Board name\>Pkg for a 
  variety configurations of a platform.

* **<Board name\>Pkg**

  This is the folder that contains edk2 package meta files for a board which is designed base on a platform. Besides the edk2 meta files, <Board name\>Pkg may also provides
  edk2 modules which are specifically to a board.

  ```
  e.g. OverdriveBoard
  e.g. GenoaBoard
           |------Board1Pkg
           |------Board2Pkg
  ```

  Below is the outline of folder structure under Platform/AMD

  ```
  Platform/AMD
            |----AmdPlatformPkg
            |----AmdMinBoardPkg
            |----OverdriveBoard
            |----GenoaBoard
            |         |------Common Modules for Genoa boards
            |         |------Board1Pkg
            |         |        |-------Board specific modules
            |         |------Board2Pkg
            |
            |----NextGenBoard
                      |------Common Modules for the next generation
                             platform boards
                      |------Board1Pkg
                      |------Board2Pkg
                               |-------Board specific modules
  ```


## Board Support
Under progress
