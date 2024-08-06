#ifndef __LED_H
#define __LED_H

#ifdef __cplusplus
extern "C" {
#endif

#define NUM_PIXELS_PER_UNIT     41      /* number of LED per unit */
#define NUM_UNIT                5       
#define MAX_BRIGHT              180.0f  
#define MIN_BRIGHT              4
#define MAX_IDX                 50
#define ALL_LED                 NUM_PIXELS_PER_UNIT*NUM_UNIT
  
#define ARR 10

#ifdef WS2812
#define NEOPIXEL_ZERO           26      /* (ARR+1)*0.32 */
#define NEOPIXEL_ONE            51      /* (ARR+1)*0.64 */
#define DMA_BUFF_SIZE           24 + 1
#endif

#ifdef SK6812
#define NEOPIXEL_ZERO           19      /* (ARR+1)*0.25 */
#define NEOPIXEL_ONE            39      /* (ARR+1)*0.5 */
#define DMA_BUFF_SIZE           32 + 1
#endif

#define RING_BUF_SIZE           ALL_LED * 3

#define MUL_VAL                 100
/* private define ------------------------------------ */
typedef struct 
{
    u16     idx;/* data */
    u8      idx_list[NUM_UNIT];     
    u32     dma_buf[DMA_BUFF_SIZE]; //chort buf 

    u8      buf[RING_BUF_SIZE];     //all buf

    u16     R_NOW[ALL_LED];         //current
    u16     G_NOW[ALL_LED];         //current
    u16     B_NOW[ALL_LED];         //current

    u16     R_DEST[ALL_LED];        //red destination 
    u16     R_ORIG[ALL_LED];        //red origin 
    u16     G_DEST[ALL_LED];        //green destination
    u16     G_ORIG[ALL_LED];        //green origin
    u16     B_DEST[ALL_LED];        //blue destination
    u16     B_ORIG[ALL_LED];        //blue origin

    CBOOL   compare;           
    u8      dx;
} DATA_HANDLE_TYPEDEF_STRUCT;

typedef struct 
{
    u32     ch_red;
    u32     ch_green;
    u32     ch_blue;
    u32     ch_bright;

    u32     bg_red;
    u32     bg_green;
    u32     bg_blue;
    u32     bg_bright;

} COLOR_TYPEDEF_STRUCT;

/* data */
static u8       ascii_table_idx[MAX_IDX];
static u32      led_segment[MAX_IDX];  
static u16      led_side[MAX_IDX];
static float    led_segment_mask_r[NUM_PIXELS_PER_UNIT];
static float    led_segment_mask_g[NUM_PIXELS_PER_UNIT];
static float    led_segment_mask_b[NUM_PIXELS_PER_UNIT];

#ifdef __cplusplus
}
#endif

#endif