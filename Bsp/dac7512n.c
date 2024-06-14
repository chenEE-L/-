#include "bsp.h"

void se_dac(uint16_t data)
{
	char i=0;
	
	data*=3.276f;//(4096/1250.0f);
	
	SPI3_CS_DISABLE();//关闭
	
	wait_spi2_bit(100);//高电平时间不小于33ns
	SPI3_CS_ENABLE();//使能
	wait_spi2_bit(100);//高电平时间不小于33ns
	for(i=0;i<16;i++)
	{
		if(data&0x8000)
			GPIO_SetBits(GPIOB, SPI3_MOSI);
		else
			GPIO_ResetBits(GPIOB, SPI3_MOSI);
		data<<=1;
		GPIO_SetBits(GPIOB, SPI3_SCLK);
		wait_spi2_bit(100);//高电平时间不小于33ns
		GPIO_ResetBits(GPIOB, SPI3_SCLK);
		wait_spi2_bit(100);//高电平时间不小于33ns
	}
	
	//while(SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE)==RESET);
	//SPI_I2S_SendData(SPI3,data);//发数据
	
	
}
