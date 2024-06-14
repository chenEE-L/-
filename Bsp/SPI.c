#include "stm32f4xx.h"
//#include "config.h"
#include "SPI.h"
#include "stm32f4xx_exti.h"
#include "math.h"

////spi1对应NSS(GPIO_Pin_4)、  SCK(GPIO_Pin_5)、 (GPIO_Pin_6)、MOSI(GPIO_Pin_7)为AF引脚


//SPI2模块对应的SCK(GPIO_Pin_13)、MISO(GPIO_Pin_14)、MOSI(GPIO_Pin_15)为AF引脚 SPI2_CS  GPIO_Pin_12
   
/////采用SPI2接口进行通信
#ifndef SPI2_SOFT

///////////SPI2硬件中断方式读写驱动部分
void NVIC_SPI2Config()
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	NVIC_InitStructure.NVIC_IRQChannel = SPI2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    //SPI_I2S_ITConfig(SPI2, SPI_I2S_IT_TXE, ENABLE);
 	  //SPI_I2S_ITConfig(SPI2, SPI_I2S_IT_RXNE, ENABLE);
}

void SPI3_Configuration()
{
    //SPI_InitTypeDef  SPI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    //启动SPI时钟

	//NVIC_SPI2Config();
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	 //(3)
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽输
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
   GPIO_Init(GPIOB, &GPIO_InitStructure);
	 
	 //(4)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽输出
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
   GPIO_Init(GPIOB, &GPIO_InitStructure); 
	 //(5)
//	 GPIO_PinAFConfig(GPIOB,GPIO_PinSource3,GPIO_AF_SPI3); 
//   GPIO_PinAFConfig(GPIOB,GPIO_PinSource4,GPIO_AF_SPI3);
//	 //(6)
//	   
//      
//    //////SPI模块配置//////
//    SPI_Cmd(SPI3,DISABLE);              //必须先禁能,才能改变MODE
//    SPI3_CS_DISABLE();    
//   

//    //GPIO_SetBits(GPIOB, GPIO_Pin_12)

//     /* SPI1 configuration */
//    SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;
//    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
//    SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;

//		SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
//    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge ;//SPI_CPHA_1Edge;
//		SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;/////4.5M波特率
//    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
//    
//    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
//    SPI_InitStructure.SPI_CRCPolynomial = 0;
//    SPI_Init(SPI3, &SPI_InitStructure);
//     /* Enable SPI2  */
//    SPI_Cmd(SPI3, ENABLE);
//		SPI3_CS_ENABLE(); 
}

    
void SPI2_Configuration()
{
    SPI_InitTypeDef  SPI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    //启动SPI时钟

	//NVIC_SPI2Config();
    
    
     //RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
     RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
		 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);   
		 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE); 
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); 
     //////下面是SPI相关GPIO初始化//////
    //SPI2模块对应的SCK、MISO、MOSI为AF引脚
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14| GPIO_Pin_15;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽输
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
   GPIO_Init(GPIOB, &GPIO_InitStructure);
 
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_14;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_InitStructure.GPIO_OType = GPIO_OType_OD; //开漏
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
   GPIO_Init(GPIOE, &GPIO_InitStructure);
	 
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_8;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_InitStructure.GPIO_OType = GPIO_OType_OD; //开漏
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
   GPIO_Init(GPIOC, &GPIO_InitStructure);
   
   GPIO_PinAFConfig(GPIOB,GPIO_PinSource13,GPIO_AF_SPI2); 
   GPIO_PinAFConfig(GPIOB,GPIO_PinSource14,GPIO_AF_SPI2); 
   GPIO_PinAFConfig(GPIOB,GPIO_PinSource15,GPIO_AF_SPI2); 
   //PB12 pin: SPI2_CS 
   GPIO_InitStructure.GPIO_Pin = SPI2_CS1|SPI2_CS2;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽输出
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
   GPIO_Init(GPIOE, &GPIO_InitStructure);  
    
   GPIO_InitStructure.GPIO_Pin = SPI2_CS3|SPI2_CS4;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽输出
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
   GPIO_Init(GPIOC, &GPIO_InitStructure);
	 
    //////SPI模块配置//////
    //一开始SD初始化阶段，SPI时钟频率必须<400K
    SPI_Cmd(SPI2, DISABLE);              //必须先禁能,才能改变MODE
    SPI2_CS1_DISABLE();
    SPI2_CS2_DISABLE();
		SPI2_CS3_DISABLE();

    //GPIO_SetBits(GPIOB, GPIO_Pin_12)

     /* SPI2 configuration */
     SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
     SPI_InitStructure.SPI_Mode = SPI_Mode_Master;  //主模式
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    /*   ADIS16488采用下面配置
		SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge ;//SPI_CPHA_1Edge;
		SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;/////9M波特率
		*/
		
		SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge ;//SPI_CPHA_2Edge;
		SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;/////
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPI2, &SPI_InitStructure);
     /* Enable SPI2  */
    SPI_Cmd(SPI2, ENABLE);
}
void wait_spi2_bit(int bit_num)
{
		static int i;
	  for(i=0;i<bit_num;i++);
}

