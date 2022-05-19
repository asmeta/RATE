
import static org.junit.Assert.*;

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import org.junit.After;
import org.junit.Before;
import org.junit.Test;

import com.yakindu.core.VirtualTimer;
import com.yakindu.core.VirtualTimer.CycleTimeEventTask;

import trafficlightcontroller.test.util.DataMaker;
import trafficlightcontroller.test.util.FSMEntry;

public class TestExecutorForTrafficLight extends TwoWayTrafficControl {

	private TwoWayTrafficControl statemachine;
	private VirtualTimer timer;
	//private static String sequenceFile = "F:\\Dati-Andrea\\GitHub\\ProTestUTA\\trafficlightcontroller\\ASM\\TrafficLight3\\TrafficLight_3_BR_2WISE_CR_MCDC_RG_UR_3WISE0_Merged.txt";
	//private static String sequenceFile = "..\\StateMachines and Sequences\\TrafficLight_NOREF\\TrafficLight_4_BR_2WISE_CR_MCDC_RG_UR_3WISE_Merged.txt";
	//private static String sequenceFile = "..\\StateMachines and Sequences\\TrafficLight2\\TrafficLight_2_BR_2WISE_CR_MCDC_RG_UR_3WISE0_Merged.txt";
	private static String sequenceFile = "..\\StateMachines and Sequences\\TrafficLight1\\TrafficLight_1_BR_2WISE_CR_MCDC_RG_UR_3WISE0_Merged.txt";
	//private String initialState = "off|off|contr_off|"; // Lv 0
	private String initialState = "off|off|contr_off|blocked_a"; // Lv 1 and Lv 2
	private int nTrafficLights = 2;

	@Before
	public void twoWayTrafficControlTest_setUp() {
		statemachine = new TwoWayTrafficControl();
		timer = new VirtualTimer(200);
		timer.schedulePeriodicalTask(new CycleTimeEventTask(statemachine), 200, 200);
		statemachine.setTimerService(timer);

		statemachine.setTrafficLightA(new TrafficLight());
		statemachine.getTrafficLightA().setTimerService(timer);
		statemachine.setTrafficLightB(new TrafficLight());
		statemachine.getTrafficLightB().setTimerService(timer);

	}

	@After
	public void twoWayTrafficControlTest_tearDown() {
		statemachine = null;
		timer = null;
	}

	public void resetStateMachine() {
		twoWayTrafficControlTest_setUp();

		statemachine.enter();
		statemachine.getTrafficLightA().enter();
		statemachine.getTrafficLightB().enter();
		assertTrue(statemachine.isStateActive(StateGetter.getControllerStateFromString(initialState))); 
		assertTrue(
				statemachine.getTrafficLightA().isStateActive(StateGetter.getSemaphoreStateFromString(initialState, 0)));
		assertTrue(
				statemachine.getTrafficLightB().isStateActive(StateGetter.getSemaphoreStateFromString(initialState, 1)));
	}

	@Test
	public void testAllLevel0() throws IOException {
		String seqsFile = "F:\\Dati-Andrea\\GitHub\\ProTestUTA\\trafficlightcontroller\\ASM\\TrafficLight0\\";

		File f = new File(seqsFile);

		// For each pathname in the pathnames array
		for (String pathname : f.list()) {
			if (pathname.endsWith("_Merged.txt")) {
				initialState = "off|off|contr_off|";
				nTrafficLights = 2;
				launchTest(seqsFile + pathname);
			}
		}
	}

	@Test
	public void testAllLevel1() throws IOException {
		String seqsFile = "F:\\Dati-Andrea\\GitHub\\ProTestUTA\\trafficlightcontroller\\ASM\\TrafficLight1\\";

		File f = new File(seqsFile);

		// For each pathname in the pathnames array
		for (String pathname : f.list()) {
			if (pathname.endsWith("_Merged.txt")) {
				initialState = "off|off|contr_off|blocked_a";
				nTrafficLights = 1;
				launchTest(seqsFile + pathname);
			}
		}
	}

	@Test
	public void testAllLevel2() throws IOException {
		String seqsFile = "F:\\Dati-Andrea\\GitHub\\ProTestUTA\\trafficlightcontroller\\ASM\\TrafficLight2\\";

		File f = new File(seqsFile);

		// For each pathname in the pathnames array
		for (String pathname : f.list()) {
			if (pathname.endsWith("_Merged.txt")) {
				initialState = "off|off|contr_off|blocked_a";
				nTrafficLights = 2;
				launchTest(seqsFile + pathname);
			}
		}
	}
	
