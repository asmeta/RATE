asm Ventilatore01

/*
	This model implements the PCV and the PSV mode.
	No inspiratory/expiratory pause is present, as well as alarms.
	No transition between PCV and PSV is possible (apnea not considered)
*/

import ../StandardLibrary
import ../CTLlibrary
import ../LTLlibrary

signature:

	// TIMER
	enum domain Timer = {TIMER_INSPIRATION_DURATION_MS, 
		TIMER_EXPIRATION_DURATION_MS, 
		TIMER_MAX_INSP_TIME_PSV,
		TIMER_MIN_EXP_TIME_PSV,
		TIMER_TRIGGERWINDOWDELAY_MS,
		TIMER_MIN_INSP_TIME_MS
	}

	domain MyTime subsetof Integer

	//*************************************************
	// DOMAINS
	//*************************************************
	enum domain States = {MAIN_REGION_STARTUP | MAIN_REGION_SELFTEST | MAIN_REGION_VENTILATIONOFF | MAIN_REGION_PCV_R1_INSPIRATION | MAIN_REGION_PCV_R1_EXPIRATION | MAIN_REGION_PSV_R1_INSPIRATION  | MAIN_REGION_PSV_R1_EXPIRATION | OFF}

	enum domain Modes = {PCV | PSV}

	enum domain ValveStatus = {OPEN | CLOSED}

	//*************************************************
	// FUNCTIONS
	//*************************************************
	dynamic monitored poweroff: Boolean
	dynamic monitored startupEnded: Boolean
	dynamic monitored selfTestPassed: Boolean
	dynamic monitored resume: Boolean
	dynamic monitored startVentilation: Boolean
	dynamic monitored stopVentilation: Boolean
	dynamic monitored mode: Modes
	dynamic monitored flowDropPSV: Boolean
	dynamic monitored pawGTMaxPinsp: Boolean
	dynamic monitored dropPAW_ITS_PCV: Boolean
	dynamic monitored dropPAW_ITS_PSV: Boolean

	dynamic controlled time: MyTime
	
	dynamic controlled stopVentilationRequested: Boolean
	dynamic controlled state: States
	dynamic controlled iValve: ValveStatus
	dynamic controlled oValve: ValveStatus

	// starting time taken from the local clock
	controlled start: Timer-> MyTime
	
	static durationTIMER_INSPIRATION_DURATION_MS : MyTime
	static durationTIMER_EXPIRATION_DURATION_MS: MyTime
	static durationTIMER_TRIGGERWINDOWDELAY_MS : MyTime
	static durationTIMER_MAX_INSP_TIME_PSV: MyTime
	static durationTIMER_MIN_EXP_TIME_PSV: MyTime
	static durationTIMER_MIN_INSP_TIME_MS: MyTime

	derived expiredTIMER_INSPIRATION_DURATION_MS : Boolean
	derived expiredTIMER_EXPIRATION_DURATION_MS: Boolean
	derived expiredTIMER_TRIGGERWINDOWDELAY_MS : Boolean
	derived expiredTIMER_MAX_INSP_TIME_PSV: Boolean
	derived expiredTIMER_MIN_EXP_TIME_PSV: Boolean
	derived expiredTIMER_MIN_INSP_TIME_MS: Boolean
													
