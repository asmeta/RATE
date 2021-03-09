asm TrafficLight_4
// ANGELO
 
import ../StandardLibrary
import ../CTLlibrary

signature:
	// DOMAINS
	enum domain ControllerStatus = {CONTR_OFF | STANDBY | OPERATE}
	enum domain ControllerSubStatusOperate = {BLOCKED_A | RELEASE_A | RELEASED_A | BLOCKED_B | RELEASE_B | RELEASED_B}
	enum domain ControllerTransition = {TURN_ON | TURN_OFF | OPERATE_T | STANDBY_T | SAFE_PERIOD}
	enum domain Lights = {ALL_OFF | RED | BLINK_YELLOW | GREEN | YELLOW}
	enum domain LightTransition = {RELEASE_PERIOD | PREPARE_PERIOD}
	enum domain LightStatus = {OFF | ATTENTION | BLOCKED | RELEASED | PREPARE_BLOCK}
	// FUNCTIONS
	controlled statusC: ControllerStatus
	controlled statusCOperate : ControllerSubStatusOperate
	monitored transitionC: ControllerTransition
	// lights
	controlled lightsA: Lights // traffic light A
	controlled lightsB: Lights // traffic light B
	controlled statusA: LightStatus
	controlled statusB: LightStatus	
	monitored transitionA: LightTransition 
	monitored transitionB: LightTransition 


definitions:
	// traffic light
	// in event li traduco con regole ogni evento in una regola 
	// entry action con un update opportuno
	// switchOn	
	rule r_raise_switchOn($s in LightStatus, $l in Lights) =
		if $s = OFF then 
			par 
				$s := ATTENTION
				//entryAttention
				$l:= BLINK_YELLOW
			endpar		
		endif 
	// switchOff	
	rule r_raise_switchOff($s in LightStatus, $l in Lights) =
		if $s = ATTENTION or  $s = BLOCKED or $s = RELEASED or $s = PREPARE_BLOCK then 
			par 
				$s := OFF
				//entryOFF
				$l:= ALL_OFF
			endpar		
		endif 
	// attention
	rule r_raise_attention($s in LightStatus, $l in Lights) =
	if $s = BLOCKED then 
		par 
			$s := ATTENTION
			//entryAttention
			$l:= BLINK_YELLOW
		endpar		
	endif 

	// eventi che il controllore subisce (raised by the traffic light)
	rule r_raise_blocked =  
		par
			if  statusCOperate = RELEASED_B then statusCOperate := BLOCKED_A endif
			if  statusCOperate = RELEASED_A then statusCOperate := BLOCKED_B endif
		endpar	
	
	rule r_raise_released = 
		par
			if  statusCOperate = RELEASE_A then statusCOperate := RELEASED_A endif
			if  statusCOperate = RELEASE_B then statusCOperate := RELEASED_B endif
		endpar	



	// block
	rule r_raise_block($s in LightStatus, $l in Lights) =
	if $s = ATTENTION then 
		par 
			$s := BLOCKED
			//entryBlocked
			$l:= RED
			// raise the event 
			r_raise_blocked[]
		endpar		
	endif 
	// release
	rule r_raise_release($s in LightStatus, $l in Lights) =
	if $s = BLOCKED then 
		par 
			$s := RELEASED
			//entryRELEASED
			$l:= GREEN
		endpar		
	endif 
	// le transizioni interne invece le metto come regole a sè
	rule r_TrafficLightA($l in Lights, $s in LightStatus) =
		par
			if $s = RELEASED then if  transitionA = RELEASE_PERIOD then par
					$s := PREPARE_BLOCK
					// entry prepare block
					$l:= YELLOW 
					// raise the event (moved from entry to exit)
					r_raise_released[]					
					endpar endif endif
			if $s = PREPARE_BLOCK then if   transitionA = PREPARE_PERIOD then par
					$s := BLOCKED
					// entry blocked
					$l:= RED
					// raise the event 
					r_raise_blocked[]  endpar endif endif
		endpar
	rule r_TrafficLightB($l in Lights, $s in LightStatus) =
		par
			if $s = RELEASED then if  transitionB = RELEASE_PERIOD then par
					$s := PREPARE_BLOCK
					// entry prepare block
					$l:= YELLOW 
					// raise the event (moved from entry to exit)
					r_raise_released[]
					endpar endif endif					
			if $s = PREPARE_BLOCK then if  transitionB = PREPARE_PERIOD then par
					$s := BLOCKED
					// entry blocked
					$l:= RED
					// raise the event 
					r_raise_blocked[]  endpar endif endif
		endpar
	// controller
	// 	 in event invece sono delle monitorate, quindi 	
	rule r_Controller =
	par
	// OFF -> STANDBY
	if statusC = CONTR_OFF and transitionC = TURN_ON then
		par 
			statusC := STANDBY
			// exit
			r_raise_switchOn[statusA,lightsA]
			r_raise_switchOn[statusB,lightsB]
			// entry
			r_raise_attention[statusA,lightsA]
			r_raise_attention[statusB,lightsB]
		endpar endif
	// STANDBY -> OPERATE	 
	if statusC = STANDBY and transitionC = OPERATE_T then
		par
			statusC := OPERATE
			statusCOperate := BLOCKED_A
			r_raise_block[statusA,lightsA]
			r_raise_block[statusB,lightsB]
		endpar endif
	// BLOCKED_A -> release A
	if statusC = OPERATE and statusCOperate = BLOCKED_A and transitionC = SAFE_PERIOD then
		par
			statusCOperate := RELEASE_A
			r_raise_release[statusA,lightsA]
			skip
		endpar endif
	if statusC = OPERATE and statusCOperate = BLOCKED_B and transitionC = SAFE_PERIOD then
		par
			statusCOperate := RELEASE_B
			r_raise_release[statusB,lightsB]
			skip
		endpar endif
	// 
	endpar
	
	
	main rule r_Main =
		par
			r_TrafficLightA[lightsA, statusA]
			r_TrafficLightB[lightsB, statusB]
			r_Controller[]
		endpar


// INITIAL STATE
default init s0:
	function lightsA = ALL_OFF
	function lightsB = ALL_OFF
	function statusC = CONTR_OFF
	function statusCOperate = BLOCKED_A
	function statusA = OFF
	function statusB = OFF