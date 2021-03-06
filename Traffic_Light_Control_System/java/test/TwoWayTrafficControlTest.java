/** Generated by YAKINDU Statechart Tools code generator. */


import org.junit.*;
import static org.junit.Assert.*;
import com.yakindu.core.VirtualTimer;
import com.yakindu.core.VirtualTimer.VirtualTimeTask;
import com.yakindu.core.VirtualTimer.CycleTimeEventTask;

/**
 * Unit TestCase for TwoWayTrafficControl
 */
@SuppressWarnings("all")
public class TwoWayTrafficControlTest {
	
	private TwoWayTrafficControl statemachine;	
	private VirtualTimer timer;
	
	
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
	
	@Test
	public void testStartUp() {
		statemachine.enter();
		statemachine.getTrafficLightA().enter();
		statemachine.getTrafficLightB().enter();
		assertTrue(statemachine.isStateActive(TwoWayTrafficControl.State.MAIN_OFF));
		assertTrue(statemachine.getTrafficLightA().isStateActive(TrafficLight.State.MAIN_OFF));
		assertTrue(statemachine.getTrafficLightB().isStateActive(TrafficLight.State.MAIN_OFF));
		statemachine.raiseOn();
		timer.cycleLeap(1l);
		statemachine.getTrafficLightA().runCycle();
		statemachine.getTrafficLightB().runCycle();
		assertTrue(statemachine.isStateActive(TwoWayTrafficControl.State.MAIN_STANDBY));
		assertTrue(statemachine.getTrafficLightA().isStateActive(TrafficLight.State.MAIN_ON));
		assertTrue(statemachine.getTrafficLightB().isStateActive(TrafficLight.State.MAIN_ON));
		assertTrue(statemachine.getTrafficLightA().isStateActive(TrafficLight.State.MAIN_ON_MAIN_ATTENTION));
		assertTrue(statemachine.getTrafficLightB().isStateActive(TrafficLight.State.MAIN_ON_MAIN_ATTENTION));
		statemachine.raiseOperate();
		timer.cycleLeap(1l);
		assertTrue(statemachine.isStateActive(TwoWayTrafficControl.State.MAIN_OPERATE));
		statemachine.getTrafficLightA().runCycle();
		statemachine.getTrafficLightB().runCycle();
		assertTrue(statemachine.getTrafficLightA().isStateActive(TrafficLight.State.MAIN_ON_MAIN_BLOCKED));
		assertTrue(statemachine.getTrafficLightB().isStateActive(TrafficLight.State.MAIN_ON_MAIN_BLOCKED));
		timer.timeLeap(statemachine.getSafePeriod()*1000l);
		assertTrue(statemachine.isStateActive(TwoWayTrafficControl.State.MAIN_OPERATE));
		statemachine.raiseStandby();
		timer.cycleLeap(1l);
		assertTrue(statemachine.isStateActive(TwoWayTrafficControl.State.MAIN_STANDBY));
		statemachine.getTrafficLightA().runCycle();
		statemachine.getTrafficLightB().runCycle();
		assertTrue(statemachine.getTrafficLightB().isStateActive(TrafficLight.State.MAIN_ON_MAIN_ATTENTION));
	}
}