	@Test
	public void testAllLevel3() throws IOException {
		String seqsFile = "F:\\Dati-Andrea\\GitHub\\ProTestUTA\\trafficlightcontroller\\ASM\\TrafficLight3\\";

		File f = new File(seqsFile);

		// For each pathname in the pathnames array
		for (String pathname : f.list()) {
			if (pathname.endsWith("_Merged.txt")) {
				initialState = "off|off|contr_off|blocked_a";
				nTrafficLights = 2;
				launchTest(seqsFile + pathname);
			}
		}
	}
	
	@Test
	public void testAllAutomata() throws IOException {
		String seqsFile = "F:\\Dati-Andrea\\GitHub\\ProTestUTA\\trafficlightcontroller\\AUTOMATA\\";

		File f = new File(seqsFile);

		// For each pathname in the pathnames array
		for (String pathname : f.list()) {
			if (pathname.endsWith(".txt")) {
				initialState = "off|off|contr_off|blocked_a";
				nTrafficLights = 2;
				launchTest(seqsFile + pathname);
			}
		}
	}

	@Test
	public void launchOneTest() throws IOException {
		launchTest(sequenceFile);
	}

	public void launchTest(String seqsFile) throws IOException {
		// Get the sequences from file
		sequenceFile = seqsFile;
		ArrayList<String[]> sequences = DataMaker.readSqsFromFile(sequenceFile);
		ArrayList<FSMEntry> entries = DataMaker.readEntriesFromFile(sequenceFile);
		int counter = 0;

		// Execute the sequences
		for (String[] v : sequences) {

			System.out.println("***** File " + seqsFile + " *****");
			System.out.println("***** Executing seq. n. " + (++counter) + " *****");

			resetStateMachine();
			String thisInitialState = initialState;
			// Fetch all the messages
			for (String msg : v) {

				// Check if it the row of the sequence -> It must contain "|"
				if (msg.indexOf("|") >= 0) {

					// Get the final state
					String finalState = StateGetter.getFinalState(thisInitialState, msg, entries);
					Boolean[] output = OutputGetter.getOutput(thisInitialState, msg, entries);
					boolean allowSafePeriod = statemachine.isStateActive(State.MAIN_OPERATE_R_ALL_BLOCKED_BEFORE_A) || statemachine.isStateActive(State.MAIN_OPERATE_R_ALL_BLOCKED_BEFORE_B);

					// Execute all the transitions in the composed one
					String[] subMsg = msg.split("\\|");
					for (int i = 0; i < subMsg.length; i++) {
						switch (i) {
						case 0:
							// TrafficLight A
							performActionSemaphoreA(subMsg[i]);
							break;
						case 1:
							// TrafficLight B
							performActionSemaphoreB(subMsg[i]);
							break;
						case 2:
							// Controller
							performActionController(subMsg[i], allowSafePeriod);
							break;
						}
					}

					// After performing all the transitions, check the final state
					System.out.println("Active State Controller: " + StateGetter.getActiveStateController(statemachine)
							+ " - Checking with " + finalState);
					assertTrue(statemachine.isStateActive(StateGetter.getControllerStateFromString(finalState)));
					System.out.println(
							"Active State SemaphoreA: " + StateGetter.getActiveStateTrafficLight(statemachine, 0)
									+ " - Checking with " + finalState);
					assertTrue(statemachine.getTrafficLightA()
							.isStateActive(StateGetter.getSemaphoreStateFromString(finalState, 0)));
					System.out.println(
							"Active State SemaphoreB: " + StateGetter.getActiveStateTrafficLight(statemachine, 1)
									+ " - Checking with " + finalState);
					assertTrue(statemachine.getTrafficLightB()
							.isStateActive(StateGetter.getSemaphoreStateFromString(finalState, 1)));
					
					// Now check the Output for the Traffic Light A
					if (output != null ) {
						
						System.out.println("Traffic Light A: " + statemachine.getTrafficLightA().lights.getRed() + ", " +
								statemachine.getTrafficLightA().lights.getGreen() + ", " +
								statemachine.getTrafficLightA().lights.getYellow() + " - Checking With: " +
								output[0] + ", " + output[1] + ", " + output[2]);
						
						assertTrue(statemachine.getTrafficLightA().lights.getRed() == output[0]);
						assertTrue(statemachine.getTrafficLightA().lights.getGreen() == output[1]);
						if (!StateGetter.getActiveStateTrafficLight(statemachine, 0).contentEquals("MAIN_ON_MAIN_ATTENTION"))
							assertTrue(statemachine.getTrafficLightA().lights.getYellow() == output[2]);
						
						System.out.println("Traffic Light B: " + statemachine.getTrafficLightB().lights.getRed() + ", " +
								statemachine.getTrafficLightB().lights.getGreen() + ", " +
								statemachine.getTrafficLightB().lights.getYellow() + " - Checking With: " +
								output[3] + ", " + output[4] + ", " + output[5]);
						
						assertTrue(statemachine.getTrafficLightB().lights.getRed() == output[3]);
						assertTrue(statemachine.getTrafficLightB().lights.getGreen() == output[4]);
						if (!StateGetter.getActiveStateTrafficLight(statemachine, 1).contentEquals("MAIN_ON_MAIN_ATTENTION"))
							assertTrue(statemachine.getTrafficLightB().lights.getYellow() == output[5]);
					}					
					thisInitialState = finalState;
				}
			}

		}
	}

