#ifndef __NEOPIXEL_H
#define __NEOPIXEL_H

#include "stm32f4xx_hal.h"
#include <stdint.h>

#define NEOPIXEL_LED_COUNT     5

#define WS2812_BITS_PER_LED    24
#define WS2812_RESET_SLOTS     60

#define BUFFER_SIZE ((NEOPIXEL_LED_COUNT * WS2812_BITS_PER_LED) + WS2812_RESET_SLOTS)

typedef struct
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;

} NeoPixel_Color;

void Neopixel_Init(TIM_HandleTypeDef *htim);

void Neopixel_SetPixel(uint16_t index, uint8_t red, uint8_t green, uint8_t blue);

void Neopixel_Clear(void);

void Neopixel_Show(void);

#endif
