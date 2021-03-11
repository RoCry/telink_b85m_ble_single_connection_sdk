/********************************************************************************************************
 * @file	flash.c
 *
 * @brief	This is the source file for B85
 *
 * @author	Driver Group
 * @date	May 8,2018
 *
 * @par     Copyright (c) 2018, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#include "flash.h"
#include "spi_i.h"
#include "irq.h"
#include "timer.h"
#include "string.h"
#include "watchdog.h"


_attribute_data_retention_   _attribute_aligned_(4)	Flash_CapacityDef	flash_capacity;
/*******************************************************************************************************************
 *												Primary interface
 ******************************************************************************************************************/

/**
 * @brief		This function to determine whether the flash is busy..
 * @return		1:Indicates that the flash is busy. 0:Indicates that the flash is free
 */
_attribute_ram_code_sec_ static inline int flash_is_busy(){
	return mspi_read() & 0x01;		//the busy bit, pls check flash spec
}

/**
 * @brief		This function serves to set flash write command.
 * @param[in]	cmd	- set command.
 * @return		none.
 */
_attribute_ram_code_sec_noinline_ static void flash_send_cmd(unsigned char cmd){
	mspi_high();
	sleep_us(1);
	mspi_low();
	mspi_write(cmd);
	mspi_wait();
}

/**
 * @brief		This function serves to send flash address.
 * @param[in]	addr	- the flash address.
 * @return		none.
 */
_attribute_ram_code_sec_noinline_ static void flash_send_addr(unsigned int addr){
	mspi_write((unsigned char)(addr>>16));
	mspi_wait();
	mspi_write((unsigned char)(addr>>8));
	mspi_wait();
	mspi_write((unsigned char)(addr));
	mspi_wait();
}

/**
 * @brief     This function serves to wait flash done.(make this a asynchorous version).
 * @return    none.
 */
_attribute_ram_code_sec_noinline_ static void flash_wait_done(void)
{
	sleep_us(100);
	flash_send_cmd(FLASH_READ_STATUS_CMD_LOWBYTE);

	int i;
	for(i = 0; i < 10000000; ++i){
		if(!flash_is_busy()){
			break;
		}
	}
	mspi_high();
}

/**
 * @brief 		This function is used to read data from flash or read the status of flash.
 * @param[in]   cmd			- the read command.
 * @param[in]   addr		- starting address.
 * @param[in]   addr_en		- whether need to send an address.
 * @param[in]   dummy_cnt	- the length(in byte) of dummy.
 * @param[out]  data		- the start address of the data buffer.
 * @param[in]   data_len	- the length(in byte) of content needs to read out.
 * @return 		none.
 */
_attribute_ram_code_sec_noinline_ void flash_mspi_read_ram(unsigned char cmd, unsigned long addr, unsigned char addr_en, unsigned char dummy_cnt, unsigned char *data, unsigned long data_len)
{
	unsigned char r = irq_disable();

	flash_send_cmd(cmd);
	if(addr_en)
	{
		flash_send_addr(addr);
	}
	for(int i = 0; i < dummy_cnt; ++i)
	{
		mspi_write(0x00);		/* dummy */
		mspi_wait();
	}
	mspi_write(0x00);			/* to issue clock */
	mspi_wait();
	mspi_ctrl_write(0x0a);		/* auto mode */
	mspi_wait();
	for(int i = 0; i < data_len; ++i)
	{
		*data++ = mspi_get();
		mspi_wait();
	}
	mspi_high();

	irq_restore(r);
}

/**
 * @brief 		This function is used to write data or status to flash.
 * @param[in]   cmd			- the write command.
 * @param[in]   addr		- starting address.
 * @param[in]   addr_en		- whether need to send an address.
 * @param[out]  data		- the start address of the data buffer.
 * @param[in]   data_len	- the length(in byte) of content needs to read out.
 * @return 		none.
 */
_attribute_ram_code_sec_noinline_ void flash_mspi_write_ram(unsigned char cmd, unsigned long addr, unsigned char addr_en, unsigned char *data, unsigned long data_len)
{
	unsigned char r = irq_disable();

	// important:  buf must not reside at flash, such as constant string.  If that case, pls copy to memory first before write
	flash_send_cmd(FLASH_WRITE_ENABLE_CMD);
	flash_send_cmd(cmd);
	if(addr_en)
	{
		flash_send_addr(addr);
	}
	for(int i = 0; i < data_len; ++i)
	{
		mspi_write(data[i]);
		mspi_wait();
	}
	mspi_high();
	flash_wait_done();

	irq_restore(r);
}

/**
 * @brief 		This function serves to erase a sector.
 * @param[in]   addr	- the start address of the sector needs to erase.
 * @return 		none.
 */
void flash_erase_sector(unsigned long addr)
{
	flash_mspi_write_ram(FLASH_SECT_ERASE_CMD, addr, 1, NULL, 0);
}

/**
 * @brief 		This function reads the content from a page to the buf.
 * @param[in]   addr	- the start address of the page.
 * @param[in]   len		- the length(in byte) of content needs to read out from the page.
 * @param[out]  buf		- the start address of the buffer.
 * @return 		none.
 */
