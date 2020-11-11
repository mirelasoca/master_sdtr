#include <SPI.h>
#include <Wire.h>
#include <stdint.h>
// set pin 10 as the slave select for the digital pot:
const int slaveSelectPin = 10;
const byte CMD0_ADSTAT = 0b10000101;  
const byte CMD1_ADSTAT = 0b01101010;  
const byte CMD0_RDSTATA = 0b10000000;  
const byte CMD1_RDSTATA = 0b00010000;  
const byte CMD0_RDCVA = 0b10000000;  
const byte CMD1_RDCVA = 0b00000100; 
const byte CMD0_RDCVC = 0b10000000;  
const byte CMD1_RDCVC = 0b00001000;
const byte CMD0_WRCFG = 0b10000000;  
const byte CMD1_WRCFG = 0b00000001;   
const byte CMD0_RDCFG = 0b10000000;  
const byte CMD1_RDCFG = 0b00000010;  
const byte ADCV0 =0b10000011;
int32_t vcells[4]={0,0,0,0};
unsigned int temp=0;
int current=0;
int current1=0;
int current2=0;
int current3=0;
int current4=0;
unsigned int vmin=0;
int updating = 0;
const byte PEC0_ADSTAT = 0b11010110;  
const byte PEC1_ADSTAT = 0b11101100;  
const byte PEC0_RDSTATA = 0b10011101;  
const byte PEC1_RDSTATA = 0b01100110; 
const byte PEC0_RDCVA =0b01110111;  
const byte PEC1_RDCVA= 0b11010110; 
const byte PEC0_RDCVC =0b00101110;  
const byte PEC1_RDCVC= 0b01000110;
const byte PEC0_WRCFG = 0b01001101;  
const byte PEC1_WRCFG = 0b01111010; 
const byte PEC0_RDCFG=0b01011011;
const byte PEC1_RDCFG=0b00011110;  

const byte PEC0_WRCFG0 = 0b00111101;  
const byte PEC1_WRCFG0 = 0b01101110; 

const byte CFGR0 = 0b00000110;
const byte CFGR1 = 0b01010010;
const byte CFGR2 = 0b10000111;
const byte CFGR3 = 0b10111011;
const byte CFGR4[7]={0x00, 0b00000010,0b00000100,0b01000000,0b10000000,0b11000000, 0b00000110};
const byte CFGR5[3]= {0x00,0b00010000, 0b00100000};
const byte PEC0_CFG[15] = {0b11110000,0b00110000,0b00010111,0b10010011, 0b11011011, 0b10001100, 0b11100011,0b11011101, 0b11111010, 0b01111110,0b00110110, 0b01100001,0b00001110};
const byte PEC1_CFG[15]= {0b00011110, 0b10011010,0b01010110, 0b00111010,0b00100100, 0b11111100,0b00101110, 0b11101000,0b00100100,0b01010110,0b10001110, 0b01011100};
const byte ADCV1= 0b01100000;
const byte PEC0_ADCV1[4] ={ 0b10000100, 0b00001111, 0b00011001, 0b10010010 };
const byte PEC1_ADCV1[4] = {0b01111000, 0b01001010, 0b00101110, 0b00011100};
const byte RDSTATA[4] = {CMD0_RDSTATA, CMD1_RDSTATA, PEC0_RDSTATA, PEC1_RDSTATA };
const byte RDCVA[4] = {CMD0_RDCVA, CMD1_RDCVA, PEC0_RDCVA, PEC1_RDCVA };
const byte RDCVC[4] = {CMD0_RDCVC, CMD1_RDCVC, PEC0_RDCVC, PEC1_RDCVC };

unsigned int vref=0;

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

typedef struct command_struct {
  uint8_t cmd0;
  uint8_t cmd1;
  uint16_t pec;
  }command_struct_t;
typedef union command_message{
  command_struct_t asStruct;
  uint8_t asArray[4];
  }command_message_t;
