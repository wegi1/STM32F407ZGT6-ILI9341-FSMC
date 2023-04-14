/*
 * ili9341.c
 *
 *  Created on: Apr 13, 2023
 *      Author: BOLO
 */


/* header file contains INCLUDES */

#include "ili9341.h"
/*---------------------------------------------------------------------*/
#include "gpio.h"




//********************************************************************************************************************************************************
//********************************************************************************************************************************************************
//********************************************************************************************************************************************************

//LCD Address structure
//typedef struct
//{
//  volatile u16 LCD_REG;
//  volatile u16 LCD_RAM;
//} LCD_TypeDef;
//#define    LCD_BASE        ((u32)(0x6C000000 | 0x000007FE))// FSMC_NE4, A10
//#define  LCD             ((LCD_TypeDef *) LCD_BASE)


//***********************************************************************************************************

// FSMC_NE1 FSMC_A18
//LCD_REG       (*(volatile u16*)((uint32_t)(0x60000000)))  // FSMC_NE1
//LCD_RAM       (*(volatile u16*)((uint32_t)(0x60080000)))  //  FSMC_A18

//********************************************************************************************************************************************************

// |------------------------------|   |------------------------------|   |------------------------------|   |------------------------------|
// | CLASSIC STM32F407ZGT6 BOARD  |   | CLASSIC STM32F407VET6 BOARD  |   |  OMDAZZ STM32F407VET6 BOARD  |   |  OMDAZZ STM32F407ZGT6 BOARD  |
// |------------------------------|   |------------------------------|   |------------------------------|   |------------------------------|
// |   LCD_PIN  |   CPU_PIN       |   |   LCD_PIN  |   CPU_PIN       |   |   LCD_PIN  |   CPU_PIN       |   |   LCD_PIN  |   CPU_PIN       |
// |------------------------------|   |------------------------------|   |------------------------------|   |------------------------------|
// | 1-GND      | GND             | X | 1-GND      | GND             | X |            |                 | X |            |                 |
// | 2-RST      | RST  [NRST]     | + | 2-RST      | RST  [NRST]     | * | 5-RST      | PC5             | + | 5-RST      | PC5             |
// |------------------------------|   |------------------------------|   |------------------------------|   |------------------------------|
// |        data lines            |   |        data lines            |   |        data lines            |   |        data lines            |
// |------------------------------|   |------------------------------|   |------------------------------|   |------------------------------|
// | 3-DB15     | PD10 [FSMC_D15] | + | 3-DB15     | PD10 [FSMC_D15] | + | 21-DB15    | PD10 [FSMC_D15] | + | 21-DB15    | PD10 [FSMC_D15] |
// | 4-DB14     | PD9  [FSMC_D14] | + | 4-DB14     | PD9  [FSMC_D14] | + | 20-DB14    | PD9  [FSMC_D14] | + | 20-DB14    | PD9  [FSMC_D14] |
// | 5-DB13     | PD8  [FSMC_D13] | + | 5-DB13     | PD8  [FSMC_D13] | + | 19-DB13    | PD8  [FSMC_D13] | + | 19-DB13    | PD8  [FSMC_D13] |
// | 6-DB12     | PE15 [FSMC_D12] | + | 6-DB12     | PE15 [FSMC_D12] | + | 18-DB12    | PE15 [FSMC_D12] | + | 18-DB12    | PE15 [FSMC_D12] |
// | 7-DB11     | PE14 [FSMC_D11] | + | 7-DB11     | PE14 [FSMC_D11] | + | 17-DB11    | PE14 [FSMC_D11] | + | 17-DB11    | PE14 [FSMC_D11] |
// | 8-DB10     | PE13 [FSMC_D10] | + | 8-DB10     | PE13 [FSMC_D10] | + | 16-DB10    | PE13 [FSMC_D10] | + | 16-DB10    | PE13 [FSMC_D10] |
// | 9-DB09     | PE12 [FSMC_D9]  | + | 9-DB09     | PE12 [FSMC_D9]  | + | 15-DB9     | PE12 [FSMC_D9]  | + | 15-DB9     | PE12 [FSMC_D9]  |
// | 10-DB08    | PE11 [FSMC_D8]  | + | 10-DB08    | PE11 [FSMC_D8]  | + | 14-DB8     | PE11 [FSMC_D8]  | + | 14-DB8     | PE11 [FSMC_D8]  |
// | 11-DB07    | PE10 [FSMC_D7]  | + | 11-DB07    | PE10 [FSMC_D7]  | + | 13-DB7     | PE10 [FSMC_D7]  | + | 13-DB7     | PE10 [FSMC_D7]  |
// | 12-DB06    | PE9  [FSMC_D6]  | + | 12-DB06    | PE9  [FSMC_D6]  | + | 12-DB6     | PE9  [FSMC_D6]  | + | 12-DB6     | PE9  [FSMC_D6]  |
// | 13-DB05    | PE8  [FSMC_D5]  | + | 13-DB05    | PE8  [FSMC_D5]  | + | 11-DB5     | PE8  [FSMC_D5]  | + | 11-DB5     | PE8  [FSMC_D5]  |
// | 14-DB04    | PE7  [FSMC_D4]  | + | 14-DB04    | PE7  [FSMC_D4]  | + | 10-DB4     | PE7  [FSMC_D4]  | + | 10-DB4     | PE7  [FSMC_D4]  |
// | 15-DB03    | PD1  [FSMC_D3]  | + | 15-DB03    | PD1  [FSMC_D3]  | + | 9-DB3      | PD1  [FSMC_D3]  | + | 9-DB3      | PD1  [FSMC_D3]  |
// | 16-DB02    | PD0  [FSMC_D2]  | + | 16-DB02    | PD0  [FSMC_D2]  | + | 8-DB2      | PD0  [FSMC_D2]  | + | 8-DB2      | PD0  [FSMC_D2]  |
// | 17-DB01    | PD15 [FSMC_D1]  | + | 17-DB01    | PD15 [FSMC_D1]  | + | 7-DB1      | PD15 [FSMC_D1]  | + | 7-DB1      | PD15 [FSMC_D1]  |
// | 18-DB00    | PD14 [FSMC_D0]  | + | 18-DB00    | PD14 [FSMC_D0]  | + | 6-DB0      | PD14 [FSMC_D0]  | + | 6-DB0      | PD14 [FSMC_D0]  |
// |------------------------------|   |------------------------------|   |------------------------------|   |------------------------------|
// |     control signals          |   |     control signals          |   |     control signals          |   |     control signals          |
// |------------------------------|   |------------------------------|   |------------------------------|   |------------------------------|
// | 19-RD      | PD4  [FSMC_NOE] | + | 19-RD      | PD4  [FSMC_NOE] | + | 4-RD       | PD4  [FSMC_NOE] | + | 4-RD       | PD4  [FSMC_NOE] |
// | 20-WR      | PD5  [FSMC_NWE] | + | 20-WR      | PD5  [FSMC_NWE] | + | 3-WR       | PD5  [FSMC_NWE] | + | 3-WR       | PD5  [FSMC_NWE] |
// | 21-RS      | PF12 [FSMC_A6 ] | * | 21-RS      | PD13 [FSMC_A18] | + | 2-RS       | PD13 [FSMC_A18] | * | 2-RS       | PG0  [FSMC_A10] |  2.DIFF PD13 [FSMC_A18]
// | 22-CS_M    | PG12 [FSMC_NE4] | * | 22-CS_M    | PD7  [FSMC_NE1] | + | 1-CS       | PD7  [FSMC_NE1] | * | 1-CS       | PG12 [FSMC_NE4] |  1.DIFF PD7  [FSMC_NE1]
// |------------------------------|   |------------------------------|   |------------------------------|   |------------------------------|
// |    TFT CONTROL SIGNALS       |   |    TFT CONTROL SIGNALS       |   |    TFT CONTROL SIGNALS       |   |    TFT CONTROL SIGNALS       |
// |------------------------------|   |------------------------------|   |------------------------------|   |------------------------------|
// | 23-SCK     | PB0             | * | 23-SCK     | PB13            | * | 34-SCK     | PB1             |   | 34-SCK     | PB1             |
// | 24-SCS     | PC13            | * | 24-SCS     | PB12            | * | 33-TCS     | PB2             |   | 33-TCS     | PB2             |
// | 25-SI      | PF11            | * | 25-SI      | PB15            | * | 30-MOSI    | PE1             | * | 30-MOSI    | PF9             |  4.DIFF | 30-MOSI    | PE1             |
// | 26-SO      | PB2             | * | 26-SO      | PB14            | * | 29-MISO    | PE0             | * | 29-MISO    | PF8             |  3.DIFF | 29-MISO    | PE0             |
// | 27-INT     | PB1             | * | 27-INT     | PC5             | * | 31-TINT    | PE2             | * | 31-TINT    | PF10            |  5.DIFF | 31-TINT    | PE2             |
// |------------------------------|   |------------------------------|   |------------------------------|   |------------------------------|
// |       LCD BACK LIGHT         |   |       LCD BACK LIGHT         |   |       LCD BACK LIGHT         |   |       LCD BACK LIGHT         |
// |------------------------------|   |------------------------------|   |------------------------------|   |------------------------------|
// | 28-BLK     | PB15            | * | 28-BLK     | PB1             | * | 23-LIG     | PB0             | + | 23-LIG     | PB0             |
// |------------------------------|   |------------------------------|   |------------------------------|   |------------------------------|
// | 29-SET     | NC              | X | 29-SET     | NC              | X |       DOESN'T MATTER         |   |       DOESN'T MATTER         |
// |------------------------------|   |------------------------------|   |------------------------------|   |------------------------------|
// |       POWER SUPPLY           | X |       POWER SUPPLY           | X | 32-NC      | PC3 [*FCS]      | + | 32-NC      | PC3 [*FCS]      |
// |------------------------------|   |------------------------------|   |------------------------------|   |------------------------------|
// | 30-GND     | GND             |   | 30-GND     | GND             |
// | 31-3V3     | 3.3V            |   | 31-3V3     | 3.3V            |
// | 32-GND     | GND             |   | 32-GND     | GND             |
// |------------------------------|   |------------------------------|