void SPI2_Write_Max31865_1(uint16_t addr,uint16_t data)
{
 //uint16_t SPI2_ReceiveData;
 SPI2_CS1_ENABLE();//使能
 //wait_spi2_bit(100);
 //SPI_Cmd(SPI2, ENABLE);
 while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE)==RESET);
 SPI_I2S_SendData(SPI2,addr);//发地址
 while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE)==RESET);
 SPI_I2S_ReceiveData(SPI2);//读数据
 while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE)==RESET);
 SPI_I2S_SendData(SPI2,data);//发数据
 while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE)==RESET);
 SPI_I2S_ReceiveData(SPI2);//读数据
// wait_spi2_bit(100);
 SPI2_CS1_DISABLE();//关闭
 //SPI_Cmd(SPI2, DISABLE); 
// wait_spi2_bit(100);
// SPI2_CS_ENABLE();//使能
}

uint16_t SPI2_Read_Max31865_1(uint16_t addr)
{
 uint16_t SPI2_ReceiveData;
 SPI2_CS1_ENABLE();//使能
 //wait_spi2_bit(100);
 while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE)==RESET);
 SPI_I2S_SendData(SPI2,addr);//发地址
 while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE)==RESET);
 SPI2_ReceiveData = SPI_I2S_ReceiveData(SPI2);//读数据
 while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE)==RESET);
 SPI_I2S_SendData(SPI2,0x00);//发ff读数据
 while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE)==RESET);
 SPI2_ReceiveData = SPI_I2S_ReceiveData(SPI2);//读数据
 //wait_spi2_bit(100);
 SPI2_CS1_DISABLE();//关闭
 //SPI_Cmd(SPI2, DISABLE); 
 //
 //SPI2_CS_ENABLE();//使能
 return SPI2_ReceiveData;
}


void SPI2_Write_Max31865_2(uint16_t addr,uint16_t data)
{
 //uint16_t SPI2_ReceiveData;
 SPI2_CS2_ENABLE();//使能
 //wait_spi2_bit(100);
 //SPI_Cmd(SPI2, ENABLE);
 while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE)==RESET);
 SPI_I2S_SendData(SPI2,addr);//发地址
 while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE)==RESET);
 SPI_I2S_ReceiveData(SPI2);//读数据
 while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE)==RESET);
 SPI_I2S_SendData(SPI2,data);//发数据
 while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE)==RESET);
 SPI_I2S_ReceiveData(SPI2);//读数据
// wait_spi2_bit(100);
 SPI2_CS2_DISABLE();//关闭
 //SPI_Cmd(SPI2, DISABLE); 
// wait_spi2_bit(100);
// SPI2_CS_ENABLE();//使能
}

uint16_t SPI2_Read_Max31865_2(uint16_t addr)
{
 uint16_t SPI2_ReceiveData;
 SPI2_CS2_ENABLE();//使能
 //wait_spi2_bit(100);
 while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE)==RESET);
 SPI_I2S_SendData(SPI2,addr);//发地址
 while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE)==RESET);
 SPI2_ReceiveData = SPI_I2S_ReceiveData(SPI2);//读数据
 while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE)==RESET);
 SPI_I2S_SendData(SPI2,0x00);//发ff读数据
 while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE)==RESET);
 SPI2_ReceiveData = SPI_I2S_ReceiveData(SPI2);//读数据
 //wait_spi2_bit(100);
 SPI2_CS2_DISABLE();//关闭
 //SPI_Cmd(SPI2, DISABLE); 
 //
 //SPI2_CS_ENABLE();//使能
 return SPI2_ReceiveData;
}


void SPI2_Write_Max31865_3(uint16_t addr,uint16_t data)
{
 //uint16_t SPI2_ReceiveData;
 SPI2_CS3_ENABLE();//使能
 //wait_spi2_bit(100);
 //SPI_Cmd(SPI2, ENABLE);
 while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE)==RESET);
 SPI_I2S_SendData(SPI2,addr);//发地址
 while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE)==RESET);
 SPI_I2S_ReceiveData(SPI2);//读数据
 while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE)==RESET);
 SPI_I2S_SendData(SPI2,data);//发数据
 while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE)==RESET);
 SPI_I2S_ReceiveData(SPI2);//读数据
