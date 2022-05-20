asm Ventilatore2

import ../StandardLibrary
import ../CTLlibrary
import ../LTLlibrary


signature:
	//*************************************************
	// DOMAINS
	//*************************************************
	enum domain States = {MAIN_REGION_STARTUP | MAIN_REGION_SELFTEST | MAIN_REGION_VENTILATIONOFF | MAIN_REGION_PCV_R1_INSPIRATION | MAIN_REGION_PCV_R1_EXPIRATION | MAIN_REGION_PSV_R1_INSPIRATION | MAIN_REGION_PSV_R1_EXPIRATION | MAIN_REGION_PCV_R1_INSPIRATORYPAUSE | MAIN_REGION_PCV_R1_EXPIRATORYPAUSE | MAIN_REGION_PSV_R1_INSPIRATORYPAUSE| MAIN_REGION_PSV_R1_EXPIRATORYPAUSE | MAIN_REGION_PCV_R1_RM | MAIN_REGION_PSV_R1_RM | OFF}

	enum domain Modes = {PCV | PSV}

	enum domain ValveStatus = {OPEN | CLOSED}
	

	//*************************************************
	// FUNCTIONS
	//*************************************************
	dynamic monitored turnOFFbtn: Boolean
	dynamic monitored startupEnded: Boolean
	dynamic monitored selfTestPassed: Boolean
	dynamic monitored resume: Boolean
	dynamic monitored startVentilation: Boolean
	dynamic monitored stopRequested: Boolean
	dynamic monitored respirationMode: Modes
	dynamic monitored flowDropPSV: Boolean
	dynamic monitored expPause: Boolean
	dynamic monitored inPause: Boolean
	dynamic monitored rm: Boolean
	dynamic monitored turnOffApneaAlarm: Boolean
	dynamic monitored pawGTMaxPinsp: Boolean
	dynamic monitored dropPAW_ITS_PCV: Boolean
	dynamic monitored dropPAW_ITS_PSV: Boolean
	
	dynamic controlled stopVentilation: Boolean
	dynamic controlled state: States
	dynamic controlled iValve: ValveStatus
	dynamic controlled oValve: ValveStatus
	dynamic controlled apneaAlarm: Boolean
	dynamic controlled apneaBackupMode: Boolean
	
	// Functions that substitute the time management and respiration phases
	dynamic monitored inspirationDurationPassed: Boolean
	dynamic monitored expirationDurationPassed: Boolean
	dynamic monitored maxInspTimePSVPassed: Boolean
	dynamic monitored minExpTimePSVPassed: Boolean
	dynamic monitored apneaLagPassed: Boolean
	dynamic monitored maxInPausePassed: Boolean
	dynamic monitored maxRmTimePassed: Boolean
	dynamic monitored maxExpPausePassed: Boolean
	dynamic monitored triggerWindowDelayPassed: Boolean
	dynamic monitored pauseTimeoutPassed: Boolean

definitions:
	//*************************************************
	// RULE DEFINITIONS
	//*************************************************
		
	rule r_startupEnded =
			state := MAIN_REGION_SELFTEST

	rule r_ventOffRequested =
			stopVentilation := true

	rule r_ventOffPCV =
		par
			state := MAIN_REGION_VENTILATIONOFF
			stopVentilation := false
			apneaBackupMode := false
		endpar
		
	rule r_ventOffPSV =
		par
			state := MAIN_REGION_VENTILATIONOFF
			stopVentilation := false
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
		endpar
		
	rule r_PCVinspOValve =
		par
			r_PCVinsp[]
			oValve := CLOSED
		endpar
	
	rule r_PCVinspApnea =
		par
			r_PCVinspOValve[]
			apneaAlarm := true
			apneaBackupMode := true
		endpar
		
	rule r_PSVinsp =
		par
			state := MAIN_REGION_PSV_R1_INSPIRATION
			iValve := OPEN
		endpar
		
	rule r_PSVinspOValve =
		par
			r_PSVinsp[]
			oValve := CLOSED
		endpar

	rule r_PCVexp =
		par
			state := MAIN_REGION_PCV_R1_EXPIRATION
			oValve := OPEN
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
		endpar