void flash_read_page(unsigned long addr, unsigned long len, unsigned char *buf)
{
	flash_mspi_read_ram(FLASH_READ_CMD, addr, 1, 0, buf, len);
}

/**
 * @brief 		This function writes the buffer's content to the flash.
 * @param[in]   addr	- the start address of the area.
 * @param[in]   len		- the length(in byte) of content needs to write into the flash.
 * @param[in]   buf		- the start address of the content needs to write into.
 * @return 		none.
 * @note        the funciton support cross-page writing,which means the len of buf can bigger than 256.
 */
void flash_write_page(unsigned long addr, unsigned long len, unsigned char *buf)
{
	unsigned int ns = PAGE_SIZE - (addr&(PAGE_SIZE - 1));
	int nw = 0;

	do{
		nw = len > ns ? ns :len;
		flash_mspi_write_ram(FLASH_WRITE_CMD, addr, 1, buf, nw);
		ns = PAGE_SIZE;
		addr += nw;
		buf += nw;
		len -= nw;
	}while(len > 0);
}

/**
 * @brief		This function reads the status of flash.
 * @param[in] 	cmd	- the cmd of read status.
 * @return 		the value of status.
 */
unsigned char flash_read_status(unsigned char cmd)
{
	unsigned char status = 0;
	flash_mspi_read_ram(cmd, 0, 0, 0, &status, 1);
	return status;
}

/**
 * @brief 		This function write the status of flash.
 * @param[in]  	type	- the type of status.8 bit or 16 bit.
 * @param[in]  	data	- the value of status.
 * @return 		none.
 */
void flash_write_status(flash_status_typedef_e type , unsigned short data)
{
	unsigned char buf[2];

	buf[0] = data;
	buf[1] = data>>8;
	if(type == FLASH_TYPE_8BIT_STATUS){
		flash_mspi_write_ram(FLASH_WRITE_STATUS_CMD_LOWBYTE, 0, 0, buf, 1);
	}else if(type == FLASH_TYPE_16BIT_STATUS_ONE_CMD){
		flash_mspi_write_ram(FLASH_WRITE_STATUS_CMD_LOWBYTE, 0, 0, buf, 2);
	}
}

/**
 * @brief	  	This function serves to read MID of flash(MAC id). Before reading UID of flash,
 * 				you must read MID of flash. and then you can look up the related table to select
 * 				the idcmd and read UID of flash.
 * @return    	MID of the flash(4 bytes).
 */
unsigned int flash_read_mid(void)
{
	unsigned int flash_mid = 0;
	flash_mspi_read_ram(FLASH_GET_JEDEC_ID, 0, 0, 0, (unsigned char*)(&flash_mid), 3);

	/*
	 * The mids of GD25LD80C and GD25LE80C are both 0x1460c8, but the status register of GD25LD80C is 8 bits,
	 * and the status register of GD25LE80C is 16 bits. The functions of the two chips are different.
	 * The software detection scheme is to write the 9th bit (0x0200) of the status register. If the write is successful,
	 * it is GD25LE80C, and the mid is changed to 0x011460c8. If it is unsuccessful, it is GD25LD80C.
	 */
	if(flash_mid == 0x001460c8)
	{
		unsigned short status = (flash_read_status(FLASH_READ_STATUS_CMD_LOWBYTE) | (flash_read_status(FLASH_READ_STATUS_CMD_HIGHBYTE) << 8));
		status |= 0x0200;
		flash_write_status(FLASH_TYPE_16BIT_STATUS_ONE_CMD, status);
		if(flash_read_status(FLASH_READ_STATUS_CMD_HIGHBYTE)&0x02)
		{
			flash_mid = 0x011460c8;
		}
	}

	return flash_mid;
}

/* according to your appliaction */
#if 0
/**
 * @brief 		This function serves to read data from the Security Registers of the flash.
 * @param[in]   addr	- the start address of the Security Registers.
 * @param[in]   len		- the length of the content to be read.
 * @param[out]  buf		- the starting address of the content to be read.
 * @return 		none.
 */
void flash_read_otp(unsigned long addr, unsigned long len, unsigned char* buf)
{
	flash_mspi_read_ram(FLASH_READ_SECURITY_REGISTERS_CMD, addr, 1, 1, buf, len);
}

/**
 * @brief 		This function serves to write data to the Security Registers of the flash you choose.
 * @param[in]   addr	- the start address of the Security Registers.
 * @param[in]   len		- the length of content to be written.
 * @param[in]   buf		- the starting address of the content to be written.
 * @return 		none.
 */
void flash_write_otp(unsigned long addr, unsigned long len, unsigned char *buf)
{
	unsigned int ns = PAGE_SIZE_OTP - (addr & (PAGE_SIZE_OTP - 1));
	int nw = 0;

	do{
		nw = len > ns ? ns :len;
		flash_mspi_write_ram(FLASH_WRITE_SECURITY_REGISTERS_CMD, addr, 1, buf, nw);
		ns = PAGE_SIZE_OTP;
		addr += nw;
		buf += nw;
		len -= nw;
	}while(len > 0);
}

