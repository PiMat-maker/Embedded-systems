#include <iostream>
#include "hal.h"

using namespace std;

void manageVDLeds(int vdleds_num[], int num_led_to_set) {
	for (int i = 0; i < 3; ++i) {
		if (num_led_to_set == i) {
			HAL_GPIO_WritePin(GPIOD, vdleds_num[i], GPIO_PIN_SET);
		}
		HAL_GPIO_WritePin(GPIOD, vdleds_num[i], GPIO_PIN_RESET);
	}
}

void readSW(unsigned int sw_num[], GPIO_PinState* states) {
	for (int i = 0; i < 4; ++i) {
		states[i] = HAL_GPIO_ReadPin(GPIOE, sw_num[i]);
	}
}

void switchOffLeds(int leds[]) {
	for (int i = 0; i < 8; ++i) {
		HAL_GPIO_WritePin(GPIOD, leds[i], GPIO_PIN_RESET);
	}
}

int umain()
{
	int delay = 500;
	int leds_num[] = {GPIO_PIN_3, GPIO_PIN_4, GPIO_PIN_5,
						GPIO_PIN_6, GPIO_PIN_8, GPIO_PIN_9, 
						GPIO_PIN_11, GPIO_PIN_12 };
	int vdleds_num[] = { GPIO_PIN_13, GPIO_PIN_14, GPIO_PIN_15 };
	unsigned int sw_num[] = { GPIO_PIN_4, GPIO_PIN_8, GPIO_PIN_10, GPIO_PIN_12 };

	GPIO_PinState states[4];
	readSW(sw_num, states);
		
	
	while (true) {
		
		switchOffLeds(leds_num);
		//manageVDLeds(vdleds_num, 1);
		HAL_GPIO_WritePin(GPIOD, vdleds_num[0], GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOD, vdleds_num[1], GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD, vdleds_num[2], GPIO_PIN_RESET);
		while (states[0] != GPIO_PIN_RESET || states[1] != GPIO_PIN_SET ||
			states[2] != GPIO_PIN_SET || states[3] != GPIO_PIN_SET) {
			
			readSW(sw_num, states);
			for (int i = 0; i < 4; ++i) {
				HAL_GPIO_WritePin(GPIOD, leds_num[i + 4], states[i]);
			}
			HAL_Delay(delay / 2);
		}

		switchOffLeds(leds_num);
		//manageVDLeds(vdleds_num, 0);
		int l = 0, r = 7;
		while (states[0] == GPIO_PIN_RESET && states[1] == GPIO_PIN_SET &&
			states[2] == GPIO_PIN_SET && states[3] == GPIO_PIN_SET){
			HAL_GPIO_WritePin(GPIOD, vdleds_num[0], GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOD, vdleds_num[1], GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOD, vdleds_num[2], GPIO_PIN_RESET);

			if (r < 0) {
				l = 0;
				r = 7;
			}
			if (l < r) {
				HAL_GPIO_WritePin(GPIOD, leds_num[l], GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOD, leds_num[r], GPIO_PIN_SET);
			}
			else {
				HAL_GPIO_WritePin(GPIOD, leds_num[l], GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOD, leds_num[r], GPIO_PIN_RESET);
			}
			++l;
			--r;
			HAL_Delay(delay);

			if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_15) == GPIO_PIN_RESET) {
				HAL_GPIO_WritePin(GPIOD, vdleds_num[0], GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOD, vdleds_num[1], GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOD, vdleds_num[2], GPIO_PIN_SET);
				HAL_Delay(delay);
				while (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_15) == GPIO_PIN_SET) {
					HAL_Delay(delay);
				}
			}
			

			readSW(sw_num, states);
		}
	}

	switchOffLeds(leds_num);
	//switchOffLeds(vdleds_num);

}
