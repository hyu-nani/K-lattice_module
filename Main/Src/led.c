/**
 * @file led.c
 * @author caldronear (hyu-nani@github.com)
 * @brief Neo pixel led control by DMA
 * @version 0.1
 * @date 2024-08-07
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include <main.h>   
#include "../Inc/led.h"
#include "../Inc/asciiTable.h"

TIM_HandleTypeDef htim3;

COLOR_TYPEDEF_STRUCT ledColor;
DATA_HANDLE_TYPEDEF_STRUCT hled;

#define DMA_HANDLE  &htim3

static void DMA_Start()void {
    while(hled.sendFlag != OK);
	HAL_TIM_PWM_Start_DMA(DMA_HANDLE, TIM_CHANNEL_1, hled.dma_buf, DMA_BUFF_SIZE);
	hled.sendFlag == BUSY;
}

/** @brief led data send to DMA
 * 
 */
static void LED_DMASend(void)
{
	DMA_Start();
	for (u32 pixelNum = 0U ; pixelNum < ALL_LED; pixelNum++)
	{
		hled.idx = 0U;
		u8 i = (u8)0U;
		//GRB
		for (i = 8U; i > 0U; i--)
		{
			hled.dma_buf[hled.idx++] = ((hled.buf[(pixelNum*3U) + 1U] >> i) & 0x01U) ? NEOPIXEL_ONE : NEOPIXEL_ZERO;
		}
		for (i = 8U; i > 0U; i--)
		{
			hled.dma_buf[hled.idx++] = ((hled.buf[(pixelNum*3U) + 0U] >> i) & 0x01U) ? NEOPIXEL_ONE : NEOPIXEL_ZERO; 
		}
		for (i = 8U; i > 0U; i--)
		{
			hled.dma_buf[hled.idx++] = ((hled.buf[(pixelNum*3U) + 2U] >> i ) & 0x01U) ? NEOPIXEL_ONE : NEOPIXEL_ZERO;
		}
		hled.dma_buf[hled.idx] = 0U;
		DMA_Start();
	}
}

/** @brief led color data stack to buf
 * 
 *  @param pixelNum set pixel num
 *  @param red red val 0-255
 *  @param green green val 0-255
 *  @param blue blue val 0-255
 *  @param bright led total bright 0-100
 */
void LED_setColor(u8 pixelNum, u8 led_R, u8 led_G, u8 led_B)
{
	hled.buf[pixelNum * 3 + 0] = (u8)((float)led_R * MAX_BRIGHT / 255.0f);
	hled.buf[pixelNum * 3 + 1] = (u8)((float)led_G * MAX_BRIGHT / 255.0f);
	hled.buf[pixelNum * 3 + 2] = (u8)((float)led_B * MAX_BRIGHT / 255.0f);
}

/**
 * @brief led change sync
 * 
 */
CSTATUS LED_SyncUpdate(void) {
	s32 i;
	CSTATUS status = OK;
	if (hled.syncStatus == INIT) {
		memcpy(&ledColor.R_ORIG[0], &ledColor.R_NOW[0], ALL_LED);
		memcpy(&ledColor.G_ORIG[0], &ledColor.G_NOW[0], ALL_LED);
		memcpy(&ledColor.B_ORIG[0], &ledColor.B_NOW[0], ALL_LED);
		hled.dxCnt = 0;
		status = ;
	}
	else if (hled.syncStatus == BUSY) {
		for (i = 0U; i < ALL_LED; i++)
		{
			ledColor.R_NOW[i] += (s16)(((float)ledColor.R_DEST[i] - (float)ledColor.R_ORIG[i]) / (float)hled.dx);
			ledColor.G_NOW[i] += (s16)(((float)ledColor.G_DEST[i] - (float)ledColor.G_ORIG[i]) / (float)hled.dx);
			ledColor.B_NOW[i] += (s16)(((float)ledColor.B_DEST[i] - (float)ledColor.B_ORIG[i]) / (float)hled.dx);
			LED_setColor(i, (u8)((float)ledColor.R_NOW[i] * mask_R[mask_size[i % NUM_PIXELS_PER_UNIT]] / MUL_VAL), 
							(u8)((float)ledColor.G_NOW[i] * mask_G[mask_size[i % NUM_PIXELS_PER_UNIT]] / MUL_VAL), 
							(u8)((float)ledColor.B_NOW[i] * mask_B[mask_size[i % NUM_PIXELS_PER_UNIT]] / MUL_VAL));
		}
		hled.dxCnt++;
		if (hled.dxCnt == hled.dx) {
			hled.syncStatus = DONE;
		}
	}
	else if (hled.syncStatus == DONE) {
		hled.isUpdateFlag = OK;
	}
	LED_DMASend();
}

