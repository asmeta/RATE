//
//
// specifica digeribile da ATGT
// con:
// 1 - time come dominio di interi da 0 a 200 o cose simili
// 2 - time come controllato incrementato ad ogni passo se minore del massimo
// 3 - duration fissata con statiche una per ogni timer
// 4 - timer come enumerativi 
//

asm Ventilatore4SimpleTimeLtdY

import ../StandardLibrary
import ../CTLlibrary
import ../LTLlibrary

signature:

	// TIMER
	enum  domain Timer = {TIMER_INSPIRATION_DURATION_MS, TIMER_EXPIRATION_DURATION_MS, TIMER_MAX_INSP_TIME_PSV, TIMER_MIN_EXP_TIME_PSV, TIMER_APNEALAG , TIMER_MAX_INS_PAUSE, TIMER_MAX_RM_TIME, 
		TIMER_MAX_EXP_PAUSE, TIMER_TRIGGERWINDOWDELAY_MS, TIMER_MIN_INSP_TIME_MS}
		//TIMER_P_TO removed
		//TIMER_MIN_INSP_TIME_MS new
//enum  domain Timer = { TIMER_INSP_DUR, TIMER_EXP_DUR, TIMERMAX_INSP_PSV, TIMER_MIN_EXP_PSV, TIMER_APN , TIMER_MAXIN_PAUSE, TIMER_MAX_RM, TIMER_MAX_EXP_PAUSE, TIMER_WD, TIMER_P_TO}


	domain MyTime subsetof Integer


	//*************************************************
	// DOMAINS
	//*************************************************
	enum domain States = {MAIN_REGION_STARTUP | MAIN_REGION_SELFTEST | MAIN_REGION_VENTILATIONOFF | MAIN_REGION_PCV_R1_INSPIRATION | MAIN_REGION_PCV_R1_EXPIRATION | MAIN_REGION_PSV_R1_INSPIRATION  | MAIN_REGION_PSV_R1_EXPIRATION  | MAIN_REGION_PCV_R1_INSPIRATORYPAUSE | MAIN_REGION_PCV_R1_EXPIRATORYPAUSE | MAIN_REGION_PSV_R1_INSPIRATORYPAUSE| MAIN_REGION_PSV_R1_EXPIRATORYPAUSE | MAIN_REGION_PCV_R1_RM | MAIN_REGION_PSV_R1_RM | OFF}
	//enum domain States = {STARTUP | SELFTEST | VENTILATIONOFF | PCV_INSP | PCV_EXP | PSV_INSP | PSV_EXP | PCV_IN_PAUSE | PCV_EXP_PAUSE | PSV_IN_PAUSE| PSV_EXP_PAUSE | PCV_RM | PSV_RM | OFF}

	enum domain Modes = {PCV | PSV}

	enum domain ValveStatus = {OPEN | CLOSED}
	
	enum domain YesOrNo = {YES | NO}

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
	dynamic monitored exp_pause: Boolean //expPause
	dynamic monitored ins_pause: Boolean //inPause
	dynamic monitored rm_request: Boolean //rm
	dynamic monitored turnOffApneaAlarm: Boolean
	dynamic monitored pawGTMaxPinsp: Boolean
	dynamic monitored dropPAW_ITS_PCV: Boolean
	dynamic monitored dropPAW_ITS_PSV: Boolean
	
	dynamic controlled time: MyTime
	
	dynamic controlled stopVentilationRequested: Boolean //stopVentilation
	dynamic controlled state: States
	dynamic controlled iValve: ValveStatus
	dynamic controlled oValve: ValveStatus
	dynamic controlled apneaAlarm: Boolean
	dynamic controlled apnea_backup_mode: Boolean //apneaBackupMode
	//dynamic controlled pauseOff: Boolean 
	

	// starting time taken from the local clock
	controlled start: Timer-> MyTime
	// duration in sec of the TIMER starting from its start
	// controlled duration: Timer -> MyTime
	
	//get the current time for the TIMER
	//derived currentTime : Timer-> MyTime
	// is the TIMER expired?
	//derived expired: Timer -> Boolean
	
	static durationTIMER_INSPIRATION_DURATION_MS : MyTime
	static durationTIMER_EXPIRATION_DURATION_MS: MyTime
	static durationTIMER_MAX_INSP_TIME_PSV: MyTime
	static durationTIMER_MIN_EXP_TIME_PSV: MyTime
	static durationTIMER_APNEALAG: MyTime
	static durationTIMER_MAX_INS_PAUSE: MyTime
	static durationTIMER_MAX_RM_TIME: MyTime
	static durationTIMER_MAX_EXP_PAUSE : MyTime
	static durationTIMER_TRIGGERWINDOWDELAY_MS : MyTime
	//static durationTIMER_P_TO: MyTime
	static durationTIMER_MIN_INSP_TIME_MS: MyTime



	derived expiredTIMER_INSPIRATION_DURATION_MS : Boolean
	derived expiredTIMER_EXPIRATION_DURATION_MS: Boolean
	derived expiredTIMER_MAX_INSP_TIME_PSV: Boolean
	derived expiredTIMER_MIN_EXP_TIME_PSV: Boolean
	derived expiredTIMER_APNEALAG: Boolean
	derived expiredTIMER_MAX_INS_PAUSE: Boolean
	derived expiredTIMER_MAX_RM_TIME: Boolean
	derived expiredTIMER_MAX_EXP_PAUSE : Boolean
	derived expiredTIMER_TRIGGERWINDOWDELAY_MS : Boolean
	//derived expiredTIMER_P_TO: Boolean
	derived expiredTIMER_MIN_INSP_TIME_MS: Boolean
	
