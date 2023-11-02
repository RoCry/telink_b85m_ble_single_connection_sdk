/********************************************************************************************************
 * @file	cstartup_825x.S
 *
 * @brief	This is the boot file for 825x
 *
 * @author	BLE GROUP
 * @date	06,2020
 *
 * @par     Copyright (c) 2020, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
 *
 *          Redistribution and use in source and binary forms, with or without
 *          modification, are permitted provided that the following conditions are met:
 *
 *              1. Redistributions of source code must retain the above copyright
 *              notice, this list of conditions and the following disclaimer.
 *
 *              2. Unless for usage inside a TELINK integrated circuit, redistributions
 *              in binary form must reproduce the above copyright notice, this list of
 *              conditions and the following disclaimer in the documentation and/or other
 *              materials provided with the distribution.
 *
 *              3. Neither the name of TELINK, nor the names of its contributors may be
 *              used to endorse or promote products derived from this software without
 *              specific prior written permission.
 *
 *              4. This software, with or without modification, must only be used with a
 *              TELINK integrated circuit. All other usages are subject to written permission
 *              from TELINK and different commercial license may apply.
 *
 *              5. Licensee shall be solely responsible for any claim to the extent arising out of or
 *              relating to such deletion(s), modification(s) or alteration(s).
 *
 *          THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 *          ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *          WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *          DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDER BE LIABLE FOR ANY
 *          DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *          (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *          LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *          ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *          (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *          SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *******************************************************************************************************/


@********************************************************************************************************
@                                   SRAM CONFIGURATION
@********************************************************************************************************
#define SRAM_32K      (0x848000)	//32KSRAM
#define SRAM_48K      (0x84c000)	//48KSRAM
#define SRAM_64K      (0x850000)	//64KSRAM

@To be compatible with compilation options in older version.
#if (MCU_STARTUP_8258_RET_16K || MCU_STARTUP_8258_RET_32K ||\
	MCU_STARTUP_8258)
	#define	SRAM_SIZE     SRAM_64K
#elif (MCU_STARTUP_8253_RET_16K || MCU_STARTUP_8253_RET_32K ||\
	MCU_STARTUP_8253)
	#define	SRAM_SIZE     SRAM_48K
#elif (MCU_STARTUP_8251_RET_16K || MCU_STARTUP_8251_RET_32K ||\
     MCU_STARTUP_8251)
	#define	SRAM_SIZE     SRAM_32K
#endif

#ifndef SRAM_SIZE
#define	SRAM_SIZE     SRAM_32K
#endif

@********************************************************************************************************
@                                   BOOT FEATURE CONFIGURATION
@********************************************************************************************************

#define  FLL_STK_EN           			    0
#define  GPIO_DEBUG_EN           			0

#ifndef __LOAD_RAM_SIZE__
#define	__LOAD_RAM_SIZE__		0xc
#endif


	.code	16

@********************************************************************************************************
@                                           MACROS AND DEFINIITIONS
@********************************************************************************************************

					@ Mode, correspords to bits 0-5 in CPSR
	.equ MODE_BITS,		0x1F	@ Bit mask for mode bits in CPSR
	.equ IRQ_MODE, 		0x12	@ Interrupt Request mode
	.equ SVC_MODE, 		0x13	@ Supervisor mode 

	.equ IRQ_STK_SIZE,	0x180
	.equ __LOAD_RAM, 	__LOAD_RAM_SIZE__
	.equ __SRAM_SIZE,   SRAM_SIZE
@********************************************************************************************************
@                                            TC32 EXCEPTION VECTORS
@********************************************************************************************************

	.section	.vectors,"ax"
	.global		__reset
	.global	 	__irq
	.global 	__start
	.global		__LOAD_RAM
	.global		__SRAM_SIZE
__start:					@ MUST,  referenced by boot.link

	.extern irq_handler

	.extern	 _retention_data_size_div_256_
	.extern  _retention_data_size_align_256_
	.extern  _ictag_start_
	.extern  _ictag_end_

	.org 0x0
	tj	__reset
@	.word	(BUILD_VERSION)
	.org 0x8
	.word	(0x544c4e4b)
	.word	(0x00880000 + _retention_use_size_div_16_)

	.org 0x10
	tj		__irq
	.org 0x18
	.word	(_bin_size_)
@********************************************************************************************************
@                                   LOW-LEVEL INITIALIZATION
@********************************************************************************************************
	.extern  main



	.org 0x20
	.align 4
	.global start_suspend
	.thumb_func
	.type start_suspend, %function

