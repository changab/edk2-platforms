/*
 *
 * Copyright (c) 2019, Hewlett Packard Enterprise Development LP. All rights reserved.<BR>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2019 Western Digital Corporation or its affiliates.
 *
 * Authors:
 *   Atish Patra <atish.patra@wdc.com>
 */

#include <libfdt.h>
#include <fdt.h>
#include <sbi/riscv_encoding.h>
#include <sbi/sbi_const.h>
#include <sbi/sbi_hart.h>
#include <sbi/sbi_console.h>
#include <sbi/sbi_platform.h>
#include <sbi/riscv_io.h>
#include <sbi_utils/irqchip/plic.h>
#include <sbi_utils/serial/sifive-uart.h>
#include <sbi_utils/sys/clint.h>
#include <U5Clint.h>

#define U540_HART_COUNT         FixedPcdGet32(PcdHartCount)
#define U540_HART_STACK_SIZE    FixedPcdGet32(PcdOpenSbiStackSize)
#define U540_BOOT_HART_ID       FixedPcdGet32(PcdBootHartId)

#define U540_SYS_CLK              FixedPcdGet32(PcdU5PlatformSystemClock)

#define U540_PLIC_ADDR            0xc000000
#define U540_PLIC_NUM_SOURCES     0x35
#define U540_PLIC_NUM_PRIORITIES  7

#define U540_UART_ADDR            FixedPcdGet32(PcdU5UartBase)

#define U540_UART_BAUDRATE        115200

/**
 * The U540 SoC has 5 HARTs but HART ID 0 has only SMode.
 * HARTs 1 is selected as boot HART
 */
#ifndef U540_ENABLED_HART_MASK
#define U540_ENABLED_HART_MASK  (1 << U540_BOOT_HART_ID)
#endif

#define U540_HARTID_DISABLED    ~(U540_ENABLED_HART_MASK)

/* PRCI clock related macros */
//TODO: Do we need a separate driver for this ?
#define U540_PRCI_BASE_ADDR                 0x10000000
#define U540_PRCI_CLKMUXSTATUSREG           0x002C
#define U540_PRCI_CLKMUX_STATUS_TLCLKSEL    (0x1 << 1)

unsigned long log2roundup(unsigned long x);

static void U540_modify_dt(void *fdt)
{
    u32 i, size;
    int chosen_offset, err;
    int cpu_offset;
    char cpu_node[32] = "";
    const char *mmu_type;

    for (i = 0; i < U540_HART_COUNT; i++) {
        sbi_sprintf(cpu_node, "/cpus/cpu@%d", i);
        cpu_offset = fdt_path_offset(fdt, cpu_node);
        mmu_type = fdt_getprop(fdt, cpu_offset, "mmu-type", NULL);
        if (mmu_type && (!AsciiStrCmp(mmu_type, "riscv,sv39") ||
            !AsciiStrCmp(mmu_type,"riscv,sv48")))
            continue;
        else
            fdt_setprop_string(fdt, cpu_offset, "status", "masked");
        memset(cpu_node, 0, sizeof(cpu_node));
    }
    size = fdt_totalsize(fdt);
    err = fdt_open_into(fdt, fdt, size + 256);
    if (err < 0)
        sbi_printf("Device Tree can't be expanded to accmodate new node");

    chosen_offset = fdt_path_offset(fdt, "/chosen");
    fdt_setprop_string(fdt, chosen_offset, "stdout-path",
               "/soc/serial@10010000:115200");

    plic_fdt_fixup(fdt, "riscv,plic0");
}

static int U540_final_init(bool cold_boot)
{
    void *fdt;
    struct sbi_scratch *ThisScratch;

    if (!cold_boot)
        return 0;

    fdt = sbi_scratch_thishart_arg1_ptr();
    U540_modify_dt(fdt);
    //
    // Set PMP of firmware regions to R and X. We will lock this in the end of PEI.
    // This region only protects SEC, PEI and Scratch buffer.
    //
    ThisScratch = sbi_scratch_thishart_ptr ();
    pmp_set(0, PMP_R | PMP_X | PMP_W, ThisScratch->fw_start, log2roundup (ThisScratch->fw_size));
    return 0;
}

static u32 U540_pmp_region_count(u32 hartid)
{
    return 1;
}

static int U540_pmp_region_info(u32 hartid, u32 index,
                 ulong *prot, ulong *addr, ulong *log2size)
{
    int ret = 0;

    switch (index) {
    case 0:
        *prot = PMP_R | PMP_W | PMP_X;
        *addr = 0;
        *log2size = __riscv_xlen;
        break;
    default:
        ret = -1;
        break;
    };

    return ret;
}

static int U540_console_init(void)
{
    unsigned long peri_in_freq;

    peri_in_freq = U540_SYS_CLK/2;
    return sifive_uart_init(U540_UART_ADDR, peri_in_freq, U540_UART_BAUDRATE);
}

static int U540_irqchip_init(bool cold_boot)
{
    int rc;
    u32 hartid = sbi_current_hartid();

    if (cold_boot) {
        rc = plic_cold_irqchip_init(U540_PLIC_ADDR,
                        U540_PLIC_NUM_SOURCES,
                        U540_HART_COUNT);
        if (rc)
            return rc;
    }

    return plic_warm_irqchip_init(hartid,
            (hartid) ? (2 * hartid - 1) : 0,
            (hartid) ? (2 * hartid) : -1);
}

static int U540_ipi_init(bool cold_boot)
{
    int rc;

    if (cold_boot) {
        rc = clint_cold_ipi_init(CLINT_REG_BASE_ADDR,
                     U540_HART_COUNT);
        if (rc)
            return rc;

    }

    return clint_warm_ipi_init();
}

static int U540_timer_init(bool cold_boot)
{
    int rc;

    if (cold_boot) {
        rc = clint_cold_timer_init(CLINT_REG_BASE_ADDR,
                       U540_HART_COUNT, TRUE);
        if (rc)
            return rc;
    }

    return clint_warm_timer_init();
}

static int U540_system_down(u32 type)
{
    /* For now nothing to do. */
    return 0;
}

const struct sbi_platform_operations platform_ops = {
    .pmp_region_count = U540_pmp_region_count,
    .pmp_region_info = U540_pmp_region_info,
    .final_init = U540_final_init,
    .console_putc = sifive_uart_putc,
    .console_getc = sifive_uart_getc,
    .console_init = U540_console_init,
    .irqchip_init = U540_irqchip_init,
    .ipi_send = clint_ipi_send,
    .ipi_clear = clint_ipi_clear,
    .ipi_init = U540_ipi_init,
    .timer_value = clint_timer_value,
    .timer_event_stop = clint_timer_event_stop,
    .timer_event_start = clint_timer_event_start,
    .timer_init = U540_timer_init,
    .system_reboot = U540_system_down,
    .system_shutdown = U540_system_down
};

const struct sbi_platform platform = {
    .opensbi_version    = OPENSBI_VERSION,                      // The OpenSBI version this platform table is built bassed on.
    .platform_version   = SBI_PLATFORM_VERSION(0x0001, 0x0000), // SBI Platform version 1.0
    .name                   = "SiFive Freedom U540",
    .features       = SBI_PLATFORM_DEFAULT_FEATURES,
    .hart_count     = U540_HART_COUNT,
    .hart_stack_size    = U540_HART_STACK_SIZE,
    .disabled_hart_mask = U540_HARTID_DISABLED,
    .platform_ops_addr  = (unsigned long)&platform_ops
};
