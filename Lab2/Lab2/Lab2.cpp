#include <iostream>
#include "hal.h"

using namespace std;

int k = 0;
int T = 0;
int turn = 1;
int leds_num[] = { GPIO_PIN_3, GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_6,
				GPIO_PIN_8, GPIO_PIN_9, GPIO_PIN_11, GPIO_PIN_12 };
unsigned int sw_num[] = { GPIO_PIN_4, GPIO_PIN_8, GPIO_PIN_10, GPIO_PIN_12 };
GPIO_PinState states[4];

void readSW() {
	for (int i = 0; i < 4; ++i) {
		states[i] = HAL_GPIO_ReadPin(GPIOE, sw_num[i]);
	}
}

void TIM6_IRQ_HANDLER() {
	int coef = 1;
	T = 0;
	readSW();
	for (int i = 3; i > -1; --i) {
		T += coef * states[i];
		coef *= 2;
	}
}

void TIM7_IRQ_HANDLER() {
	if (k - turn >= 0) {
		HAL_GPIO_TogglePin(GPIOD, leds_num[k - turn]);
		HAL_GPIO_TogglePin(GPIOD, leds_num[k - turn + 4]);
	}

	if (k != -1) {
		HAL_GPIO_WritePin(GPIOD, leds_num[k], GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD, leds_num[k + 4], GPIO_PIN_SET);
	}
	
	if (k + turn > 3 || k + turn < -1) {
		turn *= -1;
	}

	k += turn;
	WRITE_REG(TIM7_ARR, 500 + T * 100);
}

int umain()
{

	readSW();

	while (states[0] != GPIO_PIN_RESET || states[1] != GPIO_PIN_RESET ||
		states[2] != GPIO_PIN_SET || states[3] != GPIO_PIN_RESET) {
		readSW();
		HAL_Delay(500);
	}

	k = 0;
	HAL_GPIO_WritePin(GPIOD, leds_num[6], GPIO_PIN_RESET);

	registerTIM6_IRQHandler(TIM6_IRQ_HANDLER);
	registerTIM7_IRQHandler(TIM7_IRQ_HANDLER);

	__enable_irq();

	WRITE_REG(TIM6_ARR, 500);
	WRITE_REG(TIM6_DIER, TIM_DIER_UIE);
	WRITE_REG(TIM6_PSC, 0);

	WRITE_REG(TIM7_ARR, 500 + T * 100);
	WRITE_REG(TIM7_DIER, TIM_DIER_UIE);
	WRITE_REG(TIM7_PSC, 0);

	WRITE_REG(TIM6_CR1, TIM_CR1_CEN);
	WRITE_REG(TIM7_CR1, TIM_CR1_CEN);

	return 0;
}
