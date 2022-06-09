import static org.junit.Assert.*;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import org.asmeta.atgt.generator.CriteriaEnum;
import org.junit.After;
import org.junit.Before;
import org.junit.Test;

import com.yakindu.core.VirtualTimer;
import com.yakindu.core.VirtualTimer.CycleTimeEventTask;

import util.DataMaker;
import util.FSMCreator;
import util.FSMEntry;

public class TestExecutorForTrafficLight extends TwoWayTrafficControl {

	// The classes to be tested
	private TwoWayTrafficControl statemachine;
	private VirtualTimer timer;
	
	// The path of abstract tests
	private static String PATH_AT = "../abstract_tests/TrafficLight_";
	
	class Configuration {
		int nTrafficLights;
		String initialState;
		List<CriteriaEnum> criteria;
		String level;
		int nStatuses;
		int nTransitions;
		int nMessages;
		
		public Configuration(int nTrafficLights, String initialState, List<CriteriaEnum> criteria, String level, int nStatuses, int nTransitions, int nMessages) {
			this.criteria = criteria;
			this.initialState = initialState;
			this.nTrafficLights = nTrafficLights;
			this.level = level;
			this.nStatuses = nStatuses;
			this.nTransitions = nTransitions;
			this.nMessages = nMessages;
		}
		
		public Configuration(int nTrafficLights, String initialState, String level, int nStatuses, int nTransitions, int nMessages, CriteriaEnum ... criteriaEnums) {
			this(nTrafficLights, initialState, Arrays.asList(criteriaEnums), level, nStatuses, nTransitions, nMessages);
		}
	}
	
