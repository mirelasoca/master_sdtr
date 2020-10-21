/*
 * ltc6804.c
 *
 * Created: 10/19/2020 3:39:07 PM
 *  Author: Mirela
 */ 
#include "ltc6804.h"

const  uint8_t PEC0_WRCFG = 0b01001101;
const  uint8_t PEC1_WRCFG = 0b01111010;
const   uint8_t PEC0_RDCFG=0b01011011;
const uint8_t PEC1_RDCFG=0b00011110;

const uint8_t PEC0_WRCFG0 = 0b00111101;
const uint8_t PEC1_WRCFG0 = 0b01101110;

const uint8_t CFGR0 = 0b00000110;
const uint8_t CFGR1 = 0b01010010;
const uint8_t CFGR2 = 0b10000111;
const uint8_t CFGR3 = 0b10111011;
const uint8_t CFGR4[7]={0x00, 0b00000010,0b00000100,0b01000000,0b10000000,0b11000000, 0b00000110};
const uint8_t CFGR5[3]= {0x00,0b00010000, 0b00100000};
const uint8_t PEC0_CFG[15] = {0b11110000,0b00110000,0b00010111,0b10010011, 0b11011011, 0b10001100, 0b11100011,0b11011101, 0b11111010, 0b01111110,0b00110110, 0b01100001,0b00001110};
const uint8_t PEC1_CFG[15]= {0b00011110, 0b10011010,0b01010110, 0b00111010,0b00100100, 0b11111100,0b00101110, 0b11101000,0b00100100,0b01010110,0b10001110, 0b01011100};
const uint8_t ADCV1[4]={ 0b01100000, 0b01100001, 0b01100010, 0b01100011 };
const uint8_t PEC0_ADCV1[4] ={ 0b10000100, 0b00001111, 0b00011001, 0b10010010 };
const uint8_t PEC1_ADCV1[4] = {0b01111000, 0b01001010, 0b00101110, 0b00011100};

unsigned int vcells[4]={0,0,0,0};

unsigned int vmin=0;
 const uint8_t CMD0_WRCFG = 0b10000000;
 const uint8_t CMD1_WRCFG = 0b00000001;
 const uint8_t CMD0_RDCFG = 0b10000000;
 const uint8_t CMD1_RDCFG = 0b00000010;
 const uint8_t ADCV0 =0b10000011;
const uint8_t CMD0_ADSTAT = 0b10000101;
const uint8_t CMD1_ADSTAT = 0b01101010;
const uint8_t PEC0_ADSTAT = 0b11010110;
const uint8_t PEC1_ADSTAT = 0b11101100;

uint8_t RDSTATA[4] = {CMD0_RDSTATA, CMD1_RDSTATA, PEC0_RDSTATA, PEC1_RDSTATA };

uint8_t vref=0;
void write_read_config(CELL_DISCHARGE_t cells, TIMEOUT_t mins , PECFG_t pec ) 
{
	//SPI_SStoggle();
	SPI_PORT &= ~(1<<pin_SS);

	SPI_MasterTransfer(CMD0_WRCFG); // write command
	SPI_MasterTransfer(CMD1_WRCFG);
	SPI_MasterTransfer(PEC0_WRCFG);
	SPI_MasterTransfer(PEC1_WRCFG);
	SPI_MasterTransfer(CFGR0);  // registers values follow in order
	SPI_MasterTransfer(CFGR1);
	SPI_MasterTransfer(CFGR2);
	SPI_MasterTransfer(CFGR3);
	SPI_MasterTransfer(CFGR4[cells]);
	SPI_MasterTransfer(CFGR5[mins]);
	SPI_MasterTransfer(PEC0_CFG[pec]);
	SPI_MasterTransfer(PEC1_CFG[pec]);

	//SPI_SStoggle();
	SPI_PORT |= (1<<pin_SS);
	_delay_ms(3);
}
void wakeup(void) {
	SPI_MasterDisable();
	SPI_PORT |= (1<<pin_SCK);
	SPI_PORT |= (1<<pin_SS);
	_delay_ms(10);
	SPI_PORT &= ~ (1<<pin_SS);
	_delay_us(1);
	SPI_PORT &= ~ (1<<pin_SCK);
	_delay_us(10);
	SPI_MasterInit();
	SPI_MasterTransfer(0x00);
	SPI_PORT |= (1<<pin_SS);
	_delay_ms(3);
}

void adcv(MEASURE_t cell) {
	
	//SPI_SStoggle();
	SPI_PORT &= ~(1<<pin_SS);
	SPI_MasterTransfer(ADCV0);
	SPI_MasterTransfer(ADCV1[cell]);
	SPI_MasterTransfer(PEC0_ADCV1[cell]);
	SPI_MasterTransfer(PEC1_ADCV1[cell]);
	_delay_ms(3);
	//SPI_SStoggle();
	SPI_PORT |= (1<<pin_SS);
	_delay_ms(1);
}
void readstat(const uint8_t READ[4], uint8_t* data_read) {
	
	//SPI_SStoggle();
	SPI_PORT &= ~(1<<pin_SS);

	SPI_MasterTransfer(READ[0]); // sends CMD0 specific to the register
	SPI_MasterTransfer(READ[1]); // sends CMD1 specific to the register
	SPI_MasterTransfer(READ[2]); // sends PEC0 specific to the register
	SPI_MasterTransfer(READ[3]); // sends PEC1 specific to the register
	
	//SPI_SStoggle();
	//SPI_PORT |= (1<<pin_SS);

	data_read[0]=SPI_MasterTransfer(0); // reads the registers in order and the PEC
	//PORTB^=(1<<PB7);
	data_read[1]=SPI_MasterTransfer(0);
	data_read[2]=SPI_MasterTransfer(0);
	data_read[3]=SPI_MasterTransfer(0);
	data_read[4]=SPI_MasterTransfer(0);
	data_read[5]=SPI_MasterTransfer(0);
	data_read[6]=SPI_MasterTransfer(0);
	data_read[7]=SPI_MasterTransfer(0);
	_delay_ms(1);
	//SPI_MasterInit();
	SPI_PORT |= (1<<pin_SS);

}
unsigned int calculate_voltage( uint8_t reg1, uint8_t reg2) {
	unsigned int vcell = reg2<<8;
	return (unsigned int) vcell + reg1;
}

void calculate_min(void){
	vmin= vcells[cell2-1]<vcells[cell3-1]?vcells[cell2-1]:vcells[cell3-1]; //
	vmin= vcells[cell7-1]<vmin? vcells[cell7-1]:vmin;
	vmin= vcells[cell8-1]<vmin?vcells[cell8-1]:vmin;
	if(vcells[cell8-1]==vmin)
		vref=cell8-1;
	if(vcells[cell7-1]==vmin)
		vref=cell7-1;
	if(vcells[cell3-1]==vmin)
		vref=cell7-1;
	if(vcells[cell2-1]==vmin)
		vref=cell2-1;
} // a -1 is used because the readings of the cells are stored in the array starting at vcells[0] for cell 1 and so on