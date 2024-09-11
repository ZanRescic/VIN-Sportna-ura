/*
 * lcd.c
 *
 *  Created on: 29 SEP, 2022
 *  Author: wenqb
 */


#define PI 3.14159265358979323846
#include "cg9a01.h"
#include "main.h"
//#include "spi.h"
#include "math.h"
#include "font.h"
#include "string.h";
extern SPI_HandleTypeDef hspi2;
extern UART_HandleTypeDef huart3;
#define LCD_RST_1 HAL_GPIO_WritePin(LCD_RST_R_GPIO_Port,LCD_RST_R_Pin,GPIO_PIN_SET)			// LCD_RST = 1 , LCD RESET pin
#define LCD_RST_0 HAL_GPIO_WritePin(LCD_RST_R_GPIO_Port,LCD_RST_R_Pin,GPIO_PIN_RESET)		// LCD_RST = 0 , LCD RESET pin

#define LCD_CS_1 HAL_GPIO_WritePin(LCD_CS_R_GPIO_Port,LCD_CS_R_Pin,GPIO_PIN_SET)			// LCD_CS = 1, LCD select pin
#define LCD_CS_0 HAL_GPIO_WritePin(LCD_CS_R_GPIO_Port,LCD_CS_R_Pin,GPIO_PIN_RESET)			// LCD_CS = 0, LCD select pin

#define LCD_DC_1 HAL_GPIO_WritePin(LCD_DC_R_GPIO_Port,LCD_DC_R_Pin,GPIO_PIN_SET)			// LCD_DC = 1, LCD Data/Command pin
#define LCD_DC_0 HAL_GPIO_WritePin(LCD_DC_R_GPIO_Port,LCD_DC_R_Pin,GPIO_PIN_RESET)			// LCD_DC = 0，LCD Data/Command pin

HAL_StatusTypeDef SPI_Write(uint8_t* pbuff, uint16_t size);
HAL_StatusTypeDef SPI_Read(uint8_t* pbuff, uint16_t size);

unsigned char String6_12[];
unsigned short pic[];

#define 	BUFSIZE2 256
char	SendBuffer2[BUFSIZE2];

void Draw_Arc(unsigned int x0, unsigned int y0, unsigned char r, unsigned int color, float start_angle, float end_angle);

//===============================================================
//write parameter

void  Write_Cmd_Data (unsigned char CMDP)
{
    LCD_CS_0;
   	LCD_DC_1;

   	SPI_Write(&CMDP, 1);

   	LCD_CS_1;
}

//=============================================================
//write command

void Write_Cmd(unsigned char CMD)
{
    LCD_CS_0;
   	LCD_DC_0;

   	SPI_Write(&CMD, 1);

   	LCD_CS_1;
}

//==============================================================
//write  data word

void  Write_Data_U16(unsigned int y)
{
	unsigned char m,n;
	m=y>>8;
	n=y;
	Write_Data(m,n);
}


//===================================================================
//write data byte

void Write_Data(unsigned char DH,unsigned char DL)
{
    LCD_CS_0;
   	LCD_DC_1;

   	SPI_Write(&DH, 1);
   	SPI_Write(&DL, 1);

   	LCD_CS_1;
}


void Write_Bytes(unsigned char * pbuff, unsigned short size)
{
    LCD_CS_0;
   	LCD_DC_1;

   	SPI_Write(pbuff, size);

   	LCD_CS_1;
}


HAL_StatusTypeDef SPI_Write(uint8_t* pbuff, uint16_t size)
{
	//DMA, use HAL_SPI_Transmit_DMA() function
    HAL_StatusTypeDef status =  HAL_SPI_Transmit_DMA(&hspi2, pbuff, size);

    /*switch (status) {
		case HAL_OK:
			snprintf(SendBuffer2,BUFSIZE2,"HAL_OK\n\r");
			HAL_UART_Transmit(&huart3,SendBuffer2,strlen(SendBuffer2),100);
			break;
		case HAL_ERROR:
			snprintf(SendBuffer2,BUFSIZE2,"HAL_ERROR\n\r");
			HAL_UART_Transmit(&huart3,SendBuffer2,strlen(SendBuffer2),100);
			break;
		case HAL_BUSY:
			snprintf(SendBuffer2,BUFSIZE2,"HAL_BUSY\n\r");
			HAL_UART_Transmit(&huart3,SendBuffer2,strlen(SendBuffer2),100);
			break;
		case HAL_TIMEOUT:
			snprintf(SendBuffer2,BUFSIZE2,"HAL_TIMEOUT\n\r");
			HAL_UART_Transmit(&huart3,SendBuffer2,strlen(SendBuffer2),100);
			break;
		default:
			snprintf(SendBuffer2,BUFSIZE2,"OTHER\n\r");
			HAL_UART_Transmit(&huart3,SendBuffer2,strlen(SendBuffer2),100);
			break;
	}
     */
    while(HAL_SPI_GetState(&hspi2) != HAL_SPI_STATE_READY){;}
    return status;

    //no DMA, use HAL_SPI_Transmit() function
    //return HAL_SPI_Transmit(&hspi1, pbuff, size, 100);
}

