# EDK2 Manageability Package

The edk2 Manageability package is introduced to provide edk2 drivers and
libraries for the industry platform management standards, such as PLDM (Platform
Level Data Model), MCTP (Management Component Transfer Protocol) and IPMI
(Intelligent Platform Management Interface). The framework of edk2 Manageability
package is designed to flexibly support transports for the above industry standards, the transports such as KCS or I2C for IPMI, PCI VDM (Vendor Defined
Message), I2C or KCS for MCTP, or the OEM proprietary transports.

## EDK2 Manageability Package Driver Stack

Below figure shows the driver stacks which are abstracted to support disparate
transports for the platform management.
![Manageability Package Driver Stack](https://github.com/tianocore/edk2-platforms/blob/master/Features/ManageabilityPkg/Documents/Media/ManageabilityDriverStack.svg?raw=true)

## Build the Manageability Package
In order to use the modules provided by ManageabilityPkg, **PACKAGES_PATH** must
contains the path to point to [edk2-platform Features](https://github.com/tianocore/edk2-platforms/tree/master/Features):

```
$ export PACKAGES_PATH=$PWD/edk2:$PWD/edk2-platforms:$PWD/edk2-platforms/Features
```

Due to the [IPMI Migration Work](#migration-of-ipmi-driver-stack-from-outofbandmanagement) is in progress, an
additional package path is required to add to **PACKAGES_PATH** for the reference
of IPMI modules those are currently located under
[OutOfBandManagement](https://github.com/tianocore/edk2-platforms/tree/master/Features/Intel/OutOfBandManagement) folder.

```
$PWD/edk2-platforms/Features/Intel/OutOfBandManagement
```

## Migration of IPMI Driver Stack from OutOfBandManagement

This is the ongoing task that relocates IPMI modules from OutOfBandManagement to
under ManageabilityPkg. This task also requires to abstract the transport layer
from IPMI driver in order to flexibly support different transports adopted on the platform.
