/**
 * original author:  Tilen Majerle<tilen@majerle.eu>
 * modification for STM32f10x: Alexander Lutsai<s.lyra@ya.ru>

   ----------------------------------------------------------------------
   	Copyright (C) Alexander Lutsai, 2016
    Copyright (C) Tilen Majerle, 2015

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
   ----------------------------------------------------------------------
 */
#include "ssd1305.h"
#include "main.h"

// Set for LCD OLED 128*32 type WEO012832 or raystar REX012832A on SSD1305
/* Write command */
#define SSD1305_WRITECOMMAND(command)      ssd1305_I2C_Write(SSD1305_I2C_ADDR, 0x00, (command))
/* Write data */
#define SSD1305_WRITEDATA(data)            ssd1305_I2C_Write(SSD1305_I2C_ADDR, 0x40, (data))
///* Absolute value */
//#define ABS(x)   ((x) > 0 ? (x) : -(x))
#define HORIZ_MOVE -1


/* SSD1305 data buffer for BMP */
uint8_t SSD1305_Buffer_BMP[SSD1305_WIDTH * SSD1305_HEIGHT / 8 + 67 ] ={
#include "128x32.h" //
};

/* SSD1305 data buffer */
static uint8_t SSD1305_Buffer_all[SSD1305_WIDTH * SSD1305_HEIGHT / 8 + 1], *SSD1305_Buffer = SSD1305_Buffer_all+1;
//static uint8_t SSD1305_Buffer[SSD1305_WIDTH * SSD1305_HEIGHT / 8 ];//, *SSD1305_Buffer = SSD1305_Buffer_all+1;

/* Private SSD1305 structure */
typedef struct {
	uint16_t CurrentX;
	uint16_t CurrentY;
	uint8_t Inverted;
	uint8_t Initialized;
} SSD1305_t;

/* Private variable */
static SSD1305_t SSD1305;

uint8_t SSD1305_I2C_ADDR ;