/*	rule r_PSVexpIValve =
		if state = MAIN_REGION_PSV_R1_RM or state=MAIN_REGION_PSV_R1_INSPIRATION then
		//or (state = MAIN_REGION_PSV_R1_INSPIRATION and not(flowDropPSV or maxInspTimePSVPassed) and pawGTMaxPinsp)
		//or (state = MAIN_REGION_PSV_R1_INSPIRATION and (flowDropPSV or maxInspTimePSVPassed) and not(inPause) and not(rm)) then
		par
			r_PSVexp[]
			iValve := CLOSED
		endpar
		
		else
		par
			r_PSVexp[]
			iValve := CLOSED
			apneaBackupMode := false
		endpar
		endif*/
		 
		 
		
	rule r_PSVexpIValve =
		par
			r_PSVexp[]
			iValve := CLOSED
		endpar
	
	rule r_PSVexpIValveFromPCV =
		par
			r_PSVexp[]
			iValve := CLOSED
			apneaBackupMode := false
		endpar
		 
	rule r_PCVinPause =
		par
			state := MAIN_REGION_PCV_R1_INSPIRATORYPAUSE
			iValve := CLOSED
		endpar
	
	rule r_PCVrm =
			state := MAIN_REGION_PCV_R1_RM

	rule r_PCVexpPause =
		par
			state := MAIN_REGION_PCV_R1_EXPIRATORYPAUSE
			oValve := CLOSED
		endpar
		
	/*rule r_PSVinPause =
		if state = MAIN_REGION_PSV_R1_INSPIRATION then
		par
			state := MAIN_REGION_PSV_R1_INSPIRATORYPAUSE
			iValve := CLOSED
		//	apneaBackupMode := false
		endpar
		
		else 
		par
			state := MAIN_REGION_PSV_R1_INSPIRATORYPAUSE
			iValve := CLOSED
			apneaBackupMode := false
		endpar
		endif*/
		
	rule r_PSVinPause =
		par
			state := MAIN_REGION_PSV_R1_INSPIRATORYPAUSE
			iValve := CLOSED
		endpar
		
	rule r_PSVinPauseFromPCV = 
		par
			state := MAIN_REGION_PSV_R1_INSPIRATORYPAUSE
			iValve := CLOSED
			apneaBackupMode := false
		endpar
		