//********************************************************************************************************************************************************
                // differences:
// |------------------------------||------------------------------||------------------------------|
// | CLASSIC STM32F407ZGT6 BOARD  ||  OMDAZZ STM32F407ZGT6 BOARD  ||  OMDAZZ STM32F407ZGT6 BOARD  |
// |------------------------------||------------------------------||------------------------------|
// |   LCD_PIN  |   CPU_PIN       ||   LCD_PIN  |   CPU_PIN       ||   LCD_PIN  |   CPU_PIN       |
// |------------------------------||------------------------------||------------------------------|
// | 2-RST      | RST  [NRST]     || 5-RST      | PC5             ||     control signals          |
// |------------------------------||------------------------------||------------------------------|
// |     control signals          ||     control signals          || 4-RD       | PD4  [FSMC_NOE] |
// |------------------------------||------------------------------|| 3-WR       | PD5  [FSMC_NWE] |
// | 21-RS      | PF12 [FSMC_A6 ] || PG0        | [FSMC_A10]      || 2-RS       | PG0  [FSMC_A10] |
// |------------------------------||------------------------------|| 1-CS       | PG12 [FSMC_NE4] |
// |       LCD BACK LIGHT         ||       LCD BACK LIGHT         ||------------------------------|
// |------------------------------||------------------------------|
// | 28-BLK     | PB15            || 23-LIG     | PB0             |
// |------------------------------||------------------------------|