typedef struct configg_struct {
  uint8_t cfgr0;
  uint8_t cfgr1;
  uint8_t cfgr2;
  uint8_t cfgr3;
  uint8_t cfgr4;
  uint8_t cfgr5;
  uint16_t pec;
}configg_struct_t;
typedef union configg_message{
  configg_struct_t asStruct;
  uint8_t asArray[8];
}configg_message_t;
typedef enum command_type {
  WRCFGt = 0,
  RDCFGt,
  ADCVt,
  RDCVAt,
  RDCVCt,
  }command_type_t;
void set_command(command_message_t * command, command_type_t c_type);
void set_configg( configg_message_t * configg, uint8_t config4, TIMEOUT_t mins);  
void write_read(configg_message_t* configg, command_message_t* command, uint8_t* data_read );
void receiveEvent(int howMany);
void wakeup(void);
uint8_t cfg4_old = 11;
unsigned int calculate_voltage( uint8_t reg1, uint8_t reg2);
void calculate_min(void);
uint16_t calculate_pec(uint8_t* datagram, uint8_t dimension);
void send_i2cData(void);

 command_message_t W_command;
 command_message_t rca_command;
 command_message_t rcc_command;
 command_message_t adcv_command;
 configg_message_t my_configg;
 
  uint8_t data_read1[8]= {0};
  uint8_t data_read2[8]= {0};
  uint8_t incoming_i2c[8] = {0};
void setup() {
  // put your setup code here, to run once:
  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH);
  pinMode(12, INPUT);   // configguring SPI pins
  pinMode(11, OUTPUT);
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  set_command(&W_command, WRCFGt);
  set_command(&rcc_command, RDCVCt);
  set_command(&rca_command, RDCVAt);
  set_command(&adcv_command, ADCVt);
  // initialize SPI:
  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV8);
  Wire.setClock(100000);
 // Wire.begin(); 
  //Wire.onReceive(receiveEvent);

  Serial.begin(9600);
      delay(500);
      Wire.begin(24);
      Wire.onReceive(receiveEvent);
      Wire.onRequest(send_i2cData);
  wakeup();
    write_read(0, &adcv_command, 0);
    delay(50);
    wakeup();
    write_read(0, &rca_command, data_read1);
   
    vcells[cell2-1]= calculate_voltage(data_read1[2], data_read1[3]); // assembly the values
    vcells[cell3-1]= calculate_voltage(data_read1[4], data_read1[5]);

    wakeup();
    write_read(0, &rcc_command, data_read2);
    vcells[cell8-1]= calculate_voltage(data_read2[2], data_read2[3]);
   vcells[cell7-1]= calculate_voltage(data_read2[0], data_read2[1]);
}

