/*
 * ltc6804.c
 *
 * Created: 10/19/2020 3:39:07 PM
 *  Author: Mirela
 */ 
#include "ltc6804.h"
//#include <stdbool.h>
/*
const  uint8_t PEC0_WRCFG = 0b01001101;
const  uint8_t PEC1_WRCFG = 0b01111010;
const   uint8_t PEC0_RDCFG=0b01011011;
const uint8_t PEC1_RDCFG=0b00011110;

const uint8_t PEC0_WRCFG0 = 0b00111101;
const uint8_t PEC1_WRCFG0 = 0b01101110;
//const uint16_t PECW = 0b0011110101101110;
const uint8_t CFGR0 = 0b00000110;
const uint8_t CFGR1 = 0b01010010;
const uint8_t CFGR2 = 0b10000111;
const uint8_t CFGR3 = 0b10111011;
const uint8_t CFGR4[7]={0x00, 0b00000010,0b00000100,0b01000000,0b10000000,0b11000000, 0b00000110};
const uint8_t CFGR5[3]= {0x00,0b00010000, 0b00100000};
//const uint8_t PEC0_CFG[15] = {0b11110000,0b00110000,0b00010111,0b10010011, 0b11011011, 0b10001100, 0b11100011,0b11011101, 0b11111010, 0b01111110,0b00110110, 0b01100001,0b00001110};
//const uint8_t PEC1_CFG[15]= {0b00011110, 0b10011010,0b01010110, 0b00111010,0b00100100, 0b11111100,0b00101110, 0b11101000,0b00100100,0b01010110,0b10001110, 0b01011100};
const uint8_t ADCV1[4]={ 0b01100000, 0b01100001, 0b01100010, 0b01100011 };
const uint8_t ADCV11= 0b01100000;
const uint8_t PEC0_ADCV1[4] ={ 0b10000100, 0b00001111, 0b00011001, 0b10010010 };
const uint8_t PEC1_ADCV1[4] = {0b01111000, 0b01001010, 0b00101110, 0b00011100};
*/

uint16_t vcells[4]={0,0,0,0};

unsigned int vmin=0;
/*
 const uint8_t CMD0_WRCFG = 0b10000000;
 const uint8_t CMD1_WRCFG = 0b00000001;
 const uint8_t CMD0_RDCFG = 0b10000000;
 const uint8_t CMD1_RDCFG = 0b00000010;
 const uint8_t ADCV0 =0b10000011;
const uint8_t CMD0_ADSTAT = 0b10000101;
const uint8_t CMD1_ADSTAT = 0b01101010;
const uint8_t PEC0_ADSTAT = 0b11010110;
const uint8_t PEC1_ADSTAT = 0b11101100;

uint8_t RDSTATA[4] = {CMD0_RDSTATA, CMD1_RDSTATA, PEC0_RDSTATA, PEC1_RDSTATA };*/

