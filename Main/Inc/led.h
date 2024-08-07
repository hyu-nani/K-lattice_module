#ifndef __LED_H
#define __LED_H

#ifdef __cplusplus
extern "C" {
#endif

#define WS2812
//#define SK6812

#define NUM_PIXELS_PER_UNIT     41      /* number of LED per unit */
#define NUM_UNIT                5       
#define MAX_BRIGHT              255.0f  
#define MIN_BRIGHT              4
#define ALL_LED                 NUM_PIXELS_PER_UNIT*NUM_UNIT
  

#define PRESCALER               8
#define CLOCK                   64000000f
#define PWM                     0.0000012f

#define ARR                     (CLOCK*PWM)-1

#ifdef WS2812
#define NEOPIXEL_ZERO           (ARR+1)*0.32f
#define NEOPIXEL_ONE            (ARR+1)*0.64f
#define DMA_BUFF_SIZE           24 + 1
#endif

#ifdef SK6812
#define NEOPIXEL_ZERO           19      /* (ARR+1)*0.25 */
#define NEOPIXEL_ONE            39      /* (ARR+1)*0.5 */
#define DMA_BUFF_SIZE           32 + 1
#endif

#define RING_BUF_SIZE           ALL_LED * 3

#define MUL_VAL                 100.0f

/* private define ------------------------------------ */
typedef struct 
{
    u16     idx;/* data */
    u8      idx_list[NUM_UNIT];     
    u32     dma_buf[DMA_BUFF_SIZE]; //chort buf 
    u8      buf[RING_BUF_SIZE];     //all buf

    CBOOL   compare;           
    u8      DX;
    u8      sendFlag;
} DATA_HANDLE_TYPEDEF_STRUCT;

typedef struct 
{
    u32     CHAR_R;
    u32     CHAR_G;
    u32     CHAR_B;
    u32     CHAR_L;

    u32     BACK_R;
    u32     BACK_G;
    u32     BACK_B;
    u32     BACK_L;

    u16     R_NOW[ALL_LED];         //current
    u16     G_NOW[ALL_LED];         //current
    u16     B_NOW[ALL_LED];         //current

    u16     R_DEST[ALL_LED];        //red destination 
    u16     G_DEST[ALL_LED];        //green destination
    u16     B_DEST[ALL_LED];        //blue destination

    u16     R_ORIG[ALL_LED];        //red origin 
    u16     G_ORIG[ALL_LED];        //green origin
    u16     B_ORIG[ALL_LED];        //blue origin
} COLOR_TYPEDEF_STRUCT;


/* data */
static float    led_segment_mask_r[NUM_PIXELS_PER_UNIT];
static float    led_segment_mask_g[NUM_PIXELS_PER_UNIT];
static float    led_segment_mask_b[NUM_PIXELS_PER_UNIT];

void resetLedSendFlag(void);
void LED_Init(void);

#ifdef __cplusplus
}
#endif

#endif