//********************************************************************************************************************************************************
//********************************************************************************************************************************************************



/*-------------------------------------------------------------------------------------------------- -----
* Resource definition
---------------------------------------------------------------------------------------------------- ---*/
/*---------------------------------------------------------------------*/
static const uint8_t init_tab[] = {
        ILI_PCB, 3, 0x00, 0xC1, 0X30,  \
        ILI_POSC, 4, 0x64, 0x03, 0X12, 0X81,  \
        ILI_DTCA_ic, 3, 0x85, 0x10, 0x7A,  \
        ILI_PCA, 5, 0x39, 0x2C, 0x00, 0x34, 0x02,  \
        ILI_PRC,1, 0x20,  \
        ILI_DTCB, 2, 0x00, 0x00,  \
        ILI_PWCTRL1, 1, 0x1B,  \
        ILI_PWCTRL2,1, 0x01,  \
        ILI_VMCTRL1, 2, 0x30, 0x30,  \
        ILI_VMCTRL2, 1, 0XB7,  \
        ILI_MADCTL, 1, 0x48,  \
        ILI_PIXSET, 1, 0x55,  \
        ILI_FRMCTR1, 2, 0x00, 0x1A,  \
        ILI_DISCTRL, 2, 0x0A, 0xA2,  \
        ILI_E3G, 1, 0x00,  \
        ILI_GAMSET, 1, 0x01,  \
        ILI_PGAMCTRL, 15, 0x0F, 0x2A, 0x28, 0x08, 0x0E, 0x08, 0x54, 0XA9, 0x43, 0x0A, 0x0F, 0x00, 0x00, 0x00, 0x00,  \
        ILI_NGAMCTRL, 15, 0x00, 0x15, 0x17, 0x07, 0x11, 0x06, 0x2B, 0x56, 0x3C, 0x05, 0x10, 0x0F, 0x3F, 0x3F, 0x0F,  \
        ILI_PASET, 4, 0x00, 0x00, 0x01, 0x3f,  \
        ILI_CASET, 4, 0x00, 0x00, 0x00, 0xef,  \
        ILI_SLPOUT, 120,  \
        ILI_DISPON, 0,  \
        ILI_MADCTL, 1, 0xC9,  \
        0x00
};

