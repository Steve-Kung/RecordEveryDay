#ifndef __STEPMOTOR_TIM_H__
#define __STEPMOTOR_TIM_H__

/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* ���Ͷ��� ------------------------------------------------------------------*/
/* �궨�� --------------------------------------------------------------------*/
#define STEPMOTOR_TIMx                        TIM1
#define STEPMOTOR_TIM_RCC_CLK_ENABLE()        __HAL_RCC_TIM1_CLK_ENABLE()
#define STEPMOTOR_TIM_RCC_CLK_DISABLE()       __HAL_RCC_TIM1_CLK_DISABLE()
#define STEPMOTOR_TIMx_IRQn                   TIM1_CC_IRQn
#define STEPMOTOR_TIMx_IRQHandler             TIM1_CC_IRQHandler

#define STEPMOTOR_TIM_CHANNEL_x               TIM_CHANNEL_1
#define STEPMOTOR_TIM_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOA_CLK_ENABLE()     // ���������������������
#define STEPMOTOR_TIM_PUL_PORT                GPIOA                            // ��Ӧ��������PUL-��������ʹ�ù����ӷ���
#define STEPMOTOR_TIM_PUL_PIN                 GPIO_PIN_8                       // ��PLU+ֱ�ӽӿ������5V(����3.3V)

#define STEPMOTOR_DIR_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOB_CLK_ENABLE()     // �����ת������ƣ�������ղ���Ĭ����ת
#define STEPMOTOR_DIR_PORT                    GPIOB                            // ��Ӧ��������DIR-��������ʹ�ù����ӷ���
#define STEPMOTOR_DIR_PIN                     GPIO_PIN_13                      // ��DIR+ֱ�ӽӿ������5V(����3.3V)

#define STEPMOTOR_ENA_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOB_CLK_ENABLE()     // ����ѻ�ʹ�ܿ��ƣ�������ղ���Ĭ��ʹ�ܵ��
#define STEPMOTOR_ENA_PORT                    GPIOB                            // ��Ӧ��������ENA-��������ʹ�ù����ӷ���
#define STEPMOTOR_ENA_PIN                     GPIO_PIN_14                      // ��ENA+ֱ�ӽӿ������5V(����3.3V)

#define STEPMOTOR_DIR_FORWARD()               HAL_GPIO_WritePin(STEPMOTOR_DIR_PORT,STEPMOTOR_DIR_PIN,GPIO_PIN_SET)
#define STEPMOTOR_DIR_REVERSAL()              HAL_GPIO_WritePin(STEPMOTOR_DIR_PORT,STEPMOTOR_DIR_PIN,GPIO_PIN_RESET)

#define STEPMOTOR_OUTPUT_ENABLE()             HAL_GPIO_WritePin(STEPMOTOR_ENA_PORT,STEPMOTOR_ENA_PIN,GPIO_PIN_SET)
#define STEPMOTOR_OUTPUT_DISABLE()            HAL_GPIO_WritePin(STEPMOTOR_ENA_PORT,STEPMOTOR_ENA_PIN,GPIO_PIN_RESET)


// ���嶨ʱ��Ԥ��Ƶ����ʱ��ʵ��ʱ��Ƶ��Ϊ��72MHz/��STEPMOTOR_TIMx_PRESCALER+1��
#define STEPMOTOR_TIM_PRESCALER               3  // �������������ϸ������Ϊ��   32  ϸ��
//#define STEPMOTOR_TIM_PRESCALER               7  // �������������ϸ������Ϊ��   16  ϸ��
//#define STEPMOTOR_TIM_PRESCALER               15  // �������������ϸ������Ϊ��   8  ϸ��
//#define STEPMOTOR_TIM_PRESCALER               31  // �������������ϸ������Ϊ��   4  ϸ��
//#define STEPMOTOR_TIM_PRESCALER               63  // �������������ϸ������Ϊ��   2  ϸ��
//#define STEPMOTOR_TIM_PRESCALER               127  // �������������ϸ������Ϊ��   1  ϸ��


// ���嶨ʱ�����ڣ�����Ƚ�ģʽ��������Ϊ0xFFFF
#define STEPMOTOR_TIM_PERIOD                   0xFFFF
// ����߼���ʱ���ظ������Ĵ���ֵ
#define STEPMOTOR_TIM_REPETITIONCOUNTER       0

/* ��չ���� ------------------------------------------------------------------*/
extern TIM_HandleTypeDef htimx_STEPMOTOR;
extern __IO uint16_t Toggle_Pulse;
/* �������� ------------------------------------------------------------------*/

void STEPMOTOR_TIMx_Init(void);

#endif	/* __STEPMOTOR_TIM_H__ */
/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/
