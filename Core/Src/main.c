/**
 * -------------------------------------------------------------------------------
 *       (C) Copyright 2024, PSYM. All Rights Reserved
 *-------------------------------------------------------------------------------
 * @file   : main.c
 * Description :
 * @author : PSYM
 *-------------------------------------------------------------------------------
 * Update note:
 * -----------   -----------   --------------------------------------------------
 *    Date         Author                              Note
 * -----------   -----------   --------------------------------------------------
 * 2024-01-07       PSYM
 *
 */

/* ---------------------------------- ����ͷ�ļ� ---------------------------------- */
#include "main.h"
#include "gpio.h"

/* ---------------------------------- ˽�к궨�� ---------------------------------- */
/* None. */

/* ---------------------------------- ˽�����Ͷ��� ---------------------------------- */
/* None. */

/* ---------------------------------- ˽�б��� ---------------------------------- */
/* None. */

/* ---------------------------------- ��չ���� ---------------------------------- */
/* None. */

/* ---------------------------------- ˽�к���ԭ�� ---------------------------------- */
void SystemClock_Config(void);

/* ---------------------------------- ������ ---------------------------------- */

/**
 * @brief  Ӧ�ó�����ڵ�
 * @retval int
 */
int main(void)
{
    /* �����￪ʼ����Զ������ */

    /* �������������Զ������ */

    /* �����������裬��ʼ�� Flash �ӿں� SysTick ��ʱ���� */
    HAL_Init();

    /* �����￪ʼ����ϵͳ��ʼ�� */

    /* �������������ϵͳ��ʼ�� */

    /* ����ϵͳʱ�� */
    SystemClock_Config();

    /* �������ʼ���������õ����� */
    MX_GPIO_Init();

    /* �������������Զ������ */

    /* �������������Զ������ */

    /* ��ʼ��ѭ�� */
    while (1)
    {

        /* ������ѭ�� */

        /* �������������Զ������ */
    }
    /* �������������Զ������ */
}

/**
 * @brief ϵͳʱ������
 * @retval None
 */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** Initializes the RCC Oscillators according to the specified parameters
     * in the RCC_OscInitTypeDef structure.
     */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    /** Initializes the CPU, AHB and APB buses clocks
     */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
    {
        Error_Handler();
    }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief �ڷ�������ʱִ�д˺�����
 * @retval None
 */
void Error_Handler(void)
{
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();
    while (1)
    {
    }
    /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/*>>>>>>>>>> (C) COPYRIGHT PSYM <<<<<< >>>>>> END OF FILE <<<<<<<<<<*/
