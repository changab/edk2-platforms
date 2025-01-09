# AMD EDK2 Platform

This is AMD folder that provides the edk2 modules to support AMD edk2 firmware
solution for the server, client (e.g., Notebook) and S3 (Strategic Silicon Solutions)
platforms. The board packages under this folder are the firmware reference code for
booting certain AMD platforms. The definition of sub-folders is described in below sections.

## Term and Definitions

* **AGESA**

  AMD Generic Encapsulated Software Architecture that are executed as part of a
  host platform BIOS.

* **AMD Platform** (platform in short)

  AMD platform refers to a platform that supports the particular AMD SoC (processor), such as
  AMD EPYC Milan and Genoa processors.

* **AMD Board** (board in short)

  AMD board is a generic terminology refers to a board that is designed based on a
  specific AMD SoC architecture (also referred as AMD platform). More than one boards
  are possibly designed to support an AMD platform with different configuration, such as
  1-processor socket or 2-processor sockets board.

* **AMD edk2 Platform Package** (platform package in short)

  The folder has the AMD edk2 platform common modules.

* **AMD edk2 Board Package** (board package in short)

  The folder has the edk2 meta files to build the necessary edk2 firmware modules
  and generate the binary to run on a board.

## Package Definition

* **AgesaModulePkg**

  This package contains all of the private interfaces and build configuration files for the
  AGESA support.

* **AgesaPkg**

  This package contains all of the public interfaces and build configuration files
  for the AGESA support.

* **AmdCbsPkg**

  AMD Configurable BIOS Setting. Provides the edk2 formset following the UEFI HII
  spec to configure BIOS settings.

* **AmdCpmPkg**

  AMD Common Platform Module software is a BIOS procedure library designed to aid
  AMD customers to quickly implement AMD platform technology into their products.

* **AmdPlatformPkg**

  AMD platform edk2 package under this folder provides the common edk2
  modules that are leveraged by platforms. Usually those modules have no dependencies with
  particular platforms. Modules under this scope can provide a common implementation
  for all platforms, or may just provide a framework but the differences of implementation
  could be configured through the PCDs declared in AmdPlatformPkg.dec, or the board level
  library provided in the \<Board name\>Pkg.

* **AmdMinBoardPkg**

  This package provides the common edk2 modules that can be leveraged across AMD boards using
  the MinPlatform framework.

* **\<SoC name\>Board**

  This is the folder named by SoC and accommodates one or multiple board packages
  that are designed based on the same SoC platform. <SoC name>Board folder may
  contain edk2 package meta files directly or the sub-folders named by \<Board name\>Pkg for
  a variety configurations of a platform.

* **<Board name\>Pkg**

  This is the folder that contains edk2 package meta files for a board which is designed base
  on a platform. Besides the edk2 meta files, <Board name\>Pkg may also provides edk2 modules
  which are specifically to a board.

  ```
  e.g. OverdriveBoard
  e.g. GenoaBoard
           |------Board1Pkg
           |------Board2Pkg
  ```

  Below is the outline of folder structure under Platform/AMD

  ```
  Platform/AMD
            |----AgesaModulePkg
            |----AgesaPkg
            |----AmdCbsPkg
            |----AmdCpmPkg
            |----AmdPlatformPkg
            |----AmdMinBoardPkg
            |----OverdriveBoard
            |----TurinBoard
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
### <a name="Server-boards">Server boards</a>

| SoC | SoC Family| SoC SKU| SoC Name | Board   |Package|
|-----| ----------| -------| ---------|---------|-------|
| Turin SP5| 0x1A |  BRH   |Breithorn | Chalupa |TurinBoard/ChalupaBoardPkg|
|          |      |        |          | Galena  |TurinBoard/GalenaBoardPkg|
|          |      |        |          | Onyx    |TurinBoard/OnyxBoardPkg|
|          |      |        |          | Purico  |TurinBoard/PuricoBoardPkg|
|          |      |        |          | Quartz  |TurinBoard/QuartzBoardPkg|
|          |      |        |          | Ruby    |TurinBoard/RubyBoardPkg|
|          |      |        |          | Titanite|TurinBoard/TitaniteBoardPkg|
|          |      |        |          | Volcano |TurinBoard/VolcanoBoardPkg|

#### Server boards edk2 build
AMD server SoC platform firmware reference code can be built using edk2 native build system. As of now the AGESA source code is released to customer in a different way, the AGESA source files under edk2-platforms/Platform/AMD are published to make sure the platform firmware reference code can be built without errors. Those AGESA modules are considered as the NULL instance of AGESA. Customers can request the release version of
AGESA from AMD, replace the NULL instance AGESA modules.
The open-sourced AGESA modules is still under development and will be upstream to edk2-platforms as the replacement of NULL instance AGESA.

#### edk2 build steps
- Create an workspace (**[WorkSpace]**), e.g., "~/Turin" for Linux or "c:\Turin" for Windows.
- Clone the below repositories from tianocore under the **[WorkSpace]**.

  - edk2
  - edk2-platforms
  - edk2-non-osi

- Configure edk2 **PACKAGE_PATH** for consuming the modules under particular edk2 packages, replace **[WorkSpace]** with yours.

For Linux,
```
export PACKAGES_PATH=[WorkSpace]:[WorkSpace]/edk2:[WorkSpace]/edk2-platforms:[WorkSpace]/edk2-platforms/Platform/AMD:[WorkSpace]/edk2-platforms/Platform/AMD/TurinBoard:[WorkSpace]/edk2-platforms/Features:[WorkSpace]/edk2-platforms/Platform/Intel:[WorkSpace]/edk2-platforms/Features/Intel:[WorkSpace]/edk2-platforms/Features/Intel/Network:[WorkSpace]/edk2-platforms/Features/Intel/OutOfBandManagement:[WorkSpace]/edk2-platforms/Features/Intel/Debugging:[WorkSpace]/edk2-platforms/Features/Intel/SystemInformation:[WorkSpace]/edk2-platforms/Features/Intel/PowerManagement:[WorkSpace]/edk2-platforms/Features/Intel/UserInterface:[WorkSpace]/edk2-platforms/Silicon/Intel:[WorkSpace]/edk2-non-osi
```

- Configure edk2 tools path.
```
export EDK_TOOLS_PATH=[WorkSpace]/edk2/BaseTools
```
- Build edk2 base tools under **[WorkSpace]**.
```
make -C edk2/BaseTools
```
- Configure edk2 build environment, change the current working directory to **[WorkSpace]**, then execute edk2setup bash file.
```
. edksetup.sh
```

- Change edk2 WORKSPACE to **[WorkSpace]**
```
 export WORKSPACE=[WorkSpace]
```

- Build server board Project.dsc<br>
The additional build environment variables are required for the build process, refer to [server board supporting table](#Server-boards) for the values.

|Environment Variable    |Value         |
|------------------------|--------------|
|**SOC_FAMILY**          | [SoC Family] |
|**SOC_SKU**             | [SoC SKU]    |
|**SOC2**                | [SoC Name]   |

For example, building Turin SP5 Chalupa board,
```
build -a X64 -a IA32 -t GCC -DSOC_FAMILY=0x1A -DSOC_SKU=BRH -DSOC2=Breithorn -p TurinBoard/ChalupaBoardPkg/Project.dsc
```

