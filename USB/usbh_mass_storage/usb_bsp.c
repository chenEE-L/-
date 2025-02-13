/**
  ******************************************************************************
  * @file    usb_bsp.c
  * @author  MCD Application Team
  * @version V2.1.0
  * @date    19-March-2012
  * @brief   This file implements the board support package for the USB host library
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/

#include "usb_bsp.h"
#include "ucos_ii.h"
/** @addtogroup USBH_USER
* @{
*/

/** @defgroup USB_BSP
  * @brief This file is responsible to offer board support package
  * @{
  */ 

/** @defgroup USB_BSP_Private_Defines
  * @{
  */ 
//#define USE_ACCURATE_TIME
#define TIM_MSEC_DELAY                     0x01
#define TIM_USEC_DELAY                     0x02

/**
  * @}
  */ 


/** @defgroup USB_BSP_Private_TypesDefinitions
  * @{
  */ 
/**
  * @}
  */ 



/** @defgroup USB_BSP_Private_Macros
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup USBH_BSP_Private_Variables
  * @{
  */ 
ErrorStatus HSEStartUpStatus;
#ifdef USE_ACCURATE_TIME 
__IO uint32_t BSP_delay = 0;
#endif
/**
  * @}
  */ 

/** @defgroup USBH_BSP_Private_FunctionPrototypes
  * @{
  */ 

#ifdef USE_ACCURATE_TIME 
//static void BSP_SetTime(uint8_t Unit);
//static void BSP_Delay(uint32_t nTime,uint8_t Unit);
//static void USB_OTG_BSP_TimeInit ( void );
#endif
/**
  * @}
  */ 

/** @defgroup USB_BSP_Private_Functions
  * @{
  */ 

/**
  * @brief  USB_OTG_BSP_Init
  *         Initilizes BSP configurations
  * @param  None
  * @retval None
  */

void USB_OTG_BSP_Init(USB_OTG_CORE_HANDLE *pdev)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIOA时钟
	RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_OTG_FS, ENABLE);//使能USB OTG时钟	钟
	//GPIOA11,A12设置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;//PA11/12复用功能输出	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化


//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;//PA15推挽输出		
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//输出功能
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; 
//	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化


	GPIO_PinAFConfig(GPIOA,GPIO_PinSource11,GPIO_AF_OTG_FS);//PA11,AF10(USB)
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource12,GPIO_AF_OTG_FS);//PA12,AF10(USB)
}
/**
  * @brief  USB_OTG_BSP_EnableInterrupt
  *         Configures USB Global interrupt
  * @param  None
  * @retval None
  */
void USB_OTG_BSP_EnableInterrupt(USB_OTG_CORE_HANDLE *pdev)
{
  NVIC_InitTypeDef NVIC_InitStructure; 
  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
#ifdef USE_USB_OTG_HS   
  NVIC_InitStructure.NVIC_IRQChannel = OTG_HS_IRQn;
#else
  NVIC_InitStructure.NVIC_IRQChannel = OTG_FS_IRQn;  
#endif
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);  

}

/**
  * @brief  BSP_Drive_VBUS
  *         Drives the Vbus signal through IO
  * @param  state : VBUS states
  * @retval None
  */

void USB_OTG_BSP_DriveVBUS(USB_OTG_CORE_HANDLE *pdev, uint8_t state)
{  
}

/**
  * @brief  USB_OTG_BSP_ConfigVBUS
  *         Configures the IO for the Vbus and OverCurrent
  * @param  None
  * @retval None
  */

void  USB_OTG_BSP_ConfigVBUS(USB_OTG_CORE_HANDLE *pdev)
{
}

/**
  * @brief  USB_OTG_BSP_TimeInit
  *         Initializes delay unit using Timer2
  * @param  None
  * @retval None
  */
//static void USB_OTG_BSP_TimeInit ( void )
//{
//#ifdef USE_ACCURATE_TIME   
//  NVIC_InitTypeDef NVIC_InitStructure;
//  
//  /* Set the Vector Table base address at 0x08000000 */
//  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x00);
//  
//  /* Configure the Priority Group to 2 bits */
//  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
//  
//  /* Enable the TIM2 gloabal Interrupt */
//  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//  
//  NVIC_Init(&NVIC_InitStructure);
//  
//  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);  
//#endif  
//}

/**
  * @brief  USB_OTG_BSP_uDelay
  *         This function provides delay time in micro sec
  * @param  usec : Value of delay required in micro sec
  * @retval None
  */
void USB_OTG_BSP_uDelay (const uint32_t usec)
{
  
//#ifdef USE_ACCURATE_TIME    
//  BSP_Delay(usec,TIM_USEC_DELAY); 
//#else
  __IO uint32_t count = 0;
  const uint32_t utime = (120 * usec / 7);
  do
  {
    if ( ++count > utime )
    {
      return ;
    }
  }
  while (1);
//#endif   
  
}


/**
  * @brief  USB_OTG_BSP_mDelay
  *          This function provides delay time in milli sec
  * @param  msec : Value of delay required in milli sec
  * @retval None
  */
void USB_OTG_BSP_mDelay (const uint32_t msec)
{ 
	OSTimeDlyHMSM(0,0,0,msec);
//#ifdef USE_ACCURATE_TIME  
//    BSP_Delay(msec,TIM_MSEC_DELAY);   
//#else
//    USB_OTG_BSP_uDelay(msec * 1000);   
//#endif    

}


/**
  * @brief  USB_OTG_BSP_TimerIRQ
  *         Time base IRQ
  * @param  None
  * @retval None
  */

void USB_OTG_BSP_TimerIRQ (void)
{
#ifdef USE_ACCURATE_TIME 
    
  if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
  {
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    if (BSP_delay > 0x00)
    { 
      BSP_delay--;
    }
    else
    {
      TIM_Cmd(TIM2,DISABLE);
    }
  }
#endif  
} 

#ifdef USE_ACCURATE_TIME 
/**
  * @brief  BSP_Delay
  *         Delay routine based on TIM2
  * @param  nTime : Delay Time 
  * @param  unit : Delay Time unit : mili sec / micro sec
  * @retval None
  */
//static void BSP_Delay(uint32_t nTime, uint8_t unit)
//{
//  
//  BSP_delay = nTime;
//  BSP_SetTime(unit);  
//  while(BSP_delay != 0);
//  TIM_Cmd(TIM2,DISABLE);
//}

/**
  * @brief  BSP_SetTime
  *         Configures TIM2 for delay routine based on TIM2
  * @param  unit : msec /usec
  * @retval None
  */
//static void BSP_SetTime(uint8_t unit)
//{
//  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//  
//  TIM_Cmd(TIM2,DISABLE);
//  TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE); 
//  
//  
//  if(unit == TIM_USEC_DELAY)
//  {  
//    TIM_TimeBaseStructure.TIM_Period = 11;
//  }
//  else if(unit == TIM_MSEC_DELAY)
//  {
//    TIM_TimeBaseStructure.TIM_Period = 11999;
//  }
//  TIM_TimeBaseStructure.TIM_Prescaler = 5;
//  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
//  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
//  
//  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
//  TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
//  
//  TIM_ARRPreloadConfig(TIM2, ENABLE);
//  
//  /* TIM IT enable */
//  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
//  
//  /* TIM2 enable counter */ 
//  TIM_Cmd(TIM2, ENABLE);  
//} 

#endif

/**
* @}
*/ 

/**
* @}
*/ 

/**
* @}
*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
