/* ========================================
 *
 * Andrei Aldea 2019
 * IN-12 Nixie Clock Driver
 * Cypress PsoC 5 CYKIT8-59
 * Open Source Software
 * ========================================
*/
#include "project.h"

#define TIMER_CLOCK 24000000 // This value should match the clock input to the Timer

volatile uint16 ms_count = 0;

void delay (uint16 ms);
void update_disp(void);

CY_ISR(MY_ISR) {
    ms_count++;
}

#define tube_off 10
#define ind_off 3

uint32_t decimal_lookup[11] = {0x0001, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080, 0x0100, 0x0200, 0x0000}; // decimal_lookup[0]...decimal_lookup[9] outputs corresponding nixie number. decimal_lookup[10] is off.
uint32_t ind_lookup[4] = {0x01, 0x02, 0x03, 0x00}; // ind_lookup[0] is top indicator, ind_lookup[1] is bottom indicator, ind_lookup[2] is both, ind_lookup[3] is off.

uint32_t data1 = 0; // Used to store data to be shifted
uint32_t data2 = 0;
uint32_t data3 = 0;

uint8_t  tube1 = 0; // Value to display on the corresponding tube.
uint8_t  tube2 = 0;
uint8_t  tube3 = 0;
uint8_t  tube4 = 0;
uint8_t  tube5 = 0;
uint8_t  tube6 = 0;

uint8_t indicator1 = 0; // Value to display on the corresponding indicator. 
uint8_t indicator2 = 0;
uint8_t indicator3 = 0;

uint8_t output;

uint8_t seconds;
uint8_t minutes;
uint8_t hours;

uint8_t second1;
uint8_t second2;
uint8_t minute1;
uint8_t minute2;
uint8_t hour1;
uint8_t hour2;

//DS3231 i2c addressは0X68
#define sub_ad      0x68

uint8   txbuf[]={0x00};     
uint8   rxbuf[8];         

uint8 initbuf[]={0x00, // Register start address '16 -12-21 18:00:00
    
    0x00, // second
    0x05, // min
    0x10, // hour 18 = 18 hour
    0x02, // Day of the week 00 = sunday
    0x21, // day 21 = 21 days
    0x10, // month 12 = December
    0x19, // year 16 = 16 year
           };

void readRTC(void);
void writeUARTtime(void);

int main(void)
{
  Timer_1_Start(); // Configure and enable timer
  isr_1_StartEx(MY_ISR); // Point to MY_ISR to carry out the interrupt sub-routine
  SPIM_1_Start();
  UART_1_Start();
  I2C_1_Start();
  PWM_1_Start();
  ADC_DelSig_1_Start();
  PWM_1_WriteCompare(100); //Initial brightness
    
  /* Start the ADC conversion */
  ADC_DelSig_1_StartConvert();
    
    
  CyGlobalIntEnable; /* Enable global interrupts. */
  
  tube1 = tube_off; //Start Tubes Off
  tube2 = tube_off;
  tube3 = tube_off;
  tube4 = tube_off;
  tube5 = tube_off;
  tube6 = tube_off;
  indicator1 = ind_off;
  indicator2 = ind_off;
  indicator3 = ind_off;
  update_disp();
  delay(1000);

  I2C_1_MasterWriteBuf(sub_ad,initbuf,8,I2C_1_MODE_COMPLETE_XFER ); //Set Current Time
  while(0u == (I2C_1_MasterStatus() & I2C_1_MSTAT_WR_CMPLT));      //Wait for write to complete

  for(;;)
  {
    readRTC();
    writeUARTtime();
    
    //Read ADC
    if(ADC_DelSig_1_IsEndConversion(ADC_DelSig_1_RETURN_STATUS))
        {
            output = abs(ADC_DelSig_1_GetResult32());
            
            /* Saturate ADC result to positive numbers and limit brightness to acceptable values */
            if(output < 30)
            {
                output = 30;
            }  
            if(output > 240)
            {
                output = 240;
            }
        }
        
    tube1 = (rxbuf[0] & 0x7f) % 10; //Separate Digits from RX Buffer to Nixie
    tube2 = (rxbuf[0] & 0x7f) / 10;
    tube3 = (rxbuf[1] & 0x7f) % 10;
    tube4 = (rxbuf[1] & 0x7f) / 10;
    tube5 = (rxbuf[2] & 0x3f) % 10;
    tube6 = (rxbuf[2] & 0x3f) / 10;
    indicator1 = 1;
    indicator2 = 1;
    indicator3 = 1;
    PWM_1_WriteCompare(output); //Write ADC Value to Brightness PWM Equivalent
    update_disp(); //Update Display
    delay(20); //Small Delay
  }
}

