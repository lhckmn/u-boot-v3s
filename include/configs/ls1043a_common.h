/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (C) 2015 Freescale Semiconductor
 * Copyright 2019-2021 NXP
 */

#ifndef __LS1043A_COMMON_H
#define __LS1043A_COMMON_H

/* SPL build */
#ifdef CONFIG_XPL_BUILD
#define SPL_NO_FMAN
#define SPL_NO_DSPI
#define SPL_NO_PCIE
#define SPL_NO_ENV
#define SPL_NO_MISC
#define SPL_NO_USB
#define SPL_NO_SATA
#define SPL_NO_QE
#define SPL_NO_EEPROM
#endif
#if (defined(CONFIG_XPL_BUILD) && defined(CONFIG_NAND_BOOT))
#define SPL_NO_MMC
#endif
#if (defined(CONFIG_XPL_BUILD) && defined(CONFIG_SD_BOOT_QSPI))
#define SPL_NO_IFC
#endif

#include <asm/arch/stream_id_lsch2.h>
#include <asm/arch/config.h>

/* Link Definitions */

#define CFG_SYS_DDR_SDRAM_BASE	0x80000000
#define CFG_SYS_FSL_DDR_SDRAM_BASE_PHY	0
#define CFG_SYS_SDRAM_BASE		CFG_SYS_DDR_SDRAM_BASE
#define CFG_SYS_DDR_BLOCK2_BASE      0x880000000ULL

#define CPU_RELEASE_ADDR               secondary_boot_addr

/* Serial Port */
#define CFG_SYS_NS16550_CLK          (get_serial_clock())

/* NAND SPL */
#ifdef CONFIG_NAND_BOOT
#define CFG_SYS_NAND_U_BOOT_DST	CONFIG_TEXT_BASE
#define CFG_SYS_NAND_U_BOOT_START	CONFIG_TEXT_BASE
#endif

/* GPIO */

/* IFC */
#ifndef SPL_NO_IFC
#if defined(CONFIG_TFABOOT) || \
	(!defined(CONFIG_QSPI_BOOT) && !defined(CONFIG_SD_BOOT_QSPI))
/*
 * CFG_SYS_FLASH_BASE has the final address (core view)
 * CFG_SYS_FLASH_BASE_PHYS has the final address (IFC view)
 * CFG_SYS_FLASH_BASE_PHYS_EARLY has the temporary IFC address
 * CONFIG_TEXT_BASE is linked to 0x60000000 for booting
 */
#define CFG_SYS_FLASH_BASE			0x60000000
#define CFG_SYS_FLASH_BASE_PHYS		CFG_SYS_FLASH_BASE
#define CFG_SYS_FLASH_BASE_PHYS_EARLY	0x00000000
#endif
#endif

/* I2C */

/*  DSPI  */

/* FMan ucode */
#ifndef SPL_NO_FMAN
#ifdef CONFIG_SYS_DPAA_FMAN
#define CFG_SYS_FM_MURAM_SIZE	0x60000
#endif
#endif

/* Miscellaneous configurable options */

#define HWCONFIG_BUFFER_SIZE		128

#ifndef SPL_NO_MISC
#define BOOT_TARGET_DEVICES(func) \
	func(MMC, mmc, 0) \
	func(USB, usb, 0) \
	func(DHCP, dhcp, na)
#include <config_distro_bootcmd.h>