/*-------------------------------------------------------------------------------------------------------
*  EXECUTION CODE
-------------------------------------------------------------------------------------------------------*/
/********************************************************************************************************
*  Function: Driver_LcdReset
*  Object: lcd reset control
*  Input: sta
*  Output: none
*  brief: none
********************************************************************************************************/
/*****************************************************************************************************
 *
 * initialize sequence ILI_9341
 *
 ***************************************************************************************************/
void LCD_ILI9341_init(void)
{
    u32 i = 0;
    u32 i2;
    u8 counter;


    //--------------
    HAL_GPIO_WritePin(LCD_BL_GPIO_Port, LCD_BL_Pin, GPIO_PIN_SET) ;  // enable LCD back light
    //--------------


    while(init_tab[i] != 0)
    {
        LCD->LCD_REG = (u16) init_tab[i] ;
        counter = init_tab[i+1];
        i += 2;
        if(counter > 15)
        {
            HAL_Delay(counter);
        }
        else if(counter > 0)
        {
            for(i2 = 0; i2 < counter ; i2++)
            {
                LCD->LCD_RAM = init_tab[i+i2] ;
            }
            i += counter;
        }
    }


    /*****************/
    LCD_ClrScr(BLUE);  // init screen as blue
}
/********************************************************************************************************
*  Function: LCD_WriteReg
*  Object: lcd write reg
*  Input: index
*  Output: none
*  brief: none
********************************************************************************************************/
void LCD_WriteReg(u16 Index)
{
    LCD->LCD_REG = Index;
    //Driver_LcdFSMCWriteReg(Index);
}

/********************************************************************************************************
*  Function: LCD_WriteData
*  Object: lcd write data
*  Input: index and data
*  Output: none
*  brief: none
********************************************************************************************************/
void LCD_WriteData(u16 Data)
{
    LCD->LCD_RAM = Data;
}
/********************************************************************************************************
*  Function: LCD_ClrScr
*  Object: lcd clear screen
*  Input: backcolor
*  Output: none
*  brief: none
********************************************************************************************************/
void LCD_ClrScr(u16 BackColor)
{
    uint32_t i;
    LCD_OpenWin(0, 0, LCD_ROW_NUM-1, LCD_COL_NUM-1);
    for(i = (SCREEN_PIXELS); i != 0 ; i--)
        LCD->LCD_RAM = BackColor ;
}
/********************************************************************************************************
*  Function: LCD_OpenWin
*  Object: lcd open window for display
*  Input: x0,y0, x1, y1
*  Output: none
*  brief: none
********************************************************************************************************/
void LCD_OpenWin(u16 x0, u16 y0, u16 x1, u16 y1)
{
    LCD->LCD_REG  = (ILI_CASET);
    LCD->LCD_RAM = (y0>>8);
    LCD->LCD_RAM = (y0);
    LCD->LCD_RAM = (y1>>8);
    LCD->LCD_RAM = (y1);
    LCD->LCD_REG = (ILI_PASET);
    LCD->LCD_RAM = (x0>>8);
    LCD->LCD_RAM = (x0);
    LCD->LCD_RAM = (x1>>8);
    LCD->LCD_RAM = (x1);
    LCD->LCD_REG  = (ILI_RAMWR);
}
/*******************************************************************************************************
 * end of screen functions
 *******************************************************************************************************/


