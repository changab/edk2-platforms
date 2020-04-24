/** @file
The module to produce OpenSBI PPI.

Copyright (c) 2020, Hewlett Packard Development LP. All rights reserved.<BR>

SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <IndustryStandard/RiscV.h>
#include <IndustryStandard/RiscVOpensbi.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/PeiServicesLib.h>
#include <Ppi/RiscVOpenSbiPpi.h>
#include <sbi/riscv_asm.h>
#include <sbi/sbi_hart.h>
#include <sbi/sbi_types.h>
#include <sbi/sbi_platform.h>
#include <sbi/sbi_init.h>

// Silicon/RISC-V/ProcessorPkg/Include/Ppi/RiscVOpenSbiPpi.h

EFI_STATUS
EFIAPI
TranslateError(UINTN SbiError) {
  switch (SbiError) {
    case SBI_SUCCESS:
      return EFI_SUCCESS;
    case SBI_ERR_FAILED:
      return EFI_DEVICE_ERROR;
      break;
    case SBI_ERR_NOT_SUPPORTED:
      return EFI_UNSUPPORTED;
      break;
    case SBI_ERR_INVALID_PARAM:
      return EFI_INVALID_PARAMETER;
      break;
    case SBI_ERR_DENIED:
      return EFI_ACCESS_DENIED;
      break;
    case SBI_ERR_INVALID_ADDRESS:
      return EFI_LOAD_ERROR;
      break;
    case SBI_ERR_ALREADY_AVAILABLE:
      return EFI_ALREADY_STARTED;
      break;
  }

  // Reaches here only if SBI has defined a new error type
  ASSERT (FALSE);
  return EFI_UNSUPPORTED;
}

/**
  Get the implemented SBI specification version

  The minor number of the SBI specification is encoded in the low 24 bits,
  with the major number encoded in the next 7 bits.  Bit 32 must be 0 and is
  reserved for future expansion.

  @param[in]  PeiServices          The PEI Services pointer.
  @param[in]  This                 The PPI instance pointer.
  @param[out] SpecVersion          The Version of the SBI specification.
**/
VOID
EFIAPI
PeiOpenSbiGetSpecVersion (
  IN  EFI_PEI_SERVICES           **PeiServices,
  IN  PEI_RISCV_OPENSBI_BASE_PPI  *This,
  OUT UINTN                       *SpecVersion
  )
{
  struct sbiret ret = sbi_call_new_0(SBI_BASE_EXT, SBI_GET_SPEC_VERSION_FUNC);

  if (!ret.error) {
    *SpecVersion = (UINTN) ret.value;
  }

  //return TranslateError(ret.error);
}

/**
  Get the SBI implementation ID

  This ID is used to idenetify a specific SBI implementation in order to work
  around any quirks it might have.

  @param[in]  PeiServices          The PEI Services pointer.
  @param[in]  This                 The PPI instance pointer.
  @param[out] ImplId               The ID of the SBI implementation.
**/
VOID
EFIAPI
PeiOpenSbiGetImplId (
  IN  EFI_PEI_SERVICES           **PeiServices,
  IN  PEI_RISCV_OPENSBI_BASE_PPI  *This,
  OUT UINTN                       *ImplId
  )
{
  struct sbiret ret = sbi_call_new_0(SBI_BASE_EXT, SBI_GET_IMPL_ID_FUNC);
  *ImplId = (UINTN) ret.value;
}

/**
  Get the SBI implementation version

  The version of this SBI implementation.
  The encoding of this number is determined by the specific SBI implementation.

  @param[in]  PeiServices          The PEI Services pointer.
  @param[in]  This                 The PPI instance pointer.
  @param[out] ImplVersion          The version of the SBI implementation.
**/
VOID
EFIAPI
PeiOpenSbiGetImplVersion (
  IN  EFI_PEI_SERVICES           **PeiServices,
  IN  PEI_RISCV_OPENSBI_BASE_PPI  *This,
  OUT UINTN                       *ImplVersion
  )
{
  struct sbiret ret = sbi_call_new_0(SBI_BASE_EXT, SBI_GET_IMPL_VERSION_FUNC);
  *ImplVersion = (UINTN) ret.value;
}

/**
  Probe whether an SBI extension is available

  ProbeResult is set to 0 if the extension is not available or to an extension
  specified value if it is available.

  @param[in]  PeiServices          The PEI Services pointer.
  @param[in]  This                 The PPI instance pointer.
  @param[in]  ExtensionId          The extension ID.
  @param[out] ProbeResult          The return value of the probe.
**/
VOID
EFIAPI
PeiOpenSbiProbeExtension (
  IN  EFI_PEI_SERVICES           **PeiServices,
  IN  PEI_RISCV_OPENSBI_BASE_PPI  *This,
  IN  INTN                         ExtensionId,
  OUT INTN                        *ProbeResult
  )
{
  struct sbiret ret = sbi_call_new_0(SBI_BASE_EXT, SBI_PROBE_EXTENSION_FUNC);
  *ProbeResult = (UINTN) ret.value;
}