/* Initial environment variables */
#define CFG_EXTRA_ENV_SETTINGS		\
	"hwconfig=fsl_ddr:bank_intlv=auto\0"	\
	"fdt_high=0xffffffffffffffff\0"		\
	"initrd_high=0xffffffffffffffff\0"	\
	"kernel_addr=0x61000000\0"		\
	"scriptaddr=0x80000000\0"		\
	"scripthdraddr=0x80080000\0"		\
	"fdtheader_addr_r=0x80100000\0"		\
	"kernelheader_addr_r=0x80200000\0"	\
	"kernel_addr_r=0x81000000\0"		\
	"kernel_start=0x1000000\0"		\
	"kernelheader_start=0x800000\0"		\
	"fdt_addr_r=0x90000000\0"		\
	"load_addr=0xa0000000\0"		\
	"kernelheader_addr=0x60600000\0"	\
	"kernel_size=0x2800000\0"		\
	"kernelheader_size=0x40000\0"		\
	"kernel_addr_sd=0x8000\0"		\
	"kernel_size_sd=0x14000\0"		\
	"kernelhdr_addr_sd=0x3000\0"		\
	"kernelhdr_size_sd=0x10\0"		\
	"console=ttyS0,115200\0"		\
	"boot_os=y\0"				\
	BOOTENV					\
	"boot_scripts=ls1043ardb_boot.scr\0"	\
	"boot_script_hdr=hdr_ls1043ardb_bs.out\0"	\
	"scan_dev_for_boot_part="		\
		"part list ${devtype} ${devnum} devplist; "	\
		"env exists devplist || setenv devplist 1; "	\
		"for distro_bootpart in ${devplist}; do "	\
			"if fstype ${devtype} "			\
				"${devnum}:${distro_bootpart} "	\
				"bootfstype; then "		\
				"run scan_dev_for_boot; "	\
			"fi; "					\
		"done\0"			\
	"boot_a_script="					\
		"load ${devtype} ${devnum}:${distro_bootpart} "	\
			"${scriptaddr} ${prefix}${script}; "	\
		"env exists secureboot && load ${devtype} "	\
			"${devnum}:${distro_bootpart} "		\
			"${scripthdraddr} ${prefix}${boot_script_hdr}; " \
			"env exists secureboot "	\
			"&& esbc_validate ${scripthdraddr};"	\
		"source ${scriptaddr}\0"			\
	"qspi_bootcmd=echo Trying load from qspi..;"	\
		"sf probe && sf read $load_addr "	\
		"$kernel_start $kernel_size; env exists secureboot "	\
		"&& sf read $kernelheader_addr_r $kernelheader_start "	\
		"$kernelheader_size && esbc_validate ${kernelheader_addr_r}; " \
		"bootm $load_addr#$board\0"	\
	"nor_bootcmd=echo Trying load from nor..;"	\
		"cp.b $kernel_addr $load_addr "	\
		"$kernel_size; env exists secureboot "	\
		"&& cp.b $kernelheader_addr $kernelheader_addr_r "	\
		"$kernelheader_size && esbc_validate ${kernelheader_addr_r}; " \
		"bootm $load_addr#$board\0"	    \
	"nand_bootcmd=echo Trying load from NAND..;"	\
		"nand info; nand read $load_addr "	\
		"$kernel_start $kernel_size; env exists secureboot "	\
		"&& nand read $kernelheader_addr_r $kernelheader_start "	\
		"$kernelheader_size && esbc_validate ${kernelheader_addr_r}; " \
		"bootm $load_addr#$board\0"	\
	"sd_bootcmd=echo Trying load from SD ..;"       \
		"mmcinfo; mmc read $load_addr "         \
		"$kernel_addr_sd $kernel_size_sd && "     \
		"env exists secureboot && mmc read $kernelheader_addr_r "		\
		"$kernelhdr_addr_sd $kernelhdr_size_sd "		\
		" && esbc_validate ${kernelheader_addr_r};"	\
		"bootm $load_addr#$board\0"

#ifdef CONFIG_TFABOOT
#define QSPI_NOR_BOOTCOMMAND "run distro_bootcmd; run qspi_bootcmd; "	\
			   "env exists secureboot && esbc_halt;"
#define SD_BOOTCOMMAND "run distro_bootcmd; run sd_bootcmd; "  \
			   "env exists secureboot && esbc_halt;"
#define IFC_NOR_BOOTCOMMAND "run distro_bootcmd; run nor_bootcmd; "	\
			   "env exists secureboot && esbc_halt;"
#define IFC_NAND_BOOTCOMMAND "run distro_bootcmd; run nand_bootcmd; "	\
			   "env exists secureboot && esbc_halt;"
#endif
#endif

#include <asm/arch/soc.h>

#endif /* __LS1043A_COMMON_H */
