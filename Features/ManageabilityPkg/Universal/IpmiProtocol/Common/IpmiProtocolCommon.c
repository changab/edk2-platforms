/** @file

  IPMI Manageability Protocol common file.

  Copyright (C) 2023 Advanced Micro Devices, Inc. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#include <Uefi.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/ManageabilityTransportIpmiLib.h>
#include <Library/ManageabilityTransportLib.h>

#include "IpmiProtocolCommon.h"

extern CHAR16  *mTransportName;

MANAGEABILITY_TRANSPORT_HARDWARE_INFORMATION  mHardwareInformation;

/**
  This functions setup the IPMI transport hardware information according
  to the specification of transport token acquired from transport library.

  @param[in]         TransportToken       The transport interface.
  @param[out]        HardwareInformation  Pointer to receive the hardware information.

  @retval EFI_SUCCESS            Hardware information is returned in HardwareInformation.
                                 Caller must free the memory allocated for HardwareInformation
                                 once it doesn't need it.
  @retval EFI_UNSUPPORTED        No hardware information for the specification specified
                                 in the transport token.
  #retval EFI_OUT_OF_RESOURCES   Not enough memory for MANAGEABILITY_TRANSPORT_KCS_HARDWARE_INFO.
**/
EFI_STATUS
SetupIpmiTransportHardwareInformation (
  IN   MANAGEABILITY_TRANSPORT_TOKEN                 *TransportToken,
  OUT  MANAGEABILITY_TRANSPORT_HARDWARE_INFORMATION  *HardwareInformation
  )
{
  MANAGEABILITY_TRANSPORT_KCS_HARDWARE_INFO  *KcsHardwareInfo;

  KcsHardwareInfo = AllocatePool (sizeof (MANAGEABILITY_TRANSPORT_KCS_HARDWARE_INFO));
  if (KcsHardwareInfo == NULL) {
    DEBUG((DEBUG_ERROR, "%a: Not enough memory for MANAGEABILITY_TRANSPORT_KCS_HARDWARE_INFO.\n", __FUNCTION__));
    return EFI_OUT_OF_RESOURCES;
  }
  if (CompareGuid (&gManageabilityTransportKcsGuid, TransportToken->Transport->ManageabilityTransportSpecification)) {

    // This is KCS transport interface.
    KcsHardwareInfo->MemoryMap     = MANAGEABILITY_TRANSPORT_KCS_IO_MAP_IO;
    KcsHardwareInfo->IoBaseAddress.IoAddress16    = IPMI_KCS_BASE_ADDRESS;
    KcsHardwareInfo->IoDataInAddress.IoAddress16  = IPMI_KCS_REG_DATA_IN;
    KcsHardwareInfo->IoDataOutAddress.IoAddress16 = IPMI_KCS_REG_DATA_OUT;
    KcsHardwareInfo->IoCommandAddress.IoAddress16 = IPMI_KCS_REG_COMMAND;
    KcsHardwareInfo->IoStatusAddress.IoAddress16  = IPMI_KCS_REG_STATUS;
    *HardwareInformation =
       (MANAGEABILITY_TRANSPORT_HARDWARE_INFORMATION)KcsHardwareInfo;
    return EFI_SUCCESS;
  } else {
    DEBUG((DEBUG_ERROR, "%a: No implementation of setting hardware information.",__FUNCTION__));
    ASSERT(FALSE);
  }
  return EFI_UNSUPPORTED;
}

/**
  This functions setup the final header/body/trailer packets for
  the acquired transport interface.

  @param[in]         TransportToken  The transport interface.
  @param[in]         NetFunction     IPMI function.
  @param[in]         Command         IPMI command.
  @param[out]        PacketHeader    The poitner to receive header of request.
  @param[in, out]    PacketBody      The request body.
                                     When IN, it is the caller's request body.
                                     When OUT and NULL, the request body is not
                                     changed.
                                     When OUT and non-NULL, the request body is
                                     changed to comform the transport interface.
  @param[in, out]    PacketBodySize  The request body size.
                                     When IN and non-zero, it is the new data
                                     length of request body.
                                     When IN and zero, the request body is unchanged.
  @param[out]        PacketTrailer   The pointer to receive trailer of request.

  @retval EFI_SUCCESS            Request packet is returned.
  @retval EFI_UNSUPPORTED        Request packet is not returned because
                                 the unsupported transport interface.
**/
EFI_STATUS
SetupIpmiRequestTransportPacket (
  IN   MANAGEABILITY_TRANSPORT_TOKEN    *TransportToken,
  IN   UINT8                            NetFunction,
  IN   UINT8                            Command,
  OUT  MANAGEABILITY_TRANSPORT_HEADER   *PacketHeader,
  IN OUT UINT8                          **PacketBody,
  IN OUT UINT32                         *PacketBodySize,
  OUT  MANAGEABILITY_TRANSPORT_TRAILER  *PacketTrailer
  )
{
  MANAGEABILITY_IPMI_TRANSPORT_HEADER *IpmiHeader;

  if (CompareGuid (&gManageabilityTransportKcsGuid, TransportToken->Transport->ManageabilityTransportSpecification)) {

    // This is KCS transport interface.
    IpmiHeader = AllocateZeroPool (sizeof (MANAGEABILITY_IPMI_TRANSPORT_HEADER));
    if (IpmiHeader == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }
    IpmiHeader->Command = Command;
    IpmiHeader->Lun     = 0;
    IpmiHeader->NetFn   = NetFunction;
    *PacketHeader   = (MANAGEABILITY_TRANSPORT_HEADER *)IpmiHeader;
    *PacketTrailer  = NULL;
    *PacketBody     = NULL;
    *PacketBodySize = 0;
  } else {
    DEBUG((DEBUG_ERROR, "%a: No implementation of building up packet.",__FUNCTION__));
    ASSERT(FALSE);
  }
  return EFI_SUCCESS;
}

