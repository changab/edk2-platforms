/** @file
  This file provides IPMI Protocol implementation.

  Copyright (C) 2023 Advanced Micro Devices, Inc. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <PiDxe.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/IpmiBaseLib.h>
#include <Protocol/IpmiProtocol.h>

/**
  This service enables submitting commands via Ipmi.

  @param[in]         This              This point for IPMI_PROTOCOL structure.
  @param[in]         NetFunction       Net function of the command.
  @param[in]         Command           IPMI Command.
  @param[in]         RequestData       Command Request Data.
  @param[in]         RequestDataSize   Size of Command Request Data.
  @param[out]        ResponseData      Command Response Data. The completion code is the first byte of response data.
  @param[in, out]    ResponseDataSize  Size of Command Response Data.

  @retval EFI_SUCCESS            The command byte stream was successfully submit to the device and a response was successfully received.
  @retval EFI_NOT_FOUND          The command was not successfully sent to the device or a response was not successfully received from the device.
  @retval EFI_NOT_READY          Ipmi Device is not ready for Ipmi command access.
  @retval EFI_DEVICE_ERROR       Ipmi Device hardware error.
  @retval EFI_TIMEOUT            The command time out.
  @retval EFI_UNSUPPORTED        The command was not successfully sent to the device.
  @retval EFI_OUT_OF_RESOURCES   The resource allcation is out of resource or data size error.
**/
EFI_STATUS
EFIAPI
IpmiSubmitCommand (
  IN     IPMI_PROTOCOL  *This,
  IN     UINT8          NetFunction,
  IN     UINT8          Command,
  IN     UINT8          *RequestData,
  IN     UINT32         RequestDataSize,
  OUT    UINT8          *ResponseData,
  IN OUT UINT32         *ResponseDataSize
  )
{
  EFI_STATUS  Status;

  Status = IpmiSendCommand (
             NetFunction,
             Command,
             RequestData,
             RequestDataSize,
             ResponseData,
             ResponseDataSize
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Failed to send IPMI command in DXE - %r\n", __FUNCTION__, Status));
  }

  return Status;
}

static IPMI_PROTOCOL  mIpmiProtocol = {
  IpmiSubmitCommand
};

/**
  The entry point of the Ipmi DXE driver.

  @param[in] ImageHandle - Handle of this driver image
  @param[in] SystemTable - Table containing standard EFI services

  @retval EFI_SUCCESS    - IPMI Protocol is installed successfully.
  @retval Otherwise      - Other errors.
**/
EFI_STATUS
EFIAPI
IpmiEntry (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS  Status;
  EFI_HANDLE  Handle;

  Handle = NULL;
  Status = gBS->InstallProtocolInterface (
                  &Handle,
                  &gIpmiProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  (VOID **)&mIpmiProtocol
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Failed to install IPMI protocol - %r\n", __FUNCTION__, Status));
  }

  return Status;
}