/**
  Get the CPU's vendor ID

  Reads the mvendorid CSR.

  @param[in]  PeiServices          The PEI Services pointer.
  @param[in]  This                 The PPI instance pointer.
  @param[out] MvendorId            The CPU's vendor ID.
**/
VOID
EFIAPI
PeiOpenSbiGetMvendorId (
  IN  EFI_PEI_SERVICES           **PeiServices,
  IN  PEI_RISCV_OPENSBI_BASE_PPI  *This,
  OUT UINTN                       *MvendorId
  )
{
  struct sbiret ret = sbi_call_new_0(SBI_BASE_EXT, SBI_GET_MVENDORID_FUNC);
  *MvendorId = (UINTN) ret.value;
}

/**
  Get the CPU's vendor ID

  Reads the mvendorid CSR.

  @param[in]  PeiServices          The PEI Services pointer.
  @param[in]  This                 The PPI instance pointer.
  @param[out] MvendorId            The CPU's vendor ID.
**/
VOID
EFIAPI
PeiOpenSbiGetMarchId (
  IN  EFI_PEI_SERVICES           **PeiServices,
  IN  PEI_RISCV_OPENSBI_BASE_PPI  *This,
  OUT UINTN                       *MarchId
  )
{
  struct sbiret ret = sbi_call_new_0(SBI_BASE_EXT, SBI_GET_MARCHID_FUNC);
  *MarchId = (UINTN) ret.value;
}

/**
  Get the CPU's architecture ID

  Reads the marchid CSR.

  @param[in]  PeiServices          The PEI Services pointer.
  @param[in]  This                 The PPI instance pointer.
  @param[out] MarchId              The CPU's architecture ID.
**/
VOID
EFIAPI
PeiOpenSbiGetMimpId (
  IN  EFI_PEI_SERVICES           **PeiServices,
  IN  PEI_RISCV_OPENSBI_BASE_PPI  *This,
  OUT UINTN                       *MimpId
  )
{
  struct sbiret ret = sbi_call_new_0(SBI_BASE_EXT, SBI_GET_MIMPID_FUNC);
  *MimpId = (UINTN) ret.value;
}

//
// OpenSBI PPI interface function for the base extension
//
PEI_RISCV_OPENSBI_BASE_PPI mOpenSbiBasePpi = {
  0x02,  // TODO: Should be SBI spec version
  PeiOpenSbiGetSpecVersion,
  PeiOpenSbiGetImplId,
  PeiOpenSbiGetImplVersion,
  PeiOpenSbiProbeExtension,
  PeiOpenSbiGetMvendorId,
  PeiOpenSbiGetMarchId,
  PeiOpenSbiGetMimpId
};

/**
  Clear pending timer interrupt bit and set timer for next event after StimeValue.

  To clear the timer without scheduling a timer event, set StimeValue to a practically infinite value.

  @param[in]  PeiServices          The PEI Services pointer.
  @param[in]  This                 The PPI instance pointer.
  @param[in]  StimeValue           The time offset to the next scheduled timer interrupt.
**/
VOID
EFIAPI
PeiOpenSbiLegacySetTimer (
  IN EFI_PEI_SERVICES             **PeiServices,
  IN PEI_RISCV_OPENSBI_LEGACY_PPI  *This,
  IN UINT64                         StimeValue
  )
{
  // TODO
  // sbi_call_new_1(SBI_SET_TIMER_EXT, 0, StimeValue);
}

/**
  Write a byte to the system's console.

  Will block if characters are pending to be transmitted or the receiving
  terminal is not ready to receive a byte. If the console, does not exist,
  the byte is discarded.

  @param[in]  PeiServices          The PEI Services pointer.
  @param[in]  This                 The PPI instance pointer.
  @param[in]  ConsoleChar          The time offset to the next scheduled timer interrupt.
**/
VOID
EFIAPI
PeiOpenSbiLegacyConsolePutChar (
  IN EFI_PEI_SERVICES             **PeiServices,
  IN PEI_RISCV_OPENSBI_LEGACY_PPI  *This,
  IN INTN                           ConsoleChar
  )
{
  // TODO
  // sbi_call_new_1(SBI_CONSOLE_PUTCHAR_EXT, 0, ConsoleChar);
}

