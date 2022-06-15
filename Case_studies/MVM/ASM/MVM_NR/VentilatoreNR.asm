asm VentilatoreNR

import ../StandardLibrary

signature:

	enum  domain Timer = {TIMER_INSPIRATION_DURATION_MS, 
		TIMER_EXPIRATION_DURATION_MS, 
		TIMER_MAX_INSP_TIME_PSV, 
		TIMER_MIN_EXP_TIME_PSV, 
		TIMER_APNEALAG, 
		TIMER_MAX_INS_PAUSE, 
		TIMER_MAX_RM_TIME, 
		TIMER_MAX_EXP_PAUSE, 
		TIMER_TRIGGERWINDOWDELAY_MS, 
		TIMER_MIN_INSP_TIME_MS}
		
	domain MyTime subsetof Integer

	//*************************************************
	// DOMAINS
	//*************************************************
	enum domain States = {MAIN_REGION_STARTUP | MAIN_REGION_SELFTEST | MAIN_REGION_VENTILATIONOFF | MAIN_REGION_PCV_R1_INSPIRATION | MAIN_REGION_PCV_R1_EXPIRATION | MAIN_REGION_PSV_R1_INSPIRATION  | MAIN_REGION_PSV_R1_EXPIRATION  | MAIN_REGION_PCV_R1_INSPIRATORYPAUSE | MAIN_REGION_PCV_R1_EXPIRATORYPAUSE | MAIN_REGION_PSV_R1_INSPIRATORYPAUSE| MAIN_REGION_PSV_R1_EXPIRATORYPAUSE | MAIN_REGION_PCV_R1_RM | MAIN_REGION_PSV_R1_RM | OFF}
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
	dynamic monitored exp_pause: Boolean
	dynamic monitored ins_pause: Boolean
	dynamic monitored rm_request: Boolean
	dynamic monitored turnOffApneaAlarm: Boolean
	dynamic monitored pawGTMaxPinsp: Boolean
	dynamic monitored dropPAW_ITS_PCV: Boolean
	dynamic monitored dropPAW_ITS_PSV: Boolean
	
	dynamic controlled time: MyTime
	
	dynamic controlled stopVentilationRequested: Boolean
	dynamic controlled state: States
	dynamic controlled iValve: ValveStatus
	dynamic controlled oValve: ValveStatus
	dynamic controlled apneaAlarm: Boolean
	dynamic controlled apnea_backup_mode: Boolean	

	// starting time taken from the local clock
	controlled start: Timer-> MyTime
	
	static durationTIMER_INSPIRATION_DURATION_MS : MyTime
	static durationTIMER_EXPIRATION_DURATION_MS: MyTime
	static durationTIMER_MAX_INSP_TIME_PSV: MyTime
	static durationTIMER_MIN_EXP_TIME_PSV: MyTime
	static durationTIMER_APNEALAG: MyTime
	static durationTIMER_MAX_INS_PAUSE: MyTime
	static durationTIMER_MAX_RM_TIME: MyTime
	static durationTIMER_MAX_EXP_PAUSE : MyTime
	static durationTIMER_TRIGGERWINDOWDELAY_MS : MyTime
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
	derived expiredTIMER_MIN_INSP_TIME_MS: Boolean

