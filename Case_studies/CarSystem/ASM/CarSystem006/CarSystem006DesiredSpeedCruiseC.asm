//Sixth refinement of Adaptive Exterior Light and Speed Control System
//Setting and modifying desired speed and Cruise control
//from SCS-1 to SCS-17

module CarSystem006DesiredSpeedCruiseC
import ../StandardLibrary
import CarSystem006Functions
export *

signature:
	// FUNCTIONS
	
	monitored passed1Sec: Boolean // Is 1 second passed?
	monitored passed2Sec: Boolean // Are 2 seconds passed?
	controlled passed2SecYes: Boolean // Are 2 seconds passed?
	monitored accelerationUserHighCruiseControl: Boolean // User acceleration is higher than cruise control

	derived manualSpeed: Boolean // True if user changes desired speed manually	
	
	
definitions:	
	// FUNCTION DEFINITIONS
	//It is true if user changes desired speed manually	
	function manualSpeed = 
		(sCSLever = UPWARD5_SCS or sCSLever = UPWARD7_SCS or sCSLever = DOWNWARD5_SCS or sCSLever = DOWNWARD7_SCS)
	
	// RULE DEFINITIONS
	
	//SCS-2 SCS-3
	macro rule r_SCSLeverForward = 
		if (sCSLever = FORWARD_SCS or sCSLever = HEAD) then //Guard changed
			if (currentSpeed < 200) then
				if (desiredSpeed = 0) then 
					setVehicleSpeed := 0
				else
					setVehicleSpeed := desiredSpeed
				endif
			else
				if (desiredSpeed = 0) then
					par
						setVehicleSpeed := currentSpeed
						desiredSpeed := currentSpeed
					endpar
				else
					setVehicleSpeed := desiredSpeed
				endif
			endif
		endif
	
	macro rule r_UpwardDownward5EqualSpeed =
		par	
			if (sCSLever = UPWARD5_SCS) then
				desiredSpeed := setVehicleSpeed + 10 //desired speed increased by 1 km/h
			endif
			if (sCSLever = DOWNWARD5_SCS) then
				desiredSpeed := setVehicleSpeed - 10 //desired speed increased by 1 km/h
			endif
		endpar
		
	macro rule r_UpwardDownward5 ($speed in CurrentSpeed) =
		par	
			if (sCSLever = UPWARD5_SCS) then
				$speed := $speed + 10 //desired speed increased by 1 km/h
			endif
			if (sCSLever = DOWNWARD5_SCS) then
				$speed := $speed - 10 //desired speed increased by 1 km/h
			endif
		endpar
	
	macro rule r_UpwardDownward7EqualSpeed =
		par	
			if (sCSLever = UPWARD7_SCS) then
				desiredSpeed := setVehicleSpeed + (100-(setVehicleSpeed mod 100)) //desired speed increased to the next ten's place
			endif
			if (sCSLever = DOWNWARD7_SCS) then
				if (setVehicleSpeed mod 100 = 0) then
					desiredSpeed := setVehicleSpeed - 100 //desired speed decreased to the next ten's place
				else
					desiredSpeed := setVehicleSpeed - (setVehicleSpeed mod 100)
				endif
			endif
		endpar	
		
	macro rule r_UpwardDownward7 ($speed in CurrentSpeed) =
		par	
			if (sCSLever = UPWARD7_SCS) then
				$speed := $speed + (100-($speed mod 100)) //desired speed increased to the next ten's place
			endif
			if (sCSLever = DOWNWARD7_SCS) then
				if ($speed mod 100 = 0) then
					$speed := $speed - 100 //desired speed decreased to the next ten's place
				else
					$speed := $speed - ($speed mod 100)
				endif
			endif
		endpar	
		
								
	//SCS-4 SCS-5 SCS-6
	macro rule r_setDesiredSpeed = 
		if (setVehicleSpeed != 0) then
		//Lever has different value from previous state
			if (sCSLeve_Previous != sCSLever) then 
				par
					//SCS-4 //SCS-6
					r_UpwardDownward5[setVehicleSpeed] 
					//If CCM2 and desired speed is not euqal to target speed, change desired speed starting from target speed
					if (cruiseControlMode = CCM2 and (desiredSpeed != setVehicleSpeed)) then
						r_UpwardDownward5EqualSpeed[] 
					else
						r_UpwardDownward5[desiredSpeed] 
					endif
					//SCS-5 //SCS-6
					r_UpwardDownward7[setVehicleSpeed] 
					if (cruiseControlMode = CCM2 and (desiredSpeed != setVehicleSpeed)) then
						r_UpwardDownward7EqualSpeed[] 
					else
						r_UpwardDownward7[desiredSpeed] 
					endif
					//SCS-12 SCS-17
					if (sCSLever = BACKWARD_SCS) then
						setVehicleSpeed := 0
					endif
				endpar
			endif
		endif
		
	macro rule r_setVehicleSpeedLongLeverPress = 
		//SCS-7 SCS-8 SCS-9 SCS-10
		if (setVehicleSpeed != 0) then
			if (sCSLeve_Previous = sCSLever and manualSpeed) then 
				if (passed2Sec) then
					par
						//SCS-7 SCS-9
						if (sCSLever = UPWARD5_SCS or sCSLever = DOWNWARD5_SCS) then
							if (passed1Sec) then
								par
									r_UpwardDownward5[setVehicleSpeed] 
									//If CCM2 and desired speed is not euqal to target speed, change desired speed starting from target speed
									if (cruiseControlMode = CCM2 and (desiredSpeed != setVehicleSpeed)) then
										r_UpwardDownward5EqualSpeed[] 
									else
										r_UpwardDownward5[desiredSpeed] 
									endif
								endpar
							endif
						endif
						//SCS-8 SCS-10
						if (sCSLever = UPWARD7_SCS or sCSLever = DOWNWARD7_SCS) then
							if (passed2Sec) then
								par
									r_UpwardDownward7[setVehicleSpeed] 
									if (cruiseControlMode = CCM2 and (desiredSpeed != setVehicleSpeed)) then
										r_UpwardDownward7EqualSpeed[] 
									else
										r_UpwardDownward7[desiredSpeed] 
									endif
								endpar 
							endif
						endif
					endpar
				endif
			endif
		endif
	
	macro rule r_SetModifySpeed = 
		//SCS-1
		if ((not engineOn(keyState_Previous)) and engineOn(keyState)) then
			par
				setVehicleSpeed := 0
				desiredSpeed := 0
			endpar
		else
			if (engineOn(keyState_Previous) and engineOn(keyState)) then
				par	
					r_SCSLeverForward[] 
					//SCS-11
					if ((sCSLever = UPWARD5_SCS or sCSLever = UPWARD7_SCS or sCSLever = DOWNWARD5_SCS or sCSLever = DOWNWARD7_SCS) and setVehicleSpeed = 0) then
						desiredSpeed := currentSpeed
					endif
					r_setDesiredSpeed[] 
					r_setVehicleSpeedLongLeverPress[] 
				endpar
			endif
		endif
	
	macro rule  r_DesiredSpeedVehicleSpeed =
		if (brakePedal=0) then
			par
				let ($x = currentSpeed) in skip endlet
				if (cruiseControlMode=CCM2) then
					r_SetModifySpeed[] 
				endif
				if (cruiseControlMode=CCM1) then
					//SCS-15
					if (setVehicleSpeed != 0) then
						if (accelerationUserHighCruiseControl) then
							//setVehicleSpeed := 0//Deactivate cruise control
							skip
						else
							r_SetModifySpeed[] 
						endif
					else
						r_SetModifySpeed[] 
					endif
				endif
			endpar
		endif
		
	macro rule r_BrakePedal = 
		if (cruiseControlMode = CCM1 and setVehicleSpeed != 0) then
			//SCS-16
			if (brakePedal != 0) then
				setVehicleSpeed := 0
			endif
		endif