/**
  Read a byte from the system's console.

  @param[in]  PeiServices          The PEI Services pointer.
  @param[in]  This                 The PPI instance pointer.
  @param[in]  ConsoleChar          The pointer in which the read byte is written.
**/
VOID
EFIAPI
PeiOpenSbiLegacyConsoleGetChar (
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  PEI_RISCV_OPENSBI_LEGACY_PPI  *This,
  OUT INTN                          *ConsoleChar
  )
{
  // TODO
}

/**
  Clear pending IPIs for the calling hart.

  @param[in]  PeiServices          The PEI Services pointer.
  @param[in]  This                 The PPI instance pointer.
**/
VOID
EFIAPI
PeiOpenSbiLegacyClearIpi (
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  PEI_RISCV_OPENSBI_LEGACY_PPI  *This
  )
{
  // TODO
}

/**
  Send IPI to all harts specified in the mask.

  @param[in]  PeiServices          The PEI Services pointer.
  @param[in]  This                 The PPI instance pointer.
  @param[in]  HartMask             Virtual address pointing to the hart mask,
                                   which is an array of `unsigned long`s.
**/
VOID
EFIAPI
PeiOpenSbiLegacySendIpi (
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  PEI_RISCV_OPENSBI_LEGACY_PPI  *This,
  IN  CONST UINTN                   *HartMask
  )
{
  // TODO
}

/**
  Instructs remote harts to execute a FENCE.I instruction. 

  @param[in]  PeiServices          The PEI Services pointer.
  @param[in]  This                 The PPI instance pointer.
  @param[in]  HartMask             Virtual address pointing to the hart mask,
                                   which is an array of `unsigned long`s.
**/
VOID
EFIAPI
PeiOpenSbiLegacyRemoteFenceI (
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  PEI_RISCV_OPENSBI_LEGACY_PPI  *This,
  IN  CONST UINTN                   *HartMask
  )
{
  // TODO
}

/**
  Instructs remote harts to execute one or more SFENCE.VMA instructions over a range.

  @param[in]  PeiServices          The PEI Services pointer.
  @param[in]  This                 The PPI instance pointer.
  @param[in]  HartMask             Virtual address pointing to the hart mask,
                                   which is an array of `unsigned long`s.
  @param[in]  Start                The address of where the range begins.
  @param[in]  Size                 The total size of the range.
**/
VOID
EFIAPI
PeiOpenSbiLegacyRemoteSfenceVma (
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  PEI_RISCV_OPENSBI_LEGACY_PPI  *This,
  IN  CONST UINTN                   *HartMask,
  IN  UINTN                          Start,
  IN  UINTN                          Size
  )
{
  // TODO
}

/**
  Instructs remote harts to execute one or more SFENCE.VMA instructions over a range, limited to an ASID.

  @param[in]  PeiServices          The PEI Services pointer.
  @param[in]  This                 The PPI instance pointer.
  @param[in]  HartMask             Virtual address pointing to the hart mask,
                                   which is an array of `unsigned long`s.
  @param[in]  Start                The address of where the range begins.
  @param[in]  Size                 The total size of the range.
  @param[in]  Asid                 The ASID to limit the instruction to.
**/
VOID
EFIAPI
PeiOpenSbiLegacyRemoteSfenceVmaAsid (
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  PEI_RISCV_OPENSBI_LEGACY_PPI  *This,
  IN  CONST UINTN                   *HartMask,
  IN  UINTN                          Start,
  IN  UINTN                          Size,
  IN  UINTN                          Asid
  )
{
  // TODO
}

/**
  Shuts all harts down.

  Does not return.

  @param[in]  PeiServices          The PEI Services pointer.
  @param[in]  This                 The PPI instance pointer.
**/
VOID
EFIAPI
PeiOpenSbiLegacyShutdown (
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  PEI_RISCV_OPENSBI_LEGACY_PPI  *This
  )
{
  // TODO
}

//
// OpenSBI PPI interface function for the legacy extension
//
PEI_RISCV_OPENSBI_LEGACY_PPI mOpenSbiLegacyPpi = {
  0x02,  // TODO: Should be SBI spec version
  PeiOpenSbiLegacySetTimer,
  PeiOpenSbiLegacyConsolePutChar,
  PeiOpenSbiLegacyConsoleGetChar,
  PeiOpenSbiLegacyClearIpi,
  PeiOpenSbiLegacySendIpi,
  PeiOpenSbiLegacyRemoteFenceI,
  PeiOpenSbiLegacyRemoteSfenceVma,
  PeiOpenSbiLegacyRemoteSfenceVmaAsid,
  PeiOpenSbiLegacyShutdown
};

