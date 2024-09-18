/*
 * button.c
 *
 * Created: 4/25/2016 10:49:47 AM
 *  Author: ArminCo
 */ 

#include "button.h"


/* Bits is set to one if a debounced release is detected. */
volatile uint8_t buttons_release;
/* Bits is set to one if a hold down is detected.(for pedal check) */
volatile uint8_t pedalHold,armSensor;

/*! \brief This function Return non-zero if a button matching mask is pressed.
 *
 * \param button_mask	 pin mask that must be checked.
 *
 * \return uint8_t       debounced value of button(s) that send to function as buttonMask
 */
/*uint8_t buttonsDown(uint8_t buttonMask)
{
	// ATOMIC_BLOCK is needed if debounce() is called from within an ISR
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
		// And with debounced state for a one if they match
		buttonMask &= buttons_release;
		// Clear if there was a match
		buttons_release ^= buttonMask;
	}
	// Return non-zero if there was a match
	return buttonMask;
}*/


// /*! \brief This function set up the needed configuration for the port used for
//  *         the quadrature decoding.
//  *
//  * \param qPort     The port to use.
//  * \param qPin      The first input pin (QDPH0) to use (0 - 5/6).
//  * \param useIndex  True if an optional Index pin is used.
//  * \param invIO     True if IO pins should be inverted.
//  *
//  * \return bool     True if setup was ok, false if any errors.
//  */
// bool QDEC_Port_Setup(PORT_t * qPort, uint8_t qPin, bool useIndex, bool invIO)
// {
// 	/* Make setup depending on if Index signal is used. */
// 	if(useIndex){
// 		if(qPin > 5){
// 			return false;
// 		}
// 		qPort->DIRCLR = (0x07<<qPin);

// 		/* Configure Index signal sensing. */
// 		PORTCFG.MPCMASK = (0x04<<qPin);
// 		qPort->PIN0CTRL = (qPort->PIN0CTRL & ~PORT_ISC_gm) | PORT_ISC_BOTHEDGES_gc
// 		                  | (invIO ? PORT_INVEN_bm : 0);


// 	}else{
// 		if(qPin > 6){
// 			return false;
// 		}
// 		qPort->DIRCLR = (0x03<<qPin);
// 	}

// 	/* Set QDPH0 and QDPH1 sensing level. */
// 	PORTCFG.MPCMASK = (0x03<<qPin);
// 	qPort->PIN0CTRL = (qPort->PIN0CTRL & ~PORT_ISC_gm) | PORT_ISC_BOTHEDGES_gc
// 	                  | (invIO ? PORT_INVEN_bm : 0);
					  
// 	return true;
// }


// /*! \brief This function configure the event system for quadrature decoding.
//  *
//  * \param qEvMux      Which event channel to use. Only 0, 2 and 4 is available.
//  * \param qPinInput   The pin input of QDPH0 to the EVSYS.CHMUX .
//  * \param useIndex    True if an optional Index pin is used.
//  * \param qIndexState In which state the index signal will trigger.
//  *
//  * \return bool       True if setup was ok, false if any errors.
//  */
// bool QDEC_EVSYS_Setup(uint8_t qEvMux,
//                       EVSYS_CHMUX_t qPinInput,
//                       bool useIndex,
//                       EVSYS_QDIRM_t qIndexState )
// {
// 	switch (qEvMux){
// 		case 0:
		    
// 		/* Configure event channel 0 for quadrature decoding of pins. */
// 		EVSYS.CH0MUX = qPinInput;
// 		EVSYS.CH0CTRL = EVSYS_QDEN_bm | EVSYS_DIGFILT_8SAMPLES_gc;
// 		if(useIndex){
// 			/*  Configure event channel 1 as index channel. Note
// 			 *  that when enabling Index in channel n, the channel
// 			 *  n+1 must be configured for the index signal.*/
// 			EVSYS.CH1MUX = qPinInput + 2;
// 			EVSYS.CH1CTRL = EVSYS_DIGFILT_8SAMPLES_gc;
// 			EVSYS.CH0CTRL |= (uint8_t) qIndexState | EVSYS_QDIEN_bm;

// 		}
// 		break;
// 		case 2:
// 		EVSYS.CH2MUX = qPinInput;
// 		EVSYS.CH2CTRL = EVSYS_QDEN_bm | EVSYS_DIGFILT_8SAMPLES_gc;
// 		if(useIndex){
// 			EVSYS.CH3MUX = qPinInput + 2;
// 			EVSYS.CH3CTRL = EVSYS_DIGFILT_8SAMPLES_gc;
// 			EVSYS.CH2CTRL |= (uint8_t) qIndexState | EVSYS_QDIEN_bm;
// 		}
// 		break;
// 		case 4:
// 		EVSYS.CH4MUX = qPinInput;
// 		EVSYS.CH4CTRL = EVSYS_QDEN_bm | EVSYS_DIGFILT_8SAMPLES_gc;
// 		if(useIndex){
// 			EVSYS.CH5MUX = qPinInput + 2;
// 			EVSYS.CH5CTRL = EVSYS_DIGFILT_8SAMPLES_gc;
// 			EVSYS.CH4CTRL |= (uint8_t) qIndexState | EVSYS_QDIEN_bm;
// 		}
// 		break;
// 		default:
// 		return false;
// 	}
// 	return true;
// }

