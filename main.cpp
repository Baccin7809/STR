/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

#include "main.h"
#include <cstdint>
#include "miros.h"

#define BUF_TAM 5
uint8_t buffer[BUF_TAM];
uint8_t in = 0, out = 0;

rtos::OSSem sem_vazio;
rtos::OSSem sem_cheio;
uint32_t conta0=0, conta1=0, conta2=0;

uint32_t stack_blinky1[40];
rtos::OSThread blinky1;
uint32_t stack_produtor[80];
rtos::OSThread produtor;

//Variaveis para teste por debug
volatile uint8_t last_produced = 0;
volatile uint8_t last_consumed = 0;
volatile uint8_t buffer_count = 0;



uint32_t stack_consumidor[80];
rtos::OSThread consumidor;
void main_produtor() {
    uint8_t item = 0;
    while (1) {
        rtos::OSSem_wait(&sem_vazio);

        buffer[in] = item;
        last_produced = item;  // Para debug
        buffer_count++;        // Para debug
        item++;
        in = (in + 1) % BUF_TAM;

        rtos::OSSem_signal(&sem_cheio);
        rtos::OS_delay(200);
    }
}

void main_consumidor() {
    while (1) {
        rtos::OSSem_wait(&sem_cheio);

        last_consumed = buffer[out];  // Para debug
        buffer_count--;               // Para debug
        out = (out + 1) % BUF_TAM;

        rtos::OSSem_signal(&sem_vazio);
        rtos::OS_delay(500);
    }
}
void main_blinky1() {
    while (1) {
    	conta0++;
    	rtos::OS_delay(rtos::TICKS_PER_SEC * 3U / 4U);
    }
}

uint32_t stack_blinky2[40];
rtos::OSThread blinky2;
void main_blinky2() {
    while (1) {
    	conta1++;
    	rtos::OS_delay(rtos::TICKS_PER_SEC / 3U);
    }
}

uint32_t stack_blinky3[40];
rtos::OSThread blinky3;
void main_blinky3() {
    while (1) {
    	conta2++;
    	rtos::OS_delay(rtos::TICKS_PER_SEC * 3U / 5U);
    }
}

uint32_t stack_idleThread[40];

int main(void)
{

	 rtos::OS_init(stack_idleThread, sizeof(stack_idleThread));

	    rtos::OSSem_init(&sem_vazio, BUF_TAM);
	    rtos::OSSem_init(&sem_cheio, 0);

	    rtos::OSThread_start(&produtor, &main_produtor, stack_produtor, sizeof(stack_produtor));
	    rtos::OSThread_start(&consumidor, &main_consumidor, stack_consumidor, sizeof(stack_consumidor));

	    rtos::OS_run();
}


