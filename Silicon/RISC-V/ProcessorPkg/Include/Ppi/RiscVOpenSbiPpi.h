/** @file Defines the PPIs to let PEIMs call SBI

Copyright (c) 2020, Hewlett Packard Development LP. All rights reserved.<BR>

SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _PEI_RISCV_OPENSBI_PPI_H_
#define _PEI_RISCV_OPENSBI_PPI_H_

#include <sbi/sbi_scratch.h>
#include <sbi/sbi_platform.h>

///
/// Global ID for the PEI_RISCV_OPENSBI_BASE_PPI_GUID.
///
#define PEI_RISCV_OPENSBI_BASE_PPI_GUID  \
  { \
    0x9af5b3c3, 0x1e8c, 0x4219, { 0xa4, 0xfb, 0x3f, 0xb8, 0xb2, 0xb8, 0xfe, 0xa1 } \
  }

///
/// Forward declaration for PEI_RISCV_OPENSBI_BASE_PPI.
///
typedef struct _PEI_RISCV_OPENSBI_BASE_PPI PEI_RISCV_OPENSBI_BASE_PPI;

/**
  Get the implemented SBI specification version

  The minor number of the SBI specification is encoded in the low 24 bits,
  with the major number encoded in the next 7 bits.  Bit 32 must be 0 and is
  reserved for future expansion.

  @param[in]  PeiServices          The PEI Services pointer.
  @param[in]  This                 The PPI instance pointer.
  @param[out] SpecVersion          The Version of the SBI specification.
**/
typedef
VOID
(EFIAPI *PEI_RISCV_OPENSBI_GET_SPEC_VERSION)(
  IN  EFI_PEI_SERVICES           **PeiServices,
  IN  PEI_RISCV_OPENSBI_BASE_PPI  *This,
  OUT UINTN                       *SpecVersion
  );

/**
  Get the SBI implementation ID

  This ID is used to idenetify a specific SBI implementation in order to work
  around any quirks it might have.

  @param[in]  PeiServices          The PEI Services pointer.
  @param[in]  This                 The PPI instance pointer.
  @param[out] ImplId               The ID of the SBI implementation.
**/
typedef
VOID
(EFIAPI *PEI_RISCV_OPENSBI_GET_IMPL_ID)(
  IN  EFI_PEI_SERVICES          **PeiServices,
  IN  PEI_RISCV_OPENSBI_BASE_PPI *This,
  OUT UINTN                      *ImplId
  );

/**
  Get the SBI implementation version

  The version of this SBI implementation.
  The encoding of this number is determined by the specific SBI implementation.

  @param[in]  PeiServices          The PEI Services pointer.
  @param[in]  This                 The PPI instance pointer.
  @param[out] ImplVersion          The version of the SBI implementation.
**/
typedef
VOID
(EFIAPI *PEI_RISCV_OPENSBI_GET_IMPLVERSION)(
  IN  EFI_PEI_SERVICES           **PeiServices,
  IN  PEI_RISCV_OPENSBI_BASE_PPI  *This,
  OUT UINTN                       *ImplVersion
  );

/**
  Probe whether an SBI extension is available

  ProbeResult is set to 0 if the extension is not available or to an extension
  specified value if it is available.

  @param[in]  PeiServices          The PEI Services pointer.
  @param[in]  This                 The PPI instance pointer.
  @param[in]  ExtensionId          The extension ID.
  @param[out] ProbeResult          The return value of the probe.
**/
typedef
VOID
(EFIAPI *PEI_RISCV_OPENSBI_PROBE_EXTENSION)(
  IN  EFI_PEI_SERVICES           **PeiServices,
  IN  PEI_RISCV_OPENSBI_BASE_PPI  *This,
  IN  INTN                         ExtensionId,
  OUT INTN                        *ProbeResult
  );