/*----------- Java time  as monitored function (experimental) --------------*/

	
														

definitions:

	domain MyTime = {0:600}
	
	function durationTIMER_INSPIRATION_DURATION_MS 	=  20 //settato dal dottore. valore indicativo per respiratory rate 10 e I:E 0,5
    	function durationTIMER_EXPIRATION_DURATION_MS								=  40 //settato dal dottore. valore indicativo
		function durationTIMER_MAX_INSP_TIME_PSV							=  70
		function durationTIMER_MIN_EXP_TIME_PSV								=  10	//half of the last_inspiration_time. Shall be in the interval [0.4 : 2] sec.
		function durationTIMER_APNEALAG 									=  100
		function durationTIMER_MAX_INS_PAUSE 								=  200//40
		function durationTIMER_MAX_RM_TIME									=  100
		function durationTIMER_MAX_EXP_PAUSE								=  200//60
		function durationTIMER_TRIGGERWINDOWDELAY_MS						=  10 // 0.7
		//function durationTIMER_P_TO								=  120 
		function durationTIMER_MIN_INSP_TIME_MS = 10 //0.3
	
	
	
	/*******************************************************/	
	//function currentTime($t in Timer) = time
														
	//function expired($t in Timer) = (time >= start($t) + duration($t))

	function expiredTIMER_INSPIRATION_DURATION_MS  = (time >= start(TIMER_INSPIRATION_DURATION_MS) + durationTIMER_INSPIRATION_DURATION_MS)
	function expiredTIMER_EXPIRATION_DURATION_MS = (time >= start(TIMER_EXPIRATION_DURATION_MS) + durationTIMER_EXPIRATION_DURATION_MS)
	function expiredTIMER_MAX_INSP_TIME_PSV = (time >= start(TIMER_MAX_INSP_TIME_PSV) + durationTIMER_MAX_INSP_TIME_PSV)
	function expiredTIMER_MIN_EXP_TIME_PSV = (time >= start(TIMER_MIN_EXP_TIME_PSV) + durationTIMER_MIN_EXP_TIME_PSV)
	function expiredTIMER_APNEALAG = (time >= start(TIMER_APNEALAG) + durationTIMER_APNEALAG)
	function expiredTIMER_MAX_INS_PAUSE = (time >= start(TIMER_MAX_INS_PAUSE) + durationTIMER_MAX_INS_PAUSE)
	function expiredTIMER_MAX_RM_TIME = (time >= start(TIMER_MAX_RM_TIME) + durationTIMER_MAX_RM_TIME)
	function expiredTIMER_MAX_EXP_PAUSE  = (time >= start(TIMER_MAX_EXP_PAUSE) + durationTIMER_MAX_EXP_PAUSE)
	function expiredTIMER_TRIGGERWINDOWDELAY_MS  = (time >= start(TIMER_TRIGGERWINDOWDELAY_MS) + durationTIMER_TRIGGERWINDOWDELAY_MS)
	//function expiredTIMER_P_TO  = (time >= start(TIMER_P_TO) + durationTIMER_P_TO)
	function expiredTIMER_MIN_INSP_TIME_MS  = (time >= start(TIMER_MIN_INSP_TIME_MS) + durationTIMER_MIN_INSP_TIME_MS)
	
	
	/*******************************************************/
	// restart the TIMER
	//macro rule r_reset_TIMER($t in Timer) =	start($t) := currentTime($t)					
	macro rule r_reset_TIMER($t in Timer) =	start($t) := time
														
    
    
	// change or set the duration of a TIMER
    //macro rule r_set_duration($t in Timer, $ms in MyTime) = 
    //	duration($t) := $ms

 

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
			apnea_backup_mode := false
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
		
	/*rule r_PCVinspPauseTimeout =
		par
			r_PCVinsp[]
			pauseOff := true
			//r_reset_TIMER[TIMER_P_TO]
		endpar*/
		
	rule r_PCVinspOValve =
		par
			r_PCVinsp[]
			oValve := CLOSED
		endpar
	
	rule r_PCVinspApnea =
		par
			r_PCVinspOValve[]
			apneaAlarm := true
			apnea_backup_mode := true
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
		
	/*rule r_PSVinspPauseTimeout =
		par
			r_PSVinsp[]
			pauseOff := true
			//r_reset_TIMER[TIMER_P_TO]
		endpar*/
		
	rule r_PCVexp =
		par
			state := MAIN_REGION_PCV_R1_EXPIRATION 
			oValve := OPEN
			r_reset_TIMER[TIMER_EXPIRATION_DURATION_MS]
			r_reset_TIMER[TIMER_TRIGGERWINDOWDELAY_MS]
		endpar

	/*rule r_PCVexp_pauseTimeout =
		par
			r_PCVexp[]
			pauseOff := true
			//r_reset_TIMER[TIMER_P_TO]
		endpar*/
			
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
			r_reset_TIMER[TIMER_APNEALAG]
			r_reset_TIMER[TIMER_TRIGGERWINDOWDELAY_MS]
		endpar

	/*rule r_PSVexp_pauseTimeout =
		par
			r_PSVexp[]
			pauseOff := true
			//r_reset_TIMER[TIMER_P_TO]
		endpar*/
			
