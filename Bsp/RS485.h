#ifndef _RS485_H_
#define _RS485_H_

#define RS485_RX   GPIOB->BSRRH = GPIO_Pin_12  //D7∏¥Œª
#define RS485_TX   GPIOB->BSRRL = GPIO_Pin_12  //D7÷√Œª


void Rs485_Init(void);
void Rs485_TX(void);
void Rs485_RX(void);

#endif