/**
  Get the CPU's vendor ID

  Reads the mvendorid CSR.

  @param[in]  PeiServices          The PEI Services pointer.
  @param[in]  This                 The PPI instance pointer.
  @param[out] MvendorId            The CPU's vendor ID.
**/
typedef
VOID
(EFIAPI *PEI_RISCV_OPENSBI_GET_MVENDOR_ID)(
  IN  EFI_PEI_SERVICES           **PeiServices,
  IN  PEI_RISCV_OPENSBI_BASE_PPI  *This,
  OUT UINTN                       *MvendorId
  );

/**
  Get the CPU's architecture ID

  Reads the marchid CSR.

  @param[in]  PeiServices          The PEI Services pointer.
  @param[in]  This                 The PPI instance pointer.
  @param[out] MarchId              The CPU's architecture ID.
**/
typedef
VOID
(EFIAPI *PEI_RISCV_OPENSBI_GET_MARCH_ID)(
  IN  EFI_PEI_SERVICES           **PeiServices,
  IN  PEI_RISCV_OPENSBI_BASE_PPI  *This,
  OUT UINTN                       *MarchId
  );

/**
  Get the CPU's implementation ID

  Reads the mimpid CSR.

  @param[in]  PeiServices          The PEI Services pointer.
  @param[in]  This                 The PPI instance pointer.
  @param[out] MimpId               The CPU's implementation ID.
**/
typedef
VOID
(EFIAPI *PEI_RISCV_OPENSBI_GET_MIMP_ID)(
  IN  EFI_PEI_SERVICES           **PeiServices,
  IN  PEI_RISCV_OPENSBI_BASE_PPI  *This,
  OUT UINTN                       *Mimpid
  );

struct _PEI_RISCV_OPENSBI_BASE_PPI {
  UINTN                               Version;
  PEI_RISCV_OPENSBI_GET_SPEC_VERSION  OpenSbiGetSpecVersion;
  PEI_RISCV_OPENSBI_GET_IMPL_ID       OpenSbiGetImplId;
  PEI_RISCV_OPENSBI_GET_IMPLVERSION   OpenSbiGetImplVersion;
  PEI_RISCV_OPENSBI_PROBE_EXTENSION   OpenSbiProbeExtension;
  PEI_RISCV_OPENSBI_GET_MVENDOR_ID    OpenSbiGetMvendorId;
  PEI_RISCV_OPENSBI_GET_MARCH_ID      OpenSbiGetMarchId;
  PEI_RISCV_OPENSBI_GET_MIMP_ID       OpenSbiGetMimpId;
};

///
/// Global ID for the PEI_RISCV_OPENSBI_LEGACY_PPI_GUID.
///
#define PEI_RISCV_OPENSBI_LEGACY_PPI_GUID  \
  { \
    0xc80a737c, 0x566b, 0x4709, { 0x8f, 0x74, 0x58, 0x30, 0xee, 0x2a, 0xd7, 0x36 } \
  }

///
/// Forward declaration for PEI_RISCV_OPENSBI_LEGACY_PPI.
///
typedef struct _PEI_RISCV_OPENSBI_LEGACY_PPI PEI_RISCV_OPENSBI_LEGACY_PPI;

/**
  Clear pending timer interrupt bit and set timer for next event after StimeValue.

  To clear the timer without scheduling a timer event, set StimeValue to a
  practically infinite value or mask the timer interrupt by clearing sie.STIE.

  @param[in]  PeiServices          The PEI Services pointer.
  @param[in]  This                 The PPI instance pointer.
  @param[in]  StimeValue           The time offset to the next scheduled timer interrupt.
**/
typedef
VOID
(EFIAPI *PEI_RISCV_OPENSBI_LEGACY_SET_TIMER)(
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  PEI_RISCV_OPENSBI_LEGACY_PPI  *This,
  IN  UINT64                         StimeValue
  );

/**
  Write a byte to the system's console.

  Will block if characters are pending to be transmitted or the receiving
  terminal is not ready to receive a byte. If the console, does not exist,
  the byte is discarded.

  @param[in]  PeiServices          The PEI Services pointer.
  @param[in]  This                 The PPI instance pointer.
  @param[in]  ConsoleChar          The time offset to the next scheduled timer interrupt.
**/
typedef
VOID
(EFIAPI *PEI_RISCV_OPENSBI_LEGACY_CONSOLE_PUT_CHAR)(
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  PEI_RISCV_OPENSBI_LEGACY_PPI  *This,
  IN  INTN                           ConsoleChar
  );