	private void performActionController(String msg, boolean allowSafePeriod) {
		if (msg.equals("turn_on")) {
			System.out.println("Calling ON");
			runOn();
		}
		if (msg.equals("operate_t")) {
			System.out.println("Calling Operate");
			runOperate();
		}
		if (msg.equals("standby_t")) {
			System.out.println("Calling Standby");
			runStandby();
		}
		if (msg.equals("turn_off")) {
			System.out.println("Calling Off");
			runOff();
		}
		if (msg.equals("safe_period") && allowSafePeriod) {
			System.out.println("Calling Safe Period");
			runSafePeriod();
		}
	}

	private void performActionSemaphoreA(String msg) {
		if (msg.equals("release_period")) {
			System.out.println("Calling Release Period on Semaphore A");
			runReleasePeriodA();
		}
		if (msg.equals("prepare_period")) {
			System.out.println("Calling Prepare Period on Semaphore A");
			runPreparePeriodA();
		}
	}

	private void performActionSemaphoreB(String msg) {
		if (msg.equals("release_period")) {
			System.out.println("Calling Release Period on Semaphore B");
			runReleasePeriodB();
		}
		if (msg.equals("prepare_period")) {
			System.out.println("Calling Prepare Period on Semaphore B");
			runPreparePeriodB();
		}
	}

	private void runOn() {
		statemachine.raiseOn();
		runCycles();
	}

	private void runOff() {
		statemachine.raiseOff();
		runCycles();
	}

	private void runStandby() {
		statemachine.raiseStandby();
		runCycles();
	}

	private void runOperate() {
		statemachine.raiseOperate();
		runCycles();
	}

	private void runSafePeriod() {
		if (statemachine.isStateActive(TwoWayTrafficControl.State.MAIN_OPERATE_R_ALL_BLOCKED_BEFORE_A) || statemachine.isStateActive(TwoWayTrafficControl.State.MAIN_OPERATE_R_ALL_BLOCKED_BEFORE_B)) {
			if (statemachine.isStateActive(TwoWayTrafficControl.State.MAIN_OPERATE_R_ALL_BLOCKED_BEFORE_A))
				statemachine.raiseTimeEvent(1);
			else {
				// Single Traffic Light -> avoid to pass in the section of Traffic Light B management
				if (nTrafficLights == 1) {
					statemachine.raiseStandby();
					runCycles();
					statemachine.raiseOperate();
					runCycles();
					statemachine.raiseTimeEvent(1);
				}
				else {
					statemachine.raiseTimeEvent(0);
				}
			}
			runCycles();
		}
	}

	private void runReleasePeriodA() {
		statemachine.getTrafficLightA().raiseTimeEvent(0);
		runCycles();
	}

	private void runPreparePeriodA() {
		statemachine.getTrafficLightA().raiseTimeEvent(2);
		runCycles();
	}

	private void runReleasePeriodB() {
		statemachine.getTrafficLightB().raiseTimeEvent(0);
		runCycles();
	}

	private void runPreparePeriodB() {
		statemachine.getTrafficLightB().raiseTimeEvent(2);
		runCycles();
	}

	private void runCycles() {
		statemachine.runCycle();
		statemachine.getTrafficLightA().runCycle();
		statemachine.getTrafficLightB().runCycle();
	}

}
