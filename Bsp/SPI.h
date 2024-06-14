#ifndef __SPI_H__
#define __SPI_H__
#include "bsp.h"

////spi1对应NSS(GPIO_Pin_4)、  SCK(GPIO_Pin_5)、 (GPIO_Pin_6)、MOSI(GPIO_Pin_7)为AF引脚


//SPI2模块对应的SCK(GPIO_Pin_13)、MISO(GPIO_Pin_14)、MOSI(GPIO_Pin_15)为AF引脚
//#define SPI2_SOFT

#ifdef SPI2_SOFT
#define SPI2_SCLK	    GPIO_Pin_13   //GPIO_Pin_8
#define SPI2_MISO     GPIO_Pin_14  //GPIO_Pin_9
#define SPI2_MOSI     GPIO_Pin_15  //GPIO_Pin_10
#define SPI2_CS       GPIO_Pin_12  //GPIO_Pin_11
#define DRDY          GPIO_Pin_12

#define SPI2_SCLK_HIGH 	GPIO_SetBits(GPIOB, SPI2_SCLK)
#define SPI2_SCLK_LOW 	GPIO_ResetBits(GPIOB, SPI2_SCLK)

#define SPI2_MOSI_HIGH 	GPIO_SetBits(GPIOB, SPI2_MOSI)
#define SPI2_MOSI_LOW 	GPIO_ResetBits(GPIOB, SPI2_MOSI)

#define SPI2_MISO_READ   GPIO_ReadInputDataBit(GPIOB,SPI2_MISO)
#else
#define SPI2_SCLK	  GPIO_Pin_13
#define SPI2_MISO   GPIO_Pin_14
#define SPI2_MOSI   GPIO_Pin_15
#define SPI2_CS4    GPIO_Pin_9
#define SPI2_CS3    GPIO_Pin_7
#define SPI2_CS2    GPIO_Pin_15
#define SPI2_CS1    GPIO_Pin_13
#define DRDY1       GPIO_Pin_12
#define DRDY2       GPIO_Pin_14
#define DRDY3       GPIO_Pin_6
#define DRDY4       GPIO_Pin_8

#define SPI2_MISO_READ   GPIO_ReadInputDataBit(GPIOB,SPI2_MISO)


#endif


#define SPI3_SCLK	  GPIO_Pin_3
//#define SPI3_MISO   GPIO_Pin_5
#define SPI3_MOSI   GPIO_Pin_5
#define SPI3_CS     GPIO_Pin_4




#define SPI2_CS1_ENABLE()			GPIO_ResetBits(GPIOE, SPI2_CS1)
#define SPI2_CS2_ENABLE()			GPIO_ResetBits(GPIOE, SPI2_CS2)
#define SPI2_CS3_ENABLE()			GPIO_ResetBits(GPIOC, SPI2_CS3)
#define SPI2_CS4_ENABLE()			GPIO_ResetBits(GPIOC, SPI2_CS4)
#define SPI3_CS_ENABLE()			GPIO_ResetBits(GPIOB, SPI3_CS)


#define SPI2_CS1_DISABLE()			GPIO_SetBits(GPIOE, SPI2_CS1)
#define SPI2_CS2_DISABLE()			GPIO_SetBits(GPIOE, SPI2_CS2)
#define SPI2_CS3_DISABLE()			GPIO_SetBits(GPIOC, SPI2_CS3)
#define SPI2_CS4_DISABLE()			GPIO_SetBits(GPIOC, SPI2_CS4)
#define SPI3_CS_DISABLE()				GPIO_SetBits(GPIOB, SPI3_CS)

void NVIC_SPI2Config(void);
void SPI3_Configuration(void);
void SPI2_Configuration(void);
uint16_t SPI2_ReadWrite(uint16_t data);
uint16_t SPI2_ReadWrite16(uint16_t data);
uint32_t SPI2_ReadWrite32(uint32_t data);
uint16_t SPI2_Read_Max31865(uint16_t addr);
void wait_spi2_bit(int bit_num);
void SPI2_Write_Max31865_1(uint16_t addr,uint16_t data);
uint16_t SPI2_Read_Max31865_1(uint16_t addr);
void SPI2_Write_Max31865_2(uint16_t addr,uint16_t data);
uint16_t SPI2_Read_Max31865_2(uint16_t addr);
void SPI2_Write_Max31865_3(uint16_t addr,uint16_t data);
uint16_t SPI2_Read_Max31865_3(uint16_t addr);
void SPI2_Write_Max31865_4(uint16_t addr,uint16_t data);
uint16_t SPI2_Read_Max31865_4(uint16_t addr);
#endif
