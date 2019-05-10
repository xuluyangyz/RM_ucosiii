#ifndef __USART1_H
#define	__USART1_H


#include "stm32f4xx.h"
#include <stdio.h>



/**************************USART参数定义********************************/
#define             USARTx                                USART1
#define             USART_BAUD_RATE                       115200
#define             USART_xClock_FUN                      RCC_APB2PeriphClockCmd
#define             USART_CLK                             RCC_APB2Periph_USART1

#define             USART_GPIO_xClock_FUN                 RCC_AHB1PeriphClockCmd
#define             USART_GPIO_CLK                        RCC_AHB1Periph_GPIOA     
#define             USART_TX_PORT                         GPIOA   
#define             USART_TX_PIN                          GPIO_Pin_9
#define             USART_TX_AF                           GPIO_AF_USART1
#define             USART_TX_SOURCE                       GPIO_PinSource9
#define             USART_RX_PORT                         GPIOA 
#define             USART_RX_PIN                          GPIO_Pin_10
#define             USART_RX_AF                           GPIO_AF_USART1
#define             USART_RX_SOURCE                       GPIO_PinSource10

#define             USART_IRQ                             USART1_IRQn
#define             USART_INT_FUN                         USART1_IRQHandler



void                USARTx_Config                           ( void );



#endif /* __USART1_H */