uint8_t SSD1305_Init(void) {

	/* Init I2C */
	//ssd1305_I2C_Init();
	
	/* Check if LCD connected to I2C */
	//if (HAL_I2C_IsDeviceReady(&heval_I2c, SSD1305_I2C_ADDR, 1, 20000) != HAL_OK) {
		/* Return false */
	//	return 0;
	//}
	
	/* A little delay */
	uint32_t p = 100500;
	while(p>0)
		p--;
        // Next ALTERNTIVE SET OLED 128x32 from https://github.com/adafruit/Adafruit_SSD1305
//	  SSD1305_WRITECOMMAND(0xAE); //SSD1305_DISPLAYOFF
//	  SSD1305_WRITECOMMAND(0x00); //low col = 0 0x00
//	  SSD1305_WRITECOMMAND(0x10); //hi col = 0  0x10
//	  SSD1305_WRITECOMMAND(0x40); //line #0     0x40
//	  SSD1305_WRITECOMMAND(0x2E); //??
//	  SSD1305_WRITECOMMAND(0x81); //SSD1305_SETCONTRAST
//	  SSD1305_WRITECOMMAND(0x32);
//	  SSD1305_WRITECOMMAND(0x82); //SSD1305_SETBRIGHTNESS
//	  SSD1305_WRITECOMMAND(0x10);
//	  SSD1305_WRITECOMMAND(0xA1); //SSD1305_SEGREMAP | 0x01
//	  SSD1305_WRITECOMMAND(0xA6); //SSD1305_NORMALDISPLAY
//	  SSD1305_WRITECOMMAND(0xA8); //SSD1305_SETMULTIPLEX
//	  SSD1305_WRITECOMMAND(0x3F);
//	  SSD1305_WRITECOMMAND(0xAD); //SSD1305_MASTERCONFIG
//	  SSD1305_WRITECOMMAND(0x8E);
//	  SSD1305_WRITECOMMAND(0xC8); //SSD1305_COMSCANDEC
//	  SSD1305_WRITECOMMAND(0xD5); //SSD1305_SETDISPLAYCLOCKDIV
//	  SSD1305_WRITECOMMAND(0xF0);
//	  SSD1305_WRITECOMMAND(0xD8); //SSD1305_SETAREACOLOR
//	  SSD1305_WRITECOMMAND(0x05);
//	  SSD1305_WRITECOMMAND(0xD9); //SSD1305_SETPRECHARGE
//	  SSD1305_WRITECOMMAND(0xF1);
//	  SSD1305_WRITECOMMAND(0xDA); //SSD1305_SETCOMPINS
//	  SSD1305_WRITECOMMAND(0x12);
//	  SSD1305_WRITECOMMAND(0x91); //SSD1305_SETLUT
//	  SSD1305_WRITECOMMAND(0x3F);
//	  SSD1305_WRITECOMMAND(0x3F);
//	  SSD1305_WRITECOMMAND(0x3F);
//	  SSD1305_WRITECOMMAND(0x3F);
	        
	// init WEO012832 from https://github.com/olikraus/u8g2/issues/738
//        SSD1305_WRITECOMMAND(0xae);//Set Display ON/OFF =>0xae:Display off
//
//	SSD1305_WRITECOMMAND(0xa8);//Set Multiplex Ratio
//	SSD1305_WRITECOMMAND(0x1f);//63
//
//	SSD1305_WRITECOMMAND(0xd3);//Set Display Offset
//	SSD1305_WRITECOMMAND(0x00);
//	
//        SSD1305_WRITECOMMAND(0x00);//Set Lower Column Start Address for Page Addressing Mode  efault => 0x00
//	
//	SSD1305_WRITECOMMAND(0x10);//Set Higher Column Start Address for Page Addressing Mode;Default => 0x10	
//
//	SSD1305_WRITECOMMAND(0x20);//Set Memory Addressing Mode
//	SSD1305_WRITECOMMAND(0x10);
//	SSD1305_WRITECOMMAND(0xB0);
//	
//	SSD1305_WRITECOMMAND(0x81);//Set contrast Control for BANK0
//	SSD1305_WRITECOMMAND(0xFF);
//	
//	SSD1305_WRITECOMMAND(0x40);//Set Display Start Line
//	SSD1305_WRITECOMMAND(0xa1);//Set Segment Re-map =>0xa0:column addr. 0 is mapped to SEG0 
//	SSD1305_WRITECOMMAND(0xa4);//Entire Display On =>0xa4:resume to RAM content display
//	SSD1305_WRITECOMMAND(0xa6);//Set Normal/Inverse Display =>0xa6:Normal,0xa7:inverse 
//
//	SSD1305_WRITECOMMAND(0xc8);//Set COM Output Scan Direction =>0xc0:scan from COM0 to COM63, 0xc8:scan from COM63 to COM0
//
//	SSD1305_WRITECOMMAND(0xd5);//Set Display Clock Divid Ratio/Oscillator Frequency
//	SSD1305_WRITECOMMAND(0xa0);
//
//	SSD1305_WRITECOMMAND(0xd9);//Set Pre-charge Period
//	SSD1305_WRITECOMMAND(0x77);
//
//	SSD1305_WRITECOMMAND(0xda);//Set COM Pins Hardware Configuration
//	SSD1305_WRITECOMMAND(0x02);
//
//	SSD1305_WRITECOMMAND(0xdb);//Set Vcomh Deselect Level
//	SSD1305_WRITECOMMAND(0x0);
//	
//	SSD1305_WRITECOMMAND(0xaf);//Set Display ON/OFF =>0xaf:Display on
//	
//	SSD1305_WRITECOMMAND(0x8d);//SSD1305	charge pump	 set
//	SSD1305_WRITECOMMAND(0x14);				
//	SSD1305_WRITECOMMAND(0xAF); //--turn on SSD1305 panel

	/* Init LCD */
	SSD1305_WRITECOMMAND(0xAE); //display off
	SSD1305_WRITECOMMAND(0x20); //Set Memory Addressing Mode   
	SSD1305_WRITECOMMAND(0x10); //00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	SSD1305_WRITECOMMAND(0xB0); //Set Page Start Address for Page Addressing Mode,0-7
	SSD1305_WRITECOMMAND(0xC0); //Set COM Output Scan Direction
	SSD1305_WRITECOMMAND(0x00); //---set low column address
	SSD1305_WRITECOMMAND(0x10); //---set high column address
	SSD1305_WRITECOMMAND(0x40); //--set start line address
	SSD1305_WRITECOMMAND(0x81); //--set contrast control register
	SSD1305_WRITECOMMAND(0x80);//(0xFF);

	SSD1305_WRITECOMMAND(0xA0); //--set segment re-map 0 to 127
	SSD1305_WRITECOMMAND(0xA6); //--set normal display
	SSD1305_WRITECOMMAND(0xA8); //--set multiplex ratio(1 to 64)
	SSD1305_WRITECOMMAND(SSD1305_HEIGHT-1); //
	SSD1305_WRITECOMMAND(0xA4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	SSD1305_WRITECOMMAND(0xD3); //-set display offset for Vertical if i>0 not visible i strok
	SSD1305_WRITECOMMAND(0x00); //-not offset
	SSD1305_WRITECOMMAND(0xD5); //--set display clock divide ratio/oscillator frequency
	SSD1305_WRITECOMMAND(0xF0);//(0xF0); //--set divide ratio
	SSD1305_WRITECOMMAND(0xD9); //--set pre-charge period
	SSD1305_WRITECOMMAND(0x22); //(0x22); //
	//SSD1305_WRITECOMMAND(0x2E); //(0x22); // деактивация скролинга
        
	SSD1305_WRITECOMMAND(0xDA); //--set com pins hardware configuration
	SSD1305_WRITECOMMAND(0x12);
	SSD1305_WRITECOMMAND(0xDB); //--set vcomh
	SSD1305_WRITECOMMAND(0x20); //0x20,0.77xVcc
	SSD1305_WRITECOMMAND(0x8D); //--set DC-DC enable
	SSD1305_WRITECOMMAND(0x14); //
	SSD1305_WRITECOMMAND(0xAF); //--turn on SSD1305 panel
	// альтернативная инициализацичя дисплея
        // display OFF
//   SSD1305_WRITECOMMAND(0xAE);
//   
//   //clock divide, OSC freq
//   SSD1305_WRITECOMMAND(0xD5);
//   SSD1305_WRITECOMMAND(0xF0);
//   
//   // multiplex ratio
//   SSD1305_WRITECOMMAND(0xA8);
//   SSD1305_WRITECOMMAND(0x3F);
//   
//   // display offset
//   SSD1305_WRITECOMMAND(0xD3);
//   SSD1305_WRITECOMMAND(0x00);
//   
//   // set display startline(64)
//   SSD1305_WRITECOMMAND(0x40);
//   
//   // enable sharge pump
//   SSD1305_WRITECOMMAND(0x8D);
//   SSD1305_WRITECOMMAND(0x14);
//   
//   // segment remap(col.127 mapped to SEG0)
//   SSD1305_WRITECOMMAND(0xA1);
//   
//   // set COM output scan direction
//   SSD1305_WRITECOMMAND(0xC8);
//   
//   // set addressiing mode is "horisontal"
//   SSD1305_WRITECOMMAND(0x20);
//   SSD1305_WRITECOMMAND(0x10);
//   SSD1305_WRITECOMMAND(0xB0);
//
//   // set COM pins hardware configuration   
//   SSD1305_WRITECOMMAND(0xDA);
//   SSD1305_WRITECOMMAND(0x12);
//   
//   // contrast control
//   SSD1305_WRITECOMMAND(0x81);
//   SSD1305_WRITECOMMAND(0x3F);
//   
//   // pre-charge period
//   SSD1305_WRITECOMMAND(0xD9);
//   SSD1305_WRITECOMMAND(0xE1);
//   
//   // set VCOMH deselect level
//   SSD1305_WRITECOMMAND(0xDB);
//   SSD1305_WRITECOMMAND(0x40);
//   
//   // set entire display ON/OFF
//   SSD1305_WRITECOMMAND(0xA4);
//   
//   // set normal,inverse display
//   SSD1305_WRITECOMMAND(0xA6);   
//   
//   //lcd_clear();
//   
//   // display ON   
//   SSD1305_WRITECOMMAND(0xAF);
        // еще одна альтернатива!
//    SSD1305_WRITECOMMAND( 0xAE);
// 
//    SSD1305_WRITECOMMAND( 0x00 | 0x0);            // low col = 0
// 
//    SSD1305_WRITECOMMAND( 0x10 | 0x0);           // hi col = 0
//    SSD1305_WRITECOMMAND( 0x40 | 0x0);            // line #0
// 
//    SSD1305_WRITECOMMAND( 0x81);                   // Set Contrast 0x81
//    SSD1305_WRITECOMMAND( 0xCF);
//    SSD1305_WRITECOMMAND( 0xA1);                    // Segremap - 0xA1
//    SSD1305_WRITECOMMAND( 0xC8);                    // COMSCAN DEC 0xC8 C0
//    SSD1305_WRITECOMMAND( 0xA6);                    // Normal Display 0xA6 (Invert A7)
// 
//    SSD1305_WRITECOMMAND( 0xA4);                // DISPLAY ALL ON RESUME - 0xA4
//    SSD1305_WRITECOMMAND( 0xA8);                    // Set Multiplex 0xA8
//    SSD1305_WRITECOMMAND( 0x3F);                    // 1/64 Duty Cycle 
// 
//    SSD1305_WRITECOMMAND( 0xD3);                    // Set Display Offset 0xD3
//    SSD1305_WRITECOMMAND( 0x0);                     // no offset
// 
//    SSD1305_WRITECOMMAND( 0xD5);                    // Set Display Clk Div 0xD5
//    SSD1305_WRITECOMMAND( 0x80);                    // Recommneded resistor ratio 0x80
// 
//    SSD1305_WRITECOMMAND( 0xD9);                  // Set Precharge 0xd9
//    SSD1305_WRITECOMMAND( 0xF1);
// 
//    SSD1305_WRITECOMMAND( 0xDA);                    // Set COM Pins0xDA
//    SSD1305_WRITECOMMAND( 0x12);
// 
//    SSD1305_WRITECOMMAND( 0xDB);                 // Set VCOM Detect - 0xDB
//    SSD1305_WRITECOMMAND( 0x40);
// 
//    SSD1305_WRITECOMMAND( 0x20);                    // Set Memory Addressing Mode
//    SSD1305_WRITECOMMAND( 0x00);                    // 0x00 - Horizontal
// 
//    SSD1305_WRITECOMMAND( 0x40 | 0x0);              // Set start line at line 0 - 0x40 
// 
//    SSD1305_WRITECOMMAND( 0x8D);                    // Charge Pump -0x8D
//    SSD1305_WRITECOMMAND( 0x14);
// 
// 
//    SSD1305_WRITECOMMAND( 0xA4);              //--turn on all pixels - A5. Regular mode A4
//    SSD1305_WRITECOMMAND( 0xAF);            
        
	/* Clear screen */
	SSD1305_Fill(SSD1305_COLOR_BLACK);
	
	/* Update screen */
	SSD1305_UpdateScreen();
	
	/* Set default values */
	SSD1305.CurrentX = 0;
	SSD1305.CurrentY = 0;
	
	/* Initialized OK */
	SSD1305.Initialized = 1;
	
	/* Return OK */
	return 1;
}


void SSD1305_UpdateScreen(void) {
	uint8_t m;
	
	for (m = 0; m < SSD1305_HEIGHT/8; m++) { // 64-8 32-4 в зависимости от высоты число страниц
		SSD1305_WRITECOMMAND(0xB0 + m);
		SSD1305_WRITECOMMAND(0x00);
		SSD1305_WRITECOMMAND(0x10);
		
		/* Write multi data */
		ssd1305_I2C_WriteMulti( SSD1305_I2C_ADDR, 0x40, &SSD1305_Buffer_all[SSD1305_WIDTH * m], SSD1305_WIDTH);
                //HAL_Delay(3);
	}
// вставим перезапись в блок BMP
        SSD1305_GetBMP();
}


void SSD1305_SwapMem(void)
{
  static char bLow, bHigh, invL, invH;
  const char c[16]={0,8,4,12,2,10,6,14,1,9,5,13,3,11,7,15};
  for (int i=0 ; i<512; i++) // 32-256 64-512
  {
    bLow = c[SSD1305_Buffer[i]&0xf]<<4;
    invL = c[SSD1305_Buffer[i]>>4];
    bHigh = c[SSD1305_Buffer[511-i]&0xf]<<4; // 64-1023 32-511
    invH = c[SSD1305_Buffer[511-i]>>4]; // 64-1023 32-511
    //invL = c[(bLow>>4)&0xf] + (c[bLow&0x0F])<<4;
    //invH = c[(bHigh>>4)&0xf] + (c[bHigh&0x0F])<<4;
    SSD1305_Buffer[i]=(uint8_t)(bHigh+invH);
    SSD1305_Buffer[511-i]=(uint8_t)(bLow|invL) ; // 64-1023 32-511
  }
}

void SSD1305_GetBMP(void)
{
  /* Set memory */
  memset(&SSD1305_Buffer_BMP[67],  0x00, SSD1305_WIDTH * SSD1305_HEIGHT / 8);// сделаем белым
  
  
  for (int j=0 ; j<4; j++) // бежим по блокам для буф ssd1305  их 4 с нижнего то есть с четвертого у BMP это первый
  {
    for (int i=0 ; i<128; i++) // бежим по строкам их 4 в буф sdd1305, начиная с нижней (она перевернута) будем писать 
      // в буфер BMP 16 байт по горизонтали 8 по вертикали
    {
      for (int k=0 ; k<8; k++)// разбираем байт, прописываем в соот место
      {
        if(((SSD1305_Buffer[(3-j)*128 + i])<<k)&0x80) 
          SSD1305_Buffer_BMP[(j*128)+(16*k)+(i>>3)+67] |=(1<<(7-(i&0x7)));
      }
    }
  }
}

//void SSD1305_UpdateScreen(void) {
//	SSD1305_Buffer_all[0] = 0x40;
//	HAL_I2C_Master_Transmit_DMA(&heval_I2c, SSD1305_I2C_ADDR, SSD1305_Buffer_all, SSD1305_WIDTH * SSD1305_HEIGHT / 8 + 1);
//	while(HAL_DMA_GetState(hi2c1.hdmatx) != HAL_DMA_STATE_READY)
//	{
//		HAL_Delay(1); //Change for your RTOS
//	}
//}

void SSD1305_ToggleInvert(void) {
	uint16_t i;
	
	/* Toggle invert */
	SSD1305.Inverted = !SSD1305.Inverted;
	
	/* Do memory toggle */
	for (i = 0; i < sizeof(SSD1305_Buffer); i++) {
		SSD1305_Buffer[i] = ~SSD1305_Buffer[i];
	}
}

void SSD1305_Fill(uint8_t color) {
	/* Set memory */
	memset(SSD1305_Buffer, (color == SSD1305_COLOR_BLACK) ? 0x00 : 0xFF, SSD1305_WIDTH * SSD1305_HEIGHT / 8);
}

void SSD1305_DrawPixel(uint16_t x, uint16_t y, uint8_t color) {
	if (
		x >= SSD1305_WIDTH ||
		y >= SSD1305_HEIGHT
	) {
		/* Error */
		return;
	}
	
	/* Check if pixels are inverted */
	if (SSD1305.Inverted) {
		color = (uint8_t)!color;
	}
	
	/* Set color */
	if (color == SSD1305_COLOR_WHITE) {
		SSD1305_Buffer[x + (y / 8) * SSD1305_WIDTH] |= 1 << (y % 8);
	} else {
		SSD1305_Buffer[x + (y / 8) * SSD1305_WIDTH] &= ~(1 << (y % 8));
	}
}

void SSD1305_GotoXY(uint16_t x, uint16_t y) {
	/* Set write pointers */
  // можно попробовать поиграть смщанием по горизонтали
	SSD1305.CurrentX = x;
	SSD1305.CurrentY = y;
}

//char SSD1305_Putc(char ch, FontDef_t* Font, uint8_t color) {
//	uint32_t i, b, j;
//	
//	b = 0;
//	/* Go through font */
//	for (i = 0; i < Font->FontHeight; i++) {
//		for (j = 0; j < Font->FontWidth; j++) {
//			if ((Font->data[ch*Font->CharBytes + b/8] >> b%8) & 1) {
//				SSD1305_DrawPixel(SSD1305.CurrentX + j, (SSD1305.CurrentY + i), (uint8_t) color);
//			} else {
//				SSD1305_DrawPixel(SSD1305.CurrentX + j, (SSD1305.CurrentY + i), (uint8_t)!color);
//			}
//			b++;
//		}
//	}
//	
//	/* Increase pointer */
//	SSD1305.CurrentX += Font->FontWidth;
//	
//	/* Return character written */
//	return ch;
//}
char SSD1305_Putc(char ch, FontDef_t* Font, uint8_t color) {
	uint32_t i, b, j;
	static char Smbl;
	/* Check available space in LCD */
	if (
		SSD1305_WIDTH <= (SSD1305.CurrentX + Font->FontWidth) ||
		SSD1305_HEIGHT <= (SSD1305.CurrentY + Font->FontHeight)
	) {
		/* Error */
		return 0;
	}
	
	/* Go through font */
    //попререключаем таблицы
    if(ch>128)
    {
      Smbl = ch-96;
    }
    else
    {
      Smbl = ch - 32;
    }
	for (i = 0; i < Font->FontHeight; i++) {
		b = Font->data[ Smbl * Font->FontHeight + i];//(ch - 32)ch
		for (j = 0; j < Font->FontWidth; j++) {
			if ((b << j) & 0x8000) {
				SSD1305_DrawPixel(SSD1305.CurrentX + j, (SSD1305.CurrentY + i), (uint8_t) color);
			} else {
				SSD1305_DrawPixel(SSD1305.CurrentX + j, (SSD1305.CurrentY + i), (uint8_t)!color);
			}

		}
	}
	
	/* Increase pointer */
	SSD1305.CurrentX += Font->FontWidth;
	
	/* Return character written */
	return ch;
}

char SSD1305_Puts(char* str, FontDef_t* Font, uint8_t color) {
	/* Write characters */
	while (*str) {
		/* Write character by character */
		if (SSD1305_Putc(*str, Font, color) != *str) {
			/* Return error */
			return *str;
		}
		
		/* Increase string pointer */
		str++;
	}
	
	/* Everything OK, zero should be returned */
	return *str;
}

//
uint8_t *SearchSym(uint8_t* Font, char Sym)
{
  //????? ??????? ? ??????? ???????????????

  unsigned int i, MaxEl ,j=0;
  uint8_t *pu;
  
  if (Sym==0x0) Sym=' ';
  //tstch = Sym;
  i= 0;
  pu = (uint8_t*)Font;
  if(pu == (uint8_t*)(TabSG1))
    MaxEl= MAX1;
//    MaxEl= (sizeof(&TabSG1));
  if(pu == (uint8_t*)(TabSG2))
    MaxEl=  MAX2;
  if(pu == (uint8_t*)(TabSG3))
    MaxEl=  MAX3;
  if(pu == (uint8_t*)(TabSG4))
    MaxEl=  MAX4;
  if(pu == (uint8_t*)(TabSG5))
    MaxEl=  MAX5;
    
  while ((j<MaxEl)&&(*pu!=Sym)){
    i=(*(pu+1))*(*(pu+2));
    j= j + i + 3;
    pu=pu+i+3;}
  if(j>=MaxEl) // ???? ?? ????? ?????? ??????? ?????? ?? ??????? 2
    pu=(unsigned char*)TabSG2;
  return pu; // set point Need symbol print
}


 // write string from new table us MSP430
uint8_t SSD1305_PutsN(uint8_t* str, uint8_t* Font, uint8_t color) {
	/* Write characters */
	while (*str) {
		/* Write character by character */
		if (SSD1305_PutcN(*str, Font, color) != *str) {
			/* Return error */
			return *str;
		}
		
		/* Increase string pointer */
		str++;
	}
	
	/* Everything OK, zero should be returned */
	return *str;
}
// Vertical OUT mode 
uint8_t SSD1305_PutcN(uint8_t ch, uint8_t* Font, uint8_t color) {
  uint32_t   b,i, x,f;
  uint8_t SizeHeight, SizeWidth;
  uint8_t *pu;
  pu = SearchSym(Font, ch);
  SizeWidth = pu[1];
  SizeHeight = pu[2];
  // Search Symbol in table FONT
  
  /* Check available space in LCD */
  if (
      SSD1305_WIDTH < (SSD1305.CurrentX + SizeWidth) ||
        SSD1305_HEIGHT < (SSD1305.CurrentY + SizeHeight*8)
          ) {
            /* Error */
            return 0;
          }
  
  /* Go through font */
  //попререключаем таблицы
  
  for (i = 0; i < SizeHeight; i++)
  {
    for (x = 0; x < SizeWidth; x++)
    {
      b = pu[i*SizeWidth + x + 3];//(ch - 32)ch
      for (f = 0;f < 8; f++)
      {
        if ((b << f) & 0x80)
        {
          SSD1305_DrawPixel(SSD1305.CurrentX + x, (SSD1305.CurrentY + f + i*8), (uint8_t) color);
        }
        else
        {
          SSD1305_DrawPixel(SSD1305.CurrentX + x, (SSD1305.CurrentY + f + i*8), (uint8_t)!color);
        }
        
      }
    }
  }
  
  /* Increase pointer */
  SSD1305.CurrentX += SizeWidth;
  
  /* Return character written */
  return ch;
}

void SSD1305_DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t c) {
	int16_t dx, dy, sx, sy, err, e2, i, tmp; 
	
	/* Check for overflow */
	if (x0 >= SSD1305_WIDTH) {
		x0 = SSD1305_WIDTH - 1;
	}
	if (x1 >= SSD1305_WIDTH) {
		x1 = SSD1305_WIDTH - 1;
	}
	if (y0 >= SSD1305_HEIGHT) {
		y0 = SSD1305_HEIGHT - 1;
	}
	if (y1 >= SSD1305_HEIGHT) {
		y1 = SSD1305_HEIGHT - 1;
	}
	
	dx = (x0 < x1) ? (x1 - x0) : (x0 - x1); 
	dy = (y0 < y1) ? (y1 - y0) : (y0 - y1); 
	sx = (x0 < x1) ? 1 : -1; 
	sy = (y0 < y1) ? 1 : -1; 
	err = ((dx > dy) ? dx : -dy) / 2; 

	if (dx == 0) {
		if (y1 < y0) {
			tmp = y1;
			y1 = y0;
			y0 = tmp;
		}
		
		if (x1 < x0) {
			tmp = x1;
			x1 = x0;
			x0 = tmp;
		}
		
		/* Vertical line */
		for (i = y0; i <= y1; i++) {
			SSD1305_DrawPixel(x0, i, c);
		}
		
		/* Return from function */
		return;
	}
	
	if (dy == 0) {
		if (y1 < y0) {
			tmp = y1;
			y1 = y0;
			y0 = tmp;
		}
		
		if (x1 < x0) {
			tmp = x1;
			x1 = x0;
			x0 = tmp;
		}
		
		/* Horizontal line */
		for (i = x0; i <= x1; i++) {
			SSD1305_DrawPixel(i, y0, c);
		}
		
		/* Return from function */
		return;
	}
	
	while (1) {
		SSD1305_DrawPixel(x0, y0, c);
		if (x0 == x1 && y0 == y1) {
			break;
		}
		e2 = err; 
		if (e2 > -dx) {
			err -= dy;
			x0 += sx;
		} 
		if (e2 < dy) {
			err += dx;
			y0 += sy;
		} 
	}
}