	// The possible configurations
	ArrayList<Configuration> configToBeTested = new ArrayList<Configuration>() {
		
		private static final long serialVersionUID = 5014712752302397500L;
		{
			add(new Configuration(1, "off|off|contr_off|", "0", 3, 1, 0, CriteriaEnum.BASIC_RULE));
			add(new Configuration(1, "off|off|contr_off|", "0", 3, 1, 0, CriteriaEnum.COMBINATORIAL_ALL));
			add(new Configuration(1, "off|off|contr_off|", "0", 3, 1, 0, CriteriaEnum.COMPLETE_RULE));
			add(new Configuration(1, "off|off|contr_off|", "0", 3, 1, 0, CriteriaEnum.MCDC));
			add(new Configuration(1, "off|off|contr_off|", "0", 3, 1, 0, CriteriaEnum.RULE_GUARD));
			add(new Configuration(1, "off|off|contr_off|", "0", 3, 1, 0, CriteriaEnum.RULE_UPDATE));
			add(new Configuration(1, "off|off|contr_off|", "0", 3, 1, 0, CriteriaEnum.THREEWISE_ALL));
			add(new Configuration(1, "off|off|contr_off|", "0", 3, 1, 0, CriteriaEnum.BASIC_RULE, CriteriaEnum.COMBINATORIAL_ALL,
					CriteriaEnum.COMPLETE_RULE, CriteriaEnum.MCDC, CriteriaEnum.RULE_GUARD, CriteriaEnum.RULE_UPDATE, CriteriaEnum.THREEWISE_ALL));
			
			add(new Configuration(1, "off|off|contr_off|blocked_a", "1", 4, 2, 0, CriteriaEnum.BASIC_RULE));
			add(new Configuration(1, "off|off|contr_off|blocked_a", "1", 4, 2, 0, CriteriaEnum.COMBINATORIAL_ALL));
			add(new Configuration(1, "off|off|contr_off|blocked_a", "1", 4, 2, 0, CriteriaEnum.COMPLETE_RULE));
			add(new Configuration(1, "off|off|contr_off|blocked_a", "1", 4, 2, 0, CriteriaEnum.MCDC));
			add(new Configuration(1, "off|off|contr_off|blocked_a", "1", 4, 2, 0, CriteriaEnum.RULE_GUARD));
			add(new Configuration(1, "off|off|contr_off|blocked_a", "1", 4, 2, 0, CriteriaEnum.RULE_UPDATE));
			add(new Configuration(1, "off|off|contr_off|blocked_a", "1", 4, 2, 0, CriteriaEnum.THREEWISE_ALL));
			add(new Configuration(1, "off|off|contr_off|blocked_a", "1", 4, 2, 0, CriteriaEnum.BASIC_RULE, CriteriaEnum.COMBINATORIAL_ALL,
					CriteriaEnum.COMPLETE_RULE, CriteriaEnum.MCDC, CriteriaEnum.RULE_GUARD, CriteriaEnum.RULE_UPDATE, CriteriaEnum.THREEWISE_ALL));
			
			add(new Configuration(2, "off|off|contr_off|blocked_a", "2", 4, 3, 0, CriteriaEnum.BASIC_RULE));
			add(new Configuration(2, "off|off|contr_off|blocked_a", "2", 4, 3, 0, CriteriaEnum.COMBINATORIAL_ALL));
			add(new Configuration(2, "off|off|contr_off|blocked_a", "2", 4, 3, 0, CriteriaEnum.COMPLETE_RULE));
			add(new Configuration(2, "off|off|contr_off|blocked_a", "2", 4, 3, 0, CriteriaEnum.MCDC));
			add(new Configuration(2, "off|off|contr_off|blocked_a", "2", 4, 3, 0, CriteriaEnum.RULE_GUARD));
			add(new Configuration(2, "off|off|contr_off|blocked_a", "2", 4, 3, 0, CriteriaEnum.RULE_UPDATE));
			add(new Configuration(2, "off|off|contr_off|blocked_a", "2", 4, 3, 0, CriteriaEnum.THREEWISE_ALL));
			add(new Configuration(2, "off|off|contr_off|blocked_a", "2", 4, 3, 0, CriteriaEnum.BASIC_RULE, CriteriaEnum.COMBINATORIAL_ALL,
					CriteriaEnum.COMPLETE_RULE, CriteriaEnum.MCDC, CriteriaEnum.RULE_GUARD, CriteriaEnum.RULE_UPDATE, CriteriaEnum.THREEWISE_ALL));
						
			add(new Configuration(2, "off|off|contr_off|blocked_a", "3", 4, 3, 2, CriteriaEnum.BASIC_RULE));
			add(new Configuration(2, "off|off|contr_off|blocked_a", "3", 4, 3, 2, CriteriaEnum.COMBINATORIAL_ALL));
			add(new Configuration(2, "off|off|contr_off|blocked_a", "3", 4, 3, 2, CriteriaEnum.COMPLETE_RULE));
			add(new Configuration(2, "off|off|contr_off|blocked_a", "3", 4, 3, 2, CriteriaEnum.MCDC));
			add(new Configuration(2, "off|off|contr_off|blocked_a", "3", 4, 3, 2, CriteriaEnum.RULE_GUARD));
			add(new Configuration(2, "off|off|contr_off|blocked_a", "3", 4, 3, 2, CriteriaEnum.RULE_UPDATE));
			add(new Configuration(2, "off|off|contr_off|blocked_a", "3", 4, 3, 2, CriteriaEnum.THREEWISE_ALL));
			add(new Configuration(2, "off|off|contr_off|blocked_a", "3", 4, 3, 2, CriteriaEnum.BASIC_RULE, CriteriaEnum.COMBINATORIAL_ALL,
					CriteriaEnum.COMPLETE_RULE, CriteriaEnum.MCDC, CriteriaEnum.RULE_GUARD, CriteriaEnum.RULE_UPDATE, CriteriaEnum.THREEWISE_ALL));
			
			add(new Configuration(2, "off|off|contr_off|blocked_a", "NR", 4, 3, 2, CriteriaEnum.BASIC_RULE));
			add(new Configuration(2, "off|off|contr_off|blocked_a", "NR", 4, 3, 2, CriteriaEnum.COMBINATORIAL_ALL));
			add(new Configuration(2, "off|off|contr_off|blocked_a", "NR", 4, 3, 2, CriteriaEnum.COMPLETE_RULE));
			add(new Configuration(2, "off|off|contr_off|blocked_a", "NR", 4, 3, 2, CriteriaEnum.MCDC));
			add(new Configuration(2, "off|off|contr_off|blocked_a", "NR", 4, 3, 2, CriteriaEnum.RULE_GUARD));
			add(new Configuration(2, "off|off|contr_off|blocked_a", "NR", 4, 3, 2, CriteriaEnum.RULE_UPDATE));
			add(new Configuration(2, "off|off|contr_off|blocked_a", "NR", 4, 3, 2, CriteriaEnum.THREEWISE_ALL));
			add(new Configuration(2, "off|off|contr_off|blocked_a", "NR", 4, 3, 2, CriteriaEnum.BASIC_RULE, CriteriaEnum.COMBINATORIAL_ALL,
					CriteriaEnum.COMPLETE_RULE, CriteriaEnum.MCDC, CriteriaEnum.RULE_GUARD, CriteriaEnum.RULE_UPDATE, CriteriaEnum.THREEWISE_ALL));
		}
	};
	
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

