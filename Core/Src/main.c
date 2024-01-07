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

/* ---------------------------------- 包含头文件 ---------------------------------- */
#include "main.h"
#include "gpio.h"

/* ---------------------------------- 私有宏定义 ---------------------------------- */
/* None. */

/* ---------------------------------- 私有类型定义 ---------------------------------- */
/* None. */

/* ---------------------------------- 私有变量 ---------------------------------- */
/* None. */

/* ---------------------------------- 扩展变量 ---------------------------------- */
/* None. */

/* ---------------------------------- 私有函数原型 ---------------------------------- */
void SystemClock_Config(void);

/* ---------------------------------- 函数体 ---------------------------------- */

/**
 * @brief  应用程序入口点
 * @retval int
 */
int main(void)
{
    /* 在这里开始添加自定义代码 */

    /* 在这里结束添加自定义代码 */

    /* 重置所有外设，初始化 Flash 接口和 SysTick 定时器。 */
    HAL_Init();

    /* 在这里开始进行系统初始化 */

    /* 在这里结束进行系统初始化 */

    /* 配置系统时钟 */
    SystemClock_Config();

    /* 在这里初始化所有配置的外设 */
    MX_GPIO_Init();

    /* 在这里继续添加自定义代码 */

    /* 在这里结束添加自定义代码 */

    /* 开始主循环 */
    while (1)
    {

        /* 结束主循环 */

        /* 在这里继续添加自定义代码 */
    }
    /* 在这里结束添加自定义代码 */
}

/**
 * @brief 系统时钟配置
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
 * @brief 在发生错误时执行此函数。
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