/**
 * @brief 		This function serves to erase the data of the Security Registers that you choose.
 * @param[in]   addr	- the address that you want to erase.
 * @return 		none.
 * @Attention	Even you choose the middle area of the Security Registers,it will erase the whole area.
 */
void flash_erase_otp(unsigned long addr)
{
	flash_mspi_write_ram(FLASH_ERASE_SECURITY_REGISTERS_CMD, addr, 1, NULL, 0);
}

/**
 * @brief	  	This function serves to read MID of flash(MAC id).
 * @return    	MID of the flash(3 bytes).
 */
unsigned int flash_read_raw_mid(void)
{
	unsigned int flash_mid = 0;
	flash_mspi_read_ram(FLASH_GET_JEDEC_ID, 0, 0, 0, (unsigned char*)(&flash_mid), 3);
	return flash_mid;
}



/**
 * @brief	  	This function serves to read UID of flash.Before reading UID of flash, you must read MID of flash.
 * 				and then you can look up the related table to select the idcmd and read UID of flash.
 * @param[in] 	idcmd	- different flash vendor have different read-uid command. E.g: GD/PUYA:0x4B; XTX: 0x5A.
 * @param[in] 	buf		- store UID of flash.
 * @param[in] 	uidtype	- the number of uid bytes.
 * @return    	none.
 */
void flash_read_uid(unsigned char idcmd, unsigned char *buf, flash_uid_typedef_e uidtype)
{
	if(idcmd == FLASH_READ_UID_CMD_GD_PUYA_ZB_UT)	//< GD/PUYA/ZB/UT
	{
		flash_mspi_read_ram(idcmd, 0x00, 1, 1, buf, uidtype);
	}
	else if (idcmd == FLASH_XTX_READ_UID_CMD)		//< XTX
	{
		flash_mspi_read_ram(idcmd, 0x80, 1, 1, buf, uidtype);
	}
}

/*******************************************************************************************************************
 *												Secondary interface
 ******************************************************************************************************************/

/**
 * @brief		This function serves to read flash mid and uid,and check the correctness of mid and uid.
 * @param[out]	flash_mid	- Flash Manufacturer ID
 * @param[out]	flash_uid	- Flash Unique ID
 * @return		0: flash no uid or not a known flash model 	 1:the flash model is known and the uid is read.
 */
int flash_read_mid_uid_with_check(unsigned int *flash_mid ,unsigned char *flash_uid)
{
	unsigned char no_uid[16]={0x51,0x01,0x51,0x01,0x51,0x01,0x51,0x01,0x51,0x01,0x51,0x01,0x51,0x01,0x51,0x01};
	int i,f_cnt=0;
	unsigned char uid_8byte = 0;
	*flash_mid  = flash_read_mid();

	/*
	 * If add flash type, need pay attention to the read uid cmd and the bir number of status register
	   Flash Type	CMD			MID		Company
	   ZB25WD40B	0x4b	0x13325E	ZB
	   ZB25WD80B	0x4b	0x14325E	ZB
	   GD25LD05C	----	0x1060C8	GD
	   GD25LD40C	0x4b	0x1360C8	GD
	   GD25LD80C	----	0x1460C8	GD
	   GD25LE80C	0x4b	0x1460C8	GD
	   GD25LQ80C	0x4b	0x1460C8	GD
	   MD25D40D		----	0x134051	GD
	   P25Q40L		0x4b	0x136085	PUYA
	   TH25D40LA	0x4b	0x1360EB	UT
	   TH25D40UA	0x4b	0x1360EB	UT
	 */
	if((*flash_mid == 0x001460C8)||(*flash_mid == 0x011460C8)||(*flash_mid == 0x1060C8)||(*flash_mid == 0x134051)||(*flash_mid == 0x136085)||(*flash_mid == 0x1360C8)||(*flash_mid == 0x1360EB)||(*flash_mid == 0x14325E)){
		flash_read_uid(FLASH_READ_UID_CMD_GD_PUYA_ZB_UT,(unsigned char *)flash_uid, FLASH_TYPE_16BYTE_UID);
	}else if(*flash_mid==0x13325e){
		flash_read_uid(FLASH_READ_UID_CMD_GD_PUYA_ZB_UT,(unsigned char *)flash_uid, FLASH_TYPE_8BYTE_UID);
		uid_8byte = 1;
	}else{
		return 0;
	}

	if(0 == uid_8byte){
		for(i=0;i<16;i++){
			if(flash_uid[i] == no_uid[i]){
				f_cnt++;
			}
		}
	}else{
		memset(flash_uid+8,0,8);	//Clear the last eight bytes of a 16 byte array when the length of uid is 8.
	 }

	if(f_cnt == 16){	//no uid flash
		return 0;
	}else{
		return 1;
	}
}

#endif

void flash_set_capacity(Flash_CapacityDef flash_cap)
{
	flash_capacity = flash_cap;
}

Flash_CapacityDef flash_get_capacity(void)
{
	return flash_capacity;
}