/**
  Politely ask the SBI to start a given hart.

  This call may return before the hart has actually started executing, if the
  SBI implementation can guarantee that the hart is actually going to start.

  Before the hart jumps to StartAddr, the hart MUST configure PMP if present
  and switch to S-mode.

  @param[in]  PeiServices          The PEI Services pointer.
  @param[in]  This                 The PPI instance pointer.
	@param[in]  HartId               
  @param[in]  StartAddr            The physical address, where the hart starts
                                   executing from.
  @param[in]  Priv                 An XLEN-bit value, which will be in register
                                   a1 when the hart starts.
  @retval EFI_SUCCESS              Hart was stopped and will start executing from StartAddr.
  @retval EFI_LOAD_ERROR           StartAddr is not valid, possibly due to following reasons:
                                   - It is not a valid physical address.
																	 - The address is prohibited by PMP to run in
																	   supervisor mode.
	@retval EFI_INVALID_PARAMETER    HartId is not a valid hart id
	@retval EFI_ALREADY_STARTED      The hart is already running.
	@retval other                    The start request failed for unknown reasons.
**/
EFI_STATUS
EFIAPI
PeiOpenSbiHartStart (
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  PEI_RISCV_OPENSBI_HSM_PPI     *This,
  IN  UINTN                          HartId,
  IN  UINTN                          StartAddr,
  IN  UINTN                          Priv
  )
{
  struct sbiret ret = sbi_call_new_3(SBI_HSM_EXT, SBI_HART_START_FUNC, HartId, StartAddr, Priv);
  return TranslateError(ret.error);
}

/**
  Return execution of the calling hart to SBI.

  MUST be called in S-Mode with user interrupts disabled.
  This call is not expected to return, unless a failure occurs.

  @param[in]  PeiServices          The PEI Services pointer.
  @param[in]  This                 The PPI instance pointer.
  @retval     EFI_SUCCESS          Never occurs. When successful, the call does not return.
  @retval     other                Failed to stop hard for an unknown reason.
**/
EFI_STATUS
EFIAPI
PeiOpenSbiHartStop (
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  PEI_RISCV_OPENSBI_HSM_PPI     *This
  )
{
  struct sbiret ret = sbi_call_new_0(SBI_HSM_EXT, SBI_HART_STOP_FUNC);
  return TranslateError(ret.error);
}

/**
  Get the current status of a hart.

  Since harts can transition between states at any time, the status retrieved
  by this function may already be out of date, once it returns.

  Possible values for HartStatus are:
  0: STARTED
	1: STOPPED
  2: START_REQUEST_PENDING
  3: STOP_REQUEST_PENDING

  @param[in]  PeiServices          The PEI Services pointer.
  @param[in]  This                 The PPI instance pointer.
  @param[out] HartStatus           The pointer in which the hart's status is
                                   stored.
  @retval EFI_SUCCESS              The operation succeeds.
  @retval EFI_INVALID_PARAMETER    A parameter is invalid.
**/
EFI_STATUS
EFIAPI
PeiOpenSbiHartGetStatus (
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  PEI_RISCV_OPENSBI_HSM_PPI     *This,
  IN  UINTN                          HartId,
  OUT UINTN                         *HartStatus
  )
{
  struct sbiret ret = sbi_call_new_1(SBI_HSM_EXT, SBI_HART_GET_STATUS_FUNC, HartId);

  if (!ret.error) {
    *HartStatus = (UINTN) ret.value;
  }

  return TranslateError(ret.error);
}

//
// OpenSBI PPI interface function for the hart state management extension
//
PEI_RISCV_OPENSBI_HSM_PPI mOpenSbiHsmPpi = {
  0x02,  // TODO: Should be SBI spec version
  PeiOpenSbiHartStart,
  PeiOpenSbiHartStop,
  PeiOpenSbiHartGetStatus
};

/**
  Clear pending timer interrupt bit and set timer for next event after StimeValue.

	To clear the timer without scheduling a timer event, set StimeValue to a
	practically infinite value or mask the timer interrupt by clearing sie.STIE.

  @param[in]  PeiServices          The PEI Services pointer.
  @param[in]  This                 The PPI instance pointer.
  @param[in]  StimeValue           The time offset to the next scheduled timer interrupt.
**/
VOID
EFIAPI
PeiOpenSbiSetTimer (
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  PEI_RISCV_OPENSBI_TIME_PPI    *This,
  IN  UINT64                         StimeValue
  )
{
  sbi_call_new_1(SBI_TIME_EXT, 0, StimeValue);
}

PEI_RISCV_OPENSBI_TIME_PPI mOpenSbiTimePpi = {
  0x02,
  PeiOpenSbiSetTimer
};

