/********************************************************************************************************
 * @file    blm_host.h
 *
 * @brief   This is the header file for BLE SDK
 *
 * @author  BLE GROUP
 * @date    06,2020
 *
 * @par     Copyright (c) 2020, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *
 *          Licensed under the Apache License, Version 2.0 (the "License");
 *          you may not use this file except in compliance with the License.
 *          You may obtain a copy of the License at
 *
 *              http://www.apache.org/licenses/LICENSE-2.0
 *
 *          Unless required by applicable law or agreed to in writing, software
 *          distributed under the License is distributed on an "AS IS" BASIS,
 *          WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *          See the License for the specific language governing permissions and
 *          limitations under the License.
 *
 *******************************************************************************************************/
#ifndef APP_HOST_H_
#define APP_HOST_H_

#include "../feature_config.h"

#if (FEATURE_TEST_MODE == TEST_MD_MASTER)


int controller_event_callback (u32 h, u8 *p, int n);
int app_l2cap_handler (u16 conn_handle, u8 *raw_pkt);
int app_host_smp_finish (void);


extern u32 host_update_conn_param_req;
extern int central_smp_pending;

extern int central_pairing_enable;
extern int central_unpair_enable;


#endif  //end of (FEATURE_TEST_MODE == ...)

#endif /* APP_HOST_H_ */