definitions:

	domain MyTime = {0:600}		
		
	function durationTIMER_INSPIRATION_DURATION_MS 	=  20 		//settato dal dottore. valore indicativo per respiratory rate 10 e I:E 0,5
    function durationTIMER_EXPIRATION_DURATION_MS =  40 		//settato dal dottore. valore indicativo
	function durationTIMER_TRIGGERWINDOWDELAY_MS =  10 			// 0.7
	function durationTIMER_MAX_INSP_TIME_PSV = 70
	function durationTIMER_MIN_EXP_TIME_PSV = 10
	function durationTIMER_MIN_INSP_TIME_MS = 10
	
	/*******************************************************/	
	function expiredTIMER_INSPIRATION_DURATION_MS  = (time >= start(TIMER_INSPIRATION_DURATION_MS) + durationTIMER_INSPIRATION_DURATION_MS)
	function expiredTIMER_EXPIRATION_DURATION_MS = (time >= start(TIMER_EXPIRATION_DURATION_MS) + durationTIMER_EXPIRATION_DURATION_MS)
	function expiredTIMER_TRIGGERWINDOWDELAY_MS  = (time >= start(TIMER_TRIGGERWINDOWDELAY_MS) + durationTIMER_TRIGGERWINDOWDELAY_MS)
	function expiredTIMER_MAX_INSP_TIME_PSV = (time >= start(TIMER_MAX_INSP_TIME_PSV) + durationTIMER_MAX_INSP_TIME_PSV)
	function expiredTIMER_MIN_EXP_TIME_PSV = (time >= start(TIMER_MIN_EXP_TIME_PSV) + durationTIMER_MIN_EXP_TIME_PSV)
	function expiredTIMER_MIN_INSP_TIME_MS  = (time >= start(TIMER_MIN_INSP_TIME_MS) + durationTIMER_MIN_INSP_TIME_MS)
	
	/*******************************************************/
	// restart the TIMER
	macro rule r_reset_TIMER($t in Timer) =	start($t) := time
	
	//*************************************************
	// RULE DEFINITIONS
	//*************************************************
	
	rule r_startupEnded =
			state := MAIN_REGION_SELFTEST

	rule r_ventOffRequested =
			stopVentilationRequested := true

	rule r_ventOffPCV =
		par
			state := MAIN_REGION_VENTILATIONOFF
			stopVentilationRequested := false
		endpar
		
	rule r_ventOffPSV =
		par
			state := MAIN_REGION_VENTILATIONOFF
			stopVentilationRequested := false
		endpar
	
	rule r_ventOffFirstTime =
		state := MAIN_REGION_VENTILATIONOFF

	rule r_turnOff =
		par
			iValve := CLOSED
			oValve := OPEN
			state := OFF
		endpar

	rule r_PCVinsp =
		par
			state := MAIN_REGION_PCV_R1_INSPIRATION
			iValve := OPEN
			r_reset_TIMER[TIMER_INSPIRATION_DURATION_MS]
		endpar
		
	rule r_PCVinspOValve =
		par
			r_PCVinsp[]
			oValve := CLOSED
		endpar
		
	rule r_PSVinsp =
		par
			state := MAIN_REGION_PSV_R1_INSPIRATION 
			iValve := OPEN
			r_reset_TIMER[TIMER_MAX_INSP_TIME_PSV]
			r_reset_TIMER[TIMER_MIN_INSP_TIME_MS]
		endpar
	rule r_PSVinspOValve =
		par
			r_PSVinsp[]
			oValve := CLOSED
			r_reset_TIMER[TIMER_MAX_INSP_TIME_PSV]
		endpar
		
	rule r_PCVexp =
		par
			state := MAIN_REGION_PCV_R1_EXPIRATION 
			oValve := OPEN
			r_reset_TIMER[TIMER_EXPIRATION_DURATION_MS]
			r_reset_TIMER[TIMER_TRIGGERWINDOWDELAY_MS]
		endpar
			
	rule r_PCVexpIValve =
		par
			r_PCVexp[]
			iValve := CLOSED		
		endpar
		
	rule r_PSVexp =
		par
			state := MAIN_REGION_PSV_R1_EXPIRATION 
			oValve := OPEN
			r_reset_TIMER[TIMER_MIN_EXP_TIME_PSV]
			r_reset_TIMER[TIMER_TRIGGERWINDOWDELAY_MS]
		endpar
		
	rule r_PSVexpIValve =
		par
			r_PSVexp[]
			iValve := CLOSED
		endpar
	
	rule r_PSVexpIValveFromPCV =
		par
			r_PSVexp[]
			iValve := CLOSED
		endpar

	//*************************************************
	// Property Verification
	//*************************************************
	
	//when ventilation is off, out valve is open and in valve is closed
	CTLSPEC ag(state=MAIN_REGION_VENTILATIONOFF implies (iValve=CLOSED and oValve=OPEN))

	//no deadlock: it is always possible to get back in the MAIN_REGION_VENTILATIONOFF state
	//only exception not considered: turn-off button is pressed
	CTLSPEC ag(not poweroff) implies ag(ef(state=MAIN_REGION_VENTILATIONOFF))

	//once turned off, the state doesn't change anymore
	CTLSPEC ag(state=OFF implies ag(state=OFF))
	
	LTLSPEC g(iValve = CLOSED)
	//once turned off, the state doesn't change anymore
	LTLSPEC g(state=OFF implies g(state=OFF))
	//when ventilation is off, out valve is open and in valve is closed
	LTLSPEC g(state=MAIN_REGION_VENTILATIONOFF implies (iValve=CLOSED and oValve=OPEN))

	//safety properties: valves are never both open or both closed at the same time
	LTLSPEC not f(iValve=CLOSED and oValve=CLOSED)
	LTLSPEC not f(iValve=OPEN and oValve=OPEN)
	
	//*************************************************
	// MAIN Rule
	//*************************************************
	main rule r_Main =
		par 
			time:= time+1
		
			//if the button is pressed, shut down the ventilator
			if poweroff then r_turnOff[]
			
			//if stop of the ventilation is requested and current state is an expiration
			//go to state MAIN_REGION_VENTILATIONOFF immediately
			else 
				if state=MAIN_REGION_PCV_R1_EXPIRATION and (stopVentilationRequested or stopVentilation) then 
					r_ventOffPCV[]
				else
					if state=MAIN_REGION_PSV_R1_EXPIRATION and (stopVentilationRequested or stopVentilation)  then 
						r_ventOffPSV[] 
					else		
						par
							//if ventilation stop is requested and ventilation is on, 
							//store the stop request in controlled function stopVentilationRequested
							if stopVentilation then
								if state!=MAIN_REGION_PSV_R1_EXPIRATION and state!=MAIN_REGION_PCV_R1_EXPIRATION and state!=MAIN_REGION_STARTUP and state!=MAIN_REGION_SELFTEST and state!=MAIN_REGION_VENTILATIONOFF then r_ventOffRequested[] endif
							endif
					
							//transition from startup to selftest
							if state = MAIN_REGION_STARTUP then
								if startupEnded then r_startupEnded[] endif
							endif
			
							//transition from selftest to ventilation off
							if state = MAIN_REGION_SELFTEST then
								if (selfTestPassed or resume) then r_ventOffFirstTime[] endif
							endif
			
							//start ventilation, in PCV mode
							if state = MAIN_REGION_VENTILATIONOFF then
								if startVentilation	then
									par
										if mode = PCV then r_PCVinspOValve[] endif
										if mode = PSV then r_PSVinspOValve[] endif
									endpar
								endif
							endif
			
							//transition from inspiration to expiration
							if state = MAIN_REGION_PCV_R1_INSPIRATION then
								if expiredTIMER_INSPIRATION_DURATION_MS then
									par
										if mode = PSV then 
									 		r_PSVexpIValveFromPCV[]
										endif
										if mode = PCV then 
									 		r_PCVexpIValve[]
										endif
									endpar
								else 
									if pawGTMaxPinsp then 
										r_PCVexpIValve[] 
									endif
								endif
							endif
						
							if state = MAIN_REGION_PCV_R1_EXPIRATION then
								if expiredTIMER_EXPIRATION_DURATION_MS then 
									r_PCVinspOValve[] 
								else 
									if expiredTIMER_TRIGGERWINDOWDELAY_MS then
										if dropPAW_ITS_PCV then 
											r_PCVinspOValve[] 
										endif
									endif
								endif
							endif	
							
							// transition from inspiration to expiration
							if state = MAIN_REGION_PSV_R1_INSPIRATION 	then
								if ((flowDropPSV and expiredTIMER_MIN_INSP_TIME_MS) or expiredTIMER_MAX_INSP_TIME_PSV) then
									r_PSVexpIValve[]
								else 
									if pawGTMaxPinsp then 
										r_PSVexpIValve[]
									endif
								endif
							endif	
							
							//transition from expiration to inspiration, ventilation turned off if requested
							if state = MAIN_REGION_PSV_R1_EXPIRATION 	then
								if (expiredTIMER_TRIGGERWINDOWDELAY_MS and dropPAW_ITS_PSV) then 
									r_PSVinspOValve[]
								else 
									if (expiredTIMER_MIN_EXP_TIME_PSV and mode = PCV) then 
										r_PCVinspOValve[] 
									endif
								endif
							endif
						endpar
					endif
				endif
			endif
		endpar

default init s0:
	function time = 0
	function state = MAIN_REGION_STARTUP
	function iValve = CLOSED
	function oValve = OPEN
	function stopVentilationRequested = false	
	function start($t in Timer) = 0