HAL_StatusTypeDef SPI_Read(uint8_t* pbuff, uint16_t size)
{
	//DMA, use HAL_SPI_Receive_DMA() function
   HAL_StatusTypeDef status = HAL_SPI_Receive_DMA(&hspi2, pbuff, size);


   while(HAL_SPI_GetState(&hspi2) != HAL_SPI_STATE_READY){;}
   return status;

   //no DMA, use HAL_SPI_Receive() function
   //return HAL_SPI_Receive(&hspi1, pbuff, size, 100);
}


//=============================================================
//LCD Initial

void GC9A01_Initial(void)
{
  	LCD_CS_1;
	HAL_Delay(5);
	LCD_RST_0;
	HAL_Delay(10);
	LCD_RST_1;
	HAL_Delay(120);


 //************* Start Initial Sequence **********//
	Write_Cmd(0xEF);

	Write_Cmd(0xEB);
	Write_Cmd_Data(0x14);

    Write_Cmd(0xFE);
	Write_Cmd(0xEF);

	Write_Cmd(0xEB);
	Write_Cmd_Data(0x14);

	Write_Cmd(0x84);
	Write_Cmd_Data(0x40);

	Write_Cmd(0x85);
	Write_Cmd_Data(0xFF);

	Write_Cmd(0x86);
	Write_Cmd_Data(0xFF);

	Write_Cmd(0x87);
	Write_Cmd_Data(0xFF);

	Write_Cmd(0x88);
	Write_Cmd_Data(0x0A);

	Write_Cmd(0x89);
	Write_Cmd_Data(0x21);

	Write_Cmd(0x8A);
	Write_Cmd_Data(0x00);

	Write_Cmd(0x8B);
	Write_Cmd_Data(0x80);

	Write_Cmd(0x8C);
	Write_Cmd_Data(0x01);

	Write_Cmd(0x8D);
	Write_Cmd_Data(0x01);

	Write_Cmd(0x8E);
	Write_Cmd_Data(0xFF);

	Write_Cmd(0x8F);
	Write_Cmd_Data(0xFF);


	Write_Cmd(0xB6);
	Write_Cmd_Data(0x00);
	Write_Cmd_Data(0x00);

	Write_Cmd(0x36);

	if(USE_HORIZONTAL==0)Write_Cmd_Data(0x18);
	else if(USE_HORIZONTAL==1)Write_Cmd_Data(0x28);
	else if(USE_HORIZONTAL==2)Write_Cmd_Data(0x48);
	else Write_Cmd_Data(0x88);



	Write_Cmd(0x3A);
	Write_Cmd_Data(0x05);


	Write_Cmd(0x90);
	Write_Cmd_Data(0x08);
	Write_Cmd_Data(0x08);
	Write_Cmd_Data(0x08);
	Write_Cmd_Data(0x08);

	Write_Cmd(0xBD);
	Write_Cmd_Data(0x06);

	Write_Cmd(0xBC);
	Write_Cmd_Data(0x00);

	Write_Cmd(0xFF);
	Write_Cmd_Data(0x60);
	Write_Cmd_Data(0x01);
	Write_Cmd_Data(0x04);

	Write_Cmd(0xC3);
	Write_Cmd_Data(0x13);
	Write_Cmd(0xC4);
	Write_Cmd_Data(0x13);

	Write_Cmd(0xC9);
	Write_Cmd_Data(0x22);

	Write_Cmd(0xBE);
	Write_Cmd_Data(0x11);

	Write_Cmd(0xE1);
	Write_Cmd_Data(0x10);
	Write_Cmd_Data(0x0E);

	Write_Cmd(0xDF);
	Write_Cmd_Data(0x21);
	Write_Cmd_Data(0x0c);
	Write_Cmd_Data(0x02);

	Write_Cmd(0xF0);
    Write_Cmd_Data(0x45);
    Write_Cmd_Data(0x09);
 	Write_Cmd_Data(0x08);
  	Write_Cmd_Data(0x08);
 	Write_Cmd_Data(0x26);
 	Write_Cmd_Data(0x2A);

 	Write_Cmd(0xF1);
 	Write_Cmd_Data(0x43);
 	Write_Cmd_Data(0x70);
 	Write_Cmd_Data(0x72);
 	Write_Cmd_Data(0x36);
 	Write_Cmd_Data(0x37);
 	Write_Cmd_Data(0x6F);


 	Write_Cmd(0xF2);
 	Write_Cmd_Data(0x45);
 	Write_Cmd_Data(0x09);
 	Write_Cmd_Data(0x08);
 	Write_Cmd_Data(0x08);
 	Write_Cmd_Data(0x26);
 	Write_Cmd_Data(0x2A);

 	Write_Cmd(0xF3);
 	Write_Cmd_Data(0x43);
 	Write_Cmd_Data(0x70);
 	Write_Cmd_Data(0x72);
 	Write_Cmd_Data(0x36);
 	Write_Cmd_Data(0x37);
 	Write_Cmd_Data(0x6F);

	Write_Cmd(0xED);
	Write_Cmd_Data(0x1B);
	Write_Cmd_Data(0x0B);

	Write_Cmd(0xAE);
	Write_Cmd_Data(0x77);

	Write_Cmd(0xCD);
	Write_Cmd_Data(0x63);


	Write_Cmd(0x70);
	Write_Cmd_Data(0x07);
	Write_Cmd_Data(0x07);
	Write_Cmd_Data(0x04);
	Write_Cmd_Data(0x0E);
	Write_Cmd_Data(0x0F);
	Write_Cmd_Data(0x09);
	Write_Cmd_Data(0x07);
	Write_Cmd_Data(0x08);
	Write_Cmd_Data(0x03);

	Write_Cmd(0xE8);
	Write_Cmd_Data(0x34);

	Write_Cmd(0x62);
	Write_Cmd_Data(0x18);
	Write_Cmd_Data(0x0D);
	Write_Cmd_Data(0x71);
	Write_Cmd_Data(0xED);
	Write_Cmd_Data(0x70);
	Write_Cmd_Data(0x70);
	Write_Cmd_Data(0x18);
	Write_Cmd_Data(0x0F);
	Write_Cmd_Data(0x71);
	Write_Cmd_Data(0xEF);
	Write_Cmd_Data(0x70);
	Write_Cmd_Data(0x70);

	Write_Cmd(0x63);
	Write_Cmd_Data(0x18);
	Write_Cmd_Data(0x11);
	Write_Cmd_Data(0x71);
	Write_Cmd_Data(0xF1);
	Write_Cmd_Data(0x70);
	Write_Cmd_Data(0x70);
	Write_Cmd_Data(0x18);
	Write_Cmd_Data(0x13);
	Write_Cmd_Data(0x71);
	Write_Cmd_Data(0xF3);
	Write_Cmd_Data(0x70);
	Write_Cmd_Data(0x70);

	Write_Cmd(0x64);
	Write_Cmd_Data(0x28);
	Write_Cmd_Data(0x29);
	Write_Cmd_Data(0xF1);
	Write_Cmd_Data(0x01);
	Write_Cmd_Data(0xF1);
	Write_Cmd_Data(0x00);
	Write_Cmd_Data(0x07);

	Write_Cmd(0x66);
	Write_Cmd_Data(0x3C);
	Write_Cmd_Data(0x00);
	Write_Cmd_Data(0xCD);
	Write_Cmd_Data(0x67);
	Write_Cmd_Data(0x45);
	Write_Cmd_Data(0x45);
	Write_Cmd_Data(0x10);
	Write_Cmd_Data(0x00);
	Write_Cmd_Data(0x00);
	Write_Cmd_Data(0x00);

	Write_Cmd(0x67);
	Write_Cmd_Data(0x00);
	Write_Cmd_Data(0x3C);
	Write_Cmd_Data(0x00);
	Write_Cmd_Data(0x00);
	Write_Cmd_Data(0x00);
	Write_Cmd_Data(0x01);
	Write_Cmd_Data(0x54);
	Write_Cmd_Data(0x10);
	Write_Cmd_Data(0x32);
	Write_Cmd_Data(0x98);

	Write_Cmd(0x74);
	Write_Cmd_Data(0x10);
	Write_Cmd_Data(0x85);
	Write_Cmd_Data(0x80);
	Write_Cmd_Data(0x00);
	Write_Cmd_Data(0x00);
	Write_Cmd_Data(0x4E);
	Write_Cmd_Data(0x00);

    Write_Cmd(0x98);
	Write_Cmd_Data(0x3e);
	Write_Cmd_Data(0x07);

	Write_Cmd(0x35);
	Write_Cmd(0x21);

	Write_Cmd(0x11);
	HAL_Delay(120);
	Write_Cmd(0x29);
	HAL_Delay(20);
}