void update_disp(void){
  data1 = 0x00000000;
  data1 |= (((decimal_lookup[tube1]))&0x000003FF);
  data1 |= (((decimal_lookup[tube2])<<10)&0x000FFC00); 
  data1 |= (((ind_lookup[indicator1])<<20)&0x00300000);
  
  data2 = 0x00000000;
  data2 |= (((decimal_lookup[tube3]))&0x000003FF);
  data2 |= (((decimal_lookup[tube4])<<10)&0x000FFC00); 
  data2 |= (((ind_lookup[indicator2])<<20)&0x00300000);

  data3 = 0x00000000;
  data3 |= (((decimal_lookup[tube5]))&0x000003FF);
  data3 |= (((decimal_lookup[tube6])<<10)&0x000FFC00); 
  data3 |= (((ind_lookup[indicator3])<<20)&0x00300000); 

  LD_Write(1);
  SPIM_1_WriteTxData(data1>>24);
  SPIM_1_WriteTxData(data1>>16);
  SPIM_1_WriteTxData(data1>>8);
  SPIM_1_WriteTxData(data1);
  LD_Write(0);
  //delay(1);
  LD_Write(1);
  
  LD_Write(1);
  SPIM_1_WriteTxData(data2>>24);
  SPIM_1_WriteTxData(data2>>16);
  SPIM_1_WriteTxData(data2>>8);
  SPIM_1_WriteTxData(data2);
  LD_Write(0);
  //delay(1);
  LD_Write(1);
  
  LD_Write(1);
  SPIM_1_WriteTxData(data3>>24);
  SPIM_1_WriteTxData(data3>>16);
  SPIM_1_WriteTxData(data3>>8);
  SPIM_1_WriteTxData(data3);
  LD_Write(0);
  //delay(1);
  LD_Write(1);
}

void readRTC(void)
{
    I2C_1_MasterWriteBuf(sub_ad,txbuf,1,I2C_1_MODE_COMPLETE_XFER );   // Send read start
    while(0u == (I2C_1_MasterStatus() & I2C_1_MSTAT_WR_CMPLT));      // Waiting for writing
    I2C_1_MasterClearStatus();                                       //Clear Status
    I2C_1_MasterReadBuf(sub_ad,rxbuf,7,I2C_1_MODE_COMPLETE_XFER );   // Read time
    while(0u == (I2C_1_MasterStatus() & I2C_1_MSTAT_RD_CMPLT));      // Waiting for reception complete
    I2C_1_MasterClearStatus();                                       // Clear status
}

void writeUARTtime(void)
{
    char result[50]; //String buffer 
    
    UART_1_PutString("\n");
    sprintf(result, "%u", rxbuf[6]); 
    UART_1_PutString(result);
    UART_1_PutString("-");                
    sprintf(result, "%u", rxbuf[5]& 0x1f); 
    UART_1_PutString(result);
    UART_1_PutString("-");          
    sprintf(result, "%u", rxbuf[4] & 0x3f); 
    UART_1_PutString(result);
    UART_1_PutString(" \n");         
     switch (rxbuf[3] & 0x07 )
       {
      case 0:UART_1_PutString("SUN");break;                       
      case 1:UART_1_PutString("MON");break;
      case 2:UART_1_PutString("TUE");break;
      case 3:UART_1_PutString("WEN");break;
      case 4:UART_1_PutString("THE");break;
      case 5:UART_1_PutString("FRI");break;
      case 6:UART_1_PutString("SAT");break;
      break;
      }  
   UART_1_PutString("\n");    
   sprintf(result, "%d", rxbuf[2] & 0x3f); 
   UART_1_PutString(result);
   UART_1_PutString(":");
   sprintf(result, "%d", rxbuf[1] & 0x7f); 
   UART_1_PutString(result);
   UART_1_PutString(":");
   sprintf(result, "%d", rxbuf[0] & 0x7f); 
   UART_1_PutString(result);
   UART_1_PutChar('\n');
}

void delay (uint16 ms) {
    Timer_1_Enable(); // start the timeout counter
    while(ms_count < ms){}
    Timer_1_Stop();
    ms_count = 0;
}

//--The Following are unused test  lines-- 

    /*
  // Fade Out
  for(uint8_t i = 150; i!=0; i-=1){
    PWM_1_WriteCompare(i);
    delay(40);
  }

  // Fade In
  for(uint8_t i = 0; i!=150; i+=1){
    PWM_1_WriteCompare(i);
    delay(40);
  }
  PWM_1_WriteCompare(150);
    */  

  /*
  indicator1 = 2; // Both indicators on 
  indicator2 = 2;
  indicator3 = ind_off;
  for(uint8_t i = 0; i<10; i++){
    tube1 = i;
    tube2 = i;
    tube3 = i;
    tube4 = i;
    tube5 = i;
    tube6 = i;
    update_disp();
    delay(500);
  }

  tube1 = tube_off;
  tube2 = tube_off;
  tube3 = tube_off;
  tube4 = tube_off;
  tube5 = tube_off;
  tube6 = tube_off;
  indicator1 = ind_off;
  indicator2 = ind_off;
  indicator3 = ind_off;
  update_disp();
  delay(500);

  tube1 = 9;
  update_disp();
  delay(500);

  tube1 = tube_off;
  tube2 = 9;
  update_disp();
  delay(500);

  tube2 = tube_off;
  indicator1 = 0;
  update_disp();
  delay(500);

  indicator1 = 1;
  update_disp();
  delay(500);

  indicator1 = 2;
  update_disp();
  delay(500);

  indicator1 = ind_off;
  tube3 = 9;
  update_disp();
  delay(500);

  tube3 = tube_off;
  tube4 = 9;
  update_disp();
  delay(500);

  tube4 = tube_off;
  indicator2 = 0;
  update_disp();
  delay(500);

  indicator2 = 1;
  update_disp();
  delay(500);

  indicator2 = 2;
  update_disp();
  delay(500);

  indicator2 = ind_off;
  tube5 = 9;
  update_disp();
  delay(500);

  tube5 = tube_off;
  tube6 = 9;
  update_disp();
  delay(500);

  tube6 = tube_off;
  indicator3 = 0;
  update_disp();
  delay(500);

  indicator3 = 1;
  update_disp();
  delay(500);

  indicator3 = 2;
  update_disp();
  delay(500);

  indicator3 = ind_off;
  update_disp();
  delay(500);
*/