/** @brief led show segment
 * 
 *  @param ch a ascii textIdx
 *  @param led_R 0 - 255
 *  @param led_G 0 - 255
 *  @param led_B 0 - 255
 *  @param led_bright 0 - 100 [%]
 *  @param invert true = invert, false = default
 */
void LED_setSegment(u8* str, COLOR_TYPEDEF_STRUCT* color, CBOOL invert)
{
	s32 i, j, k;
    u8 bit8;

    /* range check */
	if (color->CHAR_L > 100) { color->CHAR_L = 100; }
	if (color->BACK_L > 100) { color->BACK_L = 100; }

	color->CHAR_R = (u32)((float)color->CHAR_R * (float)color->CHAR_L / 100.0f) * MUL_VAL;
	color->CHAR_G = (u32)((float)color->CHAR_G * (float)color->CHAR_L / 100.0f) * MUL_VAL;
	color->CHAR_B = (u32)((float)color->CHAR_B * (float)color->CHAR_L / 100.0f) * MUL_VAL;
	color->BACK_R = (u32)((float)color->BACK_R * (float)color->BACK_L / 100.0f) * MUL_VAL;
	color->BACK_G = (u32)((float)color->BACK_G * (float)color->BACK_L / 100.0f) * MUL_VAL;
	color->BACK_B = (u32)((float)color->BACK_B * (float)color->BACK_L / 100.0f) * MUL_VAL;
	
	for (i = 0U; i < NUM_UNIT; i++)
	{
		for (j = 0U; j < NUM_PIXELS_PER_UNIT; j++)
		{   
            bit8 = (u8)ascii_segment[(((u32)*(str + i)) * 6U) + (j / 8U)];
            if (((u32)bit8 << (j % 8U)) & 0x80U)
            {	
                if (invert == CFALSE) {
                    ledColor.R_DEST[(j + i * NUM_PIXELS_PER_UNIT)] = color->CHAR_R;
                    ledColor.G_DEST[(j + i * NUM_PIXELS_PER_UNIT)] = color->CHAR_G;
                    ledColor.B_DEST[(j + i * NUM_PIXELS_PER_UNIT)] = color->CHAR_B;
                } else {
                    ledColor.R_DEST[(j + i * NUM_PIXELS_PER_UNIT)] = color->BACK_R;
                    ledColor.G_DEST[(j + i * NUM_PIXELS_PER_UNIT)] = color->BACK_G;
                    ledColor.B_DEST[(j + i * NUM_PIXELS_PER_UNIT)] = color->BACK_B;
                }
            } else {
                if (invert == CFALSE) {
                    ledColor.R_DEST[(j + i * NUM_PIXELS_PER_UNIT)] = color->BACK_R;
                    ledColor.G_DEST[(j + i * NUM_PIXELS_PER_UNIT)] = color->BACK_G;
                    ledColor.B_DEST[(j + i * NUM_PIXELS_PER_UNIT)] = color->BACK_B;
                } else {
                    ledColor.R_DEST[(j + i * NUM_PIXELS_PER_UNIT)] = color->CHAR_R;
                    ledColor.G_DEST[(j + i * NUM_PIXELS_PER_UNIT)] = color->CHAR_G;
                    ledColor.B_DEST[(j + i * NUM_PIXELS_PER_UNIT)] = color->CHAR_B;
                }
            }
		}
	}

    
}

/**
 * @brief led All off
 */
void LED_allOff(void)
{
	for (u32 i = 0U; i < ALL_LED + 1U; i++)
	{
		LED_setColor((u8)i, (u8)0U, (u8)0, (u8)0);
	}
    LED_DMASend();
	HAL_Delay(1);
}

void resetLedSendFlag() {
    hled.sendFlag = OK;
}

void LED_INIT() {
    hled.dx = 15U;
	hled.isUpdateFlag = OK;
}

void LED_SetDX(u8 dx) {
    hled.dx = dx;
}

void LED_PROC() {
	if (hled.isUpdateFlag != OK) {
		LED_SyncUpdate();
	}
}