/*
 * button.h
 *
 * Created: 4/25/2016 10:50:01 AM
 *  Author: ArminCo
 */ 


#ifndef BUTTON_H_
#define BUTTON_H_

// #include "avr_compiler.h"
#include <stdatomic.h>
#include "stm32f3xx_hal.h"


// #define ROTARY				TCC1_CNT

#define ROTARY_SPC			4	/* SPC: STATE PER CYCLE (2 for ALPS rotary and 4 for other) */
// #define ROTARY_PINCTRL		PORT_INVEN_bm


/*! \brief These macros set a port, a pinmask and all other macros for buttons.*/
// #define KEY_PORT			PORTE

// #define ROTARY_PIN			2  //ROTARY FIRST PIN(PE.2)
 #define ROTARY_CW_DIR		0x10 /* Clockwise direction. */
 #define ROTARY_CCW_DIR		0x20 /* Counter Clockwise direction. */

/*! \brief macros for each buttons. */
/*#define KEY_NULL			0
#define KEY_ACCEPT			GPIO_PIN_10
#define KEY_STANDBY			GPIO_PIN_11
// for go to builder menu
#define KEY_BACK			GPIO_PIN_12
#define KEY_BUILDER			KEY_BACK
#define KEY_HIDDEN			GPIO_PIN_14*/

/*! \brief macros for each buttons. */
#define KEY_NULL			0
#define KEY_ACCEPT			0x01
#define KEY_STANDBY			0x02
// for go to builder menu
#define KEY_BACK			0x04
#define KEY_BUILDER			KEY_BACK
//#define KEY_HIDDEN			0x08
#define KEY_START		    0x08


#define READ_MID_KEY 	 HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10)
#define READ_STB_KEY 	 HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11)
#define READ_BACK_KEY	 HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_12)
#define READ_HOME_KEY	 HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_14)

#define KEY_GPIOA_MASK		(GPIO_PIN_12 | GPIO_PIN_11 | GPIO_PIN_10)
#define KEY_GPIOC_MASK		(GPIO_PIN_14)
#define KEY_READ			(((uint8_t)((GPIOA->IDR & KEY_GPIOA_MASK)>>10)&0x07) | ((uint8_t)((GPIOC->IDR & GPIO_PIN_14)>>11)&0x08))/*Shift keys states to bit 0-4 of a byte.*/
#define KEY_MASK			0x0F
#define KEY_RELEASE_MASK	0x0F// Which buttons act when released.
									 // zero for pins that we want act when pressed and one otherwise.

/*! \brief macros for each buttons. */
/*#define KEY_NULL			0
#define KEY_ACCEPT			0x01
#define KEY_STANDBY			0x02
// for go to builder menu
#define KEY_BACK			0x04
#define KEY_BUILDER			KEY_BACK
#define KEY_HIDDEN			0x08*/


/* Decrease 2 bit vertical counter where mask = 1.
 * Set counters to binary 11 where mask = 0.
 */
#define VC_DEC_OR_SET(high, low, mask)		\
			low = ~(low & mask);			\
			high = low ^ (high & mask)


/* Variable to tell that the button is released (and debounced).
 * Can be read with buttonsDown() which will clear it.
 */
extern  volatile uint8_t buttons_release;
/* Bits is set to one if a hold down is detected.(for pedal check) */
// volatile uint8_t pedalHold,armSensor;

uint8_t buttonsDown(uint8_t buttonMask);
// /* Variable to tell that the button is held down and debounced.
//  * Read this variable without buttonsDown() function to avoid of clear it.
//  */
// extern volatile uint8_t buttons_hold;

/* Inline functions. */

/*! \brief This function debounces up to 8 buttons joined to a port.
 *
 *	This function check button state and set the button_down variable if a 
 *	debounced button down press or release is detected.
 *	Call this function about 100 times per second. 	
 */
static inline void debounce(void)
{
    /* Eight vertical two bit counters for number of equal states. */
    static uint8_t vcount_low  = 0xFF, vcount_high = 0xFF;
    /* Keeps track of current (debounced) state. */
    static uint8_t button_state = 0;

    /* Read buttons (buttons are active low so invert with hardware in panel_library.c).
     * Xor with button_state to see which ones are about to change state.
     */
    uint8_t state_changed = ( KEY_READ ) ^ button_state;
//     uint8_t state_changed = ((KEY_READ /*| ((PORTB.IN & PIN4_bm)>>1)*/) ^ button_state) | ((PORTB.IN & PIN4_bm)>>1);
	/*											^
	 *											|
	  								 for weld connection debounce 
	 */
// 	uint8_t state_debounced = ((PORTB.IN & PIN4_bm)>>1);
	
    /* Decrease counters where state_changed = 1, set the others to 0b11. */
    VC_DEC_OR_SET(vcount_high, vcount_low, state_changed);
    
    /* Update state_changed to have a 1 only if the counter overflowed. */
    state_changed &= vcount_low & vcount_high;
	
    /* Change button_state for the buttons who's counters rolled over. */
    button_state ^= state_changed;

// 	buttons_hold = (button_state);

    /* Update buttons_release with buttons who's counters rolled over
     * and still them states is 0 (released).
     */
    buttons_release |= (button_state & (KEY_RELEASE_MASK/*|PIN3_bm*/)) & state_changed;   //release action.
	// buttons_press |= (button_state ^ (KEY_RELEASE_MASK/*|PIN3_bm*/)) & state_changed;   //press action.
/* 	pedalHold		 = (button_state & PIN3_bm); // Pedal state   // its state has holded , not release
	armSensor		 = (button_state & PIN2_bm); // sensor state    // its state has holded  , not release */
}



/* Prototyping of functions. Documentation is found in source file. */

/*
bool QDEC_Total_Setup(	PORT_t * qPort,
						uint8_t qPin,
						bool invIO,
						uint8_t qEvMux,
						EVSYS_CHMUX_t qPinInput,
						bool useIndex,
						EVSYS_QDIRM_t qIndexState,
						TC0_t * qTimer,
						TC_EVSEL_t qEventChannel,
						uint16_t lineCount);

bool QDEC_Port_Setup(PORT_t * qPort, uint8_t qPin, bool useIndex, bool invIO);

bool QDEC_EVSYS_Setup(	uint8_t qEvMux,
						EVSYS_CHMUX_t qPinInput,
						bool useIndex,
						EVSYS_QDIRM_t qIndexState );

void QDEC_TC_Dec_Setup( TC0_t * qTimer,
						TC_EVSEL_t qEventChannel,
						uint16_t lineCount);
						
void QDEC_TC_Freq_Setup(TC0_t * qTimer,
						TC_EVSEL_t qEventChannel,
						EVSYS_CHMUX_t qPinInput,
						TC_CLKSEL_t clksel);
						
uint8_t QDEC_Get_Direction(TC1_t * qTimer);
*/

#endif /* BUTTON_H_ */