void SSD1305_DrawRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t c) {
	/* Check input parameters */
	if (
		x >= SSD1305_WIDTH ||
		y >= SSD1305_HEIGHT
	) {
		/* Return error */
		return;
	}
	
	/* Check width and height */
	if ((x + w) >= SSD1305_WIDTH) {
		w = SSD1305_WIDTH - x;
	}
	if ((y + h) >= SSD1305_HEIGHT) {
		h = SSD1305_HEIGHT - y;
	}
	
	/* Draw 4 lines */
	SSD1305_DrawLine(x, y, x + w, y, c);         /* Top line */
	SSD1305_DrawLine(x, y + h, x + w, y + h, c); /* Bottom line */
	SSD1305_DrawLine(x, y, x, y + h, c);         /* Left line */
	SSD1305_DrawLine(x + w, y, x + w, y + h, c); /* Right line */
}

void SSD1305_DrawFilledRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t c) {
	uint8_t i;
	
	/* Check input parameters */
	if (
		x >= SSD1305_WIDTH ||
		y >= SSD1305_HEIGHT
	) {
		/* Return error */
		return;
	}
	
	/* Check width and height */
	if ((x + w) >= SSD1305_WIDTH) {
		w = SSD1305_WIDTH - x;
	}
	if ((y + h) >= SSD1305_HEIGHT) {
		h = SSD1305_HEIGHT - y;
	}
	
	/* Draw lines */
	for (i = 0; i <= h; i++) {
		/* Draw lines */
		SSD1305_DrawLine(x, y + i, x + w, y + i, c);
	}
}