void loop() {
  // put your main code here, to run repeatedly:
  //delay(500);
  //Wire.onReceive(receiveEvent);
  //Wire.onRequest(send_i2cData);
  

    
 
  
    //cfg4_old= incoming_i2c[0] ;
    //set_configg(&my_configg, 0, halfmin);
    //command_message_t
    
    
  

//    set_command(&command, WRCFGt);
//    set_configg(&my_configg, cell8, halfmin);
//    wakeup();
//    write_read(&my_configg, &command, 0);
//    delay(1000);
//
//    set_configg(&my_configg, cell7, halfmin);
//    wakeup();
//    write_read(&my_configg, &command, 0);
//    delay(1000);
//
//    set_configg(&my_configg, cell2, halfmin);
//    wakeup();
//    write_read(&my_configg, &command, 0);
//    delay(1000);
//
//    set_configg(&my_configg, cell3, halfmin);
//    wakeup();
//    write_read(&my_configg, &command, 0);
//    delay(1000);
//
   

}
void send_i2cData(void)
{
   //Wire.beginTransmission(10);
   for (int i = 0; i<4 ; i++)
   {
    Wire.write(vcells[i]>>8);
    Wire.write(vcells[i]&0xff);
//    Serial.print("Sending ");
//    Serial.print(vcells[i], HEX);
//    Serial.println("...");
   }
   //Wire.endTransmission();
}
void receiveEvent(int howMany)
{
  int i = 0;


  while(Wire.available())
  {
    incoming_i2c[i] = Wire.read();
    i++;
//    Serial.print("Got ");
//    Serial.println(incoming_i2c[i-1]);
  }
  if(incoming_i2c[5] !=0)
  {
    wakeup();
    write_read(0, &adcv_command, 0);
    delay(50);
    wakeup();
    write_read(0, &rca_command, data_read1);
   
    vcells[cell2-1]= calculate_voltage(data_read1[2], data_read1[3]); // assembly the values
    vcells[cell3-1]= calculate_voltage(data_read1[4], data_read1[5]);

    wakeup();
    write_read(0, &rcc_command, data_read2);
    vcells[cell8-1]= calculate_voltage(data_read2[2], data_read2[3]);
   vcells[cell7-1]= calculate_voltage(data_read2[0], data_read2[1]);
    Serial.print(vcells[0]);
    Serial.print(",");
    Serial.print(vcells[1]);
    Serial.print(",");
    Serial.print(vcells[2]);
    Serial.print(",");
    Serial.print(vcells[3]);
    Serial.println(",");
  }
 if((vcells[1]<40200)||(incoming_i2c[5]==0))
  {
  wakeup();
   // write_read(&my_configg, &W_command, 0);
   //incoming_i2c[4] =0;
   write_read((configg_message_t*)incoming_i2c, &W_command, 0); 
   }
}
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
      command->asStruct.cmd1= ADCV1;
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
  if(command!=0) // write
  {
    uint16_t pec = calculate_pec(command->asArray, 2);
    command->asArray[2] = pec>>8;
    command->asArray[3] = pec &0xFF;
    /*if(pec>>8== PEC0_WRCFG)
    {
      PORTB|=(1<<7);
    }*/
    
  }

}
void set_configg(configg_message_t * configg, uint8_t config4, TIMEOUT_t mins)
{
  configg->asStruct.cfgr0= CFGR0;
  configg->asStruct.cfgr1= CFGR1;
  configg->asStruct.cfgr2= CFGR2;
  configg->asStruct.cfgr3= CFGR3;
  configg->asStruct.cfgr4= config4;
  configg->asStruct.cfgr5= CFGR5[mins];
  uint16_t pec = calculate_pec(configg->asArray, 6);
  configg->asArray[6] = pec>>8;
  configg->asArray[7] = pec &0xFF;
  /*if((configg->asStruct.pec>>8)==PEC0_CFG[cell7halfmin])
  {
    PORTB|=(1<<7);
  }*/
}
void write_read(configg_message_t* configg, command_message_t* command, uint8_t* data_read )
{
  digitalWrite(10, LOW);
  for(int i=0; i<4; i++)
  {
    SPI.transfer(command->asArray[i]);
  }
  if(data_read==0) 
  {
    if(configg != 0)
    {
      for(int i=0; i<8; i++)
      {
        SPI.transfer(configg->asArray[i]);
      }
    
    //PORTB|=(1<<7);
    }
  }
  else
  {
    for(int i=0; i<8; i++)
    {
      data_read[i]= SPI.transfer(0);
    }
  }
  digitalWrite(10, HIGH);
  delay(3);
}
void wakeup(void) {
  SPI.endTransaction();
  digitalWrite(13, HIGH);
  digitalWrite(10, HIGH);
  delay(10);
  digitalWrite(10, LOW);
  delayMicroseconds(1);
  digitalWrite(13, LOW);
  delayMicroseconds(10);
  SPI.beginTransaction(SPISettings(10000, MSBFIRST, SPI_MODE3));
  SPI.transfer(0x00);
  digitalWrite(10, HIGH);
  delay(3);
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
uint16_t calculate_pec(uint8_t* datagram, uint8_t dimension)
{
  uint8_t bytee;
  uint16_t pec = 0;
  bool bitt, IN0, IN3, IN4, IN7, IN8, IN10, IN14;
  bool PEC[16] = {0};
  PEC[4] = 1;
  for(int i=0;i<dimension;i++)
  {
    bytee = datagram[i];
    
    for(int j=0;j<8;j++)
    {
      bitt = (bool)((bytee& 0x80)>>7);
      IN0 = bitt ^ PEC[14];
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
      bytee= bytee<<1;
    }
  }
  for(int i=14;i>=0;i--)
  {
    pec = pec | PEC[i];
    pec = pec<<1;
  }

  return pec;
}
