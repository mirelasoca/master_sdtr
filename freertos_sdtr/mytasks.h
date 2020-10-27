/*
 * mytasks.h
 *
 * Created: 7/5/2011 23:45:37
 *  Author: MMM
 */ 


#ifndef MYTASKS_H_
#define MYTASKS_H_
uint8_t i2c_rcells[10];
uint8_t i2c_discells[8];
const uint8_t i2c_nodis[8];

uint8_t txtt[16];

void vLEDFlashTask( void *pvParameters );
void vButtonCheckTask( void *pvParameters );
void vLCDUpdateTask( void *pvParameters );
void vLTCupdateDischarge( void *pvParameters );
void vLTCreadCells( void *pvParameters );
void vLTCsendDischarge( void *pvParameters );
#endif /* MYTASKS_H_ */