EFI_STATUS
EFIAPI
PeiOpenSbiSendIpi (
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  PEI_RISCV_OPENSBI_IPI_PPI     *This,
  IN  UINTN                         *HartMask,
  IN  UINTN                          HartMaskBase
  )
{
  struct sbiret ret = sbi_call_new_2(SBI_IPI_EXT, 0, (UINTN) HartMask, HartMaskBase);
  return TranslateError(ret.error);
}

PEI_RISCV_OPENSBI_IPI_PPI mOpenSbiIpiPpi = {
  0x02,
  PeiOpenSbiSendIpi
};

/**
  Instructs remote harts to execute a FENCE.I instruction. 

  @param[in]  PeiServices          The PEI Services pointer.
  @param[in]  This                 The PPI instance pointer.
  @param[in]  HartMask             Scalar bit-vector containing hart ids
  @param[in]  HartMaskBase         The starting hartid from which the bit-vector
                                   must be computed. If set to -1, HartMask is
                                   ignored and all harts are considered.
  @retval EFI_SUCCESS              IPI was sent to all the targeted harts.
  @retval EFI_INVALID_PARAMETER    Either hart_mask_base or any of the hartid
                                   from hart_mask is not valid i.e. either the
                                   hartid is not enabled by the platform or is
                                   not available to the supervisor.
**/
EFI_STATUS
EFIAPI
PeiOpenSbiRemoteFenceI (
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  PEI_RISCV_OPENSBI_RFNC_PPI    *This,
  IN  UINTN                         *HartMask,
  IN  UINTN                          HartMaskBase
  )
{
  struct sbiret ret = sbi_call_new_2(SBI_RFNC_EXT, 0, (UINTN) HartMask, HartMaskBase);
  return TranslateError(ret.error);
}

/**
  Instructs the remote harts to execute one or more SFENCE.VMA instructions.

  The SFENCE.VMA covers the range of virtual addresses between StartAaddr and Size.

  The remote fence function acts as a full tlb flush if * StartAddr and size
  are both 0 * size is equal to 2^XLEN-1

  @param[in]  PeiServices          The PEI Services pointer.
  @param[in]  This                 The PPI instance pointer.
  @param[in]  HartMask             Scalar bit-vector containing hart ids
  @param[in]  HartMaskBase         The starting hartid from which the bit-vector
                                   must be computed. If set to -1, HartMask is
                                   ignored and all harts are considered.
  @param[in]  StartAddr            The first address of the affected range.
  @param[in]  Size                 How many addresses are affected.
  @retval EFI_SUCCESS              IPI was sent to all the targeted harts.
  @retval EFI_LOAD_ERROR           StartAddr or Size is not valid.
  @retval EFI_INVALID_PARAMETER    Either hart_mask_base or any of the hartid
                                   from hart_mask is not valid i.e. either the
                                   hartid is not enabled by the platform or is
                                   not available to the supervisor.
**/
EFI_STATUS
EFIAPI
PeiOpenSbiRemoteSfenceVma (
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  PEI_RISCV_OPENSBI_RFNC_PPI    *This,
  IN  UINTN                         *HartMask,
  IN  UINTN                          HartMaskBase,
  IN  UINTN                          StartAddr,
  IN  UINTN                          Size
  )
{
  struct sbiret ret = sbi_call_new_4(SBI_RFNC_EXT, 1, (UINTN) HartMask, HartMaskBase, StartAddr, Size);
  return TranslateError(ret.error);
}

/**
  Instructs the remote harts to execute one or more SFENCE.VMA instructions.

  The SFENCE.VMA covers the range of virtual addresses between StartAaddr and Size.
  Covers only the given ASID.

  The remote fence function acts as a full tlb flush if * StartAddr and size
  are both 0 * size is equal to 2^XLEN-1

  @param[in]  PeiServices          The PEI Services pointer.
  @param[in]  This                 The PPI instance pointer.
  @param[in]  HartMask             Scalar bit-vector containing hart ids
  @param[in]  HartMaskBase         The starting hartid from which the bit-vector
                                   must be computed. If set to -1, HartMask is
                                   ignored and all harts are considered.
  @param[in]  StartAddr            The first address of the affected range.
  @param[in]  Size                 How many addresses are affected.
  @retval EFI_SUCCESS              IPI was sent to all the targeted harts.
  @retval EFI_LOAD_ERROR           StartAddr or Size is not valid.
  @retval EFI_INVALID_PARAMETER    Either hart_mask_base or any of the hartid
                                   from hart_mask is not valid i.e. either the
                                   hartid is not enabled by the platform or is
                                   not available to the supervisor.
**/
EFI_STATUS
EFIAPI
PeiOpenSbiRemoteSfenceVmaAsid (
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  PEI_RISCV_OPENSBI_RFNC_PPI    *This,
  IN  UINTN                         *HartMask,
  IN  UINTN                          HartMaskBase,
  IN  UINTN                          StartAddr,
  IN  UINTN                          Size,
  IN  UINTN                          Asid
  )
{
  struct sbiret ret = sbi_call_new_5(SBI_RFNC_EXT, 2, (UINTN) HartMask, HartMaskBase, StartAddr, Size, Asid);
  return TranslateError(ret.error);
}

