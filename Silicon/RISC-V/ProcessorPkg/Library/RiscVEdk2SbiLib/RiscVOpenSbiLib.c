/** @file
  Instance of the SBI ecall library.

  It allows calling an SBI function via an ecall from S-Mode.

  The legacy extensions are not included because they are not necessary.
  They would be:
  - SbiLegacySetTimer            -> Use SbiSetTimer
  - SbiLegacyConsolePutChar      -> No replacement - Use regular UEFI functions
  - SbiLegacyConsoleGetChar      -> No replacement - Use regular UEFI functions
  - SbiLegacyClearIpi            -> Write 0 to SSIP
  - SbiLegacySendIpi             -> Use SbiSendIpi
  - SbiLegacyRemoteFenceI        -> Use SbiRemoteFenceI
  - SbiLegacyRemoteSfenceVma     -> Use SbiRemoteSfenceVma
  - SbiLegacyRemoteSfenceVmaAsid -> Use SbiRemoteSfenceVmaAsid
  - SbiLegacyShutdown            -> Wait for new System Reset extension

  Copyright (c) 2020, Hewlett Packard Development LP. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <IndustryStandard/RiscVOpensbi.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/RiscVEdk2SbiLib.h>
#include <sbi/riscv_asm.h>
#include <sbi/sbi_hart.h>
#include <sbi/sbi_types.h>
#include <sbi/sbi_platform.h>
#include <sbi/sbi_init.h>

struct sbiret {
  long error;
  long value;
};

inline
EFIAPI
struct sbiret sbi_call(UINTN ext_id, UINTN func_id, UINTN arg0, UINTN arg1,
                           UINTN arg2, UINTN arg3, UINTN arg4, UINTN arg5)
__attribute__((always_inline));

inline
EFIAPI
struct sbiret sbi_call(UINTN ext_id, UINTN func_id, UINTN arg0, UINTN arg1,
                           UINTN arg2, UINTN arg3, UINTN arg4, UINTN arg5) {
    register uintptr_t a0 asm ("a0") = (uintptr_t)(arg0);
    register uintptr_t a1 asm ("a1") = (uintptr_t)(arg1);
    register uintptr_t a2 asm ("a2") = (uintptr_t)(arg2);
    register uintptr_t a3 asm ("a3") = (uintptr_t)(arg3);
    register uintptr_t a4 asm ("a4") = (uintptr_t)(arg4);
    register uintptr_t a5 asm ("a5") = (uintptr_t)(arg5);
    register uintptr_t a6 asm ("a6") = (uintptr_t)(func_id);
    register uintptr_t a7 asm ("a7") = (uintptr_t)(ext_id);
    asm volatile ("ecall" \
         : "+r" (a0) \
         : "r" (a1), "r" (a2), "r" (a3), "r" (a4), "r" (a5), "r" (a6), "r" (a7) \
         : "memory"); \
    struct sbiret ret = { a0, a1 };
    return ret;
}

#define sbi_call_0(ext_id, func_id) \
  sbi_call(ext_id, func_id, 0, 0, 0, 0, 0, 0)
#define sbi_call_1(ext_id, func_id, arg0) \
  sbi_call(ext_id, func_id, arg0, 0, 0, 0, 0, 0)
#define sbi_call_2(ext_id, func_id, arg0, arg1) \
  sbi_call(ext_id, func_id, arg0, arg1, 0, 0, 0, 0)
#define sbi_call_3(ext_id, func_id, arg0, arg1, arg2) \
  sbi_call(ext_id, func_id, arg0, arg1, arg2, 0, 0, 0)
#define sbi_call_4(ext_id, func_id, arg0, arg1, arg2, arg3) \
  sbi_call(ext_id, func_id, arg0, arg1, arg2, arg3, 0, 0)
#define sbi_call_5(ext_id, func_id, arg0, arg1, arg2, arg3, arg4) \
  sbi_call(ext_id, func_id, arg0, arg1, arg2, arg3, arg4, 0)
#define sbi_call_6(ext_id, func_id, arg0, arg1, arg2, arg3, arg4, arg5) \
  sbi_call(ext_id, func_id, arg0, arg1, arg2, arg3, arg4, arg5)

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

//
// OpenSBI PPI interface function for the base extension
//

/**
  Get the implemented SBI specification version

  The minor number of the SBI specification is encoded in the low 24 bits,
  with the major number encoded in the next 7 bits.  Bit 32 must be 0 and is
  reserved for future expansion.

  @param[out] SpecVersion          The Version of the SBI specification.
**/
VOID
EFIAPI
SbiGetSpecVersion (
  OUT UINTN                       *SpecVersion
  )
{
  struct sbiret ret = sbi_call_0(SBI_BASE_EXT, SBI_GET_SPEC_VERSION_FUNC);

  if (!ret.error) {
    *SpecVersion = (UINTN) ret.value;
  }

  //return TranslateError(ret.error);
}

