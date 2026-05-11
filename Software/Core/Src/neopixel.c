#include "neopixel.h"

static TIM_HandleTypeDef *np_htim;

static NeoPixel_Color leds[NEOPIXEL_LED_COUNT];

static uint16_t pwmData[BUFFER_SIZE];

#define WS2812_HIGH    79
#define WS2812_LOW     39

void Neopixel_Init(TIM_HandleTypeDef *htim)
{
    np_htim = htim;

    Neopixel_Clear();
}

void Neopixel_SetPixel(uint16_t index,
                       uint8_t red,
                       uint8_t green,
                       uint8_t blue)
{
    if(index >= NEOPIXEL_LED_COUNT)
        return;

    leds[index].red = red;
    leds[index].green = green;
    leds[index].blue = blue;
}

void Neopixel_Clear(void)
{
    for(uint16_t i = 0; i < NEOPIXEL_LED_COUNT; i++)
    {
        leds[i].red = 0;
        leds[i].green = 0;
        leds[i].blue = 0;
    }
}

static void Neopixel_FillBuffer(void)
{
    uint32_t indx = 0;

    for(uint16_t i = 0; i < NEOPIXEL_LED_COUNT; i++)
    {
        uint32_t color = 0;

        color |= (leds[i].green << 16);
        color |= (leds[i].red << 8);
        color |= leds[i].blue;

        for(int8_t bit = 23; bit >= 0; bit--)
        {
            if(color & (1 << bit))
            {
                pwmData[indx] = WS2812_HIGH;
            }
            else
            {
                pwmData[indx] = WS2812_LOW;
            }

            indx++;
        }
    }

    for(uint16_t i = 0; i < WS2812_RESET_SLOTS; i++)
    {
        pwmData[indx++] = 0;
    }
}

void Neopixel_Show(void)
{
    Neopixel_FillBuffer();

    HAL_TIM_PWM_Start_DMA(np_htim,
                          TIM_CHANNEL_1,
                          (uint32_t*)pwmData,
                          BUFFER_SIZE);
}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == TIM2)
    {
        HAL_TIM_PWM_Stop_DMA(htim, TIM_CHANNEL_1);
    }
}