// /*! \brief This function set up the needed configuration for the Timer/Counter
//  *         to handle the quadrature decoding from the event system.
//  *
//  * \param qTimer        The timer to use for QDEC.
//  * \param qEventChannel The event channel to listen to.
//  * \param lineCount     The number of lines in the quadrature encoder.
//  */
// void QDEC_TC_Dec_Setup(TC0_t * qTimer, TC_EVSEL_t qEventChannel, uint16_t lineCount)
// {
// 	/* Configure TC as a quadrature counter. */
// 	qTimer->CTRLD = (uint8_t) TC_EVACT_QDEC_gc | qEventChannel;
// 	qTimer->PER = (lineCount * ROTARY_SPC);
// 	qTimer->CTRLA = TC_CLKSEL_DIV1_gc;/* Only for Enabling the TC*/
// }

// /*! \brief Wrapperfunction to set up all parameters for the quadrature decoder.
//  *
//  *  This function combines the following functions for a total setup:
//  *  QDEC_Port_Setup, QDEC_EVSYS_Setup and QDEC_TC_Dec_Setup.
//  *
//  * \param qPort         The port to use.
//  * \param qPin          The first input pin (QDPH0) to use (0 - 5/6).
//  * \param invIO         True if IO pins should be inverted.
//  *
//  * \param qEvMux        Which event channel to use. Only 0, 2 and 4 is available.
//  * \param qPinInput     The pin input of QDPH0 to the EVSYS.CHMUX .
//  * \param useIndex      True if an optional Index pin is used.
//  * \param qIndexState   In which state the index signal will trigger.
//  *
//  * \param qTimer        The timer to use for QDEC.
//  * \param qEventChannel The event channel to listen to.
//  * \param lineCount     The number of lines in the quadrature encoder.
//  *
//  * \return bool         True if setup was ok, false if any errors.
// */
// bool QDEC_Total_Setup(PORT_t * qPort,
//                       uint8_t qPin,
//                       bool invIO,
//                       uint8_t qEvMux,
//                       EVSYS_CHMUX_t qPinInput,
//                       bool useIndex,
//                       EVSYS_QDIRM_t qIndexState,
//                       TC0_t * qTimer,
//                       TC_EVSEL_t qEventChannel,
//                       uint16_t lineCount)
// {
// 	if( !QDEC_Port_Setup(qPort, qPin, useIndex, invIO) )
// 		return false;
// 	if( !QDEC_EVSYS_Setup(qEvMux, qPinInput, useIndex, qIndexState ) )
// 		return false;
// 	QDEC_TC_Dec_Setup(qTimer, qEventChannel, lineCount);

// 	return true;
// }

// /*! \brief This function set up the needed configuration for a Timer/Counter
//  *         to handle the frequency/speed measurement from the event system.
//  *
//  * \note   The real frequency of rotation can be calculated from the capture register
//  *         by using the folowing function.
//  *         FREQ = ( F_CPU / clk_div ) / ( CAPTURE * lineCount )
//  *
//  * \param qTimer        The timer to use for QDEC.
//  * \param qEventChannel The event channel to listen to.
//  * \param qPinInput     The pin input of QDPH0 to the EVSYS.CHMUX.
//  * \param clksel        The clk div to use for timer.
//  */
// void QDEC_TC_Freq_Setup(TC0_t * qTimer,
//                         TC_EVSEL_t qEventChannel,
//                         EVSYS_CHMUX_t qPinInput,
//                         TC_CLKSEL_t clksel)
// {
// 	/* Configure channel 2 to input pin for freq calculation. */
// 	EVSYS.CH2MUX = qPinInput;
// 	EVSYS.CH2CTRL = EVSYS_DIGFILT_8SAMPLES_gc;

// 	/* Configure TC to capture frequency. */
// 	qTimer->CTRLD = (uint8_t) TC_EVACT_FRQ_gc | qEventChannel;
// 	qTimer->PER = 0xFFFF;
// 	qTimer->CTRLB = TC0_CCAEN_bm;
// 	qTimer->CTRLA = clksel;
// }

// /*! \brief This function return the direction of the counter/QDEC.
//  *
//  * \param qTimer      The timer used for QDEC.
//  *
//  * \retval CW_DIR     if clockwise/up counting,
//  * \retval CCW_DIR    if counter clockwise/down counting.
//  */
// uint8_t QDEC_Get_Direction(TC1_t * qTimer)
// {
// 	if (qTimer->CTRLFSET && TC1_DIR_bm){
// 		return ROTARY_CW_DIR;
// 	}else{
// 		return ROTARY_CCW_DIR;
// 	}
// }