void SSD1305_DrawTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint8_t color) {
	/* Draw lines */
	SSD1305_DrawLine(x1, y1, x2, y2, color);
	SSD1305_DrawLine(x2, y2, x3, y3, color);
	SSD1305_DrawLine(x3, y3, x1, y1, color);
}


void SSD1305_DrawFilledTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint8_t color) {
	int16_t deltax = 0, deltay = 0, x = 0, y = 0, xinc1 = 0, xinc2 = 0, 
	yinc1 = 0, yinc2 = 0, den = 0, num = 0, numadd = 0, numpixels = 0, 
	curpixel = 0;
	
	deltax = ABS(x2 - x1);
	deltay = ABS(y2 - y1);
	x = x1;
	y = y1;

	if (x2 >= x1) {
		xinc1 = 1;
		xinc2 = 1;
	} else {
		xinc1 = -1;
		xinc2 = -1;
	}

	if (y2 >= y1) {
		yinc1 = 1;
		yinc2 = 1;
	} else {
		yinc1 = -1;
		yinc2 = -1;
	}

	if (deltax >= deltay){
		xinc1 = 0;
		yinc2 = 0;
		den = deltax;
		num = deltax / 2;
		numadd = deltay;
		numpixels = deltax;
	} else {
		xinc2 = 0;
		yinc1 = 0;
		den = deltay;
		num = deltay / 2;
		numadd = deltax;
		numpixels = deltay;
	}

	for (curpixel = 0; curpixel <= numpixels; curpixel++) {
		SSD1305_DrawLine(x, y, x3, y3, color);

		num += numadd;
		if (num >= den) {
			num -= den;
			x += xinc1;
			y += yinc1;
		}
		x += xinc2;
		y += yinc2;
	}
}