definitions:

	domain MyTime = {0:600}
		
	function durationTIMER_INSPIRATION_DURATION_MS 	=  20 
	function durationTIMER_EXPIRATION_DURATION_MS	=  40
	function durationTIMER_MAX_INSP_TIME_PSV		=  70
	function durationTIMER_MIN_EXP_TIME_PSV			=  10
	function durationTIMER_APNEALAG 				=  100
	function durationTIMER_MAX_INS_PAUSE 			=  200
	function durationTIMER_MAX_RM_TIME				=  100
	function durationTIMER_MAX_EXP_PAUSE			=  200
	function durationTIMER_TRIGGERWINDOWDELAY_MS	=  10 
	function durationTIMER_MIN_INSP_TIME_MS 		=  10
	
	/*******************************************************/	
	function expiredTIMER_INSPIRATION_DURATION_MS  = (time >= start(TIMER_INSPIRATION_DURATION_MS) + durationTIMER_INSPIRATION_DURATION_MS)
	function expiredTIMER_EXPIRATION_DURATION_MS = (time >= start(TIMER_EXPIRATION_DURATION_MS) + durationTIMER_EXPIRATION_DURATION_MS)
	function expiredTIMER_MAX_INSP_TIME_PSV = (time >= start(TIMER_MAX_INSP_TIME_PSV) + durationTIMER_MAX_INSP_TIME_PSV)
	function expiredTIMER_MIN_EXP_TIME_PSV = (time >= start(TIMER_MIN_EXP_TIME_PSV) + durationTIMER_MIN_EXP_TIME_PSV)
	function expiredTIMER_APNEALAG = (time >= start(TIMER_APNEALAG) + durationTIMER_APNEALAG)
	function expiredTIMER_MAX_INS_PAUSE = (time >= start(TIMER_MAX_INS_PAUSE) + durationTIMER_MAX_INS_PAUSE)
	function expiredTIMER_MAX_RM_TIME = (time >= start(TIMER_MAX_RM_TIME) + durationTIMER_MAX_RM_TIME)
	function expiredTIMER_MAX_EXP_PAUSE  = (time >= start(TIMER_MAX_EXP_PAUSE) + durationTIMER_MAX_EXP_PAUSE)
	function expiredTIMER_TRIGGERWINDOWDELAY_MS  = (time >= start(TIMER_TRIGGERWINDOWDELAY_MS) + durationTIMER_TRIGGERWINDOWDELAY_MS)
	function expiredTIMER_MIN_INSP_TIME_MS  = (time >= start(TIMER_MIN_INSP_TIME_MS) + durationTIMER_MIN_INSP_TIME_MS)
	
	/*******************************************************/
	// restart the TIMER				
	macro rule r_reset_TIMER($t in Timer) =	start($t) := time

	//*************************************************
	// RULE DEFINITIONS
	//*************************************************
	
	rule r_inspiration =
		par
			iValve := OPEN
			oValve := CLOSED
		endpar		
		
	rule r_expiration =
		par
			iValve := CLOSED
			oValve := OPEN
		endpar
		
	rule r_expiratoryPause =
		par
			oValve := CLOSED
			r_reset_TIMER[TIMER_MAX_EXP_PAUSE]
		endpar
		
	rule r_inspiratoryPause =
		par
			iValve := CLOSED
			r_reset_TIMER[TIMER_MAX_INS_PAUSE]
		endpar
	
	rule r_ventilationOff = 
		par
			state := MAIN_REGION_VENTILATIONOFF
			stopVentilationRequested := false
		endpar

	rule r_ventOffPCV =
		par
			r_ventilationOff[]
			apnea_backup_mode := false
		endpar
		
	rule r_ventOffPSV =
		r_ventilationOff[]
		
	rule r_exp_pause =
		par
			if (mode = PCV) then state := MAIN_REGION_PCV_R1_EXPIRATORYPAUSE
			else state := MAIN_REGION_PSV_R1_EXPIRATORYPAUSE endif
			r_expiratoryPause[]
		endpar

	rule r_ins_pause =
		par
			if (mode = PCV) then state := MAIN_REGION_PCV_R1_INSPIRATORYPAUSE
			else state := MAIN_REGION_PSV_R1_INSPIRATORYPAUSE endif
			r_inspiratoryPause[]
		endpar	
		
	rule r_turnOff =
		par
			r_expiration[]
			state := OFF
		endpar	

	rule r_PSVins_pauseFromPCV = 
		par
			r_ins_pause[]
			r_inspiratoryPause[]
		endpar
		
	rule r_PCVinsp =
		par
			r_inspiration[]
			state := MAIN_REGION_PCV_R1_INSPIRATION
			r_reset_TIMER[TIMER_INSPIRATION_DURATION_MS]
		endpar	
		
	rule r_PCVinspApnea =
		par
			r_PCVinsp[]
			apneaAlarm := true
			apnea_backup_mode := true
		endpar	
		
	rule r_PSVinsp =
		par
			r_inspiration[]
			state := MAIN_REGION_PSV_R1_INSPIRATION 
			r_reset_TIMER[TIMER_MAX_INSP_TIME_PSV]
			r_reset_TIMER[TIMER_MIN_INSP_TIME_MS]
		endpar	
		
	rule r_PCVexp =
		par
			r_expiration[]
			state := MAIN_REGION_PCV_R1_EXPIRATION 
			r_reset_TIMER[TIMER_EXPIRATION_DURATION_MS]
			r_reset_TIMER[TIMER_TRIGGERWINDOWDELAY_MS]
		endpar

	rule r_PSVexp =
		par
			r_expiration[]
			state := MAIN_REGION_PSV_R1_EXPIRATION 
			r_reset_TIMER[TIMER_MIN_EXP_TIME_PSV]
			r_reset_TIMER[TIMER_APNEALAG]
			r_reset_TIMER[TIMER_TRIGGERWINDOWDELAY_MS]
		endpar	
	
	rule r_PSVexpIValveFromPCV =
		par
			r_PSVexp[]
			apnea_backup_mode := false
		endpar
		
	rule r_rm =
		par
			if mode = PCV then state := MAIN_REGION_PCV_R1_RM
			else state := MAIN_REGION_PSV_R1_RM endif
			r_reset_TIMER[TIMER_MAX_RM_TIME]
		endpar

	rule r_PSVrmFromPCV =
		par
			r_rm[]
			apnea_backup_mode := false
		endpar

	rule r_apneaAlarmOFF =
			apneaAlarm := false
	
	//*************************************************
	// MAIN Rule
	//*************************************************
	main rule r_Main =
		par 
			time:= time+1
		
			if poweroff then r_turnOff[]
			else 
				// request of stop the ventilation
				if (stopVentilationRequested or stopVentilation) then
					if (state=MAIN_REGION_PCV_R1_EXPIRATION) then r_ventOffPCV[] 
					else
						if (state=MAIN_REGION_PSV_R1_EXPIRATION) then r_ventOffPSV[] endif
					endif
				else
					par
						// resetting the apnea alarm
						if apneaAlarm and turnOffApneaAlarm then
							if (state=MAIN_REGION_PCV_R1_INSPIRATION or state=MAIN_REGION_PCV_R1_EXPIRATION) then r_apneaAlarmOFF[] endif
						endif

						// stop the ventilation, only during a limited set of states
						if stopVentilation then
							if state = MAIN_REGION_PCV_R1_INSPIRATION or state = MAIN_REGION_PCV_R1_EXPIRATION or state = MAIN_REGION_PSV_R1_INSPIRATION or
								state = MAIN_REGION_PCV_R1_INSPIRATORYPAUSE or state = MAIN_REGION_PCV_R1_EXPIRATORYPAUSE or state = MAIN_REGION_PSV_R1_INSPIRATORYPAUSE
								or state = MAIN_REGION_PSV_R1_EXPIRATORYPAUSE or state = MAIN_REGION_PCV_R1_RM or state = MAIN_REGION_PSV_R1_RM then stopVentilationRequested := true endif
						endif

						// startup ended
						if startupEnded then
							if state = MAIN_REGION_STARTUP then state := MAIN_REGION_SELFTEST endif
						endif
						
						// selftest ended
						if selfTestPassed or resume then
							if state = MAIN_REGION_SELFTEST then state := MAIN_REGION_VENTILATIONOFF endif
						endif
						
						// start ventilation in PCV
						if startVentilation and mode = PCV then
							if state = MAIN_REGION_VENTILATIONOFF then r_PCVinsp[] endif
						endif
						
						// start ventilation in PSV
						if startVentilation and mode = PSV then
							if state = MAIN_REGION_VENTILATIONOFF then r_PSVinsp[] endif
						endif
						
						// exit from the expiratory pause
						if expiredTIMER_MAX_EXP_PAUSE or not exp_pause then
							par
								if state = MAIN_REGION_PSV_R1_EXPIRATORYPAUSE then r_PSVinsp[] endif
								if state = MAIN_REGION_PCV_R1_EXPIRATORYPAUSE then r_PCVinsp[] endif
							endpar
						endif
						
						// exit from the inspiratory pause
						if expiredTIMER_MAX_INS_PAUSE or not ins_pause then
							par
								if state = MAIN_REGION_PSV_R1_INSPIRATORYPAUSE then r_PSVexp[] endif
								if state = MAIN_REGION_PCV_R1_INSPIRATORYPAUSE then r_PCVexp[] endif
							endpar
						endif
						
						// exit from RM
						if expiredTIMER_MAX_RM_TIME or rm_request then
							par
								if state = MAIN_REGION_PCV_R1_RM then r_PCVexp[] endif
								if state = MAIN_REGION_PSV_R1_RM then r_PSVexp[] endif
							endpar
						endif
						
						// PCV expiration
						if expiredTIMER_EXPIRATION_DURATION_MS and exp_pause and state = MAIN_REGION_PCV_R1_EXPIRATION then r_exp_pause[] endif
						if ((expiredTIMER_EXPIRATION_DURATION_MS and not exp_pause) 
							or (not expiredTIMER_EXPIRATION_DURATION_MS and expiredTIMER_TRIGGERWINDOWDELAY_MS and dropPAW_ITS_PCV)) and state = MAIN_REGION_PCV_R1_EXPIRATION then 
							r_PCVinsp[] endif
						
						// PCV inspiration
						if ins_pause and mode = PSV and expiredTIMER_INSPIRATION_DURATION_MS and state = MAIN_REGION_PCV_R1_INSPIRATION then r_PSVins_pauseFromPCV[] endif
						if rm_request and not ins_pause and mode = PSV and expiredTIMER_INSPIRATION_DURATION_MS and state = MAIN_REGION_PCV_R1_INSPIRATION then r_PSVrmFromPCV[] endif
						if not rm_request and not ins_pause and mode = PSV and expiredTIMER_INSPIRATION_DURATION_MS and state = MAIN_REGION_PCV_R1_INSPIRATION then r_PSVexpIValveFromPCV[] endif
						if ins_pause and mode = PCV and expiredTIMER_INSPIRATION_DURATION_MS and state = MAIN_REGION_PCV_R1_INSPIRATION then r_ins_pause[] endif
						if rm_request and not ins_pause and mode = PCV and expiredTIMER_INSPIRATION_DURATION_MS and state = MAIN_REGION_PCV_R1_INSPIRATION then r_rm[] endif
						if (not rm_request and not ins_pause and mode = PCV and expiredTIMER_INSPIRATION_DURATION_MS and state = MAIN_REGION_PCV_R1_INSPIRATION) 
							or (pawGTMaxPinsp and not expiredTIMER_INSPIRATION_DURATION_MS and state = MAIN_REGION_PCV_R1_INSPIRATION) then r_PCVexp[] endif
	
						// PSV inspiration
						if ins_pause and ((flowDropPSV and expiredTIMER_MIN_INSP_TIME_MS) or expiredTIMER_MAX_INSP_TIME_PSV) and state = MAIN_REGION_PSV_R1_INSPIRATION then r_ins_pause[] endif
						if rm_request and not ins_pause and ((flowDropPSV and expiredTIMER_MIN_INSP_TIME_MS) or expiredTIMER_MAX_INSP_TIME_PSV) and state = MAIN_REGION_PSV_R1_INSPIRATION then 
							r_rm[] endif
						if (not rm_request and not ins_pause and ((flowDropPSV and expiredTIMER_MIN_INSP_TIME_MS) or expiredTIMER_MAX_INSP_TIME_PSV) and state = MAIN_REGION_PSV_R1_INSPIRATION) 
							or (pawGTMaxPinsp and not ((flowDropPSV and expiredTIMER_MIN_INSP_TIME_MS) or expiredTIMER_MAX_INSP_TIME_PSV) and state = MAIN_REGION_PSV_R1_INSPIRATION) then 
							r_PSVexp[] endif
				
						// PSV expiration
						if (expiredTIMER_TRIGGERWINDOWDELAY_MS and dropPAW_ITS_PSV) and state = MAIN_REGION_PSV_R1_EXPIRATION then r_PSVinsp[] endif
						if expiredTIMER_APNEALAG and not (expiredTIMER_TRIGGERWINDOWDELAY_MS and dropPAW_ITS_PSV) and state = MAIN_REGION_PSV_R1_EXPIRATION then r_PCVinspApnea[] endif
						if (expiredTIMER_MIN_EXP_TIME_PSV and mode = PSV and exp_pause) and not expiredTIMER_APNEALAG and not (expiredTIMER_TRIGGERWINDOWDELAY_MS and dropPAW_ITS_PSV) 
							and state = MAIN_REGION_PSV_R1_EXPIRATION then r_exp_pause[] endif
						if (expiredTIMER_MIN_EXP_TIME_PSV and mode = PCV) and not expiredTIMER_APNEALAG and not (expiredTIMER_TRIGGERWINDOWDELAY_MS and dropPAW_ITS_PSV) 
							and state = MAIN_REGION_PSV_R1_EXPIRATION then r_PCVinsp[] endif				
					endpar
				endif
			endif
		endpar
		
default init s0:
	// controlled functions
    function time = 0
	function state = MAIN_REGION_STARTUP
	function iValve = CLOSED
	function oValve = OPEN
	function stopVentilationRequested = false
	function apneaAlarm = false
	function apnea_backup_mode = false		
	function start($t in Timer) = 0
