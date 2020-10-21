/*
 * ltc6804.h
 *
 * Created: 10/19/2020 3:07:04 PM
 *  Author: Mirela
 */ 


#ifndef LTC6804_H_
#define LTC6804_H_

#include<avr/io.h>
#include "main_config.h"
#include <util/delay.h>
#include "spi.h"

 #define  CMD0_RDSTATA  0b10000000
 #define CMD1_RDSTATA  0b00010000
 #define CMD0_RDCVA  0b10000000
 #define CMD1_RDCVA  0b00000100
 #define CMD0_RDCVC  0b10000000
 #define CMD1_RDCVC  0b00001000

#define PEC0_RDSTATA  0b10011101
#define PEC1_RDSTATA  0b01100110
#define PEC0_RDCVA 0b01110111
#define PEC1_RDCVA 0b11010110
#define PEC0_RDCVC 0b00101110
#define PEC1_RDCVC 0b01000110

const  uint8_t PEC0_WRCFG;
const  uint8_t PEC1_WRCFG;
const   uint8_t PEC0_RDCFG;
const  uint8_t PEC1_RDCFG;

const uint8_t PEC0_WRCFG0; 
const uint8_t PEC1_WRCFG0; 

const uint8_t CFGR0;
const uint8_t CFGR1;
const uint8_t CFGR2;
const uint8_t CFGR3;
const uint8_t CFGR4[7];
const uint8_t CFGR5[3];
const uint8_t PEC0_CFG[15];
const uint8_t PEC1_CFG[15];
const uint8_t ADCV1[4];
const uint8_t PEC0_ADCV1[4];
const uint8_t PEC1_ADCV1[4];

unsigned int vcells[4];

unsigned int vmin;
const uint8_t CMD0_WRCFG;
const uint8_t CMD1_WRCFG;
const uint8_t CMD0_RDCFG;
const uint8_t CMD1_RDCFG;
const uint8_t ADCV0;
const uint8_t CMD0_ADSTAT;
const uint8_t CMD1_ADSTAT;
const uint8_t PEC0_ADSTAT;
const uint8_t PEC1_ADSTAT;

uint8_t RDSTATA[4];
uint8_t RDCVA[4];
uint8_t RDCVC[4];
uint8_t vref;

typedef enum MEASURE {
	allcells,
	celll7,
	celll2_8,
	celll3
} MEASURE_t;
typedef enum CELL_DISCHARGE { no_cell,
	cell2,
	cell3,
	cell7,
	cell8,
	cell2_3,
	cell7_8
} CELL_DISCHARGE_t; // cell 2, 3, 7 and 8 correspond to cells 1, 2, 3 and 4 from the battery, respective

typedef enum TIMEOUT {
	disabled,
	halfmin,
	onemin
}TIMEOUT_t;
typedef enum PECFG  {
	nodischarge,
	cell2halfmin,
	cell3halfmin,
	cell7halfmin,
	cell8halfmin,
	cell2_3halfmin,
	cell7_8halfmin,
	cell2_1min,
	cell3_1min,
	cell7_1min,
	cell8_1min,
	cell2_3_1min,
	cell7_8_1min
	
}PECFG_t;

void write_read_config(CELL_DISCHARGE_t cells, TIMEOUT_t mins , PECFG_t pec ) ;
void adcv(MEASURE_t cell);
void wakeup(void);
void readstat(const uint8_t READ[4], uint8_t* data_read);
unsigned int calculate_voltage( uint8_t reg1, uint8_t reg2);
void calculate_min(void);

#endif /* LTC6804_H_ */