	public void resetStateMachine(Configuration c) {
		twoWayTrafficControlTest_setUp();

		statemachine.enter();
		statemachine.getTrafficLightA().enter();
		statemachine.getTrafficLightB().enter();
		assertTrue(statemachine.isStateActive(StateGetter.getControllerStateFromString(c.initialState))); 
		assertTrue(
				statemachine.getTrafficLightA().isStateActive(StateGetter.getSemaphoreStateFromString(c.initialState, 0)));
		assertTrue(
				statemachine.getTrafficLightB().isStateActive(StateGetter.getSemaphoreStateFromString(c.initialState, 1)));
	}

	@Test
	public void testAllLevel0() throws Exception {
		testAllLevelX("0");
	}

	private void testAllLevelX(String level) throws Exception {
		for (Configuration c : configToBeTested) {
			if (c.level.equals(level)) {
				String path = PATH_AT + c.level + "/";
				
				// Execute tests
				launchTest(c, path);
			}
		}
	}

	@Test
	public void testAllLevel1() throws Exception {
		testAllLevelX("1");
	}

	@Test
	public void testAllLevel2() throws Exception {
		testAllLevelX("2");
	}
	
	@Test
	public void testAllLevel3() throws Exception {
		testAllLevelX("3");
	}
	
	@Test
	public void testAllLevelNR() throws Exception {
		testAllLevelX("NR");
	}
	
	@Test
	public void launchOneTest() throws Exception {
		launchTest(configToBeTested.get(27), PATH_AT + "3/");
	}