/**
  Instructs the remote harts to execute one or more SFENCE.GVMA instructions.

  The SFENCE.GVMA covers the range of virtual addresses between StartAaddr and Size.
  Covers only the given VMID.
  This function call is only valid for harts implementing the hypervisor extension.

  The remote fence function acts as a full tlb flush if * StartAddr and size
  are both 0 * size is equal to 2^XLEN-1

  @param[in]  PeiServices          The PEI Services pointer.
  @param[in]  This                 The PPI instance pointer.
  @param[in]  HartMask             Scalar bit-vector containing hart ids
  @param[in]  HartMaskBase         The starting hartid from which the bit-vector
                                   must be computed. If set to -1, HartMask is
                                   ignored and all harts are considered.
  @param[in]  StartAddr            The first address of the affected range.
  @param[in]  Size                 How many addresses are affected.
  @retval EFI_SUCCESS              IPI was sent to all the targeted harts.
  @retval EFI_LOAD_ERROR           StartAddr or Size is not valid.
  @retval EFI_UNSUPPORTED          SBI does not implement this function or one
                                   of the target harts does not support the
                                   hypervisor extension.
  @retval EFI_INVALID_PARAMETER    Either hart_mask_base or any of the hartid
                                   from hart_mask is not valid i.e. either the
                                   hartid is not enabled by the platform or is
                                   not available to the supervisor.
**/
EFI_STATUS
EFIAPI
PeiOpenSbiRemoteHFenceGvmaVmid (
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  PEI_RISCV_OPENSBI_RFNC_PPI    *This,
  IN  UINTN                         *HartMask,
  IN  UINTN                          HartMaskBase,
  IN  UINTN                          StartAddr,
  IN  UINTN                          Size,
  IN  UINTN                          Vmid
  )
{
  struct sbiret ret = sbi_call_new_5(SBI_RFNC_EXT, 3, (UINTN) HartMask, HartMaskBase, StartAddr, Size, Vmid);
  return TranslateError(ret.error);
}

/**
  Instructs the remote harts to execute one or more SFENCE.GVMA instructions.

  The SFENCE.GVMA covers the range of virtual addresses between StartAaddr and Size.
  This function call is only valid for harts implementing the hypervisor extension.

  The remote fence function acts as a full tlb flush if * StartAddr and size
  are both 0 * size is equal to 2^XLEN-1

  @param[in]  PeiServices          The PEI Services pointer.
  @param[in]  This                 The PPI instance pointer.
  @param[in]  HartMask             Scalar bit-vector containing hart ids
  @param[in]  HartMaskBase         The starting hartid from which the bit-vector
                                   must be computed. If set to -1, HartMask is
                                   ignored and all harts are considered.
  @param[in]  StartAddr            The first address of the affected range.
  @param[in]  Size                 How many addresses are affected.
  @retval EFI_SUCCESS              IPI was sent to all the targeted harts.
  @retval EFI_LOAD_ERROR           StartAddr or Size is not valid.
  @retval EFI_UNSUPPORTED          SBI does not implement this function or one
                                   of the target harts does not support the
                                   hypervisor extension.
  @retval EFI_INVALID_PARAMETER    Either hart_mask_base or any of the hartid
                                   from hart_mask is not valid i.e. either the
                                   hartid is not enabled by the platform or is
                                   not available to the supervisor.
**/
EFI_STATUS
EFIAPI
PeiOpenSbiRemoteHFenceGvma (
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  PEI_RISCV_OPENSBI_RFNC_PPI    *This,
  IN  UINTN                         *HartMask,
  IN  UINTN                          HartMaskBase,
  IN  UINTN                          StartAddr,
  IN  UINTN                          Size
  )
{
  struct sbiret ret = sbi_call_new_4(SBI_RFNC_EXT, 4, (UINTN) HartMask, HartMaskBase, StartAddr, Size);
  return TranslateError(ret.error);
}

