#include "main.h"
#include "segment.h"

#define UDELAY_VAR	10

const uint32_t digit[] =
{
    FND_0,   FND_1,  FND_2, FND_3, FND_4, FND_5, FND_6, FND_7,         	//0~7
    FND_8,   FND_9,  FND_A, FND_b, FND_C, FND_d, FND_E, FND_F,         	//8~15
    FND_BAR, FND_n,  BLANK, FND_L, FND_o, FND_H, FND_i, FND_h,		   	//16~23
	FND_t,   FND_r,  FND_P, FND_UBAR, FND_u,							//24
};

//uint8_t leddata[10] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
uint8_t leddata[10] = {0x00,};
//======================================================

//======================================================
void Seg_data_set(uint8_t segnum, uint8_t seg_data, uint8_t dot)
{
	if(segnum > 6)
		return;
	
	leddata[segnum] &= 0x80;
    leddata[segnum] |= digit[seg_data];
}
//-----------------------------------------------------------------------------
void Led_data_set(uint8_t segnum, uint8_t led_data, uint8_t onoff)
{
	if(segnum > 9)
		return;

    if(onoff)
		leddata[segnum] |= led_data;
    else
		leddata[segnum] &= ~led_data;
}
//-----------------------------------------------------------------------------
void LD1963_Init(void)
{
	uint8_t i, tx_data;
	
	NSS_HIGH;
	SCK_HIGH;
	
	tx_data = 0x02;							//0b00000010;
	Spi_Send_Data(&tx_data, 1, 1);			// Command 1   Display Mode Setting  6Grids, 12Segment

	tx_data = 0x8F;							//0b10001111;
	Spi_Send_Data(&tx_data, 1, 1);			// Command 4   Display Control b3: 0:Display Off, 1:Display ON   b2-b0: Dimming

	tx_data = 0x40;							//0b01000000;
	Spi_Send_Data(&tx_data, 1, 1);			// Command 2   Data Setting b2 0:Increment address, 1:Fixes Address, 00:Write data, 10: Read Key

	tx_data = 0xC0;							//0b11000000;
	Spi_Send_Data(&tx_data, 1, 0);			// Command 3   Address Setting   b3-b0: Address 00H to 0DH

	tx_data = 0x00;
	for(i=0; i < 0x0D; i++)
		Spi_Send_Data(&tx_data, 1, 0);		// Command 3   Address Setting   b3-b0: Address 00H to 0DH
	
	Spi_Send_Data(&tx_data, 1, 1);
}
//-----------------------------------------------------------------------------
void Send_LED_Data(void)
{
	uint8_t tx_data;

	tx_data = 0xC0;							//0b11000000;				
	Spi_Send_Data(&tx_data, 1, 0);			// Command 3   Address Setting   b3-b0: Address 00H to 0DH
	Spi_Send_Data(leddata, 10, 1);
}
//-----------------------------------------------------------------------------
void Spi_Send_Data(uint8_t *p_tx_buffer, uint8_t tx_len, uint8_t flg_nss)
{
	uint8_t i, j, mask;

	uDelay(UDELAY_VAR);
	NSS_LOW;
	uDelay(UDELAY_VAR);

	for(i=0; i<tx_len; i++)
	{
		mask = 0x01;
		for(j=0; j<8; j++)
		{
			SCK_LOW;
			
			if(p_tx_buffer[i] & mask)
				MOSI_HIGH;
			else
				MOSI_LOW;
			
			uDelay(UDELAY_VAR);
			SCK_HIGH;
			uDelay(UDELAY_VAR);
			mask <<= 1;
		}
	}

	if(flg_nss)
	{
		uDelay(UDELAY_VAR);
		NSS_HIGH;
	}
}
//-----------------------------------------------------------------------------
void uDelay(volatile uint32_t d_count)
{
	while(d_count--)
	{
		__nop();__nop();__nop();
		__nop();__nop();__nop();
		__nop();__nop();__nop();
		__nop();__nop();__nop();
		__nop();__nop();__nop();
		__nop();__nop();__nop();
	}
}

