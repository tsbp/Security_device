/*
 * LED_7_SEGMENT.h
 *
 *  Created on: 29 вер. 2016
 *      Author: Voodoo
 */

#ifndef INCLUDE_DRIVER_LED_7_SEGMENT_H_
#define INCLUDE_DRIVER_LED_7_SEGMENT_H_
//==============================================================================
#define SEG_A	(1 << 0)
#define SEG_B	(1 << 3)
#define SEG_C	(1 << 5)
#define SEG_D	(1 << 7)
#define SEG_E	(1 << 1)
#define SEG_F	(1 << 2)
#define SEG_G	(1 << 4)
#define SEG_H	(1 << 6)
//==============================================================================
#define DIG_0	(SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F)
#define DIG_1	(SEG_C | SEG_B )
#define DIG_2	(SEG_A | SEG_B | SEG_D | SEG_E | SEG_G)
#define DIG_3	(SEG_A | SEG_B | SEG_C | SEG_D | SEG_G )
#define DIG_4	(SEG_B | SEG_C | SEG_F | SEG_G )
#define DIG_5	(SEG_A | SEG_C | SEG_D | SEG_G | SEG_F)
#define DIG_6	(SEG_A | SEG_G | SEG_C | SEG_D | SEG_E | SEG_F)
#define DIG_7	(SEG_A | SEG_B | SEG_C )
#define DIG_8	(SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G)
#define DIG_9	(SEG_A | SEG_B | SEG_C | SEG_D | SEG_G | SEG_F )
#define DIG__	(0)
#define DIG_F	(SEG_A | SEG_E | SEG_F | SEG_G)
#define DIG_n	(SEG_C | SEG_E | SEG_G)
#define DIG_A	(SEG_A | SEG_B | SEG_C| SEG_E | SEG_F | SEG_G)
#define DIG_L	(SEG_D | SEG_E | SEG_F)
#define DIG_r	(SEG_E | SEG_G)
#define DIG_S	(SEG_A | SEG_C | SEG_D | SEG_F | SEG_G)
#define DIG_t	(SEG_D | SEG_E | SEG_F | SEG_G)
#define DIG_b	(SEG_C | SEG_D | SEG_E | SEG_F | SEG_G)
//==============================================================================
extern void display(uint8 *aDisp, uint8 aBlink);
//==============================================================================
#endif /* INCLUDE_DRIVER_LED_7_SEGMENT_H_ */