/*	rule r_PSVexpIValve =
		if state = MAIN_REGION_PSV_R1_RM or state = MAIN_REGION_PSV_R1_INSPIRATION  then
		//or (state = MAIN_REGION_PSV_R1_INSPIRATION  and not(flowDropPSV or expiredTIMER_MAX_INSP_TIME_PSV) and pawGTMaxPinsp)
		//or (state = MAIN_REGION_PSV_R1_INSPIRATION  and (flowDropPSV or expiredTIMER_MAX_INSP_TIME_PSV) and not(ins_pause) and not(rm))) then
		par
			r_PSVexp[]
			iValve := CLOSED
		endpar
		
		else //QUANDO ENTRA QUA SI PIANTA
		par
			r_PSVexp[]
			iValve := CLOSED
			apnea_backup_mode := false
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
			apnea_backup_mode := false
		endpar
		 
	rule r_PCVins_pause =
		par
			state := MAIN_REGION_PCV_R1_INSPIRATORYPAUSE
			iValve := CLOSED
			r_reset_TIMER[TIMER_MAX_INS_PAUSE]
		endpar
	
	rule r_PCVrm =
		par
			state := MAIN_REGION_PCV_R1_RM
			r_reset_TIMER[TIMER_MAX_RM_TIME]
		endpar
		
	rule r_PCVexp_pause =
		par
			state := MAIN_REGION_PCV_R1_EXPIRATORYPAUSE
			oValve := CLOSED
			r_reset_TIMER[TIMER_MAX_EXP_PAUSE]
		endpar
		
	rule r_PSVins_pause =
		par
			state := MAIN_REGION_PSV_R1_INSPIRATORYPAUSE
			iValve := CLOSED
			r_reset_TIMER[TIMER_MAX_INS_PAUSE]
		endpar

		
	rule r_PSVins_pauseFromPCV = 
		par
			state := MAIN_REGION_PSV_R1_INSPIRATORYPAUSE
			iValve := CLOSED
			apnea_backup_mode := false
			r_reset_TIMER[TIMER_MAX_INS_PAUSE]
		endpar
		
	rule r_PSVrm =
		par
			state := MAIN_REGION_PSV_R1_RM
			r_reset_TIMER[TIMER_MAX_RM_TIME]
		endpar
	
	rule r_PSVrmFromPCV =
		par
			state := MAIN_REGION_PSV_R1_RM
			apnea_backup_mode := false
			r_reset_TIMER[TIMER_MAX_RM_TIME]
		endpar	
		
	rule r_PSVexp_pause =
		par
			state := MAIN_REGION_PSV_R1_EXPIRATORYPAUSE
			oValve := CLOSED
			r_reset_TIMER[TIMER_MAX_EXP_PAUSE]
		endpar
	
	rule r_apneaAlarmOFF =
			apneaAlarm := false
			
	/*rule r_enablePause =
			pauseOff := false*/

	//*************************************************
	// Property Verification
	//*************************************************

	//safety: valves are never both open or both closed at the same time
	//OLD, not true anymore with the introduction of pauses
	//CTLSPEC not ef(iValve=CLOSED and oValve=CLOSED)
	//CTLSPEC not ef(iValve=OPEN and oValve=OPEN)
	
	//negation of the old ones (excluding the case where the ventilator is turned off)
	CTLSPEC ag(not poweroff) implies ef(iValve=CLOSED and oValve=CLOSED)
	CTLSPEC ag(not poweroff) implies ef(iValve=OPEN and oValve=OPEN)
	
	//safety properties still true excluding pause states
	CTLSPEC not ef(iValve=CLOSED and oValve=CLOSED and state!=MAIN_REGION_PCV_R1_INSPIRATORYPAUSE and state!=MAIN_REGION_PCV_R1_EXPIRATORYPAUSE and state!=MAIN_REGION_PSV_R1_INSPIRATORYPAUSE and state!=MAIN_REGION_PSV_R1_EXPIRATORYPAUSE)
	CTLSPEC not ef(iValve=OPEN and oValve=OPEN and state!=MAIN_REGION_PCV_R1_INSPIRATORYPAUSE and state!=MAIN_REGION_PCV_R1_EXPIRATORYPAUSE and state!=MAIN_REGION_PSV_R1_INSPIRATORYPAUSE and state!=MAIN_REGION_PSV_R1_EXPIRATORYPAUSE)
	
	//both valves closed only happens during pauses
	CTLSPEC ag((iValve=CLOSED and oValve=CLOSED) implies (state=MAIN_REGION_PCV_R1_INSPIRATORYPAUSE or state=MAIN_REGION_PCV_R1_EXPIRATORYPAUSE or state=MAIN_REGION_PSV_R1_INSPIRATORYPAUSE or state=MAIN_REGION_PSV_R1_EXPIRATORYPAUSE))
	
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
	//both valves closed only happens during pauses
	LTLSPEC g((iValve=CLOSED and oValve=CLOSED) implies (state=MAIN_REGION_PCV_R1_INSPIRATORYPAUSE or state=MAIN_REGION_PCV_R1_EXPIRATORYPAUSE or state=MAIN_REGION_PSV_R1_INSPIRATORYPAUSE or state=MAIN_REGION_PSV_R1_EXPIRATORYPAUSE))
	//safety properties valves are never both open or both closed at the same time excluding pause states
	LTLSPEC not f(iValve=CLOSED and oValve=CLOSED and state!=MAIN_REGION_PCV_R1_INSPIRATORYPAUSE and state!=MAIN_REGION_PCV_R1_EXPIRATORYPAUSE and state!=MAIN_REGION_PSV_R1_INSPIRATORYPAUSE and state!=MAIN_REGION_PSV_R1_EXPIRATORYPAUSE)
	LTLSPEC not f(iValve=OPEN and oValve=OPEN and state!=MAIN_REGION_PCV_R1_INSPIRATORYPAUSE and state!=MAIN_REGION_PCV_R1_EXPIRATORYPAUSE and state!=MAIN_REGION_PSV_R1_INSPIRATORYPAUSE and state!=MAIN_REGION_PSV_R1_EXPIRATORYPAUSE)
	
	
	//*************************************************
	// MAIN Rule
	//*************************************************
	main rule r_Main =
	//if time < 600 then
		par 
			time:= time+1
		
			//if the button is pressed, shut down the ventilator
			if poweroff then r_turnOff[]
			
				//if stop of the ventilation is requested and current state is an expiration
				//go to state MAIN_REGION_VENTILATIONOFF immediately
			else if state=MAIN_REGION_PCV_R1_EXPIRATION and (stopVentilationRequested or stopVentilation)  then r_ventOffPCV[]
			else if state=MAIN_REGION_PSV_R1_EXPIRATION and (stopVentilationRequested or stopVentilation)  then r_ventOffPSV[] 
				
			else
		
				par
				//if (expiredTIMER_P_TO and state!=MAIN_REGION_PCV_R1_INSPIRATORYPAUSE and state!=MAIN_REGION_PSV_R1_INSPIRATORYPAUSE and state!=MAIN_REGION_PCV_R1_EXPIRATORYPAUSE and state!=MAIN_REGION_PSV_R1_EXPIRATORYPAUSE)
				
				/*if (state!=MAIN_REGION_PCV_R1_INSPIRATORYPAUSE and state!=MAIN_REGION_PSV_R1_INSPIRATORYPAUSE and state!=MAIN_REGION_PCV_R1_EXPIRATORYPAUSE and state!=MAIN_REGION_PSV_R1_EXPIRATORYPAUSE)
					then r_enablePause[] endif*/
				
				if apneaAlarm then
					if turnOffApneaAlarm = true and (state=MAIN_REGION_PCV_R1_INSPIRATION or state=MAIN_REGION_PCV_R1_EXPIRATION ) then r_apneaAlarmOFF[] endif
					endif
				
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
		
				//start ventilation, either pcv or psv
				if state = MAIN_REGION_VENTILATIONOFF then
					if startVentilation	then
						par
							if mode = PCV	then r_PCVinspOValve[]endif
							if mode = PSV	then r_PSVinspOValve[] endif
						endpar
					endif
				endif
		
				//transition from inspiration to pause/rm/expiration, mode change if requested
				if state = MAIN_REGION_PCV_R1_INSPIRATION then
					if expiredTIMER_INSPIRATION_DURATION_MS then
					par
						if mode = PSV then 
							if (ins_pause/*  and not pauseOff*/) then r_PSVins_pauseFromPCV[]
								else if rm_request then r_PSVrmFromPCV[]
										else r_PSVexpIValveFromPCV[]
									endif
								endif
						endif
						if mode = PCV then 
						 	if (ins_pause /*and not pauseOff*/) then r_PCVins_pause[]
								else if rm_request then r_PCVrm[]
									else r_PCVexpIValve[]
									endif
							endif
						endif
					endpar
					else if pawGTMaxPinsp then r_PCVexpIValve[]	endif
					endif
				endif
				
				if state = MAIN_REGION_PCV_R1_INSPIRATORYPAUSE then
					if expiredTIMER_MAX_INS_PAUSE then r_PCVexp[]
						else if not ins_pause then r_PCVexp[] endif
					endif
				endif
				
				if state= MAIN_REGION_PCV_R1_RM then
					if expiredTIMER_MAX_RM_TIME or rm_request then r_PCVexpIValve[]
					endif
				endif
		
				
				if state = MAIN_REGION_PCV_R1_EXPIRATION 	then
					if expiredTIMER_EXPIRATION_DURATION_MS then 
						if (exp_pause /* and not pauseOff*/) then r_PCVexp_pause[]
							else r_PCVinspOValve[] 
						endif
					else if expiredTIMER_TRIGGERWINDOWDELAY_MS then
						if dropPAW_ITS_PCV then r_PCVinspOValve[]		
						/* 	else if stopVentilationRequested then r_ventOffPCV[]			
							endif */
						endif
					/*else if stopVentilationRequested then r_ventOffPCV[]	
					endif*/
					endif
					endif
				endif
				
				if state = MAIN_REGION_PCV_R1_EXPIRATORYPAUSE then
					if expiredTIMER_MAX_EXP_PAUSE then r_PCVinsp[]
						else if not exp_pause then r_PCVinsp[] endif
					endif
				endif	
					
				//transition from inspiration to expiration, by time or by flow drop
				if state = MAIN_REGION_PSV_R1_INSPIRATION 	then
					if ((flowDropPSV and expiredTIMER_MIN_INSP_TIME_MS) or expiredTIMER_MAX_INSP_TIME_PSV) then
						if (ins_pause/*  and not pauseOff*/) then r_PSVins_pause[]
							else if rm_request then r_PSVrm[]
								else r_PSVexpIValve[]
							endif
						endif
					else if pawGTMaxPinsp then r_PSVexpIValve[]
					endif
					endif
				endif
				
				if state = MAIN_REGION_PSV_R1_INSPIRATORYPAUSE then
					if expiredTIMER_MAX_INS_PAUSE then r_PSVexp[]
						else if not ins_pause then r_PSVexp[] endif
					endif
				endif
						
				if state= MAIN_REGION_PSV_R1_RM then
					if (expiredTIMER_MAX_RM_TIME or rm_request) then r_PSVexpIValve[]
					endif
				endif
				
				//transition from expiration to inspiration, mode change if requested, ventilation turned off if requested
				if state = MAIN_REGION_PSV_R1_EXPIRATION 	then
					if (expiredTIMER_TRIGGERWINDOWDELAY_MS and dropPAW_ITS_PSV) then r_PSVinspOValve[]
					else if expiredTIMER_APNEALAG then r_PCVinspApnea[]
					else if (expiredTIMER_MIN_EXP_TIME_PSV and mode = PSV and exp_pause/* and not pauseOff*/) then r_PSVexp_pause[] 
					/*else if stopVentilationRequested then r_ventOffPSV[]*/
					else if (expiredTIMER_MIN_EXP_TIME_PSV and mode = PCV) then r_PCVinspOValve[] 
				endif
				endif
				endif
				endif
			endif /*endif*/
		
				if state = MAIN_REGION_PSV_R1_EXPIRATORYPAUSE then
					if expiredTIMER_MAX_EXP_PAUSE then r_PSVinsp[]
						else if not exp_pause then r_PSVinsp[] endif
					endif
				endif	
					
			endpar
			endif
			endif
			endif
		endpar
	//endif

default init s0:
//controlled functions
    function time = 0
	function state = MAIN_REGION_STARTUP
	function iValve = CLOSED
	function oValve = OPEN
	function stopVentilationRequested = false
	function apneaAlarm = false
	function apnea_backup_mode = false
	//function pauseOff = false
		
	function start($t in Timer) = 0
