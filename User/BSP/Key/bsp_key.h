#ifndef __KEY_H
#define	__KEY_H



#include "stm32f4xx.h"



/************************************ ≈‰÷√ KEY1 *********************************/
#define               KEY1_GPIO_xClock_FUN               RCC_AHB1PeriphClockCmd
#define               KEY1_GPIO_CLK                      RCC_AHB1Periph_GPIOA
#define               KEY1_GPIO_PORT    	                GPIOA			   
#define               KEY1_GPIO_PIN		                  GPIO_Pin_0



/************************************ ≈‰÷√ KEY2 *********************************/
#define               KEY2_GPIO_xClock_FUN               RCC_AHB1PeriphClockCmd
#define               KEY2_GPIO_CLK                      RCC_AHB1Periph_GPIOC
#define               KEY2_GPIO_PORT    	                GPIOC
#define               KEY2_GPIO_PIN		                  GPIO_Pin_13



void    Key_Initial    ( void );
uint8_t Key_Scan       ( GPIO_TypeDef * GPIOx, uint16_t GPIO_Pin, uint8_t ucPushState, uint8_t * pKeyPress );
uint8_t Key_ReadStatus ( GPIO_TypeDef * GPIOx, uint16_t GPIO_Pin, uint8_t ucPushState );


#endif /* __KEY_H */



