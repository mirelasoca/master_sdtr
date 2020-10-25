/*
 * main_mcu_init.h
 *
 * Created: 01.01.2009
 * Author : Bogdan Tarnauca
 */

#ifndef _MAIN_MCU_INIT_H_
	#define _MAIN_MCU_INIT_H_

#include "main_config.h"

#include "avr_serial_com_hal.h"
#include "avr_sys_tick.h"


#include<avr/wdt.h>
#include<avr/interrupt.h>

/*!  @fn      init_data(void)
 *   @brief   Initialize any variables and/or data structures prior to their use.
 *   @return  none.
 */
void init_data(void);

/*!  @fn      watchdog_init(void)
 *   @brief   Initialize the watchdog. Do not forget to issue wdt_reset() to prevent the watchdog from biting :)
 *   @return  none.
 */
void watchdog_init(void);

/*!  @fn      port_init(void)
 *   @brief   Initialize port settings (direction, pull-ups)
 *   @return  none.
 */
void port_init(void);

/*!  @fn      timer0_init(void)
 *   @brief   Set-up timer0 as 10ms tick generator
 *   @return  none.
 */
void timer0_init(void);

/*!  @fn      init_devices(void)
 *   @brief   Initialize all required peripherals and/or connected devices
 *   @return  none.
 */
void init_devices(void);


#endif /* _MAIN_MCU_INIT_H_ */
 