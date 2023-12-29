/********************************************************************************************************
 * @file    drivers.h
 *
 * @brief   This is the header file for BLE SDK
 *
 * @author  BLE GROUP
 * @date    06,2022
 *
 * @par     Copyright (c) 2022, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#pragma once

#include "config.h"

#if(MCU_CORE_TYPE == MCU_CORE_825x)
	#include "drivers/8258/driver.h"
	#include "drivers/8258/driver_ext/driver_ext.h"
#elif(MCU_CORE_TYPE == MCU_CORE_827x)
	#include "drivers/8278/driver.h"
	#include "drivers/8278/driver_ext/driver_ext.h"
#else
	#error MCU core typr error !
#endif
