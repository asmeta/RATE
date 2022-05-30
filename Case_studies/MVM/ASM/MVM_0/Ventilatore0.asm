asm Ventilatore0

import ../StandardLibrary
import ../CTLlibrary
import ../LTLlibrary


signature:
	//*************************************************
	// DOMAINS
	//*************************************************
	enum domain States = {MAIN_REGION_STARTUP | MAIN_REGION_SELFTEST | MAIN_REGION_VENTILATIONOFF | MAIN_REGION_PCV_R1_INSPIRATION | MAIN_REGION_PCV_R1_EXPIRATION | MAIN_REGION_PSV_R1_INSPIRATION  | MAIN_REGION_PSV_R1_EXPIRATION  | MAIN_REGION_PCV_R1_INSPIRATORYPAUSE | MAIN_REGION_PCV_R1_EXPIRATORYPAUSE | MAIN_REGION_PSV_R1_INSPIRATORYPAUSE| MAIN_REGION_PSV_R1_EXPIRATORYPAUSE | MAIN_REGION_PCV_R1_RM | MAIN_REGION_PSV_R1_RM | OFF}
	
	enum domain Modes = {PCV | PSV}

	enum domain ValveStatus = {OPEN | CLOSED}

	//*************************************************
	// FUNCTIONS
	//*************************************************
	dynamic monitored poweroff: Boolean //turnOFFbtn
	dynamic monitored startupEnded: Boolean
	dynamic monitored selfTestPassed: Boolean
	dynamic monitored resume: Boolean
	dynamic monitored startVentilation: Boolean
	dynamic monitored stopVentilation: Boolean //stopRequested
	dynamic monitored mode: Modes //respirationMode
	dynamic monitored flowDropPSV: Boolean
	
	dynamic controlled stopVentilationRequested: Boolean //stopVentilation
	dynamic controlled state: States
	dynamic controlled iValve: ValveStatus
	dynamic controlled oValve: ValveStatus

	// Functions that substitute the time management 
	dynamic monitored inspirationDurationPassed: Boolean
	dynamic monitored expirationDurationPassed: Boolean
	dynamic monitored maxInspTimePSVPassed: Boolean
	dynamic monitored minExpTimePSVPassed: Boolean


definitions:
	//*************************************************
	// RULE DEFINITIONS
	//*************************************************
	rule r_startupEnded =
			state := MAIN_REGION_SELFTEST

	rule r_ventOffRequested =
			stopVentilationRequested := true

	rule r_ventOff =
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
			oValve := CLOSED 
		endpar

	rule r_PSVinsp =
		par
			state := MAIN_REGION_PSV_R1_INSPIRATION
			iValve := OPEN
			oValve := CLOSED 
		endpar

	rule r_PCVexp =
		par
			state := MAIN_REGION_PCV_R1_EXPIRATION
			iValve := CLOSED 
			oValve := OPEN
		endpar

	rule r_PSVexp =
		par
			state := MAIN_REGION_PSV_R1_EXPIRATION
			iValve := CLOSED
			oValve := OPEN
		endpar

	//*************************************************
	// Property Verification
	//*************************************************

	//safety: valves are never both open or both closed at the same time
	CTLSPEC not ef(iValve=CLOSED and oValve=CLOSED)
	CTLSPEC not ef(iValve=OPEN and oValve=OPEN)
	CTLSPEC not ef(iValve=oValve)
	//when ventilation is off, out valve is open and in valve is closed
	CTLSPEC ag(state=MAIN_REGION_VENTILATIONOFF implies (iValve=CLOSED and oValve=OPEN))

	//no deadlock: it is always possible to get back in the MAIN_REGION_VENTILATIONOFF state
	//only exception: turn-off button is pressed
	CTLSPEC ag(not poweroff) implies ag(ef(state=MAIN_REGION_VENTILATIONOFF))

	//once turned off, the state doesn't change anymore
	CTLSPEC ag(state=OFF implies ag(state=OFF))
	
	//*************************************************
	// MAIN Rule
	//*************************************************
	main rule r_Main =

	//if the button is pressed, shut down the ventilator
	if poweroff then r_turnOff[]

		//if ventilation stop is requested and current state is an expiration
		//go to state MAIN_REGION_VENTILATIONOFF immediately
		else if (stopVentilation and (state=MAIN_REGION_PCV_R1_EXPIRATION or state=MAIN_REGION_PSV_R1_EXPIRATION)) then r_ventOff[]

		else

		par

		//if ventilation stop is requested and ventilation is on, 
		//store the request in controlled function stopVentilationRequested
		if stopVentilation and state!=MAIN_REGION_STARTUP and state!=MAIN_REGION_SELFTEST and state!=MAIN_REGION_VENTILATIONOFF then r_ventOffRequested[] endif

		//transition from startup to selftest
		if state = MAIN_REGION_STARTUP then
			if startupEnded then r_startupEnded[] endif
		endif

		//transition from selftest to ventilation off
		if state = MAIN_REGION_SELFTEST then
			if (selfTestPassed or resume) then r_ventOffFirstTime[] endif
		endif

		//start ventilation, either pcv or psv
		if state = MAIN_REGION_VENTILATIONOFF then
			if startVentilation	then
				if mode = PCV	then r_PCVinsp[]
					else if mode = PSV	then r_PSVinsp[] endif
				endif
			endif
		endif

		//transition from inspiration to expiration, mode change if requested
		if state = MAIN_REGION_PCV_R1_INSPIRATION then
			if inspirationDurationPassed	then
				if mode = PCV then r_PCVexp[]
					else if mode = PSV then r_PSVexp[] endif
				endif
			endif
		endif

		//transition from expiration to inspiration, ventilation turned off if requested
		if state = MAIN_REGION_PCV_R1_EXPIRATION	then
			if stopVentilationRequested then r_ventOff[]
				else if expirationDurationPassed then r_PCVinsp[] endif
			endif
		endif

		//transition from inspiration to expiration, by time or by flow drop
		if state = MAIN_REGION_PSV_R1_INSPIRATION	then
			if flowDropPSV then r_PSVexp[]
				else if maxInspTimePSVPassed then r_PSVexp[] endif
			endif
		endif

		//transition from expiration to inspiration, mode change if requested, ventilation turned off if requested
		if state = MAIN_REGION_PSV_R1_EXPIRATION	then
			if stopVentilationRequested then r_ventOff[]
			else if minExpTimePSVPassed	then
				if mode = PSV then r_PSVinsp[]
					else if mode = PCV then r_PCVinsp[] endif
				endif
			endif
			endif
		endif

	endpar
	endif
	endif

default init s0:
//initialising controlled functions
	function state = MAIN_REGION_STARTUP
	function iValve = CLOSED
	function oValve = OPEN
	function stopVentilationRequested = false