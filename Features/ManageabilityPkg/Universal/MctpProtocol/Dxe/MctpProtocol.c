/** @file
  This file provides edk2 MCTP Protocol implementation.

  Copyright (C) 2023 Advanced Micro Devices, Inc. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <PiDxe.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/ManageabilityTransportLib.h>
#include <Library/ManageabilityTransportHelperLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include <IndustryStandard/Mctp.h>

/**
  The entry point of the MCTP DXE driver.

  @param[in] ImageHandle - Handle of this driver image
  @param[in] SystemTable - Table containing standard EFI services

  @retval EFI_SUCCESS    - IPMI Protocol is installed successfully.
  @retval Otherwise      - Other errors.
**/
EFI_STATUS
EFIAPI
DxeMctpEntry (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{

  return EFI_SUCCESS;
}

/**
  This is the unload handler for MCTP protocol module.

  Release the MANAGEABILITY_TRANSPORT_TOKEN acquired at entry point.

  @param[in] ImageHandle           The drivers' driver image.

  @retval    EFI_SUCCESS           The image is unloaded.
  @retval    Others                Failed to unload the image.

**/
EFI_STATUS
EFIAPI
MctpUnloadImage (
  IN EFI_HANDLE  ImageHandle
  )
{

  return EFI_SUCCESS;
}