void SSD1305_DrawCircle(int16_t x0, int16_t y0, int16_t r, uint8_t c) {
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

    SSD1305_DrawPixel(x0, y0 + r, c);
    SSD1305_DrawPixel(x0, y0 - r, c);
    SSD1305_DrawPixel(x0 + r, y0, c);
    SSD1305_DrawPixel(x0 - r, y0, c);

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        SSD1305_DrawPixel(x0 + x, y0 + y, c);
        SSD1305_DrawPixel(x0 - x, y0 + y, c);
        SSD1305_DrawPixel(x0 + x, y0 - y, c);
        SSD1305_DrawPixel(x0 - x, y0 - y, c);

        SSD1305_DrawPixel(x0 + y, y0 + x, c);
        SSD1305_DrawPixel(x0 - y, y0 + x, c);
        SSD1305_DrawPixel(x0 + y, y0 - x, c);
        SSD1305_DrawPixel(x0 - y, y0 - x, c);
    }
}

void SSD1305_DrawFilledCircle(int16_t x0, int16_t y0, int16_t r, uint8_t c) {
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

    SSD1305_DrawPixel(x0, y0 + r, c);
    SSD1305_DrawPixel(x0, y0 - r, c);
    SSD1305_DrawPixel(x0 + r, y0, c);
    SSD1305_DrawPixel(x0 - r, y0, c);
    SSD1305_DrawLine(x0 - r, y0, x0 + r, y0, c);

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        SSD1305_DrawLine(x0 - x, y0 + y, x0 + x, y0 + y, c);
        SSD1305_DrawLine(x0 + x, y0 - y, x0 - x, y0 - y, c);

        SSD1305_DrawLine(x0 + y, y0 + x, x0 - y, y0 + x, c);
        SSD1305_DrawLine(x0 + y, y0 - x, x0 - y, y0 - x, c);
    }
}