/**
  Get the SBI implementation ID

  This ID is used to idenetify a specific SBI implementation in order to work
  around any quirks it might have.

  @param[out] ImplId               The ID of the SBI implementation.
**/
VOID
EFIAPI
SbiGetImplId (
  OUT UINTN                       *ImplId
  )
{
  struct sbiret ret = sbi_call_0(SBI_BASE_EXT, SBI_GET_IMPL_ID_FUNC);
  *ImplId = (UINTN) ret.value;
}

/**
  Get the SBI implementation version

  The version of this SBI implementation.
  The encoding of this number is determined by the specific SBI implementation.

  @param[out] ImplVersion          The version of the SBI implementation.
**/
VOID
EFIAPI
SbiGetImplVersion (
  OUT UINTN                       *ImplVersion
  )
{
  struct sbiret ret = sbi_call_0(SBI_BASE_EXT, SBI_GET_IMPL_VERSION_FUNC);
  *ImplVersion = (UINTN) ret.value;
}

/**
  Probe whether an SBI extension is available

  ProbeResult is set to 0 if the extension is not available or to an extension
  specified value if it is available.

  @param[in]  ExtensionId          The extension ID.
  @param[out] ProbeResult          The return value of the probe.
**/
VOID
EFIAPI
SbiProbeExtension (
  IN  INTN                         ExtensionId,
  OUT INTN                        *ProbeResult
  )
{
  struct sbiret ret = sbi_call_0(SBI_BASE_EXT, SBI_PROBE_EXTENSION_FUNC);
  *ProbeResult = (UINTN) ret.value;
}

/**
  Get the CPU's vendor ID

  Reads the mvendorid CSR.

  @param[out] MvendorId            The CPU's vendor ID.
**/
VOID
EFIAPI
SbiGetMvendorId (
  OUT UINTN                       *MvendorId
  )
{
  struct sbiret ret = sbi_call_0(SBI_BASE_EXT, SBI_GET_MVENDORID_FUNC);
  *MvendorId = (UINTN) ret.value;
}

/**
  Get the CPU's vendor ID

  Reads the mvendorid CSR.

  @param[out] MvendorId            The CPU's vendor ID.
**/
VOID
EFIAPI
SbiGetMarchId (
  OUT UINTN                       *MarchId
  )
{
  struct sbiret ret = sbi_call_0(SBI_BASE_EXT, SBI_GET_MARCHID_FUNC);
  *MarchId = (UINTN) ret.value;
}

/**
  Get the CPU's architecture ID

  Reads the marchid CSR.

  @param[out] MarchId              The CPU's architecture ID.
**/
VOID
EFIAPI
SbiGetMimpId (
  OUT UINTN                       *MimpId
  )
{
  struct sbiret ret = sbi_call_0(SBI_BASE_EXT, SBI_GET_MIMPID_FUNC);
  *MimpId = (UINTN) ret.value;
}

//
// SBI interface function for the hart state management extension
//