/**
  Instructs the remote harts to execute one or more SFENCE.VVMA instructions.

  The SFENCE.GVMA covers the range of virtual addresses between StartAaddr and Size.
  Covers only the given ASID.
  This function call is only valid for harts implementing the hypervisor extension.

  The remote fence function acts as a full tlb flush if * StartAddr and size
  are both 0 * size is equal to 2^XLEN-1

  @param[in]  PeiServices          The PEI Services pointer.
  @param[in]  This                 The PPI instance pointer.
  @param[in]  HartMask             Scalar bit-vector containing hart ids
  @param[in]  HartMaskBase         The starting hartid from which the bit-vector
                                   must be computed. If set to -1, HartMask is
                                   ignored and all harts are considered.
  @param[in]  StartAddr            The first address of the affected range.
  @param[in]  Size                 How many addresses are affected.
  @retval EFI_SUCCESS              IPI was sent to all the targeted harts.
  @retval EFI_LOAD_ERROR           StartAddr or Size is not valid.
  @retval EFI_UNSUPPORTED          SBI does not implement this function or one
                                   of the target harts does not support the
                                   hypervisor extension.
  @retval EFI_INVALID_PARAMETER    Either hart_mask_base or any of the hartid
                                   from hart_mask is not valid i.e. either the
                                   hartid is not enabled by the platform or is
                                   not available to the supervisor.
**/
EFI_STATUS
EFIAPI
PeiOpenSbiRemoteHFenceVvmaAsid (
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  PEI_RISCV_OPENSBI_RFNC_PPI    *This,
  IN  UINTN                         *HartMask,
  IN  UINTN                          HartMaskBase,
  IN  UINTN                          StartAddr,
  IN  UINTN                          Size,
  IN  UINTN                          Asid
  )
{
  struct sbiret ret = sbi_call_new_5(SBI_RFNC_EXT, 5, (UINTN) HartMask, HartMaskBase, StartAddr, Size, Asid);
  return TranslateError(ret.error);
}

/**
  Instructs the remote harts to execute one or more SFENCE.VVMA instructions.

  The SFENCE.GVMA covers the range of virtual addresses between StartAaddr and Size.
  This function call is only valid for harts implementing the hypervisor extension.

  The remote fence function acts as a full tlb flush if * StartAddr and size
  are both 0 * size is equal to 2^XLEN-1

  @param[in]  PeiServices          The PEI Services pointer.
  @param[in]  This                 The PPI instance pointer.
  @param[in]  HartMask             Scalar bit-vector containing hart ids
  @param[in]  HartMaskBase         The starting hartid from which the bit-vector
                                   must be computed. If set to -1, HartMask is
                                   ignored and all harts are considered.
  @param[in]  StartAddr            The first address of the affected range.
  @param[in]  Size                 How many addresses are affected.
  @retval EFI_SUCCESS              IPI was sent to all the targeted harts.
  @retval EFI_LOAD_ERROR           StartAddr or Size is not valid.
  @retval EFI_UNSUPPORTED          SBI does not implement this function or one
                                   of the target harts does not support the
                                   hypervisor extension.
  @retval EFI_INVALID_PARAMETER    Either hart_mask_base or any of the hartid
                                   from hart_mask is not valid i.e. either the
                                   hartid is not enabled by the platform or is
                                   not available to the supervisor.
**/
EFI_STATUS
EFIAPI
PeiOpenSbiRemoteHFenceVvma (
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  PEI_RISCV_OPENSBI_RFNC_PPI    *This,
  IN  UINTN                         *HartMask,
  IN  UINTN                          HartMaskBase,
  IN  UINTN                          StartAddr,
  IN  UINTN                          Size
  )
{
  struct sbiret ret = sbi_call_new_4(SBI_RFNC_EXT, 6, (UINTN) HartMask, HartMaskBase, StartAddr, Size);
  return TranslateError(ret.error);
}

PEI_RISCV_OPENSBI_RFNC_PPI mOpenSbiRfncPpi = {
  0x02,
  PeiOpenSbiRemoteFenceI,
  PeiOpenSbiRemoteSfenceVma,
  PeiOpenSbiRemoteSfenceVmaAsid,
  PeiOpenSbiRemoteHFenceGvmaVmid,
  PeiOpenSbiRemoteHFenceGvma,
  PeiOpenSbiRemoteHFenceVvmaAsid,
  PeiOpenSbiRemoteHFenceVvma
};