/**
  Read a byte from the system's console.

  @param[in]  PeiServices          The PEI Services pointer.
  @param[in]  This                 The PPI instance pointer.
  @param[in]  ConsoleChar          The pointer in which the read byte is written.
**/
typedef
VOID
(EFIAPI *PEI_RISCV_OPENSBI_LEGACY_CONSOLE_GET_CHAR)(
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  PEI_RISCV_OPENSBI_LEGACY_PPI  *This,
  OUT INTN                          *ConsoleChar
  );

/**
  Clear pending IPIs for the calling hart.

  @param[in]  PeiServices          The PEI Services pointer.
  @param[in]  This                 The PPI instance pointer.
**/
typedef
VOID
(EFIAPI *PEI_RISCV_OPENSBI_LEGACY_CLEAR_IPI)(
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  PEI_RISCV_OPENSBI_LEGACY_PPI  *This
  );

/**
  Send IPI to all harts specified in the mask.

  @param[in]  PeiServices          The PEI Services pointer.
  @param[in]  This                 The PPI instance pointer.
  @param[in]  HartMask             Virtual address pointing to the hart mask,
                                   which is an array of `unsigned long`s.
**/
typedef
VOID
(EFIAPI *PEI_RISCV_OPENSBI_LEGACY_SEND_IPI)(
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  PEI_RISCV_OPENSBI_LEGACY_PPI  *This,
  IN  CONST UINTN                   *HartMask
  );

/**
  Instructs remote harts to execute a FENCE.I instruction. 

  @param[in]  PeiServices          The PEI Services pointer.
  @param[in]  This                 The PPI instance pointer.
  @param[in]  HartMask             Virtual address pointing to the hart mask,
                                   which is an array of `unsigned long`s.
**/
typedef
VOID
(EFIAPI *PEI_RISCV_OPENSBI_LEGACY_REMOTE_FENCE_I)(
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  PEI_RISCV_OPENSBI_LEGACY_PPI  *This,
  IN  CONST UINTN                   *HartMask
  );

/**
  Instructs remote harts to execute one or more SFENCE.VMA instructions over a range.

  @param[in]  PeiServices          The PEI Services pointer.
  @param[in]  This                 The PPI instance pointer.
  @param[in]  HartMask             Virtual address pointing to the hart mask,
                                   which is an array of `unsigned long`s.
  @param[in]  Start                The address of where the range begins.
  @param[in]  Size                 The total size of the range.
**/
typedef
VOID
(EFIAPI *PEI_RISCV_OPENSBI_LEGACY_REMOTE_SFENCE_VMA)(
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  PEI_RISCV_OPENSBI_LEGACY_PPI  *This,
  IN  CONST UINTN                   *HartMask,
  IN  UINTN                          Start,
  IN  UINTN                          Size
  );

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
typedef
VOID
(EFIAPI *PEI_RISCV_OPENSBI_LEGACY_REMOTE_SFENCE_VMA_ASID)(
  IN  EFI_PEI_SERVICES              **PeiServices,
  IN  PEI_RISCV_OPENSBI_LEGACY_PPI   *This,
  IN  CONST UINTN                    *HartMask,
  IN  UINTN                           Start,
  IN  UINTN                           Size,
  IN  UINTN                           Asid
  );

/**
  Shuts all harts down.

  Does not return.

  @param[in]  PeiServices          The PEI Services pointer.
  @param[in]  This                 The PPI instance pointer.
**/
typedef
VOID
(EFIAPI *PEI_RISCV_OPENSBI_LEGACY_SHUTDOWN)(
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  PEI_RISCV_OPENSBI_LEGACY_PPI  *This
  );