/**
  Politely ask the SBI to start a given hart.

  This call may return before the hart has actually started executing, if the
  SBI implementation can guarantee that the hart is actually going to start.

  Before the hart jumps to StartAddr, the hart MUST configure PMP if present
  and switch to S-mode.

  @param[in]  HartId               The id of the hart to start.
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
SbiHartStart (
  IN  UINTN                          HartId,
  IN  UINTN                          StartAddr,
  IN  UINTN                          Priv
  )
{
  struct sbiret ret = sbi_call_3(SBI_HSM_EXT, SBI_HART_START_FUNC, HartId, StartAddr, Priv);
  return TranslateError(ret.error);
}

/**
  Return execution of the calling hart to SBI.

  MUST be called in S-Mode with user interrupts disabled.
  This call is not expected to return, unless a failure occurs.

  @retval     EFI_SUCCESS          Never occurs. When successful, the call does not return.
  @retval     other                Failed to stop hard for an unknown reason.
**/
EFI_STATUS
EFIAPI
SbiHartStop (
  )
{
  struct sbiret Ret = sbi_call_0(SBI_HSM_EXT, SBI_HART_STOP_FUNC);
  return TranslateError(Ret.error);
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

  @param[out] HartStatus           The pointer in which the hart's status is
                                   stored.
  @retval EFI_SUCCESS              The operation succeeds.
  @retval EFI_INVALID_PARAMETER    A parameter is invalid.
**/
EFI_STATUS
EFIAPI
SbiHartGetStatus (
  IN  UINTN                          HartId,
  OUT UINTN                         *HartStatus
  )
{
  struct sbiret ret = sbi_call_1(SBI_HSM_EXT, SBI_HART_GET_STATUS_FUNC, HartId);

  if (!ret.error) {
    *HartStatus = (UINTN) ret.value;
  }

  return TranslateError(ret.error);
}

/**
  Clear pending timer interrupt bit and set timer for next event after StimeValue.

	To clear the timer without scheduling a timer event, set StimeValue to a
	practically infinite value or mask the timer interrupt by clearing sie.STIE.

  @param[in]  StimeValue           The time offset to the next scheduled timer interrupt.
**/
VOID
EFIAPI
SbiSetTimer (
  IN  UINT64                         StimeValue
  )
{
  sbi_call_1(SBI_TIME_EXT, 0, StimeValue);
}

EFI_STATUS
EFIAPI
SbiSendIpi (
  IN  UINTN                         *HartMask,
  IN  UINTN                          HartMaskBase
  )
{
  struct sbiret ret = sbi_call_2(SBI_IPI_EXT, 0, (UINTN) HartMask, HartMaskBase);
  return TranslateError(ret.error);
}

/**
  Instructs remote harts to execute a FENCE.I instruction.

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
SbiRemoteFenceI (
  IN  UINTN                         *HartMask,
  IN  UINTN                          HartMaskBase
  )
{
  struct sbiret ret = sbi_call_2(SBI_RFNC_EXT, 0, (UINTN) HartMask, HartMaskBase);
  return TranslateError(ret.error);
}

/**
  Instructs the remote harts to execute one or more SFENCE.VMA instructions.

  The SFENCE.VMA covers the range of virtual addresses between StartAaddr and Size.

  The remote fence function acts as a full tlb flush if * StartAddr and size
  are both 0 * size is equal to 2^XLEN-1

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
SbiRemoteSfenceVma (
  IN  UINTN                         *HartMask,
  IN  UINTN                          HartMaskBase,
  IN  UINTN                          StartAddr,
  IN  UINTN                          Size
  )
{
  struct sbiret ret = sbi_call_4(SBI_RFNC_EXT, 1, (UINTN) HartMask, HartMaskBase, StartAddr, Size);
  return TranslateError(ret.error);
}

/**
  Instructs the remote harts to execute one or more SFENCE.VMA instructions.

  The SFENCE.VMA covers the range of virtual addresses between StartAaddr and Size.
  Covers only the given ASID.

  The remote fence function acts as a full tlb flush if * StartAddr and size
  are both 0 * size is equal to 2^XLEN-1

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
SbiRemoteSfenceVmaAsid (
  IN  UINTN                         *HartMask,
  IN  UINTN                          HartMaskBase,
  IN  UINTN                          StartAddr,
  IN  UINTN                          Size,
  IN  UINTN                          Asid
  )
{
  struct sbiret ret = sbi_call_5(SBI_RFNC_EXT, 2, (UINTN) HartMask, HartMaskBase, StartAddr, Size, Asid);
  return TranslateError(ret.error);
}

/**
  Instructs the remote harts to execute one or more SFENCE.GVMA instructions.

  The SFENCE.GVMA covers the range of virtual addresses between StartAaddr and Size.
  Covers only the given VMID.
  This function call is only valid for harts implementing the hypervisor extension.

  The remote fence function acts as a full tlb flush if * StartAddr and size
  are both 0 * size is equal to 2^XLEN-1

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
SbiRemoteHFenceGvmaVmid (
  IN  UINTN                         *HartMask,
  IN  UINTN                          HartMaskBase,
  IN  UINTN                          StartAddr,
  IN  UINTN                          Size,
  IN  UINTN                          Vmid
  )
{
  struct sbiret ret = sbi_call_5(SBI_RFNC_EXT, 3, (UINTN) HartMask, HartMaskBase, StartAddr, Size, Vmid);
  return TranslateError(ret.error);
}

/**
  Instructs the remote harts to execute one or more SFENCE.GVMA instructions.

  The SFENCE.GVMA covers the range of virtual addresses between StartAaddr and Size.
  This function call is only valid for harts implementing the hypervisor extension.

  The remote fence function acts as a full tlb flush if * StartAddr and size
  are both 0 * size is equal to 2^XLEN-1

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
SbiRemoteHFenceGvma (
  IN  UINTN                         *HartMask,
  IN  UINTN                          HartMaskBase,
  IN  UINTN                          StartAddr,
  IN  UINTN                          Size
  )
{
  struct sbiret ret = sbi_call_4(SBI_RFNC_EXT, 4, (UINTN) HartMask, HartMaskBase, StartAddr, Size);
  return TranslateError(ret.error);
}

/**
  Instructs the remote harts to execute one or more SFENCE.VVMA instructions.

  The SFENCE.GVMA covers the range of virtual addresses between StartAaddr and Size.
  Covers only the given ASID.
  This function call is only valid for harts implementing the hypervisor extension.

  The remote fence function acts as a full tlb flush if * StartAddr and size
  are both 0 * size is equal to 2^XLEN-1

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
SbiRemoteHFenceVvmaAsid (
  IN  UINTN                         *HartMask,
  IN  UINTN                          HartMaskBase,
  IN  UINTN                          StartAddr,
  IN  UINTN                          Size,
  IN  UINTN                          Asid
  )
{
  struct sbiret ret = sbi_call_5(SBI_RFNC_EXT, 5, (UINTN) HartMask, HartMaskBase, StartAddr, Size, Asid);
  return TranslateError(ret.error);
}

/**
  Instructs the remote harts to execute one or more SFENCE.VVMA instructions.

  The SFENCE.GVMA covers the range of virtual addresses between StartAaddr and Size.
  This function call is only valid for harts implementing the hypervisor extension.

  The remote fence function acts as a full tlb flush if * StartAddr and size
  are both 0 * size is equal to 2^XLEN-1

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
SbiRemoteHFenceVvma (
  IN  UINTN                         *HartMask,
  IN  UINTN                          HartMaskBase,
  IN  UINTN                          StartAddr,
  IN  UINTN                          Size
  )
{
  struct sbiret ret = sbi_call_4(SBI_RFNC_EXT, 6, (UINTN) HartMask, HartMaskBase, StartAddr, Size);
  return TranslateError(ret.error);
}

//
// SBI interface function for the vendor extension
//

/**
  Call a function in a vendor defined SBI extension

  ASSERT() if the ExtensionId is not in the designated SBI Vendor Extension
  Space.

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
SbiVendorCall (
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
        ret = sbi_call_0(ExtensionId, FunctionId);
        break;
      case 1:
        ret = sbi_call_1(ExtensionId, FunctionId, VA_ARG(Args, UINTN));
        break;
      case 2:
        ret = sbi_call_2(ExtensionId, FunctionId, VA_ARG(Args, UINTN), VA_ARG(Args, UINTN));
        break;
      case 3:
        ret = sbi_call_3(ExtensionId, FunctionId, VA_ARG(Args, UINTN), VA_ARG(Args, UINTN), VA_ARG(Args, UINTN));
        break;
      case 4:
        ret = sbi_call_4(ExtensionId, FunctionId, VA_ARG(Args, UINTN), VA_ARG(Args, UINTN), VA_ARG(Args, UINTN), VA_ARG(Args, UINTN));
        break;
      case 5:
        ret = sbi_call_5(ExtensionId, FunctionId, VA_ARG(Args, UINTN), VA_ARG(Args, UINTN), VA_ARG(Args, UINTN), VA_ARG(Args, UINTN), VA_ARG(Args, UINTN));
        break;
      case 6:
        ret = sbi_call_6(ExtensionId, FunctionId, VA_ARG(Args, UINTN), VA_ARG(Args, UINTN), VA_ARG(Args, UINTN), VA_ARG(Args, UINTN), VA_ARG(Args, UINTN), VA_ARG(Args, UINTN));
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
// SBI Firmware extension
//

/**
  Get scratch space of the current hart.

  Please consider using the wrapper SbiGetFirmwareContext if you only need to
  access the firmware context.

  @param[out] ScratchSpace         The scratch space pointer.
  @retval EFI_SUCCESS              The operation succeeds.
**/
EFI_STATUS
EFIAPI
SbiGetMscratch (
  OUT struct sbi_scratch             **ScratchSpace
  )
{
  struct sbiret ret = sbi_call_0(SBI_FW_EXT, SBI_EXT_FW_MSCRATCH_FUNC);

  if (!ret.error) {
    *ScratchSpace = (struct sbi_scratch *) ret.value;
  }

  return EFI_SUCCESS;
}

/**
  Get scratch space of the given hart id.

  @param[in]  HartId               The hart id.
  @param[out] ScratchSpace         The scratch space pointer.
  @retval EFI_SUCCESS              The operation succeeds.
**/
EFI_STATUS
EFIAPI
SbiGetMscratchHartid (
  IN  UINTN                            HartId,
  OUT struct sbi_scratch             **ScratchSpace
  )
{
  struct sbiret ret = sbi_call_1(SBI_FW_EXT, SBI_EXT_FW_MSCRATCH_HARTID_FUNC, HartId);

  if (!ret.error) {
    *ScratchSpace = (struct sbi_scratch *) ret.value;
  }

  return EFI_SUCCESS;
}

/**
  Get firmware context of the calling hart.

  @param[out] FirmwareContext      The firmware context pointer.
  @retval EFI_SUCCESS              The operation succeeds.
**/
EFI_STATUS
EFIAPI
SbiGetFirmwareContext (
  OUT EFI_RISCV_OPENSBI_FIRMWARE_CONTEXT **FirmwareContext
  )
{
  struct sbi_scratch *ScratchSpace;
  struct sbi_platform *SbiPlatform;
  struct sbiret ret = sbi_call_0(SBI_FW_EXT, SBI_EXT_FW_MSCRATCH_FUNC);

  if (!ret.error) {
    ScratchSpace = (struct sbi_scratch *) ret.value;
    SbiPlatform = (struct sbi_platform *) sbi_platform_ptr(ScratchSpace);
    *FirmwareContext = (EFI_RISCV_OPENSBI_FIRMWARE_CONTEXT *) SbiPlatform->firmware_context;
  }

  return EFI_SUCCESS;
}

/**
  Set firmware context of the calling hart.

  @param[in] FirmwareContext       The firmware context pointer.
  @retval EFI_SUCCESS              The operation succeeds.
**/
EFI_STATUS
EFIAPI
SbiSetFirmwareContext (
  IN EFI_RISCV_OPENSBI_FIRMWARE_CONTEXT *FirmwareContext
  )
{
  struct sbi_scratch *ScratchSpace;
  struct sbi_platform *SbiPlatform;
  struct sbiret ret = sbi_call_0(SBI_FW_EXT, SBI_EXT_FW_MSCRATCH_FUNC);

  if (!ret.error) {
    ScratchSpace = (struct sbi_scratch *) ret.value;
    SbiPlatform = (struct sbi_platform *) sbi_platform_ptr(ScratchSpace);
    SbiPlatform->firmware_context = (long unsigned int) FirmwareContext;
  }

  return EFI_SUCCESS;
}