//===============================================================
//clear screen
//point to point clear , slowly
void ClearScreen(unsigned int bColor)
{
 unsigned int i,j;

 LCD_SetPos(0,0,GC9A01_TFTWIDTH-1,GC9A01_TFTHEIGHT-1);//240x240

 for (i=0;i<GC9A01_TFTWIDTH;i++)
 {
	 for (j=0;j<GC9A01_TFTHEIGHT;j++)
		 Write_Data_U16(bColor);
 }

}

//===============================================================
//clear screen2
//Use DMA，faster
void ClearScreen2(unsigned int bColor)
{
	ClearWindow(0,0,GC9A01_TFTWIDTH,GC9A01_TFTHEIGHT,bColor);
}

void ClearWindow(unsigned int startX, unsigned int startY, unsigned int endX, unsigned int endY, unsigned int bColor)
{
 unsigned int i;

 //Exchange high 8bit and low 8bit of bColor for DMA batch transmit
 unsigned char hb = (bColor&0xFFFF) >> 8;
 unsigned char lb = bColor & 0xFF;
 unsigned short tempColor = lb * 256 + hb;

 unsigned int totalSize = (endX-startX) * (endY - startY) * 2; // total clear window data size
 unsigned int bufSize = 512;  // define bufSize, need less than DMA transmit size

 unsigned int loopNum = (totalSize - (totalSize % bufSize)) / bufSize; // transmit loop times
 unsigned int modNum = totalSize % bufSize;  // remainder data bytes


 //use a tempBuf to initial bColor data, bufSize < DMA transmit size
 unsigned short tempBuf[bufSize];
 unsigned char * ptempBuf;

 //init tempBuf data to tempColor( Exchange high 8bit and low 8bit of bColor )
 for(i=0; i<bufSize; i++){
	 tempBuf[i] = tempColor;
 }

 // Clear window size: from (startX, startY) to (endX, endY)
 LCD_SetPos(startX,startY,endX-1,endY-1);// (endX-startX) * (endY - startY)

 // transmit bufSize byte one time, loopNum loops
 ptempBuf = (unsigned char *)tempBuf;
 for(i=0; i<loopNum; i++){
	 Write_Bytes(ptempBuf, bufSize);
 }

 // transmit remainder data, modNum bytes
 Write_Bytes(ptempBuf, modNum);

}