///
/// This PPI contains a set of services to interact with OpenSBI
///
struct _PEI_RISCV_OPENSBI_LEGACY_PPI {
  UINTN                                           Version;
  PEI_RISCV_OPENSBI_LEGACY_SET_TIMER              OpenSbiLegacySetTimer;
  PEI_RISCV_OPENSBI_LEGACY_CONSOLE_PUT_CHAR       OpenSbiLegacyConsolePutChar;
  PEI_RISCV_OPENSBI_LEGACY_CONSOLE_GET_CHAR       OpenSbiLegacyConsoleGetChar;
  PEI_RISCV_OPENSBI_LEGACY_CLEAR_IPI              OpenSbiLegacyClearIpi;
  PEI_RISCV_OPENSBI_LEGACY_SEND_IPI               OpenSbiLegacySendIpi;
  PEI_RISCV_OPENSBI_LEGACY_REMOTE_FENCE_I         OpenSbiLegacyRemoteFenceI;
  PEI_RISCV_OPENSBI_LEGACY_REMOTE_SFENCE_VMA      OpenSbiLegacyRemoteSfenceVma;
  PEI_RISCV_OPENSBI_LEGACY_REMOTE_SFENCE_VMA_ASID OpenSbiLegacyRemoteSfenceVmaAsid;
  PEI_RISCV_OPENSBI_LEGACY_SHUTDOWN               OpenSbiLegacyShutdown;
};

///
/// Global ID for the PEI_RISCV_OPENSBI_HSM_PPI_GUID.
///
#define PEI_RISCV_OPENSBI_HSM_PPI_GUID  \
  { \
    0x13421726, 0xa40c, 0x479c, { 0x94, 0xd2, 0x8f, 0xf2, 0x97, 0x32, 0xfb, 0xb4 } \
  }

///
/// Forward declaration for PEI_RISCV_OPENSBI_HSM_PPI.
///
typedef struct _PEI_RISCV_OPENSBI_HSM_PPI PEI_RISCV_OPENSBI_HSM_PPI;

/**
  Politely ask the SBI to start a given hart.

  This call may return before the hart has actually started executing, if the
  SBI implementation can guarantee that the hart is actually going to start.

  Before the hart jumps to StartAddr, the hart MUST configure PMP if present
  and switch to S-mode.

  @param[in]  PeiServices          The PEI Services pointer.
  @param[in]  This                 The PPI instance pointer.
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
typedef
EFI_STATUS
(EFIAPI *PEI_RISCV_OPENSBI_HART_START)(
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  PEI_RISCV_OPENSBI_HSM_PPI     *This,
  IN  UINTN                          HartId,
  IN  UINTN                          StartAddr,
  IN  UINTN                          Priv
  );

/**
  Return execution of the calling hart to SBI.

  MUST be called in S-Mode with user interrupts disabled.
  This call is not expected to return, unless a failure occurs.

  @param[in]  PeiServices          The PEI Services pointer.
  @param[in]  This                 The PPI instance pointer.
  @retval     EFI_SUCCESS          Never occurs. When successful, the call does not return.
  @retval     other                Failed to stop hard for an unknown reason.
**/
typedef
EFI_STATUS
(EFIAPI *PEI_RISCV_OPENSBI_HART_STOP)(
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  PEI_RISCV_OPENSBI_HSM_PPI     *This
  );

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
typedef
EFI_STATUS
(EFIAPI *PEI_RISCV_OPENSBI_HART_GET_STATUS)(
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  PEI_RISCV_OPENSBI_HSM_PPI     *This,
  IN  UINTN                          HartId,
  OUT UINTN                         *HartStatus
  );

///
/// Hart State Management Extension
///
struct _PEI_RISCV_OPENSBI_HSM_PPI {
  UINTN                                    Version;
  PEI_RISCV_OPENSBI_HART_START             OpenSbiHartStart;
  PEI_RISCV_OPENSBI_HART_STOP              OpenSbiHartStop;
  PEI_RISCV_OPENSBI_HART_GET_STATUS        OpenSbiHartGetStatus;
};