/*	rule r_PSVrm =
	if state = MAIN_REGION_PSV_R1_INSPIRATION then
		state := MAIN_REGION_PSV_R1_RM		
	else
		par
			state := MAIN_REGION_PSV_R1_RM
			apneaBackupMode := false
		endpar	
	endif*/
	rule r_PSVrm =
		state := MAIN_REGION_PSV_R1_RM
		
	rule r_PSVrmFromPCV =
		par
			state := MAIN_REGION_PSV_R1_RM
			apneaBackupMode := false
		endpar	
	
	rule r_PSVexpPause =
		par
			state := MAIN_REGION_PSV_R1_EXPIRATORYPAUSE
			oValve := CLOSED
		endpar
	
	rule r_apneaAlarmOFF =
			apneaAlarm := false


	//*************************************************
	// Property Verification
	//*************************************************

	//safety: valves are never both open or both closed at the same time
	//OLD, not true anymore with the introduction of pauses
	//CTLSPEC not ef(iValve=CLOSED and oValve=CLOSED)
	//CTLSPEC not ef(iValve=OPEN and oValve=OPEN)
	
	//negation of the old ones (excluding the case where the ventilator is turned off)
	CTLSPEC ag(not turnOFFbtn) implies ef(iValve=CLOSED and oValve=CLOSED)
	CTLSPEC ag(not turnOFFbtn) implies ef(iValve=OPEN and oValve=OPEN)
	
	//safety properties still true excluding pause states
	CTLSPEC not ef(iValve=CLOSED and oValve=CLOSED and state!=MAIN_REGION_PCV_R1_INSPIRATORYPAUSE and state!=MAIN_REGION_PCV_R1_EXPIRATORYPAUSE and state!=MAIN_REGION_PSV_R1_INSPIRATORYPAUSE and state!=MAIN_REGION_PSV_R1_EXPIRATORYPAUSE)
	CTLSPEC not ef(iValve=OPEN and oValve=OPEN and state!=MAIN_REGION_PCV_R1_INSPIRATORYPAUSE and state!=MAIN_REGION_PCV_R1_EXPIRATORYPAUSE and state!=MAIN_REGION_PSV_R1_INSPIRATORYPAUSE and state!=MAIN_REGION_PSV_R1_EXPIRATORYPAUSE)
	
	//both valves closed only happens during pauses
	CTLSPEC ag((iValve=CLOSED and oValve=CLOSED) implies (state=MAIN_REGION_PCV_R1_INSPIRATORYPAUSE or state=MAIN_REGION_PCV_R1_EXPIRATORYPAUSE or state=MAIN_REGION_PSV_R1_INSPIRATORYPAUSE or state=MAIN_REGION_PSV_R1_EXPIRATORYPAUSE))
	
	//when ventilation is off, out valve is open and in valve is closed
	CTLSPEC ag(state=MAIN_REGION_VENTILATIONOFF implies (iValve=CLOSED and oValve=OPEN))

	//no deadlock: it is always possible to get back in the MAIN_REGION_VENTILATIONOFF state
	//only exception not considered: turn-off button is pressed
	CTLSPEC ag(not turnOFFbtn) implies ag(ef(state=MAIN_REGION_VENTILATIONOFF))

	//once turned off, the state doesn't change anymore
	CTLSPEC ag(state=OFF implies ag(state=OFF))
	

	//*************************************************
	// MAIN Rule
	//*************************************************
	main rule r_Main =
	

	//if the button is pressed, shut down the ventilator
	if turnOFFbtn then r_turnOff[]
	
		//if stop of the ventilation is requested and current state is an expiration
		//go to state MAIN_REGION_VENTILATIONOFF immediately
		else if stopRequested and state=MAIN_REGION_PCV_R1_EXPIRATION then r_ventOffPCV[]
		else if stopRequested and state=MAIN_REGION_PSV_R1_EXPIRATION then r_ventOffPSV[]
		
		else

		par
		
			
		//turn off apnea alarm if requested
		if apneaAlarm then
			if turnOffApneaAlarm and (state=MAIN_REGION_PCV_R1_INSPIRATION or state=MAIN_REGION_PCV_R1_EXPIRATION) then r_apneaAlarmOFF[] endif
			endif
		
		//if ventilation stop is requested and ventilation is on, 
		//store the stop request in controlled function stopVentilation
		if stopRequested and state!=MAIN_REGION_STARTUP and state!=MAIN_REGION_SELFTEST and state!=MAIN_REGION_VENTILATIONOFF then r_ventOffRequested[] endif

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
				if respirationMode = PCV	then r_PCVinspOValve[]
					else if respirationMode = PSV	then r_PSVinspOValve[] endif
				endif
			endif
		endif

		//transition from inspiration to pause/rm/expiration, mode change if requested
		if state = MAIN_REGION_PCV_R1_INSPIRATION then
			if inspirationDurationPassed	then
				if respirationMode = PSV then 
					if (inPause) then r_PSVinPauseFromPCV[]
						else if rm then r_PSVrmFromPCV[]
							else r_PSVexpIValveFromPCV[]
						endif
					endif
					else if (inPause) then r_PCVinPause[]
						else if rm then r_PCVrm[]
							else r_PCVexpIValve[]
						endif
					endif
				endif
				else if pawGTMaxPinsp then r_PCVexpIValve[]
				endif
			endif
		endif
		
		//transition from inspiratory pause to expiration
		if state = MAIN_REGION_PCV_R1_INSPIRATORYPAUSE then
			if maxInPausePassed then r_PCVexp[]
			 else if not inPause then r_PCVexp[] endif
			endif
		endif
		
		//transition from rm to expiration
		if state= MAIN_REGION_PCV_R1_RM then
			if (maxRmTimePassed or rm) then r_PCVexpIValve[]
			endif
		endif

		//transition from expiration to pause/inspiration, ventilation turned off if requested
		if state = MAIN_REGION_PCV_R1_EXPIRATION	then
			if expirationDurationPassed then 
				if (expPause) then r_PCVexpPause[]
					else r_PCVinspOValve[] 
				endif
			else if triggerWindowDelayPassed then
				if dropPAW_ITS_PCV then r_PCVinspOValve[]		
			else if stopVentilation then r_ventOffPCV[]			
			endif 
			endif
			else if stopVentilation then r_ventOffPCV[]	
			endif
			endif
			endif
		endif
		
		//transition from expiratory pause to inspiration
		if state = MAIN_REGION_PCV_R1_EXPIRATORYPAUSE then
			if maxExpPausePassed then r_PCVinsp[]
			else if not expPause then r_PCVinsp[] endif
			endif
		endif	
			
		//transition from inspiration to pause/rm/expiration
		if state = MAIN_REGION_PSV_R1_INSPIRATION	then
			if (flowDropPSV or maxInspTimePSVPassed) then
				if (inPause) then r_PSVinPause[]
					else if rm then r_PSVrm[]
						else r_PSVexpIValve[]
					endif
				endif
			else if pawGTMaxPinsp then r_PSVexpIValve[]
			endif
			endif
		endif
		
		//transition from inspiratory pause to expiration
		if state = MAIN_REGION_PSV_R1_INSPIRATORYPAUSE then
			if maxInPausePassed then r_PSVexp[]
			else if not inPause then r_PSVexp[] endif
			endif
		endif
				
		//transition from rm to expiration
		if state= MAIN_REGION_PSV_R1_RM then
			if (maxRmTimePassed or rm) then r_PSVexpIValve[]
			endif
		endif
		
		//transition from expiration to pause/inspiration, mode change if requested (or in case of apnea),
		// ventilation turned off if requested
			if state = MAIN_REGION_PSV_R1_EXPIRATION	then
			if triggerWindowDelayPassed then
				if dropPAW_ITS_PSV then r_PSVinspOValve[]
				endif
			else if apneaLagPassed then r_PCVinspApnea[]
			else if (minExpTimePSVPassed and respirationMode = PSV and expPause) then r_PSVexpPause[] 
			else if stopVentilation then r_ventOffPSV[]
			else if (minExpTimePSVPassed and respirationMode = PCV) then r_PCVinspOValve[] 
		endif
		endif
		endif
		endif
	endif endif

		//transition from expiratory pause to inspiration
		if state = MAIN_REGION_PSV_R1_EXPIRATORYPAUSE then
			if maxExpPausePassed then  r_PSVinsp[]
			 else if not expPause then r_PSVinsp[] endif
			endif
		endif	
			
	endpar
	endif
	endif
	endif

default init s0:
//controlled functions
	function state = MAIN_REGION_STARTUP
	function iValve = CLOSED
	function oValve = OPEN
	function stopVentilation = false
	function apneaAlarm = false
	function apneaBackupMode = false