//===============================================================
 void LCD_SetPos(unsigned int Xstart,unsigned int Ystart,unsigned int Xend,unsigned int Yend)
{
	Write_Cmd(0x2a);
	Write_Cmd_Data(Xstart>>8);
	Write_Cmd_Data(Xstart);
 	Write_Cmd_Data(Xend>>8);
	Write_Cmd_Data(Xend);

	Write_Cmd(0x2b);
	Write_Cmd_Data(Ystart>>8);
	Write_Cmd_Data(Ystart);
	Write_Cmd_Data(Yend>>8);
	Write_Cmd_Data(Yend);

  	Write_Cmd(0x2c);//LCD_WriteCMD(GRAMWR);
}


#define ZIFU_WIDTH 8
#define ZIFU_HEIGHT 12







 //show one Character
 void showzifu(unsigned int x,unsigned int y,unsigned char value,unsigned int dcolor,unsigned int bgcolor)
 {
 	unsigned char i,j;
 	unsigned char *temp=String6_12;

    LCD_SetPos(x,y,x+7,y+11);
 	temp+=(value-32)*12;
 	for(j=0;j<12;j++)
 	{
 		for(i=0;i<8;i++)
 		{
 		 	if((*temp&(1<<(7-i)))!=0)
 			{
 				Write_Data(dcolor>>8,dcolor);
 			}
 			else
 			{
 				Write_Data(bgcolor>>8,bgcolor);
 			}
 		}
 		temp++;
 	 }
 }


  void showzifu_dma_scaled(unsigned int x, unsigned int y, unsigned char value, unsigned int dcolor, unsigned int bgcolor, unsigned int scale)
  {
      unsigned char i, j;
      unsigned char *temp = String6_12;
      unsigned int bufSize = 512;  // Define bufSize, similar to ClearWindow
      unsigned short tempBuf[bufSize];  // Buffer to store pixel data for DMA
      unsigned short pixelIndex = 0;

      unsigned short dcolor_swapped = ((dcolor & 0xFF) << 8) | ((dcolor >> 8) & 0xFF); // Swap high and low byte
      unsigned short bgcolor_swapped = ((bgcolor & 0xFF) << 8) | ((bgcolor >> 8) & 0xFF); // Swap high and low byte

      // Set the new position for the scaled character
      LCD_SetPos(x, y, x + (8 * scale) - 1, y + (12 * scale) - 1);

      temp += (value - 32) * 12;  // Point to the bitmap of the character

      // Loop through the character bitmap
      for (j = 0; j < 12; j++) {
          for (int y_scale = 0; y_scale < scale; y_scale++) {  // Y-axis scaling based on 'scale'
              for (i = 0; i < 8; i++) {
                  unsigned short pixelColor = ((*temp & (1 << (7 - i))) != 0) ? dcolor_swapped : bgcolor_swapped;

                  for (int x_scale = 0; x_scale < scale; x_scale++) {  // X-axis scaling based on 'scale'
                      tempBuf[pixelIndex++] = pixelColor;  // Add the scaled pixel

                      // If the buffer is full, transmit using DMA
                      if (pixelIndex >= bufSize) {
                          Write_Bytes((unsigned char *)tempBuf, bufSize * 2);  // Send the buffer via DMA
                          pixelIndex = 0;  // Reset the buffer index
                      }
                  }
              }

              // If the buffer is full, transmit using DMA
              if (pixelIndex >= bufSize) {
                  Write_Bytes((unsigned char *)tempBuf, bufSize * 2);  // Send the buffer via DMA
                  pixelIndex = 0;  // Reset the buffer index
              }
          }
          temp++;  // Move to the next row in the character bitmap
      }

      // Send remaining pixels in the buffer
      if (pixelIndex > 0) {
          Write_Bytes((unsigned char *)tempBuf, pixelIndex * 2);  // Send the remaining data
      }
  }

  void showzifustr_scaled(unsigned int x, unsigned int y, unsigned char *str, unsigned int dcolor, unsigned int bgcolor, unsigned int scale, unsigned int space)
  {
      unsigned int x1 = x;
      unsigned int y1 = y;
      while (*str != '\0') {
          showzifu_dma_scaled(x1, y1, *str, dcolor, bgcolor, scale);
          x1 += space;
          str++;
      }
  }



 void showzifu_dma(unsigned int x, unsigned int y, unsigned char value, unsigned int dcolor, unsigned int bgcolor)
 {
     unsigned char i, j;
     unsigned char *temp = String6_12;
     unsigned int bufSize = 512;  // Define bufSize, similar to ClearWindow
     unsigned short tempBuf[bufSize];  // Buffer to store pixel data for DMA
     unsigned short pixelIndex = 0;

     unsigned short dcolor_swapped = ((dcolor & 0xFF) << 8) | ((dcolor >> 8) & 0xFF); // Swap high and low byte
     unsigned short bgcolor_swapped = ((bgcolor & 0xFF) << 8) | ((bgcolor >> 8) & 0xFF); // Swap high and low byte

     LCD_SetPos(x, y, x + 7, y + 11);  // Set position on display for the character (8x12)

     temp += (value - 32) * 12;  // Point to the bitmap of the character

     // Loop through the character bitmap
     for (j = 0; j < 12; j++) {
         for (i = 0; i < 8; i++) {
             if ((*temp & (1 << (7 - i))) != 0) {
                 tempBuf[pixelIndex++] = dcolor_swapped;  // Use swapped dcolor
             } else {
                 tempBuf[pixelIndex++] = bgcolor_swapped;  // Use swapped bgcolor
             }

             // If the buffer is full, transmit using DMA
             if (pixelIndex >= bufSize) {
                 Write_Bytes((unsigned char *)tempBuf, bufSize * 2);  // Send the buffer via DMA
                 pixelIndex = 0;  // Reset the buffer index
             }
         }
         temp++;
     }

     // Send remaining pixels in the buffer
     if (pixelIndex > 0) {
         Write_Bytes((unsigned char *)tempBuf, pixelIndex * 2);  // Send the remaining data
     }
 }



 //show String
 void showzifustr(unsigned int x,unsigned int y,unsigned char *str,unsigned int dcolor,unsigned int bgcolor)
 {
 	unsigned int x1,y1;
 	x1=x;
 	y1=y;
 	while(*str!='\0')
 	{
 		showzifu_dma(x1,y1,*str,dcolor,bgcolor);
 		//x1+=7;
 		x1 += 18;
 		str++;
 	}
 }


 void LCD_DrawPoint(unsigned int x,unsigned int y,unsigned int color)
 {
 	LCD_SetPos(x,y,x,y);
 	Write_Data_U16(color);
 }


 void LCD_DrawLine(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2,unsigned int color)
 {
 	unsigned int t;
 	int xerr=0,yerr=0,delta_x,delta_y,distance;
 	int incx,incy,uRow,uCol;
 	delta_x=x2-x1;
 	delta_y=y2-y1;
 	uRow=x1;
 	uCol=y1;
 	if(delta_x>0)incx=1;
 	else if (delta_x==0)incx=0;
 	else {incx=-1;delta_x=-delta_x;}
 	if(delta_y>0)incy=1;
 	else if (delta_y==0)incy=0;
 	else {incy=-1;delta_y=-delta_x;}
 	if(delta_x>delta_y)distance=delta_x;
 	else distance=delta_y;
 	for(t=0;t<distance+1;t++)
 	{
 		LCD_DrawPoint(uRow,uCol,color);
 		xerr+=delta_x;
 		yerr+=delta_y;
 		if(xerr>distance)
 		{
 			xerr-=distance;
 			uRow+=incx;
 		}
 		if(yerr>distance)
 		{
 			yerr-=distance;
 			uCol+=incy;
 		}
 	}
 }


 void LCD_DrawRectangle(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2,unsigned int color)
 {
 	LCD_DrawLine(x1,y1,x2,y1,color);
 	LCD_DrawLine(x1,y1,x1,y2,color);
 	LCD_DrawLine(x1,y2,x2,y2,color);
 	LCD_DrawLine(x2,y1,x2,y2,color);
 }


 void Draw_Circle(unsigned int x0,unsigned int y0,unsigned char r,unsigned int color)
 {
 	int a,b;
 	a=0;b=r;
 	while(a<=b)
 	{
 		LCD_DrawPoint(x0-b,y0-a,color);             //3
 		LCD_DrawPoint(x0+b,y0-a,color);             //0
 		LCD_DrawPoint(x0-a,y0+b,color);             //1
 		LCD_DrawPoint(x0-a,y0-b,color);             //2
 		LCD_DrawPoint(x0+b,y0+a,color);             //4
 		LCD_DrawPoint(x0+a,y0-b,color);             //5
 		LCD_DrawPoint(x0+a,y0+b,color);             //6
 		LCD_DrawPoint(x0-b,y0+a,color);             //7
 		a++;
 		if((a*a+b*b)>(r*r))
 		{
 			b--;
 		}
 	}
 }


 void Draw_Arc_DMA(unsigned int x0, unsigned int y0, unsigned char r, unsigned int color, int quadrant)
 {
     unsigned char thickness = 5;
     unsigned int bufSize = 512;
     unsigned short tempBuf[bufSize];

     unsigned char hb = (color & 0xFFFF) >> 8;
     unsigned char lb = color & 0xFF;
     unsigned short tempColor = lb * 256 + hb;

     for (int i = 0; i < bufSize / 2; i++) {
         tempBuf[i] = tempColor;
     }

     for (unsigned char t = 0; t < thickness; t++) {
         int a = 0;
         int b = r - t;
         int err = 0;

         while (a <= b) {
             if (quadrant == 1) {

                 LCD_SetPos(x0 + a, y0 - b, x0 + a, y0 - b);
                 Write_Bytes((unsigned char *)tempBuf, 2);

                 LCD_SetPos(x0 + b, y0 - a, x0 + b, y0 - a);
                 Write_Bytes((unsigned char *)tempBuf, 2);
             }
             else if (quadrant == 2) {

                 LCD_SetPos(x0 - a, y0 - b, x0 - a, y0 - b);
                 Write_Bytes((unsigned char *)tempBuf, 2);

                 LCD_SetPos(x0 - b, y0 - a, x0 - b, y0 - a);
                 Write_Bytes((unsigned char *)tempBuf, 2);
             }
             else if (quadrant == 3) {
                 LCD_SetPos(x0 - a, y0 + b, x0 - a, y0 + b);
                 Write_Bytes((unsigned char *)tempBuf, 2);

                 LCD_SetPos(x0 - b, y0 + a, x0 - b, y0 + a);
                 Write_Bytes((unsigned char *)tempBuf, 2);
             }
             else if (quadrant == 4) {
                 LCD_SetPos(x0 + a, y0 + b, x0 + a, y0 + b);
                 Write_Bytes((unsigned char *)tempBuf, 2);
                 LCD_SetPos(x0 + b, y0 + a, x0 + b, y0 + a);
                 Write_Bytes((unsigned char *)tempBuf, 2);
             }

             a++;
             if (err <= 0) {
                 err += 2 * a + 1;
             }
             if (err > 0) {
                 b--;
                 err -= 2 * b + 1;
             }
         }
     }
 }

 void Draw_Filled_Circle_DMA(unsigned int x0, unsigned int y0, unsigned char r, unsigned int color)
 {
     int a = 0;
     int b = r;
     int err = 0;


     unsigned char hb = (color & 0xFFFF) >> 8;
     unsigned char lb = color & 0xFF;
     unsigned short tempColor = lb * 256 + hb;

     unsigned int bufSize = 512;
     unsigned short tempBuf[bufSize];
     unsigned int pixelIndex = 0;

     for (int i = 0; i < bufSize / 2; i++) {
         tempBuf[i] = tempColor;
     }

     while (a <= b) {

         LCD_SetPos(x0 - b, y0 - a, x0 + b, y0 - a);
         Write_Bytes((unsigned char *)tempBuf, (2 * (b - (-b) + 1)));

         LCD_SetPos(x0 - b, y0 + a, x0 + b, y0 + a);
         Write_Bytes((unsigned char *)tempBuf, (2 * (b - (-b) + 1)));

         LCD_SetPos(x0 - a, y0 - b, x0 + a, y0 - b);
         Write_Bytes((unsigned char *)tempBuf, (2 * (a - (-a) + 1)));

         LCD_SetPos(x0 - a, y0 + b, x0 + a, y0 + b);
         Write_Bytes((unsigned char *)tempBuf, (2 * (a - (-a) + 1)));
         a++;
         if (err <= 0) {
             err += 2 * a + 1;
         }
         if (err > 0) {
             b--;
             err -= 2 * b + 1;
         }
     }
 }




 void drawCircleDMA(unsigned int x0, unsigned int y0, unsigned int r, unsigned int color)
 {
     int x = r;
     int y = 0;
     int err = 0;

     unsigned int bufSize = 512;
     unsigned short tempBuf[bufSize];
     unsigned short pixelIndex = 0;

     unsigned short color_swapped = ((color & 0xFF) << 8) | ((color >> 8) & 0xFF);

     while (x >= y)
     {

         LCD_SetPos(x0 + x, y0 + y, x0 + x, y0 + y);
         tempBuf[pixelIndex++] = color_swapped;

         LCD_SetPos(x0 + y, y0 + x, x0 + y, y0 + x);
         tempBuf[pixelIndex++] = color_swapped;


         LCD_SetPos(x0 - y, y0 + x, x0 - y, y0 + x);
         tempBuf[pixelIndex++] = color_swapped;

         LCD_SetPos(x0 - x, y0 + y, x0 - x, y0 + y);
         tempBuf[pixelIndex++] = color_swapped;


         LCD_SetPos(x0 - x, y0 - y, x0 - x, y0 - y);
         tempBuf[pixelIndex++] = color_swapped;

         LCD_SetPos(x0 - y, y0 - x, x0 - y, y0 - x);
         tempBuf[pixelIndex++] = color_swapped;


         LCD_SetPos(x0 + y, y0 - x, x0 + y, y0 - x);
         tempBuf[pixelIndex++] = color_swapped;

         LCD_SetPos(x0 + x, y0 - y, x0 + x, y0 - y);
         tempBuf[pixelIndex++] = color_swapped;


         if (pixelIndex >= bufSize)
         {
             Write_Bytes((unsigned char *)tempBuf, bufSize * 2);
             pixelIndex = 0;
         }


         if (err <= 0)
         {
             y += 1;
             err += 2 * y + 1;
         }
         if (err > 0)
         {
             x -= 1;
             err -= 2 * x + 1;
         }
     }


     if (pixelIndex > 0)
     {
         Write_Bytes((unsigned char *)tempBuf, pixelIndex * 2);
     }
 }





 //============================================================
 //show gray

 void DispRGBGray(void)
 {

 	unsigned int  A,B,C;
     unsigned int  i,j,k,DH;

     LCD_SetPos(0,0,GC9A01_TFTWIDTH-1,GC9A01_TFTHEIGHT-1);//240x240

 	for(k=0;k<60;k++)
 	{
     	A=0;
     	B=0;
     	C=0;
 		for(i=0;i<16;i++)
 		{
     		for(j=0;j<15;j++)
     		{
      			DH=(A<<11)+(B<<5)+C;
      			Write_Data_U16(DH);
     		}
     		A=A+2;
    			B=B+4;
     		C=C+2;
   		}
 	}

 	for(k=0;k<60;k++)
 	{
     	A=0;
     	B=0;
     	C=0;
 		for(i=0;i<16;i++)
 		{
     		for(j=0;j<15;j++)
     		{
      			DH=(A<<11)+B+C;
      			Write_Data_U16(DH);
     		}
     		A=A+2;
   		}
 	}

 	for(k=0;k<60;k++)
 	{
     	A=0;
     	B=0;
     	C=0;
 		for(i=0;i<16;i++)
 		{
     		for(j=0;j<15;j++)
     		{
      			DH=A+(B<<5)+C;
      			Write_Data_U16(DH);
     		}
     		B=B+4;
   		}
 	}

 	for(k=0;k<60;k++)
 	{
     	A=0;
     	B=0;
     	C=0;
 		for(i=0;i<16;i++)
 		{
     		for(j=0;j<15;j++)
     		{
      			DH=A+B+C;
      			Write_Data_U16(DH);
     		}
     		C=C+2;
   		}
 	}
 }




 //============================================================
 //show picture
 //point to point show picture data, slowly
 void show_picture(void)
 {
 	unsigned char i,j;
 	unsigned int n=0;


    LCD_SetPos(60,40,179,199);

 	for(j=0;j<160;j++)
 	{
 		for(i=0;i<120;i++)
 		{
             Write_Data_U16(pic[n++]);
 	    }
 	}

     return;
 }


 //use DMA batch transmit, faster
 void show_picture2(uint16_t x,uint16_t y,uint16_t b,uint16_t h)
 {
	 int i;
	 unsigned char * pPic;
	 unsigned int totalSize = 157*160*2;
	 unsigned int bufSize = 512*2; //bufSize, need less than DMA transmit data size

 	 unsigned int loopNum = (totalSize - (totalSize % bufSize)) / bufSize;
 	 unsigned int modNum = totalSize % bufSize;


    LCD_SetPos(x,y,(x+b)-1,(y+h)-1);

    //Exchange high 8bit and low 8bit of every point, for picture DMA transmit
    inversPicData();

    pPic = (unsigned char *)pic;

    //transmit bufSize bytes of picture data , loop loopNum times
    for(i=0; i<loopNum; i++){
    	Write_Bytes(pPic+i*bufSize, bufSize);
    }
    //transmit remainder data of picture
    Write_Bytes(pPic+i*bufSize, modNum);

    //Exchange high 8bit and low 8bit of every point, recover picture data
    inversPicData();

    return;
 }