///
/// Global ID for the PEI_RISCV_OPENSBI_TIME_PPI_GUID.
///
#define PEI_RISCV_OPENSBI_TIME_PPI_GUID  \
  { \
    0x63ab8596, 0xc37b, 0x49d1, { 0xa1, 0xd1, 0x4a, 0x21, 0x3b, 0x2b, 0x5a, 0x27 } \
  }

///
/// Forward declaration for PEI_RISCV_OPENSBI_TIME_PPI.
///
typedef struct _PEI_RISCV_OPENSBI_TIME_PPI PEI_RISCV_OPENSBI_TIME_PPI;

/**
  Clear pending timer interrupt bit and set timer for next event after StimeValue.

	To clear the timer without scheduling a timer event, set StimeValue to a
	practically infinite value or mask the timer interrupt by clearing sie.STIE.

  @param[in]  PeiServices          The PEI Services pointer.
  @param[in]  This                 The PPI instance pointer.
  @param[in]  StimeValue           The time offset to the next scheduled timer interrupt.
**/
typedef
VOID
(EFIAPI *PEI_RISCV_OPENSBI_SET_TIMER)(
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  PEI_RISCV_OPENSBI_TIME_PPI    *This,
  IN  UINT64                         StimeValue
  );

///
/// Timer extension
///
struct _PEI_RISCV_OPENSBI_TIME_PPI {
  UINTN                                    Version;
  PEI_RISCV_OPENSBI_SET_TIMER              OpenSbiSetTimer;
};

///
/// Global ID for the PEI_RISCV_OPENSBI_IPI_PPI_GUID.
///
#define PEI_RISCV_OPENSBI_IPI_PPI_GUID  \
  { \
    0x527f6f8f, 0x8b16, 0x4a02, { 0x95, 0x4e, 0xb5, 0x03, 0x4c, 0x8c, 0x84, 0x5b } \
  }

///
/// Forward declaration for PEI_RISCV_OPENSBI_IPI_PPI.
///
typedef struct _PEI_RISCV_OPENSBI_IPI_PPI PEI_RISCV_OPENSBI_IPI_PPI;

