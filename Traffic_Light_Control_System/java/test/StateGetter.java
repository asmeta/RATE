import java.util.ArrayList;

import trafficlightcontroller.test.util.FSMEntry;

public class StateGetter {
	
	public static TwoWayTrafficControl.State getControllerStateFromString(String state) {
			
		String controllerState = state.split("\\|")[2];
		
		if (controllerState.equals("contr_off"))
			return TwoWayTrafficControl.State.MAIN_OFF;
		
		if (controllerState.equals("operate"))
			return TwoWayTrafficControl.State.MAIN_OPERATE;
		
		if (controllerState.equals("standby"))
			return TwoWayTrafficControl.State.MAIN_STANDBY;		
		
		return null;
	}
	
	
	public static TrafficLight.State getSemaphoreStateFromString(String state, int i) {
		String controllerState = state.split("\\|")[i];
		
		if (controllerState.equals("off"))
			return TrafficLight.State.MAIN_OFF;
		
		if (controllerState.equals("attention"))
			return TrafficLight.State.MAIN_ON_MAIN_ATTENTION;
		
		if (controllerState.equals("blocked"))
			return TrafficLight.State.MAIN_ON_MAIN_BLOCKED;	
		
		if (controllerState.equals("prepare_block"))
			return TrafficLight.State.MAIN_ON_MAIN_PREPAREBLOCK;	
		
		if (controllerState.equals("released"))
			return TrafficLight.State.MAIN_ON_MAIN_RELEASED;	
		
		return null;
	}


	public static String getFinalState(String initialState, String msg, ArrayList<FSMEntry> entries) {
		for (FSMEntry entry : entries) {
			if (entry.getStateFrom().equals(initialState) && entry.getReceivedMessage().equals(msg))
				return entry.getStateTo();
		}
		
		return null;
	}
	
	public static String getActiveStateController(TwoWayTrafficControl statemachine) {
		String state = "";
		
		for (TwoWayTrafficControl.State s : TwoWayTrafficControl.State.values())
			if (statemachine.isStateActive(s))
				state = s.toString();
		
		return state;
	}
	
	public static String getActiveStateTrafficLight(TwoWayTrafficControl statemachine, int i) {
		String state = "";
		
		for (TrafficLight.State s : TrafficLight.State.values())
			if (i == 0) {
				if (statemachine.getTrafficLightA().isStateActive(s))
					state = s.toString();
			} else {
				if (statemachine.getTrafficLightB().isStateActive(s))
					state = s.toString();
			}
		
		return state;
	}
	
}