// wait_spi2_bit(100);
 SPI2_CS3_DISABLE();//关闭
 //SPI_Cmd(SPI2, DISABLE); 
// wait_spi2_bit(100);
// SPI2_CS_ENABLE();//使能
}

uint16_t SPI2_Read_Max31865_3(uint16_t addr)
{
 uint16_t SPI2_ReceiveData;
 SPI2_CS3_ENABLE();//使能
 //wait_spi2_bit(100);
 while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE)==RESET);
 SPI_I2S_SendData(SPI2,addr);//发地址
 while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE)==RESET);
 SPI2_ReceiveData = SPI_I2S_ReceiveData(SPI2);//读数据
 while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE)==RESET);
 SPI_I2S_SendData(SPI2,0x00);//发ff读数据
 while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE)==RESET);
 SPI2_ReceiveData = SPI_I2S_ReceiveData(SPI2);//读数据
 //wait_spi2_bit(100);
 SPI2_CS3_DISABLE();//关闭
 //SPI_Cmd(SPI2, DISABLE); 
 //
 //SPI2_CS_ENABLE();//使能
 return SPI2_ReceiveData;
}


void SPI2_Write_Max31865_4(uint16_t addr,uint16_t data)
{
 //uint16_t SPI2_ReceiveData;
 SPI2_CS4_ENABLE();//使能
 //wait_spi2_bit(100);
 //SPI_Cmd(SPI2, ENABLE);
 while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE)==RESET);
 SPI_I2S_SendData(SPI2,addr);//发地址
 while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE)==RESET);
 SPI_I2S_ReceiveData(SPI2);//读数据
 while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE)==RESET);
 SPI_I2S_SendData(SPI2,data);//发数据
 while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE)==RESET);
 SPI_I2S_ReceiveData(SPI2);//读数据
// wait_spi2_bit(100);
 SPI2_CS4_DISABLE();//关闭
 //SPI_Cmd(SPI2, DISABLE); 
// wait_spi2_bit(100);
// SPI2_CS_ENABLE();//使能
}

uint16_t SPI2_Read_Max31865_4(uint16_t addr)
{
 uint16_t SPI2_ReceiveData;
 SPI2_CS4_ENABLE();//使能
 //wait_spi2_bit(100);
 while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE)==RESET);
 SPI_I2S_SendData(SPI2,addr);//发地址
 while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE)==RESET);
 SPI2_ReceiveData = SPI_I2S_ReceiveData(SPI2);//读数据
 while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE)==RESET);
 SPI_I2S_SendData(SPI2,0x00);//发ff读数据
 while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE)==RESET);
 SPI2_ReceiveData = SPI_I2S_ReceiveData(SPI2);//读数据
 //wait_spi2_bit(100);
 SPI2_CS4_DISABLE();//关闭
 //SPI_Cmd(SPI2, DISABLE); 
 //
 //SPI2_CS_ENABLE();//使能
 return SPI2_ReceiveData;
}

uint32_t SPI2_ReadWrite32(uint32_t data)
{
	uint32_t SPI2_ReceiveData;
	uint16_t data_send;
	//int i;
	
	SPI2_CS1_ENABLE();
	
	//wait_spi2_bit(100);
	
	while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE)==RESET);
	data_send = (uint16_t)((data>>16)&0x0ffff);
	SPI_I2S_SendData(SPI2,data_send);
  
	while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE)==RESET);
	SPI2_ReceiveData = SPI_I2S_ReceiveData(SPI2);
	SPI2_ReceiveData <<= 16;
	//printf("--1--%x",SPI2_ReceiveData);
	wait_spi2_bit(50);
	while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE)==RESET);
	data_send = (uint16_t)(data&0x0ffff);
	SPI_I2S_SendData(SPI2,data_send);
	
	
	while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE)==RESET);
	SPI2_ReceiveData |= SPI_I2S_ReceiveData(SPI2);
	//printf("--2--%x",SPI2_ReceiveData);
	wait_spi2_bit(100);
	SPI2_CS1_DISABLE();
	wait_spi2_bit(100);
	return SPI2_ReceiveData;	
}


uint16_t SPI2_ReadWrite16(uint16_t data)
{
	uint16_t SPI2_ReceiveData;
	SPI2_CS1_ENABLE();
	wait_spi2_bit(100);
	while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE)==RESET);
	
	SPI_I2S_SendData(SPI2,data);
	
	while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE)==RESET);
	SPI2_ReceiveData = SPI_I2S_ReceiveData(SPI2);
	
	SPI2_CS1_DISABLE();
	wait_spi2_bit(100);
	return SPI2_ReceiveData;	
}