/**
  Send IPI to all harts specified in the mask.

  The interrupts are registered as supervisor software interrupts at the
  receiving hart.

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
typedef
EFI_STATUS
(EFIAPI *PEI_RISCV_OPENSBI_SEND_IPI)(
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  PEI_RISCV_OPENSBI_IPI_PPI     *This,
  IN  UINTN                         *HartMask,
  IN  UINTN                          HartMaskBase
  );

///
/// IPI extension
///
struct _PEI_RISCV_OPENSBI_IPI_PPI {
  UINTN                                    Version;
  PEI_RISCV_OPENSBI_SEND_IPI               OpenSbiSendIpi;
};

///
/// Global ID for the PEI_RISCV_OPENSBI_RFNC_PPI_GUID.
///
#define PEI_RISCV_OPENSBI_RFNC_PPI_GUID  \
  { \
    0xc652511a, 0x63b6, 0x4aca, { 0x8b, 0x66, 0x99, 0x83, 0xcf, 0x6a, 0xd9, 0x69 } \
  }

///
/// Forward declaration for PEI_RISCV_OPENSBI_RFNC_PPI.
///
typedef struct _PEI_RISCV_OPENSBI_RFNC_PPI PEI_RISCV_OPENSBI_RFNC_PPI;

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
typedef
EFI_STATUS
(*EFIAPI PEI_RISCV_OPENSBI_REMOTE_FENCE_I)(
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  PEI_RISCV_OPENSBI_RFNC_PPI    *This,
  IN  UINTN                         *HartMask,
  IN  UINTN                          HartMaskBase
  );

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
typedef
EFI_STATUS
(*EFIAPI PEI_RISCV_OPENSBI_REMOTE_SFENCE_VMA)(
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  PEI_RISCV_OPENSBI_RFNC_PPI    *This,
  IN  UINTN                         *HartMask,
  IN  UINTN                          HartMaskBase,
  IN  UINTN                          StartAddr,
  IN  UINTN                          Size
  );

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
typedef
EFI_STATUS
(*EFIAPI PEI_RISCV_OPENSBI_REMOTE_SFENCE_VMA_ASID)(
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  PEI_RISCV_OPENSBI_RFNC_PPI    *This,
  IN  UINTN                         *HartMask,
  IN  UINTN                          HartMaskBase,
  IN  UINTN                          StartAddr,
  IN  UINTN                          Size,
  IN  UINTN                          Asid
  );

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
typedef
EFI_STATUS
(*EFIAPI PEI_RISCV_OPENSBI_REMOTE_HFENCE_GVMA_VMID)(
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  PEI_RISCV_OPENSBI_RFNC_PPI    *This,
  IN  UINTN                         *HartMask,
  IN  UINTN                          HartMaskBase,
  IN  UINTN                          StartAddr,
  IN  UINTN                          Size,
  IN  UINTN                          Vmid
  );

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
typedef
EFI_STATUS
(*EFIAPI PEI_RISCV_OPENSBI_REMOTE_HFENCE_GVMA)(
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  PEI_RISCV_OPENSBI_RFNC_PPI    *This,
  IN  UINTN                         *HartMask,
  IN  UINTN                          HartMaskBase,
  IN  UINTN                          StartAddr,
  IN  UINTN                          Size
  );

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
typedef
EFI_STATUS
(*EFIAPI PEI_RISCV_OPENSBI_REMOTE_HFENCE_VVMA_ASID)(
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  PEI_RISCV_OPENSBI_RFNC_PPI    *This,
  IN  UINTN                         *HartMask,
  IN  UINTN                          HartMaskBase,
  IN  UINTN                          StartAddr,
  IN  UINTN                          Size,
  IN  UINTN                          Asid
  );

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
typedef
EFI_STATUS
(*EFIAPI PEI_RISCV_OPENSBI_REMOTE_HFENCE_VVMA)(
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  PEI_RISCV_OPENSBI_RFNC_PPI    *This,
  IN  UINTN                         *HartMask,
  IN  UINTN                          HartMaskBase,
  IN  UINTN                          StartAddr,
  IN  UINTN                          Size
  );

///
/// Remote fence extension
///
struct _PEI_RISCV_OPENSBI_RFNC_PPI {
  UINTN                                     Version;
  PEI_RISCV_OPENSBI_REMOTE_FENCE_I          OpenSbiRemoteFenceI;
  PEI_RISCV_OPENSBI_REMOTE_SFENCE_VMA       OpenSbiRemoteSfenceVma;
  PEI_RISCV_OPENSBI_REMOTE_SFENCE_VMA_ASID  OpenSbiRemoteSfenceVmaAsid;
  PEI_RISCV_OPENSBI_REMOTE_HFENCE_GVMA_VMID OpenSbiRemoteHFenceGvmaVmid;
  PEI_RISCV_OPENSBI_REMOTE_HFENCE_GVMA      OpenSbiRemoteHFenceGvma;
  PEI_RISCV_OPENSBI_REMOTE_HFENCE_VVMA_ASID OpenSbiRemoteHFenceVvmaAsid;
  PEI_RISCV_OPENSBI_REMOTE_HFENCE_VVMA      OpenSbiRemoteHFenceVvma;
};

///
/// Global ID for the PEI_RISCV_OPENSBI_VENDOR_PPI_GUID.
///
#define PEI_RISCV_OPENSBI_VENDOR_PPI_GUID  \
  { \
    0x44ff1fcf, 0x1d04, 0x4fba, { 0x8e, 0x44, 0x5f, 0x68, 0xfe, 0x07, 0xcd, 0xe4 } \
  }

///
/// Forward declaration for PEI_RISCV_OPENSBI_VENDOR_PPI.
///
typedef struct _PEI_RISCV_OPENSBI_VENDOR_PPI PEI_RISCV_OPENSBI_VENDOR_PPI;

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
typedef
EFI_STATUS
(EFIAPI *PEI_RISCV_OPENSBI_VENDOR_CALL)(
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  PEI_RISCV_OPENSBI_VENDOR_PPI  *This,
  IN  UINTN                          ExtensionId,
  IN  UINTN                          FunctionId,
  IN  UINTN                          NumArgs,
  ...
  );

///
/// Vendor Specific extension space: Extension Ids 0x09000000 through 0x09FFFFFF
///
struct _PEI_RISCV_OPENSBI_VENDOR_PPI {
  UINTN                         Version;
  PEI_RISCV_OPENSBI_VENDOR_CALL OpenSbiVendorCall;
};

///
/// Global ID for the PEI_RISCV_OPENSBI_LIBRARY_PPI_GUID.
///
#define PEI_RISCV_OPENSBI_LIBRARY_PPI_GUID  \
  { \
    0x34922165, 0xa5ca, 0x470f, { 0x83, 0x2f, 0x14, 0xc9, 0x42, 0xfa, 0x83, 0xdf  } \
  }

///
/// Forward declaration for PEI_RISCV_OPENSBI_LIBRARY_PPI.
///
typedef struct _PEI_RISCV_OPENSBI_LIBRARY_PPI PEI_RISCV_OPENSBI_LIBRARY_PPI;


///
/// Library API
///
struct _PEI_RISCV_OPENSBI_LIBRARY_PPI {
  UINTN                                           Version;
};

///
/// Global ID for the PEI_RISCV_OPENSBI_FIRMWARE_PPI_GUID.
///
#define PEI_RISCV_OPENSBI_FIRMWARE_PPI_GUID  \
  { \
    0xd2bb94eb, 0x35eb, 0x4fa2, { 0xac, 0x0d, 0x7a, 0xbe, 0xf9, 0x30, 0xc1, 0x96  } \
  }

///
/// Forward declaration for PEI_RISCV_OPENSBI_FIRMWARE_PPI.
///
typedef struct _PEI_RISCV_OPENSBI_FIRMWARE_PPI PEI_RISCV_OPENSBI_FIRMWARE_PPI;

/**
  Get scratch space of the current hart.

  @param[in]  PeiServices          The PEI Services pointer.
  @param[in]  This                 The PPI instance pointer.
  @param[out] ScratchSpace         The scratch space pointer.
  @retval EFI_SUCCESS              The operation succeeds.
**/
typedef
EFI_STATUS
(EFIAPI* PEI_RISCV_OPENSBI_GET_MSCRATCH)(
  IN  EFI_PEI_SERVICES               **PeiServices,
  IN  PEI_RISCV_OPENSBI_FIRMWARE_PPI  *This,
  OUT struct sbi_scratch             **ScratchSpace
  );

/**
  Get scratch space of the given hart id.

  @param[in]  PeiServices          The PEI Services pointer.
  @param[in]  This                 The PPI instance pointer.
  @param[in]  HartId               The hart id.
  @param[out] ScratchSpace         The scratch space pointer.
  @retval EFI_SUCCESS              The operation succeeds.
**/
typedef
EFI_STATUS
(EFIAPI* PEI_RISCV_OPENSBI_GET_MSCRATCH_HARTID)(
  IN  EFI_PEI_SERVICES               **PeiServices,
  IN  PEI_RISCV_OPENSBI_FIRMWARE_PPI  *This,
  IN  UINTN                            HartId,
  OUT struct sbi_scratch             **ScratchSpace
  );

///
/// Firmware SBI Extension
///
/// This SBI Extension is defined and used by EDK2 only in order to be able to
/// run PI and DXE phase in S-Mode.
///
struct _PEI_RISCV_OPENSBI_FIRMWARE_PPI {
  UINTN                                     Version;
  PEI_RISCV_OPENSBI_GET_MSCRATCH            OpenSbiGetMscratch;
  PEI_RISCV_OPENSBI_GET_MSCRATCH_HARTID     OpenSbiGetMscratchHartid;
};

#endif
