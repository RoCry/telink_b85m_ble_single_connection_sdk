/********************************************************************************************************
 * @file    user_config.c
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
#include "user_config.h"

#if PM_DEEPSLEEP_RETENTION_ENABLE
asm(".equ __PM_DEEPSLEEP_RETENTION_ENABLE,	1");
#else
asm(".equ __PM_DEEPSLEEP_RETENTION_ENABLE,	0");
#endif
asm(".global __PM_DEEPSLEEP_RETENTION_ENABLE");