unsigned int vref=0;
/*
void set_command(command_message_t * command, command_type_t c_type)
{
	switch(c_type)
	{
		case WRCFGt:
		{
			command->asStruct.cmd0= CMD0_WRCFG;
			command->asStruct.cmd1= CMD1_WRCFG;
			break;
		}
		
		case RDCFGt:
		{
			command->asStruct.cmd0= CMD0_RDCFG;
			command->asStruct.cmd1= CMD1_RDCFG;
			break;
		}
		
		case ADCVt:
		{
			command->asStruct.cmd0= ADCV0;
			command->asStruct.cmd1= ADCV11;
			break;
		}
		case RDCVCt:
		{
			command->asStruct.cmd0= CMD0_RDCVC;
			command->asStruct.cmd1= CMD1_RDCVC;
			break;
		}
		
		case RDCVAt:
		{
			command->asStruct.cmd0= CMD0_RDCVA;
			command->asStruct.cmd1= CMD1_RDCVA;
			break;
		}
		
		default:
		{
			command->asStruct.cmd0= CMD0_RDCVA;
			command->asStruct.cmd1= CMD1_RDCVA;
			break;
		}
		
	}

		uint16_t pec = calculate_pec(command->asArray, 2);
		command->asArray[2] = pec>>8;
		command->asArray[3] = pec &0xFF;
		/ *if(pec>>8== PEC0_WRCFG)
		{
			PORTB|=(1<<7);
		}* /
		
	

}
void set_config(config_message_t * config, CELL_DISCHARGE_t cells, TIMEOUT_t mins)
{
	config->asStruct.cfgr0= CFGR0;
	config->asStruct.cfgr1= CFGR1;
	config->asStruct.cfgr2= CFGR2;
	config->asStruct.cfgr3= CFGR3;
	config->asStruct.cfgr4= CFGR4[cells];
	config->asStruct.cfgr5= CFGR5[mins];
	uint16_t pec = calculate_pec(config->asArray, 6);
	config->asArray[6] = pec>>8;
	config->asArray[7] = pec &0xFF;
	/ *if((config->asStruct.pec>>8)==PEC0_CFG[cell7halfmin])
	{
		PORTB|=(1<<7);
	}* /
}
void write_read(config_message_t* config, command_message_t* command, uint8_t* data_read )
{
	SPI_PORT &= ~(1<<pin_SS);
	for(int i=0; i<4; i++)
	{
		SPI_MasterTransfer(command->asArray[i]);
	}
	if(data_read==0)
	{
		if(config!=0)
		{
			for(int i=0; i<8; i++)
			{
				SPI_MasterTransfer(config->asArray[i]);
			}
			//PORTB|=(1<<7);
			if((config->asArray[6])==PEC0_CFG[cell7halfmin])
			{
				PORTB|=(1<<7);
			}
		}
		
	}
	else
	{
		for(int i=0; i<8; i++)
		{
			data_read[i]= SPI_MasterTransfer(0);
		}
	}
	SPI_PORT |= (1<<pin_SS);
	_delay_ms(3);
}
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
void readstat( uint8_t* READ, uint8_t* data_read) {
	
	//SPI_SStoggle();
	SPI_PORT &= ~(1<<pin_SS);

	data_read[0]=SPI_MasterTransfer(READ[0]); // sends CMD0 specific to the register
	data_read[1]=SPI_MasterTransfer(READ[1]); // sends CMD1 specific to the register
	data_read[2]=SPI_MasterTransfer(READ[2]); // sends PEC0 specific to the register
	data_read[3]=SPI_MasterTransfer(READ[3]); // sends PEC1 specific to the register
	
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
	sprintf(txMessage, "\n\r\n\rEnd of read \n\r>>");
	abtSerTransmitData(&serialCom0, (uint8_t *)txMessage, strlen(txMessage), true, true);

}
unsigned int calculate_voltage( uint8_t reg1, uint8_t reg2) {
	unsigned int vcell = reg2<<8;
	return (unsigned int) vcell + reg1;
}*/

void calculate_min(void){
	vmin= vcells[cell2]<vcells[cell3]?vcells[cell2]:vcells[cell3]; //
	vmin= vcells[cell7]<vmin? vcells[cell7]:vmin;
	vmin= vcells[cell8]<vmin?vcells[cell8]:vmin;
	if(vcells[cell8]==vmin)
		vref=cell8;
	if(vcells[cell7]==vmin)
		vref=cell7;
	if(vcells[cell3]==vmin)
		vref=cell7;
	if(vcells[cell2]==vmin)
		vref=cell2;
} // a -1 is used because the readings of the cells are stored in the array starting at vcells[0] for cell 1 and so on
/*
uint16_t calculate_pec(uint8_t* datagram, uint8_t dimension)
{
	uint8_t byte;
	uint16_t pec = 0;
	bool bit, IN0, IN3, IN4, IN7, IN8, IN10, IN14;
	bool PEC[16] = {0};
	PEC[4] = 1;
	for(int i=0;i<dimension;i++)
	{
		byte = datagram[i];
		
		for(int j=0;j<8;j++)
		{
			bit = (bool)((byte& 0x80)>>7);
			IN0 = bit ^ PEC[14];
			IN3 = IN0 ^ PEC[2];
			IN4 = IN0 ^ PEC[3];
			IN7 = IN0 ^ PEC[6];
			IN8 = IN0 ^ PEC[7];
			IN10 = IN0 ^ PEC[9];
			IN14 = IN0 ^ PEC[13];
			PEC[14] = IN14;
			PEC[13] = PEC[12];
			PEC[12] = PEC[11];
			PEC[11] = PEC[10];
			PEC[10] = IN10;
			PEC[9] = PEC[8];
			PEC[8] = IN8;
			PEC[7] = IN7;
			PEC[6] = PEC [5];
			PEC[5] = PEC [4];
			PEC[4] = IN4;
			PEC[3] = IN3;
			PEC[2] = PEC[1];
			PEC[1] = PEC[0];
			PEC [0] = IN0;
			byte= byte<<1;
		}
	}
	for(int i=14;i>=0;i--)
	{
		pec = pec | PEC[i];
		pec = pec<<1;
	}
	/ *if((pec&0x1)==0)
	{
		PORTB|= (1<<7);
	}* /
	return pec;
}*/