// void show_picture3(void)
//  {
// 	 int i;
// 	 unsigned char * pPic;
// 	 unsigned int totalSize = 240*240*2;
// 	 unsigned int bufSize = 512; //bufSize, need less than DMA transmit data size
//
//  	 unsigned int loopNum = (totalSize - (totalSize % bufSize)) / bufSize;
//  	 unsigned int modNum = totalSize % bufSize;
//
//
//     LCD_SetPos(0,0,239,239);
//
//     //Exchange high 8bit and low 8bit of every point, for picture DMA transmit
//     inversPicData1();
//
//     pPic = (unsigned char *)pic;
//
//     //transmit bufSize bytes of picture data , loop loopNum times
//     for(i=0; i<loopNum; i++){
//     	Write_Bytes(pPic+i*bufSize, bufSize);
//     }
//     //transmit remainder data of picture
//     Write_Bytes(pPic+i*bufSize, modNum);
//
//     //Exchange high 8bit and low 8bit of every point, recover picture data
//     inversPicData1();
//
//     return;
//  }

// void show_picture4(void)
//   {
//  	 int i;
//  	 unsigned char * pPic;
//  	 unsigned int totalSize = 240*240*2;
//  	 unsigned int bufSize = 512; //bufSize, need less than DMA transmit data size
//
//   	 unsigned int loopNum = (totalSize - (totalSize % bufSize)) / bufSize;
//   	 unsigned int modNum = totalSize % bufSize;
//
//
//      LCD_SetPos(0,0,239,239);
//
//      //Exchange high 8bit and low 8bit of every point, for picture DMA transmit
//      inversPicData2();
//
//      pPic = (unsigned char *)pic2;
//
//      //transmit bufSize bytes of picture data , loop loopNum times
//      for(i=0; i<loopNum; i++){
//      	Write_Bytes(pPic+i*bufSize, bufSize);
//      }
//      //transmit remainder data of picture
//      Write_Bytes(pPic+i*bufSize, modNum);
//
//      //Exchange high 8bit and low 8bit of every point, recover picture data
//      inversPicData2();
//
//      return;
//   }