void ssd1305_image(uint8_t *img, uint8_t frame, uint8_t x, uint8_t y)
{
	uint32_t i, b, j;
	
	b = 0;
	if(frame >= img[2])
		return;
	uint32_t start = (frame * (img[3] + (img[4] << 8)));
	
	/* Go through font */
	for (i = 0; i < img[1]; i++) {
		for (j = 0; j < img[0]; j++) {

			SSD1305_DrawPixel(x + j, (y + i), (uint8_t) (img[b/8 + 5 + start] >> (b%8)) & 1);
			b++;
		}
	}
}

void SSD1305_ON(void) {
	SSD1305_WRITECOMMAND(0x8D);  
	SSD1305_WRITECOMMAND(0x14);  
	SSD1305_WRITECOMMAND(0xAF);  
}
void SSD1305_OFF(void) {
	SSD1305_WRITECOMMAND(0x8D);  
	SSD1305_WRITECOMMAND(0x10);
	SSD1305_WRITECOMMAND(0xAE);  
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//  _____ ___   _____ 
// |_   _|__ \ / ____|
//   | |    ) | |     
//   | |   / /| |     
//  _| |_ / /_| |____ 
// |_____|____|\_____|
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////

void ssd1305_I2C_Init() {
	//MX_I2C1_Init();
	uint32_t p = 250000;
	while(p>0)
		p--;
	//HAL_I2C_DeInit(&heval_I2c);
	//p = 250000;
	//while(p>0)
	//	p--;
	//MX_I2C1_Init();
}

void ssd1305_I2C_WriteMulti(uint8_t address, uint8_t reg, uint8_t* data, uint16_t count) {
  //uint8_t dt[129];
  data[0]= reg;
	//HAL_I2C_Master_Transmit(&heval_I2c, address, &reg, 1, 10);
	//HAL_I2C_Master_Transmit(&heval_I2c, address, 0, 1, 10);
	//for(i = 1; i <= count; i++)
	//	dt[i] = data[i-1];
	HAL_I2C_Master_Transmit(&hi2c1, address, data, count+1, 1000);
}


void ssd1305_I2C_WriteMulti_DMA(uint8_t address, uint8_t reg, uint8_t* data, uint16_t count) {	
	HAL_I2C_Master_Transmit(&hi2c1, address, &reg, 1, 100);
	HAL_I2C_Master_Transmit_DMA(&hi2c1, address, data, count);
}


void ssd1305_I2C_Write(uint8_t address, uint8_t reg, uint8_t data) {
	uint8_t dt[2];
	dt[0] = reg;
	dt[1] = data;
	HAL_I2C_Master_Transmit(&hi2c1, address, dt, 2, 10);
}