start_suspend:
	tpush   {r2-r3}

    tmovs r2, #129    @0x81
    tloadr r3, __suspend_data      @0x80006f
    tstorerb r2, [r3, #0]  @*(volatile unsigned char *)0x80006f = 0x81

    tmov r8, r8
    tmov r8, r8
    tmov r8, r8
    tmov r8, r8
    tmov r8, r8
    tmov r8, r8
    tmov r8, r8
    tmov r8, r8
    tmov r8, r8
    tmov r8, r8
    tmov r8, r8
    tmov r8, r8
    tmov r8, r8
    tmov r8, r8
    tmov r8, r8
    tmov r8, r8
	
	tmov r8, r8
    tmov r8, r8
    tmov r8, r8
    tmov r8, r8
    tmov r8, r8
    tmov r8, r8
    tmov r8, r8
    tmov r8, r8
    tmov r8, r8
    tmov r8, r8
    tmov r8, r8
    tmov r8, r8
    tmov r8, r8
    tmov r8, r8
    tmov r8, r8
    tmov r8, r8
	
	tmov r8, r8
    tmov r8, r8
    tmov r8, r8
    tmov r8, r8
    tmov r8, r8
    tmov r8, r8
    tmov r8, r8
    tmov r8, r8
    tmov r8, r8
    tmov r8, r8
    tmov r8, r8
    tmov r8, r8
    tmov r8, r8
    tmov r8, r8
    tmov r8, r8
    tmov r8, r8
	
	tmov r8, r8
    tmov r8, r8
    tmov r8, r8
    tmov r8, r8
    tmov r8, r8
    tmov r8, r8
    tmov r8, r8
    tmov r8, r8
    tmov r8, r8
    tmov r8, r8
    tmov r8, r8
    tmov r8, r8
    tmov r8, r8
    tmov r8, r8
    tmov r8, r8
    tmov r8, r8

    tpop {r2-r3}
    tjex lr

__suspend_data:
	.word   (0x80006f)



__reset:

#if GPIO_DEBUG_EN
	@ add debug, PB4 output 1
	tloadr     	r1, DEBUG_GPIO    @0x80058a  PB oen
	tmov		r0, #139      @0b 11101111
	tstorerb	r0, [r1, #0]

	tmov		r0, #16			@0b 00010000
	tstorerb	r0, [r1, #1]	@0x800583  PB output
#endif

SET_BOOT:
	tmov        r2, #4
	tloadrb     r1, [r2]		@read form core_840004
	tmov     	r0, #165    @A5
	tcmp        r0, r1
	tjne		SET_BOOT_END

	tmov        r2, #5
	tloadrb     r1, [r2]		@read form core_840005
	tloadr     	r0, BOOT_SEL_D
	tstorerb	r1, [r0, #0]
SET_BOOT_END:







@send flash cmd 0xab to wakeup flash;
FLASH_WAKEUP_BEGIN:
	tloadr      r0,FLASH_RECOVER + 0
	tmov		r1,#0
	tstorerb    r1,[r0,#1]
	tmov        r1,#171						@Flash deep cmd: 0xAB
	tstorerb    r1,[r0,#0]
	tmov		r2,#0
	tmov        r3,#6
TNOP:
	tadd        r2,#1
	tcmp        r2,r3
	tjle        TNOP
	tmov		r1,#1
	tstorerb    r1,[r0,#1]
FLASH_WAKEUP_END:




@********************************************************************************************************
@                              		 FILL .DATA AND .BSS WITH 0xFF
@********************************************************************************************************
#if FLL_STK_EN

	tloadr	r0, FLL_D     @r0 = 0xffffffff
	tloadr	r1, FLL_D+4   @r1 = _start_data_
	tloadr	r2, FLL_D+8   @r2 = 0x850000

FLL_STK:
	tcmp	r1, r2
	tjge	FLL_STK_END   @r1>=r2 jump to FLL_STK_END
	tstorer r0, [r1, #0]  @*(unsigned int*)(r1)=0xffffffff
	tadd    r1, #4        @r1 = r1+4
	tj		FLL_STK       @jump to FLL_STK
FLL_STK_END:

#endif

	tloadr	r0, DAT0
	tmcsr	r0			
	tloadr	r0, DAT0 + 8
	tmov	r13, r0  

	tloadr	r0, DAT0 + 4
	tmcsr	r0	
	tloadr	r0, DAT0 + 12
	tmov	r13, r0  
@********************************************************************************************************
@                                    .BSS INITIALIZATION FOR 0
@********************************************************************************************************
	tmov	r0, #0				@r0 = 0
	tloadr	r1, DAT0 + 16		@r1 = _start_bss_
	tloadr	r2, DAT0 + 20		@r2 = _end_bss_

ZERO_BSS_BEGIN:
	tcmp	r1, r2
	tjge	ZERO_BSS_END		@r1>=r2 jump to ZERO_BSS_END
	tstorer	r0, [r1, #0]		@*(unsigned int*)(_start_bss_)=r0=0
	tadd    r1, #4				@r1 = r1 + 4
	tj		ZERO_BSS_BEGIN		@jump to ZERO_BSS_BEGIN
ZERO_BSS_END:

@********************************************************************************************************
@                                    IC TAG INITIALIZATION
@********************************************************************************************************
	tloadr	r1, DAT0 + 28		@r1 = _ictag_start_
	tloadr	r2, DAT0 + 32		@r2 = _ictag_end_

ZERO_TAG_BEGIN:
	tcmp	r1, r2
	tjge	ZERO_TAG_END		@r1>=r2 jump to ZERO_TAG_END
	tstorer	r0, [r1, #0]		@*(unsigned int*)(_ictag_start_)=r0=0
	tadd    r1, #4				@r1 = r1 + 4
	tj		ZERO_TAG_BEGIN		@jump to ZERO_TAG_BEGIN
ZERO_TAG_END:

@********************************************************************************************************
@                                    IC CACHE INITIALIZATION
@********************************************************************************************************
SETIC:
	tloadr     	r1, DAT0 + 24	@ r1 = 0x80060c
	tloadr      r0, DAT0 + 36	@ r0 = _retention_data_size_div_256_
	tstorerb	r0, [r1, #0]	@ *(unsigned int*)(0x80060c) = r0
	tadd    	r0, #1			@ r0 = r0 + 1
	tstorerb	r0, [r1, #1]	@ *(unsigned int*)(0x80060d) = r0

@********************************************************************************************************
@                                    DATA INITIALIZATION
@********************************************************************************************************
	tloadr		r1, DATA_I				@ r1 = _dstored_
	tloadr		r2, DATA_I+4			@ r2 = _start_data_
	tloadr		r3, DATA_I+8			@ r3 = _end_data_
COPY_DATA:
	tcmp		r2, r3
	tjge		COPY_DATA_END			@ r2>=r3 jump to COPY_DATA_END
	tloadr		r0, [r1, #0]			@ r0 = *(unsigned int*)(_dstored_)
	tstorer 	r0, [r2, #0]			@ *(unsigned int*)(_start_data_) = r0
	tadd    	r1, #4					@ r1 = r1 + 4
	tadd		r2, #4					@ r2 = r2 + 4
	tj			COPY_DATA				@ jump to COPY_DATA_BEGIN
COPY_DATA_END:

#if 0
SETSPISPEED:
	tloadr     	r1, DAT0 + 36
	tmov		r0, #0xbb		@0x0b for fast read; 0xbb for dual dat/adr
	tstorerb	r0, [r1, #0]
	tmov		r0, #3			@3 for dual dat/adr
	tstorerb	r0, [r1, #1]
#endif

	tjl	main
END:	tj	END

	.balign	4
DAT0:
	.word	0x12			    @IRQ    @0
	.word	0x13			    @SVC    @4
	.word	(irq_stk + IRQ_STK_SIZE)    @8
	.word	(SRAM_SIZE)		    @12  stack end
	.word	(_start_bss_)               @16
	.word	(_end_bss_)                 @20
	.word	(0x80060c)                  @24
	.word	_ictag_start_               @28		@ IC tag start
	.word	_ictag_end_	            	@32		@ IC tag end
	.word	_retention_data_size_div_256_ @36
	.word	_retention_data_size_align_256_ @40
    .word   (_retention_data_end_)      @44
    .word   (_rstored_)                 @48
    .word	(_retention_use_size_div_16_)     @52

DATA_I:
	.word	_dstored_					@0
	.word	_start_data_				@4
	.word	_end_data_					@8

#if FLL_STK_EN
FLL_D:
	.word	0xffffffff					@0
	.word	(_start_data_)				@4
	.word	(SRAM_SIZE)					@8
#endif

#if GPIO_DEBUG_EN
DEBUG_GPIO:
	.word	(0x80058a)                  @  PBx oen
#endif

BOOT_SEL_D:
	.word	(0x80063e)

FLASH_RECOVER:
	.word	(0x80000c)                  @0

	.align 4
__irq:
	tpush    	{r14}
	tpush    	{r0-r7}
	tmrss    	r0
	
	tmov		r1, r8
	tmov		r2, r9
	tmov		r3, r10
	tmov		r4, r11
	tmov		r5, r12
	tpush		{r0-r5}
	
	tjl      	irq_handler

	tpop		{r0-r5}
	tmov		r8, r1
	tmov		r9, r2
	tmov		r10,r3
	tmov		r11,r4
	tmov		r12,r5

	tmssr    	r0
	tpop		{r0-r7}
	treti    	{r15}

ASMEND:

	.section .bss
	.align 4
	.lcomm irq_stk, IRQ_STK_SIZE
	.end