// Exchange high 8bit and low 8bit of picture data, for DMA transmit
 void inversPicData(void){
 	int i;
 	unsigned char hb, lb;

 	for(i=0; i<160*157; i++){  			//160*120 is the size of picture
 			hb = pic[i] >> 8;  			//high 8 bit
 			lb = pic[i] & 0xFF; 		//low 8 bit
 			pic[i] = lb * 256 + hb; 	//exchange high 8 bit and low 8 bit
 	}
 }


 void Draw_Arc(unsigned int x0, unsigned int y0, unsigned char r, unsigned int color, float start_angle, float end_angle)
 {
     int a, b;
     a = 0;
     b = r;

     // Convert start and end angles to radians
     float start_rad = start_angle * PI / 180.0;
     float end_rad = end_angle * PI / 180.0;

     // Loop through each point in the circle and check if it's within the arc's angle range
     while (a <= b)
     {
         // Calculate angles for each octant of the circle
         float angle1 = atan2(-a, -b); // Angle for quadrant 3
         float angle2 = atan2(-a, b);  // Angle for quadrant 4
         float angle3 = atan2(b, -a);  // Angle for quadrant 2
         float angle4 = atan2(b, a);   // Angle for quadrant 1

         // Draw points if within the start and end angles
         if (angle1 >= start_rad && angle1 <= end_rad)
             LCD_DrawPoint(x0 - b, y0 - a, color);  // Quadrant 3
         if (angle2 >= start_rad && angle2 <= end_rad)
             LCD_DrawPoint(x0 + b, y0 - a, color);  // Quadrant 4
         if (angle3 >= start_rad && angle3 <= end_rad)
             LCD_DrawPoint(x0 - a, y0 + b, color);  // Quadrant 2
         if (angle4 >= start_rad && angle4 <= end_rad)
             LCD_DrawPoint(x0 + a, y0 + b, color);  // Quadrant 1

         // Continue drawing circle
         a++;
         if ((a * a + b * b) > (r * r))
         {
             b--;
         }
     }
 }


// void inversPicData1(void){
// 	int i;
// 	unsigned char hb, lb;
//
// 	for(i=0; i<240*240; i++){  			//160*120 is the size of picture
// 			hb = pic[i] >> 8;  			//high 8 bit
// 			lb = pic[i] & 0xFF; 		//low 8 bit
// 			pic[i] = lb * 256 + hb; 	//exchange high 8 bit and low 8 bit
// 	}
// }

// void inversPicData2(void){
// 	int i;
// 	unsigned char hb, lb;
//
// 	for(i=0; i<240*240; i++){  			//160*120 is the size of picture
// 			hb = pic2[i] >> 8;  			//high 8 bit
// 			lb = pic2[i] & 0xFF; 		//low 8 bit
// 			pic2[i] = lb * 256 + hb; 	//exchange high 8 bit and low 8 bit
// 	}
// }
