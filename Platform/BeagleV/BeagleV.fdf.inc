## @file
#  Definitions of Flash definition file on BeagleV RISC-V platform
#
#  Copyright (c) 2019, Hewlett Packard Enterprise Development LP. All rights reserved.<BR>
#
#  SPDX-License-Identifier: BSD-2-Clause-Patent
#
##
[Defines]
DEFINE BLOCK_SIZE        = 0x1000

DEFINE FW_BASE_ADDRESS   = 0x80000000
DEFINE FW_SIZE           = 0x00240000 # $(FW_BLOCKS) * 0x1000
DEFINE FW_BLOCKS         = 0x240      # $(CODE_BLOCKS) + $(VARS_BLOCKS)

#
# 0x000000-0x7DFFFF code (uncompressed)
# 0x7E0000-0x800000 variables
#
DEFINE CODE_BASE_ADDRESS = 0x80000000 # $(FW_BASE_ADDRESS)
DEFINE CODE_SIZE         = 0x00200000 # $(CODE_BLOCKS) * 0x1000
DEFINE CODE_BLOCKS       = 0x200      # FV_MAIN_OFFSET + FV_MAIN_SIZE
DEFINE VARS_BLOCKS       = 0x20       # $(VARS_SIZE) / 0x1000

#
# SEC + opensbi library is the root FW domain.
# The base address must be round up to log2.
#
DEFINE SECFV_OFFSET      = 0x00000000
DEFINE SECFV_SIZE        = 0x00030000
DEFINE ROOT_FW_DOMAIN_SIZE = $(SECFV_SIZE)

#
# Other FV regions are in the second FW domain.
# The size of memory region must be power of 2.
# The base address must be aligned with the size.
#
# FW memory region
#
DEFINE PEIFV_OFFSET      = 0x00030000
DEFINE PEIFV_SIZE        = 0x00080000
DEFINE FVMAIN_OFFSET     = 0x000b0000
DEFINE FVMAIN_SIZE       = 0x00100000

#DEFINE EMPTY_OFFSET    = 0x001b0000
#DEFINE EMPTY_SIZE      = 0x00010000

#
# EFI Variable memory region.
# The total size of EFI Variable FD must include
# all of sub regions of EFI Variable
#
DEFINE VARS_OFFSET       = 0x00200000
DEFINE VARS_SIZE         = 0x00007000

DEFINE VARS_FTW_WORKING_OFFSET       = 0x00207000
DEFINE VARS_FTW_WORKING_SIZE         = 0x00001000
DEFINE VARS_FTW_SPARE_OFFSET         = 0x00208000
DEFINE VARS_FTW_SPARE_SIZE           = 0x00018000

#
# Device Tree memory region
#
DEFINE DTB_OFFSET        = 0x00220000
DEFINE DTB_SIZE          = 0x00020000

DEFINE FW_DOMAIN_SIZE    = $(FVMAIN_OFFSET) + $(FVMAIN_SIZE) - $(PEIFV_OFFSET)
DEFINE VARIABLE_FW_SIZE  = $(VARS_FTW_SPARE_OFFSET) + $(VARS_FTW_SPARE_SIZE) - $(VARS_OFFSET)

SET gUefiRiscVPlatformPkgTokenSpaceGuid.PcdRootFirmwareDomainBaseAddress = $(CODE_BASE_ADDRESS) + $(SECFV_OFFSET)
SET gUefiRiscVPlatformPkgTokenSpaceGuid.PcdRootFirmwareDomainSize        = $(ROOT_FW_DOMAIN_SIZE)
SET gUefiRiscVPlatformPkgTokenSpaceGuid.PcdFirmwareDomainBaseAddress     = $(CODE_BASE_ADDRESS) + $(PEIFV_OFFSET)
SET gUefiRiscVPlatformPkgTokenSpaceGuid.PcdFirmwareDomainSize            = $(FW_DOMAIN_SIZE)

SET gUefiRiscVPlatformPkgTokenSpaceGuid.PcdVariableFdBaseAddress = $(FW_BASE_ADDRESS) + $(VARS_OFFSET)
SET gUefiRiscVPlatformPkgTokenSpaceGuid.PcdVariableFdSize        = $(VARS_SIZE) + $(VARS_FTW_WORKING_SIZE) + $(VARS_FTW_SPARE_SIZE)
SET gUefiRiscVPlatformPkgTokenSpaceGuid.PcdVariableFdBlockSize   = $(BLOCK_SIZE)
SET gUefiRiscVPlatformPkgTokenSpaceGuid.PcdVariableFirmwareRegionBaseAddress = $(CODE_BASE_ADDRESS) + $(VARS_OFFSET)
SET gUefiRiscVPlatformPkgTokenSpaceGuid.PcdVariableFirmwareRegionSize        = $(VARIABLE_FW_SIZE)

SET gUefiRiscVPlatformPkgTokenSpaceGuid.PcdOpenSbiStackSize = 8192
SET gUefiRiscVPlatformPkgTokenSpaceGuid.PcdScratchRamBase   = $(CODE_BASE_ADDRESS) + $(SCRATCH_OFFSET)
SET gUefiRiscVPlatformPkgTokenSpaceGuid.PcdScratchRamSize   = $(SCRATCH_SIZE)
SET gUefiRiscVPlatformPkgTokenSpaceGuid.PcdTemporaryRamBase = $(CODE_BASE_ADDRESS) + $(FW_SIZE)
SET gUefiRiscVPlatformPkgTokenSpaceGuid.PcdTemporaryRamSize = 0x10000


SET gUefiRiscVPkgTokenSpaceGuid.PcdRiscVMachineTimerFrequencyInHerz    = 6250000
# TODO: This can be dynamically configured in QEMU
SET gUefiRiscVPlatformPkgTokenSpaceGuid.PcdHartCount                   = 2          # Total cores on BeagleV platform
SET gUefiRiscVPlatformPkgTokenSpaceGuid.PcdBootHartId                  = 0          # Boot hart ID
SET gUefiRiscVPlatformPkgTokenSpaceGuid.PcdBootableHartNumber          = 1          # The bootable hart core number.
                                                                                    # Which is incorporate with OpenSBI
                                                                                    # platform hart_index2id value.
SET gUefiRiscVPlatformPkgTokenSpaceGuid.PcdPeiCorePrivilegeMode        = 1          # Set PeiCore to S-Mode