	public void launchTest(Configuration c, String path) throws Exception {
		String fsm = "";
		
		// Build the FSM from multiple Avallas
		File folder = new File(path);
		File[] listOfFiles = folder.listFiles();
		ArrayList<String> avallaFiles = new ArrayList<String>();
		for (File f:listOfFiles) {
			for (CriteriaEnum ce : c.criteria) {
				if (f.getName().startsWith("test" + ce.getAbbrvName())) {
					avallaFiles.add(f.getAbsolutePath());
					break;
				}
			}
			
		}
		String[] avallaFilesList = new String[avallaFiles.size()]; 
		avallaFilesList = avallaFiles.toArray(avallaFilesList);
		fsm = FSMCreator.getContentFSMFromMultipleAvalla(avallaFilesList, true, c.nMessages, c.nTransitions, c.nStatuses);
		File tempFile = new File("./temp.txt");
		tempFile.createNewFile();
		BufferedWriter bw = new BufferedWriter(new FileWriter(tempFile));
		bw.append(fsm);
		bw.close();
		
		// Get the sequences from file
		ArrayList<String[]> sequences = DataMaker.readSqsFromFile(tempFile.getAbsolutePath());
		ArrayList<FSMEntry> entries = DataMaker.readEntriesFromFile(tempFile.getAbsolutePath());
		int counter = 0;

		// Execute the sequences
		for (String[] v : sequences) {

			System.out.println("***** File " + tempFile.getAbsolutePath() + " *****");
			System.out.println("***** Executing seq. n. " + (++counter) + " *****");

			resetStateMachine(c);
			String thisInitialState = c.initialState;
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
					boolean doneTransSemA = false;
					boolean doneTransSemB = false;
					boolean doneTransController = false;
					for (int i = 0; i < subMsg.length; i++) {
						switch (i) {
						case 0:
							// TrafficLight A
							doneTransSemA = performActionSemaphoreA(subMsg[i]);
							break;
						case 1:
							// TrafficLight B
							doneTransSemB = performActionSemaphoreB(subMsg[i]);
							break;
						case 2:
							// Controller
							doneTransController = performActionController(subMsg[i], allowSafePeriod, c);
							break;
						}
					}

					// After performing all the transitions, check the final state
					if (doneTransController) {
						System.out.println("Active State Controller: " + StateGetter.getActiveStateController(statemachine)
								+ " - Checking with " + finalState);
						assertTrue(statemachine.isStateActive(StateGetter.getControllerStateFromString(finalState)));
					}
					if (doneTransSemA) {
						System.out.println(
								"Active State SemaphoreA: " + StateGetter.getActiveStateTrafficLight(statemachine, 0)
										+ " - Checking with " + finalState);
						assertTrue(statemachine.getTrafficLightA()
								.isStateActive(StateGetter.getSemaphoreStateFromString(finalState, 0)));
					}
					if (doneTransSemB) {
						System.out.println(
								"Active State SemaphoreB: " + StateGetter.getActiveStateTrafficLight(statemachine, 1)
										+ " - Checking with " + finalState);
						assertTrue(statemachine.getTrafficLightB()
								.isStateActive(StateGetter.getSemaphoreStateFromString(finalState, 1)));
					}
					
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
					if (doneTransSemA || doneTransSemB || doneTransController)
						thisInitialState = finalState;
				}
			}

		}
		tempFile.delete();
		
	}

	private boolean performActionController(String msg, boolean allowSafePeriod, Configuration c) {
		if (msg.equals("turn_on")) {
			System.out.println("Calling ON");
			runOn();
			return true;
		}
		if (msg.equals("operate_t")) {
			System.out.println("Calling Operate");
			runOperate();
			return true;
		}
		if (msg.equals("standby_t")) {
			System.out.println("Calling Standby");
			runStandby();
			return true;
		}
		if (msg.equals("turn_off")) {
			System.out.println("Calling Off");
			runOff();
			return true;
		}
		if (msg.equals("safe_period") && allowSafePeriod) {
			System.out.println("Calling Safe Period");
			runSafePeriod(c);
			return true;
		}
		return false;
	}

	private boolean performActionSemaphoreA(String msg) {
		if (msg.equals("release_period")) {
			System.out.println("Calling Release Period on Semaphore A");
			runReleasePeriodA();
			return true;
		}
		if (msg.equals("prepare_period")) {
			System.out.println("Calling Prepare Period on Semaphore A");
			runPreparePeriodA();
			return true;
		}
		return false;
	}

	private boolean performActionSemaphoreB(String msg) {
		if (msg.equals("release_period")) {
			System.out.println("Calling Release Period on Semaphore B");
			runReleasePeriodB();
			return true;
		}
		if (msg.equals("prepare_period")) {
			System.out.println("Calling Prepare Period on Semaphore B");
			runPreparePeriodB();
			return true;
		}
		return false;
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

	private void runSafePeriod(Configuration c) {
		if (statemachine.isStateActive(TwoWayTrafficControl.State.MAIN_OPERATE_R_ALL_BLOCKED_BEFORE_A) || statemachine.isStateActive(TwoWayTrafficControl.State.MAIN_OPERATE_R_ALL_BLOCKED_BEFORE_B)) {
			if (statemachine.isStateActive(TwoWayTrafficControl.State.MAIN_OPERATE_R_ALL_BLOCKED_BEFORE_A))
				statemachine.raiseTimeEvent(1);
			else {
				// Single Traffic Light -> avoid to pass in the section of Traffic Light B management
				if (c.nTrafficLights == 1) {
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