/**
  Common code to submit IPMI commands

  @param[in]         TransportToken    TRansport token.
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
CommonIpmiSubmitCommand (
  IN     MANAGEABILITY_TRANSPORT_TOKEN  *TransportToken,
  IN     UINT8                          NetFunction,
  IN     UINT8                          Command,
  IN     UINT8                          *RequestData,
  IN     UINT32                         RequestDataSize,
  OUT    UINT8                          *ResponseData,
  IN OUT UINT32                         *ResponseDataSize
  )
{
  EFI_STATUS                                 Status;
  UINT8                                      *ThisRequesData;
  UINT32                                     ThisRequesDataSize;
  MANAGEABILITY_TRANSFER_TOKEN               TransferToken;
  MANAGEABILITY_TRANSPORT_HEADER             IpmiTransportHeader;
  MANAGEABILITY_TRANSPORT_TRAILER            IpmiTransportTrailer;
  MANAGEABILITY_TRANSPORT_ADDITIONAL_STATUS  TransportAdditionalStatus;

  if (TransportToken == NULL) {
    DEBUG ((DEBUG_ERROR, "%a: No transport toke for IPMI\n", __FUNCTION__));
    return EFI_UNSUPPORTED;
  }
  Status = TransportToken->Transport->Fucntion.Version1_0->TransportStatus (
                                                             TransportToken,
                                                             &TransportAdditionalStatus
                                                             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Transport %s for IPMI has problem - (%r)\n", __FUNCTION__, mTransportName, Status));
    return Status;
  }

  ThisRequesData       = RequestData;
  ThisRequesDataSize   = RequestDataSize;
  IpmiTransportHeader  = NULL;
  IpmiTransportTrailer = NULL;
  Status = SetupIpmiRequestTransportPacket (
             TransportToken,
             NetFunction,
             Command,
             &IpmiTransportHeader,
             &ThisRequesData,
             &ThisRequesDataSize,
             &IpmiTransportTrailer
             );
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Fail to build packets - (%r)\n", __FUNCTION__, Status));
    return Status;
  }
  TransferToken.TransmitHeader  = IpmiTransportHeader;
  TransferToken.TransmitTrailer = IpmiTransportTrailer;
  ZeroMem (&TransferToken, sizeof (MANAGEABILITY_TRANSFER_TOKEN));

  // Transmit packet.
  if (ThisRequesData == NULL || ThisRequesDataSize == 0) {
    TransferToken.TransmitPackage.TransmitPayload    = RequestData;
    TransferToken.TransmitPackage.TransmitSizeInByte = RequestDataSize;
  } else {
    TransferToken.TransmitPackage.TransmitPayload    = ThisRequesData;
    TransferToken.TransmitPackage.TransmitSizeInByte = ThisRequesDataSize;
  }
  TransferToken.TransmitPackage.TransmitTimeoutInMillisecond = MANAGEABILITY_TRANSPORT_NO_TIMEOUT;

  // Receive packet.
  TransferToken.ReceivePackage.ReceiveBuffer                 = ResponseData;
  TransferToken.ReceivePackage.ReceiveSizeInByte             = *ResponseDataSize;
  TransferToken.ReceivePackage.TransmitTimeoutInMillisecond  = MANAGEABILITY_TRANSPORT_NO_TIMEOUT;
  TransportToken->Transport->Fucntion.Version1_0->TransportTransmitReceive (
                                                     TransportToken,
                                                     &TransferToken
                                                     );

  if (IpmiTransportHeader != NULL) {
    FreePool ((VOID *)IpmiTransportHeader);
  }
  if (IpmiTransportTrailer != NULL) {
    FreePool ((VOID *)IpmiTransportTrailer);
  }
  if (ThisRequesData != NULL) {
    FreePool ((VOID *)ThisRequesData);
  }

  // Return transfer status.
  //
  Status                    = TransferToken.TransferStatus;
  TransportAdditionalStatus = TransferToken.TransportAdditionalStatus;
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Failed to send IPMI command over %s\n", __FUNCTION__, mTransportName));
    return Status;
  }
  *ResponseDataSize = TransferToken.ReceivePackage.ReceiveSizeInByte;
  return Status;
}
