/********************************************************************************************************
 * @file	smp_central.h
 *
 * @brief	This is the header file for BLE SDK
 *
 * @author	BLE GROUP
 * @date	06,2020
 *
 * @par		Copyright (c) 2020, Telink Semiconductor (Shanghai) Co., Ltd.
 *			All rights reserved.
 *
 *          The information contained herein is confidential property of Telink
 *          Semiconductor (Shanghai) Co., Ltd. and is available under the terms
 *          of Commercial License Agreement between Telink Semiconductor (Shanghai)
 *          Co., Ltd. and the licensee or the terms described here-in. This heading
 *          MUST NOT be removed from this file.
 *
 *          Licensee shall not delete, modify or alter (or permit any third party to delete, modify, or
 *          alter) any information contained herein in whole or in part except as expressly authorized
 *          by Telink semiconductor (shanghai) Co., Ltd. Otherwise, licensee shall be solely responsible
 *          for any claim to the extent arising out of or relating to such deletion(s), modification(s)
 *          or alteration(s).
 *
 *          Licensees are granted free, non-transferable use of the information in this
 *          file under Mutual Non-Disclosure Agreement. NO WARRENTY of ANY KIND is provided.
 *
 *******************************************************************************************************/
/*
 * smp_central.h
 *
 *  Created on: 2018-12-4
 *      Author: Administrator
 */

#ifndef SMP_CENTRAL_H_
#define SMP_CENTRAL_H_



typedef struct {  //82
	u8		flag;
	u8		peer_addr_type;  //address used in link layer connection
	u8		peer_addr[6];

	u8 		peer_key_size;
	u8		peer_id_adrType; //peer identity address information in key distribution, used to identify
	u8		peer_id_addr[6];


	u8 		peer_ltk[16];      //peer_ltk[16]
	u8		random[8];  //8
	u16 	ediv;       //2
	u8 		rsvd[6];    //6

	u8		peer_irk[16];
	u8		peer_csrk[16];

}smp_m_param_save_t;




//  6 byte slave_MAC   8 byte rand  2 byte ediv
// 16 byte ltk
#define PAIR_INFO_SECTOR_SIZE	 				64

#define PAIR_OFFSET_SLAVE_MAC	 				2

#define PAIR_OFFSET_RAND		 				8
#define PAIR_OFFSET_EDIV		 				16
#define PAIR_OFFSET_ATT			 				18   //ATT handle
#define PAIR_OFFSET_LTK			 				32
#define PAIR_OFFSET_IRK			 				48

#if (LL_MASTER_MULTI_CONNECTION)
	#define	PAIR_SLAVE_MAX_NUM            			8
#else
	#define	PAIR_SLAVE_MAX_NUM            			1
#endif


typedef struct {
	u8 curNum;
	u8 curIndex;
	u8 isBond_fastSmp;
	u8 rsvd;  //auto smp, no need SEC_REQ
	u32 bond_flash_idx[PAIR_SLAVE_MAX_NUM];  //mark paired slave mac address in flash
	mac_adr_t bond_device[PAIR_SLAVE_MAX_NUM];
} bond_slave_t;





#define SLAVE_TRIGGER_SMP_FIRST_PAIRING				0   	//first pair, slave send security_request to trigger master's pairing&encryption
#define MASTER_TRIGGER_SMP_FIRST_PAIRING			BIT(0)

#define SLAVE_TRIGGER_SMP_AUTO_CONNECT				0   	//auto connect, slave send security_request to trigger master's encryption
#define MASTER_TRIGGER_SMP_AUTO_CONNECT				BIT(1)





/******************************* User Interface  *****************************************/
void    blm_host_smp_setSecurityTrigger(u8 trigger);
u8		blm_host_smp_getSecurityTrigger(void);




void 	blm_smp_configParingSecurityInfoStorageAddr (int addr);


void 	blm_host_smp_handler(u16 conn_handle, u8 *p);
int 	tbl_bond_slave_search(u8 adr_type, u8 * addr);
int 	tbl_bond_slave_delete_by_adr(u8 adr_type, u8 *addr);
void 	tbl_bond_slave_unpair_proc(u8 adr_type, u8 *addr);

void	blm_smp_encChangeEvt(u8 status, u16 connhandle, u8 enc_enable);









/************************* Stack Interface, user can not use!!! ***************************/


int 	blc_smp_central_init (void);


#endif /* SMP_CENTRAL_H_ */