uint16_t SPI2_Receive_byte(void)
{
	uint16_t SPI2_ReceiveData;
	//SPI2_CS_ENABLE();
	while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE)==RESET);
	SPI_I2S_SendData(SPI2,0x00);
	
	while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE)==RESET);
	SPI2_ReceiveData = SPI_I2S_ReceiveData(SPI2);
	//SPI2_CS_DISABLE();
	return SPI2_ReceiveData;
	
}

//中断处理函数
void EXTI9_5_IRQHandler(void)
{
	uint32_t read_data=0;
	static int spi_int_counter = 0;

	
    if (EXTI_GetITStatus(EXTI_Line9) != RESET)
    {
        //添加中断处理程序
        read_data <<= 2;
				if(SPI2_MISO_READ != 0)
				{
					read_data +=1;
				}	
				spi_int_counter++;
				if(spi_int_counter == 32)
				{
					spi_int_counter = 0;
					
				}
        //EXTI_ClearFlag(EXTI_Line8);
        EXTI_ClearITPendingBit(EXTI_Line9);                       //本函数与上一句功能相同
    }
}



#else 
/*************************************************************************************************

///////////SPI2软件方式读写驱动部分

*************************************************************************************************/
void SPI2_Configuration()
{
//    SPI_InitTypeDef  SPI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
//    EXTI_InitTypeDef EXTI_InitStructure;
//    NVIC_InitTypeDef NVIC_InitStructure;
    //启动SPI时钟

	//NVIC_SPI2Config();
    
    RCC_APB2PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);    
    
 
    
     //////下面是SPI相关GPIO初始化//////
    //SPI2模块对应的SCK(GPIO_Pin_13)、MISO(GPIO_Pin_14)、MOSI(GPIO_Pin_15)为AF引脚
   

   //PB12 pin: SPI2_CS 
   GPIO_InitStructure.GPIO_Pin = SPI2_MOSI;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽输出
   GPIO_Init(GPIOB, &GPIO_InitStructure);  
	
  GPIO_InitStructure.GPIO_Pin = SPI2_CS;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽输出
   GPIO_Init(GPIOB, &GPIO_InitStructure);  
	 
	 GPIO_InitStructure.GPIO_Pin =  SPI2_MISO;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽输出
   GPIO_Init(GPIOB, &GPIO_InitStructure); 

    GPIO_InitStructure.GPIO_Pin   = SPI2_SCLK;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode   = GPIO_Mode_AF ;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽输出
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
   SPI2_CS_DISABLE();
   GPIO_SetBits(GPIOB, SPI2_SCLK);
	 

}


static void wait_spi2_bit(int bit_num)
{
		int i;
	  for(i=0;i<bit_num;i++);
}

uint32_t SPI2_ReadWrite32(uint32_t data)
{
	uint32_t read_data=0;
	int i;
  SPI2_SCLK_LOW;
	wait_spi2_bit(10);
	SPI2_CS_ENABLE();
	wait_spi2_bit(5);	
	//////SEND 32 bit data

	for(i=0;i<32;i++)
	{		
		/////准备输出数据
		if((data&0x80000000) != 0) SPI2_MOSI_HIGH;
		else SPI2_MOSI_LOW;		
		
		data <<=  1;	
		wait_spi2_bit(2);	
		SPI2_SCLK_LOW;
		
		wait_spi2_bit(5);	
		
		/////读输入数据
		read_data <<= 1;
		if(SPI2_MISO_READ != 0)
		{
			read_data +=1;
		}
		wait_spi2_bit(2);	
		SPI2_SCLK_HIGH;	
		wait_spi2_bit(5);		
			
	}
		SPI2_SCLK_LOW;
		wait_spi2_bit(5);	
	SPI2_CS_DISABLE();
	wait_spi2_bit(10);
	
	return read_data;
	
	//SPI2_CS_DISABLE();
}

uint16_t SPI2_ReadWrite16(uint16_t data)
{
	uint16_t read_data=0;
	int i;
  SPI2_SCLK_HIGH;
	wait_spi2_bit(10);
	SPI2_CS_ENABLE();
	wait_spi2_bit(10);	
	//////SEND 32 bit data

	for(i=0;i<16;i++)
	{		
		/////准备输出数据
		if((data&0x8000) != 0) SPI2_MOSI_HIGH;
		else SPI2_MOSI_LOW;			
		data <<=  1;	
		wait_spi2_bit(5);	
		
		SPI2_SCLK_LOW;
		wait_spi2_bit(10);	
		
		SPI2_SCLK_HIGH;			
		/////读输入数据
		read_data <<= 1;
		if(SPI2_MISO_READ != 0)
		{
			read_data +=1;
		}
		wait_spi2_bit(2);			
			
	}
		
	SPI2_CS_DISABLE();
	wait_spi2_bit(10);
	SPI2_SCLK_HIGH;	
	
	return read_data;

}

#endif 

