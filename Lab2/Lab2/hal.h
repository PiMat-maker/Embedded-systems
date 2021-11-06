#pragma once
typedef enum {
	GPIO_PIN_RESET = 0,
	GPIO_PIN_SET
} GPIO_PinState;

#define GPIOA		(AHBIPERIPH_BASE + 0x0000UL)
#define GPIOB		(AHBIPERIPH_BASE + 0x0400UL)
#define GPIOC		(AHBIPERIPH_BASE + 0x0800UL)
#define GPIOD		(AHBIPERIPH_BASE + 0x0C00UL)
#define GPIOE		(AHBIPERIPH_BASE + 0x1000UL)

#define GPIO_PIN_0	(0x0001)
#define GPIO_PIN_1	(0x0002)
#define GPIO_PIN_2	(0x0004)
#define GPIO_PIN_3	(0x0008)
#define GPIO_PIN_4	(0x0010)
#define GPIO_PIN_5	(0x0020)
#define GPIO_PIN_6	(0x0040)
#define GPIO_PIN_7	(0x0080)
#define GPIO_PIN_8	(0x0100)
#define GPIO_PIN_9	(0x0200)
#define GPIO_PIN_10	(0x0400)
#define GPIO_PIN_11	(0x0800)
#define GPIO_PIN_12	(0x1000)
#define GPIO_PIN_13	(0x2000)
#define GPIO_PIN_14	(0x4000)
#define GPIO_PIN_15	(0x8000)


void HAL_GPIO_TogglePin(uint gpio_port, uint pin);
GPIO_PinState HAL_GPIO_ReadPin(uint gpio_port, uint pin);
void HAL_GPIO_WritePin(uint gpio_port, uint pin, GPIO_PinState pin_state);
void HAL_Delay(uint delay_ms);

#endif