/**
  Call a function in a vendor defined SBI extension

  ASSERT() if the ExtensionId is not in the designated SBI Vendor Extension
  Space.

  @param[in]  PeiServices          The PEI Services pointer.
  @param[in]  This                 The PPI instance pointer.
  @param[in]  ExtensionId          The SBI vendor extension ID.
  @param[in]  FunctionId           The function ID to call in this extension.
  @param[in]  NumArgs              How many arguments are passed.
  @param[in]  ...                  Actual Arguments to the function.
  @retval EFI_SUCCESS if the SBI function was called and it was successful
  @retval EFI_INVALID_PARAMETER if NumArgs exceeds 6
  @retval others if the called SBI function returns an error
**/
EFI_STATUS
EFIAPI
PeiOpenSbiVendorCall (
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  PEI_RISCV_OPENSBI_VENDOR_PPI  *This,
  IN  UINTN                          ExtensionId,
  IN  UINTN                          FunctionId,
  IN  UINTN                          NumArgs,
  ...
  )
{
    struct sbiret ret;
    VA_LIST Args;
    VA_START(Args, NumArgs);

    ASSERT (ExtensionId >= 0x09000000 && ExtensionId <= 0x09FFFFFF);

    switch (NumArgs) {
      case 0:
        ret = sbi_call_new_0(ExtensionId, FunctionId);
        break;
      case 1:
        ret = sbi_call_new_1(ExtensionId, FunctionId, VA_ARG(Args, UINTN));
        break;
      case 2:
        ret = sbi_call_new_2(ExtensionId, FunctionId, VA_ARG(Args, UINTN), VA_ARG(Args, UINTN));
        break;
      case 3:
        ret = sbi_call_new_3(ExtensionId, FunctionId, VA_ARG(Args, UINTN), VA_ARG(Args, UINTN), VA_ARG(Args, UINTN));
        break;
      case 4:
        ret = sbi_call_new_4(ExtensionId, FunctionId, VA_ARG(Args, UINTN), VA_ARG(Args, UINTN), VA_ARG(Args, UINTN), VA_ARG(Args, UINTN));
        break;
      case 5:
        ret = sbi_call_new_5(ExtensionId, FunctionId, VA_ARG(Args, UINTN), VA_ARG(Args, UINTN), VA_ARG(Args, UINTN), VA_ARG(Args, UINTN), VA_ARG(Args, UINTN));
        break;
      case 6:
        ret = sbi_call_new_6(ExtensionId, FunctionId, VA_ARG(Args, UINTN), VA_ARG(Args, UINTN), VA_ARG(Args, UINTN), VA_ARG(Args, UINTN), VA_ARG(Args, UINTN), VA_ARG(Args, UINTN));
        break;
      default:
        // Too many args. In theory SBI can handle more arguments when they are
        // passed on the stack but no SBI extension uses this, therefore it's
        // not yet implemented here.
        return EFI_INVALID_PARAMETER;
     }

    VA_END(Args);
    return TranslateError(ret.error);
}

//
// OpenSBI PPI interface function for the vendor extension
//
PEI_RISCV_OPENSBI_VENDOR_PPI mOpenSbiVendorPpi = {
  0x00,
  PeiOpenSbiVendorCall
};

//
// OpenSBI PPI interface function for the OpenSBI library itself
// Can only be called from M-Mode!
//
PEI_RISCV_OPENSBI_LIBRARY_PPI mOpenSbiLibraryPpi = {
  // TODO: Actually I don't think we need this PPI
  0x00
};

/**
  Get scratch space of the current hart.

  @param[in]  PeiServices          The PEI Services pointer.
  @param[in]  This                 The PPI instance pointer.
  @param[out] ScratchSpace         The scratch space pointer.
  @retval EFI_SUCCESS              The operation succeeds.
**/
EFI_STATUS
EFIAPI
PeiOpenSbiGetMscratch (
  IN  EFI_PEI_SERVICES               **PeiServices,
  IN  PEI_RISCV_OPENSBI_FIRMWARE_PPI  *This,
  OUT struct sbi_scratch             **ScratchSpace
  )
{
  struct sbiret ret = sbi_call_new_0(SBI_FW_EXT, 0x0);

  if (!ret.error) {
    *ScratchSpace = (struct sbi_scratch *) ret.value;
  }

  return EFI_SUCCESS;
}

/**
  Get scratch space of the given hart id.

  @param[in]  PeiServices          The PEI Services pointer.
  @param[in]  This                 The PPI instance pointer.
  @param[in]  HartId               The hart id.
  @param[out] ScratchSpace         The scratch space pointer.
  @retval EFI_SUCCESS              The operation succeeds.
**/
EFI_STATUS
EFIAPI
PeiOpenSbiGetMscratchHartid (
  IN  EFI_PEI_SERVICES               **PeiServices,
  IN  PEI_RISCV_OPENSBI_FIRMWARE_PPI  *This,
  IN  UINTN                            HartId,
  OUT struct sbi_scratch             **ScratchSpace
  )
{
  struct sbiret ret = sbi_call_new_1(SBI_FW_EXT, 0x1, HartId);

  if (!ret.error) {
    *ScratchSpace = (struct sbi_scratch *) ret.value;
  }

  return EFI_SUCCESS;
}

PEI_RISCV_OPENSBI_FIRMWARE_PPI mOpenSbiFirmwarePpi = {
  0x00,
  PeiOpenSbiGetMscratch,
  PeiOpenSbiGetMscratchHartid
};
