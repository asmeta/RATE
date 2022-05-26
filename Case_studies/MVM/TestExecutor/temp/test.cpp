#include <string>
#include "gtest/gtest.h"
#include "MVMStateMachineCore.h"
#include "sc_runner_timed.h"
#include "sc_types.h"
using namespace std;

void startMachine();
void enterExpiratoryPause();
MVMStateMachineCore* statechart;

class DropPAW_ITS_PCVMock{
	public:
	sc_boolean dropPAW_ITS_PCVBehaviorDefault;

	sc_boolean dropPAW_ITS_PCVDefault(){
		sc_boolean defaultValue = false;
		return defaultValue;
	}

	sc_boolean getBehavior(){
		return dropPAW_ITS_PCVBehaviorDefault;
	}

	void setDefaultBehavior(bool val){
		dropPAW_ITS_PCVBehaviorDefault = val;
	}

	void initializeBehavior() {
		setDefaultBehavior(false);
	}

	void reset() {
		initializeBehavior();
	}
};

static DropPAW_ITS_PCVMock* dropPAW_ITS_PCVMock;

class FinishMock{
	public:
	int callCount;

	sc_boolean calledAtLeast(const int times){
		return (callCount >= times);
	}

	sc_boolean calledAtLeastOnce(){
		return (callCount>0);
	}

	void finish() {
		++callCount;
	}
	void reset() {
		callCount = 0;
	}
};
static FinishMock* finishMock;

class PawGTMaxPinspMock{
	typedef bool sc_boolean;
	public:
	sc_boolean pawGTMaxPinspBehaviorDefault;

	sc_boolean getBehavior(){
		return pawGTMaxPinspBehaviorDefault;
	}

	void setDefaultBehavior(bool defaultBehavior){
		pawGTMaxPinspBehaviorDefault = defaultBehavior;
	}

	void initializeBehavior() {
		setDefaultBehavior(false);
	}

	void reset() {
		initializeBehavior();
	}
};
static PawGTMaxPinspMock* pawGTMaxPinspMock;

class DropPAW_ITS_PSVMock{
	public:
	sc_boolean dropPAW_ITS_PSVBehaviorDefault;

	sc_boolean dropPAW_ITS_PSVDefault(){
		sc_boolean defaultValue = false;
		return defaultValue;
	}

	sc_boolean getBehavior(){
		return dropPAW_ITS_PSVBehaviorDefault;
	}

	void setDefaultBehavior(sc_boolean defaultBehavior){
		dropPAW_ITS_PSVBehaviorDefault = defaultBehavior;
	}

	void initializeBehavior() {
		setDefaultBehavior(false);
	}

	void reset() {
		initializeBehavior();
	}
};

static DropPAW_ITS_PSVMock* dropPAW_ITS_PSVMock;

class Min_exp_time_psvMock{
	typedef int32_t (Min_exp_time_psvMock::*functiontype)();
	public:
	int32_t (Min_exp_time_psvMock::*min_exp_time_psvBehaviorDefault)();

	int32_t min_exp_time_psv1(){
		return (2000);
	}

	int32_t min_exp_time_psvDefault(){
		int32_t defaultValue = 1000;
		return (defaultValue);
	}

	functiontype getBehavior(){
		return min_exp_time_psvBehaviorDefault;
	}

	void setDefaultBehavior(int32_t (Min_exp_time_psvMock::*defaultBehavior)()){
		min_exp_time_psvBehaviorDefault = defaultBehavior;
	}

	void initializeBehavior() {
		setDefaultBehavior(&Min_exp_time_psvMock::min_exp_time_psvDefault);
	}

	void reset() {
		initializeBehavior();
	}
};

static Min_exp_time_psvMock* min_exp_time_psvMock;

class FlowDropPSVMock{
	public:
	sc_boolean flowDropPSVBehaviorDefault;

	sc_boolean flowDropPSVDefault(){
		sc_boolean defaultValue = false;
		return (defaultValue);
	}

	sc_boolean getBehavior(){
		return flowDropPSVBehaviorDefault;
	}

	void setDefaultBehavior(sc_boolean defaultBehavior){
		flowDropPSVBehaviorDefault = defaultBehavior;
	}

	void initializeBehavior() {
		setDefaultBehavior(false);
	}

	void reset() {
		initializeBehavior();
	}
};
static FlowDropPSVMock* flowDropPSVMock;

enum valve_status { OPEN, CLOSED };

class MockDefault : public MVMStateMachineCore::OperationCallback {
	private:
	valve_status inValveStatus;
	valve_status outValveStatus;

	public:
	MockDefault() {
		inValveStatus = CLOSED;
		outValveStatus = OPEN;
	}
	void finish() {
		finishMock->finish();
	}
	void start() {
	}
	void closeInputValve() {
		inValveStatus = CLOSED;
	}
	void openInputValve(MVM_PIO p) {
		inValveStatus = OPEN;
	}
	void closeOutputValve() {
		outValveStatus = CLOSED;
	}
	void openOutputValve() {
		outValveStatus = OPEN;
	}
	void autoBreath(sc_boolean b) {
	}
	sc_boolean pawGTMaxPinsp() {
		return pawGTMaxPinspMock->getBehavior();
	}
	sc_boolean dropPAW_ITS_PCV() {
		return dropPAW_ITS_PCVMock->getBehavior();
	}
	int32_t min_exp_time_psv() {
		return (min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))();
	}
	sc_boolean flowDropPSV() {
		return flowDropPSVMock->getBehavior();
	}
	sc_boolean dropPAW_ITS_PSV() {
		return dropPAW_ITS_PSVMock->getBehavior();
	}
	void apneaAlarm() {
	}
	valve_status getInValveStatus() {
		return inValveStatus;
	}
	valve_status getOutValveStatus() {
		return outValveStatus;
	}
};

//! The timers are managed by a timer service. */
static TimedSctUnitRunner * runner;
MockDefault *defaultMock;

class TestClass : public ::testing::Test{
	protected:
	virtual void SetUp() {
		statechart = new MVMStateMachineCore();
		defaultMock = new MockDefault();

		finishMock = new FinishMock();

		flowDropPSVMock = new FlowDropPSVMock();
		flowDropPSVMock->initializeBehavior();

		dropPAW_ITS_PCVMock = new DropPAW_ITS_PCVMock();
		dropPAW_ITS_PCVMock->initializeBehavior();

		statechart->setOperationCallback(defaultMock);

		pawGTMaxPinspMock = new PawGTMaxPinspMock();
		pawGTMaxPinspMock->initializeBehavior();

		min_exp_time_psvMock = new Min_exp_time_psvMock();
		min_exp_time_psvMock->initializeBehavior();

		dropPAW_ITS_PSVMock = new DropPAW_ITS_PSVMock();
		dropPAW_ITS_PSVMock->initializeBehavior();

		runner = new TimedSctUnitRunner(
			statechart,
			false,
			100
		);
		statechart->setTimerService(runner);
	}
	virtual void TearDown() {
		delete defaultMock;
		delete flowDropPSVMock;
		delete statechart;
		delete runner;
		delete dropPAW_ITS_PCVMock;
		delete pawGTMaxPinspMock;
		delete min_exp_time_psvMock;
		delete dropPAW_ITS_PSVMock;
		delete finishMock;
	}
};



TEST_F(TestClass, BR_r_Main_FFFTF12) {
	statechart->enter();
	//// test name test@BR_r_Main_FFFTF12
	//// generated for (test goal): BR_r_Main_FFFTF12: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (apneaAlarm and not((turnOffApneaAlarm = true) and ((state = MAIN_REGION_PCV_R1_INSPIRATION) or (state = MAIN_REGION_PCV_R1_EXPIRATION))))))
	statechart->exit();
}
TEST_F(TestClass, BR_r_Main_FFFTF32) {
	statechart->enter();
	//// test name test@BR_r_Main_FFFTF32
	//// generated for (test goal): BR_r_Main_FFFTF32: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_STARTUP) and not startupEnded)))
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	statechart->exit();
}
TEST_F(TestClass, BR_r_Main_FFFTF42) {
	statechart->enter();
	//// test name test@BR_r_Main_FFFTF42
	//// generated for (test goal): BR_r_Main_FFFTF42: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_SELFTEST) and not(selfTestPassed or resume))))
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	statechart->raiseStartupEnded();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_SELFTEST));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_SELFTEST));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	statechart->exit();
}
TEST_F(TestClass, BR_r_Main_FFFTF82) {
	statechart->enter();
	//// test name test@BR_r_Main_FFFTF82
	//// generated for (test goal): BR_r_Main_FFFTF82: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_RM) and not(expiredTIMER_MAX_RM_TIME or rm_request))))
	statechart->exit();
}
TEST_F(TestClass, BR_r_Main_FFFTFF102) {
	statechart->enter();
	//// test name test@BR_r_Main_FFFTFF102
	//// generated for (test goal): BR_r_Main_FFFTFF102: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_EXPIRATORYPAUSE) and (not expiredTIMER_MAX_EXP_PAUSE and exp_pause))))
	statechart->exit();
}
TEST_F(TestClass, BR_r_Main_FFFTFF122) {
	statechart->enter();
	//// test name test@BR_r_Main_FFFTFF122
	//// generated for (test goal): BR_r_Main_FFFTFF122: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PSV_R1_INSPIRATORYPAUSE) and (not expiredTIMER_MAX_INS_PAUSE and ins_pause))))
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	statechart->raiseStartupEnded();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_SELFTEST));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	statechart->raiseResume();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_VENTILATIONOFF));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	statechart->raiseStartVentilation();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , OPEN);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_INSPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , CLOSED);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , OPEN);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_INSPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , CLOSED);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , OPEN);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_INSPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , CLOSED);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , OPEN);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_INSPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , CLOSED);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , OPEN);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_INSPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , CLOSED);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , OPEN);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_INSPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , CLOSED);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	statechart->raiseStopVentilation();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , true);
	EXPECT_EQ(defaultMock->getInValveStatus() , OPEN);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_INSPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , CLOSED);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , true);
	EXPECT_EQ(defaultMock->getInValveStatus() , OPEN);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_INSPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , CLOSED);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , true);
	EXPECT_EQ(defaultMock->getInValveStatus() , OPEN);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_INSPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , CLOSED);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , true);
	EXPECT_EQ(defaultMock->getInValveStatus() , OPEN);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_INSPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , CLOSED);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(true);
	statechart->setIns_pause(true);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , true);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_INSPIRATORYPAUSE));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , CLOSED);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(true);
	statechart->setIns_pause(true);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , true);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_INSPIRATORYPAUSE));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , CLOSED);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	statechart->exit();
}
TEST_F(TestClass, BR_r_Main_FFFTFF152) {
	statechart->enter();
	//// test name test@BR_r_Main_FFFTFF152
	//// generated for (test goal): BR_r_Main_FFFTFF152: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PSV_R1_EXPIRATORYPAUSE) and (not expiredTIMER_MAX_EXP_PAUSE and exp_pause))))
	statechart->exit();
}
TEST_F(TestClass, BR_r_Main_FFFTFF62) {
	statechart->enter();
	//// test name test@BR_r_Main_FFFTFF62
	//// generated for (test goal): BR_r_Main_FFFTFF62: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_INSPIRATION) and (not expiredTIMER_INSPIRATION_DURATION_MS and not pawGTMaxPinsp))))
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	statechart->raiseStartupEnded();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_SELFTEST));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	statechart->raiseResume();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_VENTILATIONOFF));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	statechart->raiseStartVentilation();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , OPEN);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PCV_R1_INSPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , CLOSED);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , OPEN);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PCV_R1_INSPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , CLOSED);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	statechart->exit();
}
TEST_F(TestClass, BR_r_Main_FFFTFF72) {
	statechart->enter();
	//// test name test@BR_r_Main_FFFTFF72
	//// generated for (test goal): BR_r_Main_FFFTFF72: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_INSPIRATORYPAUSE) and (not expiredTIMER_MAX_INS_PAUSE and ins_pause))))
	statechart->exit();
}
TEST_F(TestClass, BR_r_Main_FFFTFF92) {
	statechart->enter();
	//// test name test@BR_r_Main_FFFTFF92
	//// generated for (test goal): BR_r_Main_FFFTFF92: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_EXPIRATION) and (not expiredTIMER_EXPIRATION_DURATION_MS and not expiredTIMER_TRIGGERWINDOWDELAY_MS))))
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	statechart->raiseStartupEnded();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_SELFTEST));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	statechart->raiseSelfTestPassed();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_VENTILATIONOFF));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	statechart->raiseStartVentilation();
	statechart->raiseStopVentilation();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , OPEN);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PCV_R1_INSPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , CLOSED);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(true);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PCV_R1_EXPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PCV_R1_EXPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	statechart->exit();
}
TEST_F(TestClass, BR_r_Main_FFFTFFFT142) {
	statechart->enter();
	//// test name test@BR_r_Main_FFFTFFFT142
	//// generated for (test goal): BR_r_Main_FFFTFFFT142: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PSV_R1_EXPIRATION) and (not(expiredTIMER_TRIGGERWINDOWDELAY_MS and dropPAW_ITS_PSV) and (not expiredTIMER_APNEALAG and (not((expiredTIMER_MIN_EXP_TIME_PSV and (mode = PSV)) and exp_pause) and (expiredTIMER_MIN_EXP_TIME_PSV and (mode = PCV))))))))
	statechart->exit();
}
TEST_F(TestClass, BR_r_Main_FFFTFFT142) {
	statechart->enter();
	//// test name test@BR_r_Main_FFFTFFT142
	//// generated for (test goal): BR_r_Main_FFFTFFT142: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PSV_R1_EXPIRATION) and (not(expiredTIMER_TRIGGERWINDOWDELAY_MS and dropPAW_ITS_PSV) and (not expiredTIMER_APNEALAG and ((expiredTIMER_MIN_EXP_TIME_PSV and (mode = PSV)) and exp_pause))))))
	statechart->exit();
}
TEST_F(TestClass, BR_r_Main_FFFTFT102) {
	statechart->enter();
	//// test name test@BR_r_Main_FFFTFT102
	//// generated for (test goal): BR_r_Main_FFFTFT102: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_EXPIRATORYPAUSE) and (not expiredTIMER_MAX_EXP_PAUSE and not exp_pause))))
	statechart->exit();
}
TEST_F(TestClass, BR_r_Main_FFFTFT142) {
	statechart->enter();
	//// test name test@BR_r_Main_FFFTFT142
	//// generated for (test goal): BR_r_Main_FFFTFT142: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PSV_R1_EXPIRATION) and (not(expiredTIMER_TRIGGERWINDOWDELAY_MS and dropPAW_ITS_PSV) and expiredTIMER_APNEALAG))))
	statechart->exit();
}
TEST_F(TestClass, BR_r_Main_FFFTFT152) {
	statechart->enter();
	//// test name test@BR_r_Main_FFFTFT152
	//// generated for (test goal): BR_r_Main_FFFTFT152: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PSV_R1_EXPIRATORYPAUSE) and (not expiredTIMER_MAX_EXP_PAUSE and not exp_pause))))
	statechart->exit();
}
TEST_F(TestClass, BR_r_Main_FFFTFT72) {
	statechart->enter();
	//// test name test@BR_r_Main_FFFTFT72
	//// generated for (test goal): BR_r_Main_FFFTFT72: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_INSPIRATORYPAUSE) and (not expiredTIMER_MAX_INS_PAUSE and not ins_pause))))
	statechart->exit();
}
TEST_F(TestClass, BR_r_Main_FFFTFTF92) {
	statechart->enter();
	//// test name test@BR_r_Main_FFFTFTF92
	//// generated for (test goal): BR_r_Main_FFFTFTF92: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_EXPIRATION) and (not expiredTIMER_EXPIRATION_DURATION_MS and (expiredTIMER_TRIGGERWINDOWDELAY_MS and not dropPAW_ITS_PCV)))))
	statechart->exit();
}
TEST_F(TestClass, BR_r_Main_FFFTFTT92) {
	statechart->enter();
	//// test name test@BR_r_Main_FFFTFTT92
	//// generated for (test goal): BR_r_Main_FFFTFTT92: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_EXPIRATION) and (not expiredTIMER_EXPIRATION_DURATION_MS and (expiredTIMER_TRIGGERWINDOWDELAY_MS and dropPAW_ITS_PCV)))))
	statechart->exit();
}
TEST_F(TestClass, BR_r_Main_FFFTT102) {
	statechart->enter();
	//// test name test@BR_r_Main_FFFTT102
	//// generated for (test goal): BR_r_Main_FFFTT102: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_EXPIRATORYPAUSE) and expiredTIMER_MAX_EXP_PAUSE)))
	statechart->exit();
}
TEST_F(TestClass, BR_r_Main_FFFTT12) {
	statechart->enter();
	//// test name test@BR_r_Main_FFFTT12
	//// generated for (test goal): BR_r_Main_FFFTT12: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (apneaAlarm and ((turnOffApneaAlarm = true) and ((state = MAIN_REGION_PCV_R1_INSPIRATION) or (state = MAIN_REGION_PCV_R1_EXPIRATION))))))
	statechart->exit();
}
TEST_F(TestClass, BR_r_Main_FFFTT122) {
	statechart->enter();
	//// test name test@BR_r_Main_FFFTT122
	//// generated for (test goal): BR_r_Main_FFFTT122: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PSV_R1_INSPIRATORYPAUSE) and expiredTIMER_MAX_INS_PAUSE)))
	statechart->exit();
}
TEST_F(TestClass, BR_r_Main_FFFTT132) {
	statechart->enter();
	//// test name test@BR_r_Main_FFFTT132
	//// generated for (test goal): BR_r_Main_FFFTT132: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PSV_R1_RM) and (expiredTIMER_MAX_RM_TIME or rm_request))))
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	statechart->raiseStartupEnded();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_SELFTEST));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	statechart->raiseSelfTestPassed();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_VENTILATIONOFF));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	statechart->raiseStartVentilation();
	statechart->raiseStopVentilation();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , OPEN);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_INSPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , CLOSED);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , OPEN);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_INSPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , CLOSED);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	statechart->raiseStopVentilation();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , true);
	EXPECT_EQ(defaultMock->getInValveStatus() , OPEN);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_INSPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , CLOSED);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , true);
	EXPECT_EQ(defaultMock->getInValveStatus() , OPEN);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_INSPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , CLOSED);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , true);
	EXPECT_EQ(defaultMock->getInValveStatus() , OPEN);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_INSPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , CLOSED);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , true);
	EXPECT_EQ(defaultMock->getInValveStatus() , OPEN);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_INSPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , CLOSED);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , true);
	EXPECT_EQ(defaultMock->getInValveStatus() , OPEN);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_INSPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , CLOSED);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , true);
	EXPECT_EQ(defaultMock->getInValveStatus() , OPEN);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_INSPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , CLOSED);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , true);
	EXPECT_EQ(defaultMock->getInValveStatus() , OPEN);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_INSPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , CLOSED);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , true);
	EXPECT_EQ(defaultMock->getInValveStatus() , OPEN);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_INSPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , CLOSED);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(true);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , true);
	EXPECT_EQ(defaultMock->getInValveStatus() , OPEN);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_RM));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , CLOSED);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(true);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , true);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_EXPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	statechart->exit();
}
TEST_F(TestClass, BR_r_Main_FFFTT142) {
	statechart->enter();
	//// test name test@BR_r_Main_FFFTT142
	//// generated for (test goal): BR_r_Main_FFFTT142: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PSV_R1_EXPIRATION) and (expiredTIMER_TRIGGERWINDOWDELAY_MS and dropPAW_ITS_PSV))))
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	statechart->raiseStartupEnded();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_SELFTEST));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	statechart->raiseResume();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_VENTILATIONOFF));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	statechart->raiseStartVentilation();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , OPEN);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_INSPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , CLOSED);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(true);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_EXPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_EXPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_EXPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_EXPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_EXPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_EXPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_EXPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_EXPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_EXPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(true);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_EXPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(true);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(true);
	statechart->setRm_request(true);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , OPEN);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_INSPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , CLOSED);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	statechart->exit();
}
TEST_F(TestClass, BR_r_Main_FFFTT152) {
	statechart->enter();
	//// test name test@BR_r_Main_FFFTT152
	//// generated for (test goal): BR_r_Main_FFFTT152: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PSV_R1_EXPIRATORYPAUSE) and expiredTIMER_MAX_EXP_PAUSE)))
	statechart->exit();
}
TEST_F(TestClass, BR_r_Main_FFFTT72) {
	statechart->enter();
	//// test name test@BR_r_Main_FFFTT72
	//// generated for (test goal): BR_r_Main_FFFTT72: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_INSPIRATORYPAUSE) and expiredTIMER_MAX_INS_PAUSE)))
	statechart->exit();
}
TEST_F(TestClass, BR_r_Main_FFFTT82) {
	statechart->enter();
	//// test name test@BR_r_Main_FFFTT82
	//// generated for (test goal): BR_r_Main_FFFTT82: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_RM) and (expiredTIMER_MAX_RM_TIME or rm_request))))
	statechart->exit();
}
TEST_F(TestClass, BR_r_Main_FFFTTF162) {
	statechart->enter();
	//// test name test@BR_r_Main_FFFTTF162
	//// generated for (test goal): BR_r_Main_FFFTTF162: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_INSPIRATION) and (expiredTIMER_INSPIRATION_DURATION_MS and (mode != PSV)))))
	statechart->exit();
}
TEST_F(TestClass, BR_r_Main_FFFTTF262) {
	statechart->enter();
	//// test name test@BR_r_Main_FFFTTF262
	//// generated for (test goal): BR_r_Main_FFFTTF262: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_INSPIRATION) and (expiredTIMER_INSPIRATION_DURATION_MS and (mode != PCV)))))
	statechart->exit();
}
TEST_F(TestClass, BR_r_Main_FFFTTF92) {
	statechart->enter();
	//// test name test@BR_r_Main_FFFTTF92
	//// generated for (test goal): BR_r_Main_FFFTTF92: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_EXPIRATION) and (expiredTIMER_EXPIRATION_DURATION_MS and not exp_pause))))
	statechart->exit();
}
TEST_F(TestClass, BR_r_Main_FFFTTFF112) {
	statechart->enter();
	//// test name test@BR_r_Main_FFFTTFF112
	//// generated for (test goal): BR_r_Main_FFFTTFF112: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PSV_R1_INSPIRATION) and (((flowDropPSV and expiredTIMER_MIN_INSP_TIME_MS) or expiredTIMER_MAX_INSP_TIME_PSV) and (not ins_pause and not rm_request)))))
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	statechart->raiseStartupEnded();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_SELFTEST));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	statechart->raiseSelfTestPassed();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_VENTILATIONOFF));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	statechart->raiseStartVentilation();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , OPEN);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_INSPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , CLOSED);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	statechart->raiseStopVentilation();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , true);
	EXPECT_EQ(defaultMock->getInValveStatus() , OPEN);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_INSPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , CLOSED);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , true);
	EXPECT_EQ(defaultMock->getInValveStatus() , OPEN);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_INSPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , CLOSED);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , true);
	EXPECT_EQ(defaultMock->getInValveStatus() , OPEN);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_INSPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , CLOSED);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , true);
	EXPECT_EQ(defaultMock->getInValveStatus() , OPEN);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_INSPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , CLOSED);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , true);
	EXPECT_EQ(defaultMock->getInValveStatus() , OPEN);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_INSPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , CLOSED);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , true);
	EXPECT_EQ(defaultMock->getInValveStatus() , OPEN);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_INSPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , CLOSED);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , true);
	EXPECT_EQ(defaultMock->getInValveStatus() , OPEN);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_INSPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , CLOSED);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , true);
	EXPECT_EQ(defaultMock->getInValveStatus() , OPEN);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_INSPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , CLOSED);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , true);
	EXPECT_EQ(defaultMock->getInValveStatus() , OPEN);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_INSPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , CLOSED);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(true);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , true);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_EXPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	statechart->exit();
}
TEST_F(TestClass, BR_r_Main_FFFTTFT112) {
	statechart->enter();
	//// test name test@BR_r_Main_FFFTTFT112
	//// generated for (test goal): BR_r_Main_FFFTTFT112: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PSV_R1_INSPIRATION) and (((flowDropPSV and expiredTIMER_MIN_INSP_TIME_MS) or expiredTIMER_MAX_INSP_TIME_PSV) and (not ins_pause and rm_request)))))
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	statechart->raiseStartupEnded();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_SELFTEST));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	statechart->raiseSelfTestPassed();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_VENTILATIONOFF));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	statechart->raiseStartVentilation();
	statechart->raiseStopVentilation();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , OPEN);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_INSPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , CLOSED);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , OPEN);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_INSPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , CLOSED);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	statechart->raiseStopVentilation();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , true);
	EXPECT_EQ(defaultMock->getInValveStatus() , OPEN);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_INSPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , CLOSED);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , true);
	EXPECT_EQ(defaultMock->getInValveStatus() , OPEN);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_INSPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , CLOSED);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , true);
	EXPECT_EQ(defaultMock->getInValveStatus() , OPEN);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_INSPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , CLOSED);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , true);
	EXPECT_EQ(defaultMock->getInValveStatus() , OPEN);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_INSPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , CLOSED);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , true);
	EXPECT_EQ(defaultMock->getInValveStatus() , OPEN);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_INSPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , CLOSED);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , true);
	EXPECT_EQ(defaultMock->getInValveStatus() , OPEN);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_INSPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , CLOSED);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , true);
	EXPECT_EQ(defaultMock->getInValveStatus() , OPEN);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_INSPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , CLOSED);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , true);
	EXPECT_EQ(defaultMock->getInValveStatus() , OPEN);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_INSPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , CLOSED);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(true);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , true);
	EXPECT_EQ(defaultMock->getInValveStatus() , OPEN);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_RM));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , CLOSED);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	statechart->exit();
}
TEST_F(TestClass, BR_r_Main_FFFTTT112) {
	statechart->enter();
	//// test name test@BR_r_Main_FFFTTT112
	//// generated for (test goal): BR_r_Main_FFFTTT112: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PSV_R1_INSPIRATION) and (((flowDropPSV and expiredTIMER_MIN_INSP_TIME_MS) or expiredTIMER_MAX_INSP_TIME_PSV) and ins_pause))))
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	statechart->raiseStartupEnded();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_SELFTEST));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	statechart->raiseResume();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_VENTILATIONOFF));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	statechart->raiseStartVentilation();
	statechart->raiseStopVentilation();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , OPEN);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_INSPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , CLOSED);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , OPEN);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_INSPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , CLOSED);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , OPEN);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_INSPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , CLOSED);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	statechart->raiseStopVentilation();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , true);
	EXPECT_EQ(defaultMock->getInValveStatus() , OPEN);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_INSPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , CLOSED);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , true);
	EXPECT_EQ(defaultMock->getInValveStatus() , OPEN);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_INSPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , CLOSED);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , true);
	EXPECT_EQ(defaultMock->getInValveStatus() , OPEN);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_INSPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , CLOSED);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , true);
	EXPECT_EQ(defaultMock->getInValveStatus() , OPEN);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_INSPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , CLOSED);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , true);
	EXPECT_EQ(defaultMock->getInValveStatus() , OPEN);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_INSPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , CLOSED);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , true);
	EXPECT_EQ(defaultMock->getInValveStatus() , OPEN);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_INSPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , CLOSED);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , true);
	EXPECT_EQ(defaultMock->getInValveStatus() , OPEN);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_INSPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , CLOSED);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(true);
	statechart->setIns_pause(true);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , true);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_INSPIRATORYPAUSE));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , CLOSED);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	statechart->exit();
}
TEST_F(TestClass, BR_r_Main_FFFTTT92) {
	statechart->enter();
	//// test name test@BR_r_Main_FFFTTT92
	//// generated for (test goal): BR_r_Main_FFFTTT92: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_EXPIRATION) and (expiredTIMER_EXPIRATION_DURATION_MS and exp_pause))))
	statechart->exit();
}
TEST_F(TestClass, BR_r_Main_FFFTTTFF162) {
	statechart->enter();
	//// test name test@BR_r_Main_FFFTTTFF162
	//// generated for (test goal): BR_r_Main_FFFTTTFF162: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_INSPIRATION) and (expiredTIMER_INSPIRATION_DURATION_MS and ((mode = PSV) and (not ins_pause and not rm_request))))))
	statechart->exit();
}
TEST_F(TestClass, BR_r_Main_FFFTTTFF262) {
	statechart->enter();
	//// test name test@BR_r_Main_FFFTTTFF262
	//// generated for (test goal): BR_r_Main_FFFTTTFF262: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_INSPIRATION) and (expiredTIMER_INSPIRATION_DURATION_MS and ((mode = PCV) and (not ins_pause and not rm_request))))))
	statechart->exit();
}
TEST_F(TestClass, BR_r_Main_FFFTTTFT162) {
	statechart->enter();
	//// test name test@BR_r_Main_FFFTTTFT162
	//// generated for (test goal): BR_r_Main_FFFTTTFT162: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_INSPIRATION) and (expiredTIMER_INSPIRATION_DURATION_MS and ((mode = PSV) and (not ins_pause and rm_request))))))
	statechart->exit();
}
TEST_F(TestClass, BR_r_Main_FFFTTTFT262) {
	statechart->enter();
	//// test name test@BR_r_Main_FFFTTTFT262
	//// generated for (test goal): BR_r_Main_FFFTTTFT262: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_INSPIRATION) and (expiredTIMER_INSPIRATION_DURATION_MS and ((mode = PCV) and (not ins_pause and rm_request))))))
	statechart->exit();
}
TEST_F(TestClass, BR_r_Main_FFFTTTT162) {
	statechart->enter();
	//// test name test@BR_r_Main_FFFTTTT162
	//// generated for (test goal): BR_r_Main_FFFTTTT162: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_INSPIRATION) and (expiredTIMER_INSPIRATION_DURATION_MS and ((mode = PSV) and ins_pause)))))
	statechart->exit();
}
TEST_F(TestClass, BR_r_Main_FFFTTTT262) {
	statechart->enter();
	//// test name test@BR_r_Main_FFFTTTT262
	//// generated for (test goal): BR_r_Main_FFFTTTT262: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_INSPIRATION) and (expiredTIMER_INSPIRATION_DURATION_MS and ((mode = PCV) and ins_pause)))))
	statechart->exit();
}
TEST_F(TestClass, BR_r_Main_FFT2) {
	statechart->enter();
	//// test name test@BR_r_Main_FFT2
	//// generated for (test goal): BR_r_Main_FFT2: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)))
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	statechart->raiseStartupEnded();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_SELFTEST));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	statechart->raiseSelfTestPassed();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_VENTILATIONOFF));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	statechart->raiseStartVentilation();
	statechart->raiseStopVentilation();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , OPEN);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_INSPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , CLOSED);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(true);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	statechart->raiseStopVentilation();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , true);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_EXPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_VENTILATIONOFF));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	statechart->exit();
}
TEST_F(TestClass, BR_r_Main_FT2) {
	statechart->enter();
	//// test name test@BR_r_Main_FT2
	//// generated for (test goal): BR_r_Main_FT2: not poweroff and ((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation))
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	statechart->raiseStartupEnded();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_SELFTEST));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	statechart->raiseSelfTestPassed();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_VENTILATIONOFF));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	statechart->raiseStartVentilation();
	statechart->raiseStopVentilation();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , OPEN);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PCV_R1_INSPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , CLOSED);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(true);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	statechart->raiseStopVentilation();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , true);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PCV_R1_EXPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_VENTILATIONOFF));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	statechart->exit();
}
TEST_F(TestClass, BR_r_Main_T2) {
	statechart->enter();
	//// test name test@BR_r_Main_T2
	//// generated for (test goal): BR_r_Main_T2: poweroff
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	statechart->raisePoweroff();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION__final_));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	statechart->exit();
}
TEST_F(TestClass, MCDC_r_Main_F_F_F_T102) {
	statechart->enter();
	//// test name test@MCDC_r_Main_F_F_F_T102
	//// generated for (test goal): MCDC_r_Main_F_F_F_T102: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (state = MAIN_REGION_PCV_R1_EXPIRATORYPAUSE)))
	statechart->exit();
}
TEST_F(TestClass, MCDC_r_Main_F_F_F_T12) {
	statechart->enter();
	//// test name test@MCDC_r_Main_F_F_F_T12
	//// generated for (test goal): MCDC_r_Main_F_F_F_T12: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and apneaAlarm))
	statechart->exit();
}
TEST_F(TestClass, MCDC_r_Main_F_F_F_T152) {
	statechart->enter();
	//// test name test@MCDC_r_Main_F_F_F_T152
	//// generated for (test goal): MCDC_r_Main_F_F_F_T152: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (state = MAIN_REGION_PSV_R1_EXPIRATORYPAUSE)))
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	statechart->raiseStartupEnded();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_SELFTEST));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	statechart->raiseResume();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_VENTILATIONOFF));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	statechart->raiseStartVentilation();
	statechart->raiseStopVentilation();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , OPEN);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_INSPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , CLOSED);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(true);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_EXPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_EXPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_EXPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_EXPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_EXPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_EXPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_EXPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_EXPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_EXPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(true);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_EXPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(true);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(true);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_EXPIRATORYPAUSE));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , CLOSED);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(true);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	statechart->raiseStopVentilation();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , true);
	EXPECT_EQ(defaultMock->getInValveStatus() , OPEN);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_INSPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , CLOSED);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	statechart->exit();
}
TEST_F(TestClass, MCDC_r_Main_F_F_F_T72) {
	statechart->enter();
	//// test name test@MCDC_r_Main_F_F_F_T72
	//// generated for (test goal): MCDC_r_Main_F_F_F_T72: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (state = MAIN_REGION_PCV_R1_INSPIRATORYPAUSE)))
	statechart->exit();
}
TEST_F(TestClass, MCDC_r_Main_F_F_F_T82) {
	statechart->enter();
	//// test name test@MCDC_r_Main_F_F_F_T82
	//// generated for (test goal): MCDC_r_Main_F_F_F_T82: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (state = MAIN_REGION_PCV_R1_RM)))
	statechart->exit();
}
TEST_F(TestClass, MCDC_r_Main_F_F_F_T_F102) {
	statechart->enter();
	//// test name test@MCDC_r_Main_F_F_F_T_F102
	//// generated for (test goal): MCDC_r_Main_F_F_F_T_F102: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_EXPIRATORYPAUSE) and not expiredTIMER_MAX_EXP_PAUSE)))
	statechart->exit();
}
TEST_F(TestClass, MCDC_r_Main_F_F_F_T_F72) {
	statechart->enter();
	//// test name test@MCDC_r_Main_F_F_F_T_F72
	//// generated for (test goal): MCDC_r_Main_F_F_F_T_F72: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_INSPIRATORYPAUSE) and not expiredTIMER_MAX_INS_PAUSE)))
	statechart->exit();
}
TEST_F(TestClass, MCDC_r_Main_F_F_F_T_FF82) {
	statechart->enter();
	//// test name test@MCDC_r_Main_F_F_F_T_FF82
	//// generated for (test goal): MCDC_r_Main_F_F_F_T_FF82: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_RM) and (not expiredTIMER_MAX_RM_TIME and not rm_request))))
	statechart->exit();
}
TEST_F(TestClass, MCDC_r_Main_F_F_F_T_FT112) {
	statechart->enter();
	//// test name test@MCDC_r_Main_F_F_F_T_FT112
	//// generated for (test goal): MCDC_r_Main_F_F_F_T_FT112: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PSV_R1_INSPIRATION) and (not(flowDropPSV and expiredTIMER_MIN_INSP_TIME_MS) and expiredTIMER_MAX_INSP_TIME_PSV))))
	statechart->exit();
}
TEST_F(TestClass, MCDC_r_Main_F_F_F_T_FT12) {
	statechart->enter();
	//// test name test@MCDC_r_Main_F_F_F_T_FT12
	//// generated for (test goal): MCDC_r_Main_F_F_F_T_FT12: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (apneaAlarm and ((turnOffApneaAlarm != true) and ((state = MAIN_REGION_PCV_R1_INSPIRATION) or (state = MAIN_REGION_PCV_R1_EXPIRATION))))))
	statechart->exit();
}
TEST_F(TestClass, MCDC_r_Main_F_F_F_T_FT142) {
	statechart->enter();
	//// test name test@MCDC_r_Main_F_F_F_T_FT142
	//// generated for (test goal): MCDC_r_Main_F_F_F_T_FT142: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PSV_R1_EXPIRATION) and (not expiredTIMER_TRIGGERWINDOWDELAY_MS and dropPAW_ITS_PSV))))
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	statechart->raiseStartupEnded();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_SELFTEST));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	statechart->raiseSelfTestPassed();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_VENTILATIONOFF));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	statechart->raiseStartVentilation();
	statechart->raiseStopVentilation();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , OPEN);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_INSPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , CLOSED);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(true);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_EXPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(true);
	statechart->setRm_request(true);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_EXPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	statechart->exit();
}
TEST_F(TestClass, MCDC_r_Main_F_F_F_T_FT82) {
	statechart->enter();
	//// test name test@MCDC_r_Main_F_F_F_T_FT82
	//// generated for (test goal): MCDC_r_Main_F_F_F_T_FT82: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_RM) and (not expiredTIMER_MAX_RM_TIME and rm_request))))
	statechart->exit();
}
TEST_F(TestClass, MCDC_r_Main_F_F_F_T_FTF112) {
	statechart->enter();
	//// test name test@MCDC_r_Main_F_F_F_T_FTF112
	//// generated for (test goal): MCDC_r_Main_F_F_F_T_FTF112: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PSV_R1_INSPIRATION) and ((not flowDropPSV and expiredTIMER_MIN_INSP_TIME_MS) and not expiredTIMER_MAX_INSP_TIME_PSV))))
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	statechart->raiseStartupEnded();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_SELFTEST));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	statechart->raiseResume();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_VENTILATIONOFF));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	statechart->raiseStartVentilation();
	statechart->raiseStopVentilation();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , OPEN);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_INSPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , CLOSED);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	statechart->raiseStopVentilation();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , true);
	EXPECT_EQ(defaultMock->getInValveStatus() , OPEN);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_INSPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , CLOSED);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , true);
	EXPECT_EQ(defaultMock->getInValveStatus() , OPEN);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_INSPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , CLOSED);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , true);
	EXPECT_EQ(defaultMock->getInValveStatus() , OPEN);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_INSPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , CLOSED);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , true);
	EXPECT_EQ(defaultMock->getInValveStatus() , OPEN);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_INSPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , CLOSED);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , true);
	EXPECT_EQ(defaultMock->getInValveStatus() , OPEN);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_INSPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , CLOSED);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , true);
	EXPECT_EQ(defaultMock->getInValveStatus() , OPEN);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_INSPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , CLOSED);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , true);
	EXPECT_EQ(defaultMock->getInValveStatus() , OPEN);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_INSPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , CLOSED);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , true);
	EXPECT_EQ(defaultMock->getInValveStatus() , OPEN);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_INSPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , CLOSED);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , true);
	EXPECT_EQ(defaultMock->getInValveStatus() , OPEN);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_INSPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , CLOSED);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(true);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , true);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_EXPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	statechart->exit();
}
TEST_F(TestClass, MCDC_r_Main_F_F_F_T_FTTTT22) {
	statechart->enter();
	//// test name test@MCDC_r_Main_F_F_F_T_FTTTT22
	//// generated for (test goal): MCDC_r_Main_F_F_F_T_FTTTT22: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (stopVentilation and (((((state = MAIN_REGION_PSV_R1_EXPIRATION) and (state != MAIN_REGION_PCV_R1_EXPIRATION)) and (state != MAIN_REGION_STARTUP)) and (state != MAIN_REGION_SELFTEST)) and (state != MAIN_REGION_VENTILATIONOFF)))))
	statechart->exit();
}
TEST_F(TestClass, MCDC_r_Main_F_F_F_T_F_F102) {
	statechart->enter();
	//// test name test@MCDC_r_Main_F_F_F_T_F_F102
	//// generated for (test goal): MCDC_r_Main_F_F_F_T_F_F102: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_EXPIRATORYPAUSE) and (not expiredTIMER_MAX_EXP_PAUSE and not exp_pause))))
	statechart->exit();
}
TEST_F(TestClass, MCDC_r_Main_F_F_F_T_F_F72) {
	statechart->enter();
	//// test name test@MCDC_r_Main_F_F_F_T_F_F72
	//// generated for (test goal): MCDC_r_Main_F_F_F_T_F_F72: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_INSPIRATORYPAUSE) and (not expiredTIMER_MAX_INS_PAUSE and not ins_pause))))
	statechart->exit();
}
TEST_F(TestClass, MCDC_r_Main_F_F_F_T_F_F_FTT142) {
	statechart->enter();
	//// test name test@MCDC_r_Main_F_F_F_T_F_F_FTT142
	//// generated for (test goal): MCDC_r_Main_F_F_F_T_F_F_FTT142: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PSV_R1_EXPIRATION) and (not(expiredTIMER_TRIGGERWINDOWDELAY_MS and dropPAW_ITS_PSV) and (not expiredTIMER_APNEALAG and ((not expiredTIMER_MIN_EXP_TIME_PSV and (mode = PSV)) and exp_pause))))))
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	statechart->raiseStartupEnded();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_SELFTEST));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	statechart->raiseSelfTestPassed();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_VENTILATIONOFF));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	statechart->raiseStartVentilation();
	statechart->raiseStopVentilation();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , OPEN);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_INSPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , CLOSED);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(true);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_EXPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(true);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_EXPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	statechart->exit();
}
TEST_F(TestClass, MCDC_r_Main_F_F_F_T_F_F_F_TF142) {
	statechart->enter();
	//// test name test@MCDC_r_Main_F_F_F_T_F_F_F_TF142
	//// generated for (test goal): MCDC_r_Main_F_F_F_T_F_F_F_TF142: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PSV_R1_EXPIRATION) and (not(expiredTIMER_TRIGGERWINDOWDELAY_MS and dropPAW_ITS_PSV) and (not expiredTIMER_APNEALAG and (not((expiredTIMER_MIN_EXP_TIME_PSV and (mode = PSV)) and exp_pause) and (expiredTIMER_MIN_EXP_TIME_PSV and (mode != PCV))))))))
	statechart->exit();
}
TEST_F(TestClass, MCDC_r_Main_F_F_F_T_F_F_TF142) {
	statechart->enter();
	//// test name test@MCDC_r_Main_F_F_F_T_F_F_TF142
	//// generated for (test goal): MCDC_r_Main_F_F_F_T_F_F_TF142: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PSV_R1_EXPIRATION) and (not(expiredTIMER_TRIGGERWINDOWDELAY_MS and dropPAW_ITS_PSV) and (not expiredTIMER_APNEALAG and ((expiredTIMER_MIN_EXP_TIME_PSV and (mode = PSV)) and not exp_pause))))))
	statechart->exit();
}
TEST_F(TestClass, MCDC_r_Main_F_F_F_T_F_F_TFT142) {
	statechart->enter();
	//// test name test@MCDC_r_Main_F_F_F_T_F_F_TFT142
	//// generated for (test goal): MCDC_r_Main_F_F_F_T_F_F_TFT142: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PSV_R1_EXPIRATION) and (not(expiredTIMER_TRIGGERWINDOWDELAY_MS and dropPAW_ITS_PSV) and (not expiredTIMER_APNEALAG and ((expiredTIMER_MIN_EXP_TIME_PSV and (mode != PSV)) and exp_pause))))))
	statechart->exit();
}
TEST_F(TestClass, MCDC_r_Main_F_F_F_T_F_F_TT142) {
	statechart->enter();
	//// test name test@MCDC_r_Main_F_F_F_T_F_F_TT142
	//// generated for (test goal): MCDC_r_Main_F_F_F_T_F_F_TT142: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PSV_R1_EXPIRATION) and (not(expiredTIMER_TRIGGERWINDOWDELAY_MS and dropPAW_ITS_PSV) and (not expiredTIMER_APNEALAG and ((expiredTIMER_MIN_EXP_TIME_PSV and (mode = PSV)) and exp_pause))))))
	statechart->exit();
}
TEST_F(TestClass, MCDC_r_Main_F_F_F_T_F_F_TTT142) {
	statechart->enter();
	//// test name test@MCDC_r_Main_F_F_F_T_F_F_TTT142
	//// generated for (test goal): MCDC_r_Main_F_F_F_T_F_F_TTT142: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PSV_R1_EXPIRATION) and (not(expiredTIMER_TRIGGERWINDOWDELAY_MS and dropPAW_ITS_PSV) and (not expiredTIMER_APNEALAG and ((expiredTIMER_MIN_EXP_TIME_PSV and (mode = PSV)) and exp_pause))))))
	statechart->exit();
}
TEST_F(TestClass, MCDC_r_Main_F_F_F_T_F_T102) {
	statechart->enter();
	//// test name test@MCDC_r_Main_F_F_F_T_F_T102
	//// generated for (test goal): MCDC_r_Main_F_F_F_T_F_T102: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_EXPIRATORYPAUSE) and (not expiredTIMER_MAX_EXP_PAUSE and exp_pause))))
	statechart->exit();
}
TEST_F(TestClass, MCDC_r_Main_F_F_F_T_F_T142) {
	statechart->enter();
	//// test name test@MCDC_r_Main_F_F_F_T_F_T142
	//// generated for (test goal): MCDC_r_Main_F_F_F_T_F_T142: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PSV_R1_EXPIRATION) and (not(expiredTIMER_TRIGGERWINDOWDELAY_MS and dropPAW_ITS_PSV) and expiredTIMER_APNEALAG))))
	statechart->exit();
}
TEST_F(TestClass, MCDC_r_Main_F_F_F_T_F_T152) {
	statechart->enter();
	//// test name test@MCDC_r_Main_F_F_F_T_F_T152
	//// generated for (test goal): MCDC_r_Main_F_F_F_T_F_T152: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PSV_R1_EXPIRATORYPAUSE) and (not expiredTIMER_MAX_EXP_PAUSE and exp_pause))))
	statechart->exit();
}
TEST_F(TestClass, MCDC_r_Main_F_F_F_T_F_T72) {
	statechart->enter();
	//// test name test@MCDC_r_Main_F_F_F_T_F_T72
	//// generated for (test goal): MCDC_r_Main_F_F_F_T_F_T72: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_INSPIRATORYPAUSE) and (not expiredTIMER_MAX_INS_PAUSE and ins_pause))))
	statechart->exit();
}
TEST_F(TestClass, MCDC_r_Main_F_F_F_T_F_T92) {
	statechart->enter();
	//// test name test@MCDC_r_Main_F_F_F_T_F_T92
	//// generated for (test goal): MCDC_r_Main_F_F_F_T_F_T92: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_EXPIRATION) and (not expiredTIMER_EXPIRATION_DURATION_MS and expiredTIMER_TRIGGERWINDOWDELAY_MS))))
	statechart->exit();
}
TEST_F(TestClass, MCDC_r_Main_F_F_F_T_F_T_F92) {
	statechart->enter();
	//// test name test@MCDC_r_Main_F_F_F_T_F_T_F92
	//// generated for (test goal): MCDC_r_Main_F_F_F_T_F_T_F92: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_EXPIRATION) and (not expiredTIMER_EXPIRATION_DURATION_MS and (expiredTIMER_TRIGGERWINDOWDELAY_MS and not dropPAW_ITS_PCV)))))
	statechart->exit();
}
TEST_F(TestClass, MCDC_r_Main_F_F_F_T_F_T_T92) {
	statechart->enter();
	//// test name test@MCDC_r_Main_F_F_F_T_F_T_T92
	//// generated for (test goal): MCDC_r_Main_F_F_F_T_F_T_T92: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_EXPIRATION) and (not expiredTIMER_EXPIRATION_DURATION_MS and (expiredTIMER_TRIGGERWINDOWDELAY_MS and dropPAW_ITS_PCV)))))
	statechart->exit();
}
TEST_F(TestClass, MCDC_r_Main_F_F_F_T_T102) {
	statechart->enter();
	//// test name test@MCDC_r_Main_F_F_F_T_T102
	//// generated for (test goal): MCDC_r_Main_F_F_F_T_T102: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_EXPIRATORYPAUSE) and expiredTIMER_MAX_EXP_PAUSE)))
	statechart->exit();
}
TEST_F(TestClass, MCDC_r_Main_F_F_F_T_T122) {
	statechart->enter();
	//// test name test@MCDC_r_Main_F_F_F_T_T122
	//// generated for (test goal): MCDC_r_Main_F_F_F_T_T122: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PSV_R1_INSPIRATORYPAUSE) and expiredTIMER_MAX_INS_PAUSE)))
	statechart->exit();
}
TEST_F(TestClass, MCDC_r_Main_F_F_F_T_T152) {
	statechart->enter();
	//// test name test@MCDC_r_Main_F_F_F_T_T152
	//// generated for (test goal): MCDC_r_Main_F_F_F_T_T152: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PSV_R1_EXPIRATORYPAUSE) and expiredTIMER_MAX_EXP_PAUSE)))
	statechart->exit();
}
TEST_F(TestClass, MCDC_r_Main_F_F_F_T_T62) {
	statechart->enter();
	//// test name test@MCDC_r_Main_F_F_F_T_T62
	//// generated for (test goal): MCDC_r_Main_F_F_F_T_T62: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_INSPIRATION) and expiredTIMER_INSPIRATION_DURATION_MS)))
	statechart->exit();
}
TEST_F(TestClass, MCDC_r_Main_F_F_F_T_T72) {
	statechart->enter();
	//// test name test@MCDC_r_Main_F_F_F_T_T72
	//// generated for (test goal): MCDC_r_Main_F_F_F_T_T72: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_INSPIRATORYPAUSE) and expiredTIMER_MAX_INS_PAUSE)))
	statechart->exit();
}
TEST_F(TestClass, MCDC_r_Main_F_F_F_T_T92) {
	statechart->enter();
	//// test name test@MCDC_r_Main_F_F_F_T_T92
	//// generated for (test goal): MCDC_r_Main_F_F_F_T_T92: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_EXPIRATION) and expiredTIMER_EXPIRATION_DURATION_MS)))
	statechart->exit();
}
TEST_F(TestClass, MCDC_r_Main_F_F_F_T_TF132) {
	statechart->enter();
	//// test name test@MCDC_r_Main_F_F_F_T_TF132
	//// generated for (test goal): MCDC_r_Main_F_F_F_T_TF132: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PSV_R1_RM) and (expiredTIMER_MAX_RM_TIME and not rm_request))))
	statechart->exit();
}
TEST_F(TestClass, MCDC_r_Main_F_F_F_T_TF142) {
	statechart->enter();
	//// test name test@MCDC_r_Main_F_F_F_T_TF142
	//// generated for (test goal): MCDC_r_Main_F_F_F_T_TF142: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PSV_R1_EXPIRATION) and (expiredTIMER_TRIGGERWINDOWDELAY_MS and not dropPAW_ITS_PSV))))
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	statechart->raiseStartupEnded();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_SELFTEST));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	statechart->raiseSelfTestPassed();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_VENTILATIONOFF));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	statechart->raiseStartVentilation();
	statechart->raiseStopVentilation();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , OPEN);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_INSPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , CLOSED);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(true);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_EXPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_EXPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_EXPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_EXPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_EXPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_EXPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_EXPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_EXPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_EXPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(true);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_EXPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(true);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , OPEN);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PCV_R1_INSPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , CLOSED);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	statechart->exit();
}
TEST_F(TestClass, MCDC_r_Main_F_F_F_T_TF82) {
	statechart->enter();
	//// test name test@MCDC_r_Main_F_F_F_T_TF82
	//// generated for (test goal): MCDC_r_Main_F_F_F_T_TF82: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_RM) and (expiredTIMER_MAX_RM_TIME and not rm_request))))
	statechart->exit();
}
TEST_F(TestClass, MCDC_r_Main_F_F_F_T_TFF112) {
	statechart->enter();
	//// test name test@MCDC_r_Main_F_F_F_T_TFF112
	//// generated for (test goal): MCDC_r_Main_F_F_F_T_TFF112: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PSV_R1_INSPIRATION) and ((flowDropPSV and not expiredTIMER_MIN_INSP_TIME_MS) and not expiredTIMER_MAX_INSP_TIME_PSV))))
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	statechart->raiseStartupEnded();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_SELFTEST));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	statechart->raiseSelfTestPassed();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_VENTILATIONOFF));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	statechart->raiseStartVentilation();
	statechart->raiseStopVentilation();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , OPEN);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_INSPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , CLOSED);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(true);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(true);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	statechart->raiseStopVentilation();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , true);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_EXPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	statechart->exit();
}
TEST_F(TestClass, MCDC_r_Main_F_F_F_T_TFF12) {
	statechart->enter();
	//// test name test@MCDC_r_Main_F_F_F_T_TFF12
	//// generated for (test goal): MCDC_r_Main_F_F_F_T_TFF12: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (apneaAlarm and ((turnOffApneaAlarm = true) and ((state != MAIN_REGION_PCV_R1_INSPIRATION) and (state != MAIN_REGION_PCV_R1_EXPIRATION))))))
	statechart->exit();
}
TEST_F(TestClass, MCDC_r_Main_F_F_F_T_TFT12) {
	statechart->enter();
	//// test name test@MCDC_r_Main_F_F_F_T_TFT12
	//// generated for (test goal): MCDC_r_Main_F_F_F_T_TFT12: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (apneaAlarm and ((turnOffApneaAlarm = true) and ((state != MAIN_REGION_PCV_R1_INSPIRATION) and (state = MAIN_REGION_PCV_R1_EXPIRATION))))))
	statechart->exit();
}
TEST_F(TestClass, MCDC_r_Main_F_F_F_T_TFT22) {
	statechart->enter();
	//// test name test@MCDC_r_Main_F_F_F_T_TFT22
	//// generated for (test goal): MCDC_r_Main_F_F_F_T_TFT22: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (stopVentilation and (((((state != MAIN_REGION_PSV_R1_EXPIRATION) and (state != MAIN_REGION_PCV_R1_EXPIRATION)) and (state != MAIN_REGION_STARTUP)) and (state = MAIN_REGION_SELFTEST)) and (state != MAIN_REGION_VENTILATIONOFF)))))
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	statechart->raiseStartupEnded();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_SELFTEST));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	statechart->raiseStopVentilation();
	statechart->raiseResume();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_VENTILATIONOFF));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	statechart->exit();
}
TEST_F(TestClass, MCDC_r_Main_F_F_F_T_TFTT22) {
	statechart->enter();
	//// test name test@MCDC_r_Main_F_F_F_T_TFTT22
	//// generated for (test goal): MCDC_r_Main_F_F_F_T_TFTT22: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (stopVentilation and (((((state != MAIN_REGION_PSV_R1_EXPIRATION) and (state != MAIN_REGION_PCV_R1_EXPIRATION)) and (state = MAIN_REGION_STARTUP)) and (state != MAIN_REGION_SELFTEST)) and (state != MAIN_REGION_VENTILATIONOFF)))))
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	statechart->raiseStopVentilation();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	statechart->exit();
}
TEST_F(TestClass, MCDC_r_Main_F_F_F_T_TFTTT22) {
	statechart->enter();
	//// test name test@MCDC_r_Main_F_F_F_T_TFTTT22
	//// generated for (test goal): MCDC_r_Main_F_F_F_T_TFTTT22: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (stopVentilation and (((((state != MAIN_REGION_PSV_R1_EXPIRATION) and (state = MAIN_REGION_PCV_R1_EXPIRATION)) and (state != MAIN_REGION_STARTUP)) and (state != MAIN_REGION_SELFTEST)) and (state != MAIN_REGION_VENTILATIONOFF)))))
	statechart->exit();
}
TEST_F(TestClass, MCDC_r_Main_F_F_F_T_TT12) {
	statechart->enter();
	//// test name test@MCDC_r_Main_F_F_F_T_TT12
	//// generated for (test goal): MCDC_r_Main_F_F_F_T_TT12: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (apneaAlarm and ((turnOffApneaAlarm = true) and ((state = MAIN_REGION_PCV_R1_INSPIRATION) or (state = MAIN_REGION_PCV_R1_EXPIRATION))))))
	statechart->exit();
}
TEST_F(TestClass, MCDC_r_Main_F_F_F_T_TTF12) {
	statechart->enter();
	//// test name test@MCDC_r_Main_F_F_F_T_TTF12
	//// generated for (test goal): MCDC_r_Main_F_F_F_T_TTF12: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (apneaAlarm and ((turnOffApneaAlarm = true) and ((state = MAIN_REGION_PCV_R1_INSPIRATION) and (state != MAIN_REGION_PCV_R1_EXPIRATION))))))
	statechart->exit();
}
TEST_F(TestClass, MCDC_r_Main_F_F_F_T_T_F162) {
	statechart->enter();
	//// test name test@MCDC_r_Main_F_F_F_T_T_F162
	//// generated for (test goal): MCDC_r_Main_F_F_F_T_T_F162: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_INSPIRATION) and (expiredTIMER_INSPIRATION_DURATION_MS and (mode != PSV)))))
	statechart->exit();
}
TEST_F(TestClass, MCDC_r_Main_F_F_F_T_T_F262) {
	statechart->enter();
	//// test name test@MCDC_r_Main_F_F_F_T_T_F262
	//// generated for (test goal): MCDC_r_Main_F_F_F_T_T_F262: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_INSPIRATION) and (expiredTIMER_INSPIRATION_DURATION_MS and (mode != PCV)))))
	statechart->exit();
}
TEST_F(TestClass, MCDC_r_Main_F_F_F_T_T_F92) {
	statechart->enter();
	//// test name test@MCDC_r_Main_F_F_F_T_T_F92
	//// generated for (test goal): MCDC_r_Main_F_F_F_T_T_F92: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_EXPIRATION) and (expiredTIMER_EXPIRATION_DURATION_MS and not exp_pause))))
	statechart->exit();
}
TEST_F(TestClass, MCDC_r_Main_F_F_F_T_T_T162) {
	statechart->enter();
	//// test name test@MCDC_r_Main_F_F_F_T_T_T162
	//// generated for (test goal): MCDC_r_Main_F_F_F_T_T_T162: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_INSPIRATION) and (expiredTIMER_INSPIRATION_DURATION_MS and (mode = PSV)))))
	statechart->exit();
}
TEST_F(TestClass, MCDC_r_Main_F_F_F_T_T_T262) {
	statechart->enter();
	//// test name test@MCDC_r_Main_F_F_F_T_T_T262
	//// generated for (test goal): MCDC_r_Main_F_F_F_T_T_T262: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_INSPIRATION) and (expiredTIMER_INSPIRATION_DURATION_MS and (mode = PCV)))))
	statechart->exit();
}
TEST_F(TestClass, MCDC_r_Main_F_F_F_T_T_T92) {
	statechart->enter();
	//// test name test@MCDC_r_Main_F_F_F_T_T_T92
	//// generated for (test goal): MCDC_r_Main_F_F_F_T_T_T92: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_EXPIRATION) and (expiredTIMER_EXPIRATION_DURATION_MS and exp_pause))))
	statechart->exit();
}
TEST_F(TestClass, MCDC_r_Main_F_F_F_T_T_T_F162) {
	statechart->enter();
	//// test name test@MCDC_r_Main_F_F_F_T_T_T_F162
	//// generated for (test goal): MCDC_r_Main_F_F_F_T_T_T_F162: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_INSPIRATION) and (expiredTIMER_INSPIRATION_DURATION_MS and ((mode = PSV) and not ins_pause)))))
	statechart->exit();
}
TEST_F(TestClass, MCDC_r_Main_F_F_F_T_T_T_F262) {
	statechart->enter();
	//// test name test@MCDC_r_Main_F_F_F_T_T_T_F262
	//// generated for (test goal): MCDC_r_Main_F_F_F_T_T_T_F262: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_INSPIRATION) and (expiredTIMER_INSPIRATION_DURATION_MS and ((mode = PCV) and not ins_pause)))))
	statechart->exit();
}
TEST_F(TestClass, MCDC_r_Main_F_F_F_T_T_T_F_F162) {
	statechart->enter();
	//// test name test@MCDC_r_Main_F_F_F_T_T_T_F_F162
	//// generated for (test goal): MCDC_r_Main_F_F_F_T_T_T_F_F162: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_INSPIRATION) and (expiredTIMER_INSPIRATION_DURATION_MS and ((mode = PSV) and (not ins_pause and not rm_request))))))
	statechart->exit();
}
TEST_F(TestClass, MCDC_r_Main_F_F_F_T_T_T_F_F262) {
	statechart->enter();
	//// test name test@MCDC_r_Main_F_F_F_T_T_T_F_F262
	//// generated for (test goal): MCDC_r_Main_F_F_F_T_T_T_F_F262: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_INSPIRATION) and (expiredTIMER_INSPIRATION_DURATION_MS and ((mode = PCV) and (not ins_pause and not rm_request))))))
	statechart->exit();
}
TEST_F(TestClass, MCDC_r_Main_F_F_F_T_T_T_F_T162) {
	statechart->enter();
	//// test name test@MCDC_r_Main_F_F_F_T_T_T_F_T162
	//// generated for (test goal): MCDC_r_Main_F_F_F_T_T_T_F_T162: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_INSPIRATION) and (expiredTIMER_INSPIRATION_DURATION_MS and ((mode = PSV) and (not ins_pause and rm_request))))))
	statechart->exit();
}
TEST_F(TestClass, MCDC_r_Main_F_F_F_T_T_T_F_T262) {
	statechart->enter();
	//// test name test@MCDC_r_Main_F_F_F_T_T_T_F_T262
	//// generated for (test goal): MCDC_r_Main_F_F_F_T_T_T_F_T262: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_INSPIRATION) and (expiredTIMER_INSPIRATION_DURATION_MS and ((mode = PCV) and (not ins_pause and rm_request))))))
	statechart->exit();
}
TEST_F(TestClass, MCDC_r_Main_F_F_F_T_T_T_T162) {
	statechart->enter();
	//// test name test@MCDC_r_Main_F_F_F_T_T_T_T162
	//// generated for (test goal): MCDC_r_Main_F_F_F_T_T_T_T162: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_INSPIRATION) and (expiredTIMER_INSPIRATION_DURATION_MS and ((mode = PSV) and ins_pause)))))
	statechart->exit();
}
TEST_F(TestClass, MCDC_r_Main_F_F_F_T_T_T_T262) {
	statechart->enter();
	//// test name test@MCDC_r_Main_F_F_F_T_T_T_T262
	//// generated for (test goal): MCDC_r_Main_F_F_F_T_T_T_T262: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_INSPIRATION) and (expiredTIMER_INSPIRATION_DURATION_MS and ((mode = PCV) and ins_pause)))))
	statechart->exit();
}
TEST_F(TestClass, MCDC_r_Main_F_F_TFT2) {
	statechart->enter();
	//// test name test@MCDC_r_Main_F_F_TFT2
	//// generated for (test goal): MCDC_r_Main_F_F_TFT2: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PSV_R1_EXPIRATION) and (not stopVentilationRequested and stopVentilation)))
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	statechart->raiseStartupEnded();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_SELFTEST));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	statechart->raiseSelfTestPassed();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_VENTILATIONOFF));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	statechart->raiseStartVentilation();
	statechart->raiseStopVentilation();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , OPEN);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_INSPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , CLOSED);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(true);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_EXPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	statechart->raiseStopVentilation();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_VENTILATIONOFF));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	statechart->exit();
}
TEST_F(TestClass, MCDC_r_Main_F_TFT2) {
	statechart->enter();
	//// test name test@MCDC_r_Main_F_TFT2
	//// generated for (test goal): MCDC_r_Main_F_TFT2: not poweroff and ((state = MAIN_REGION_PCV_R1_EXPIRATION) and (not stopVentilationRequested and stopVentilation))
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	statechart->raiseStartupEnded();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_SELFTEST));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	statechart->raiseResume();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_VENTILATIONOFF));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	statechart->raiseStartVentilation();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , OPEN);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PCV_R1_INSPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , CLOSED);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(true);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PCV_R1_EXPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	statechart->raiseStopVentilation();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_VENTILATIONOFF));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	statechart->exit();
}
TEST_F(TestClass, pair_10_2_11_2) {
	statechart->enter();
	//// test name test@pair_10_2_11_2
	//// generated for (test goal): startupEnded=true - turnOffApneaAlarm=true
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	statechart->raiseStartupEnded();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_SELFTEST));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	statechart->exit();
}
TEST_F(TestClass, pair_10_2_12_2) {
	statechart->enter();
	//// test name test@pair_10_2_12_2
	//// generated for (test goal): startupEnded=true - exp_pause=true
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(true);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	statechart->raiseStartupEnded();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_SELFTEST));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	statechart->exit();
}
TEST_F(TestClass, pair_10_2_13_2) {
	statechart->enter();
	//// test name test@pair_10_2_13_2
	//// generated for (test goal): startupEnded=true - selfTestPassed=true
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	statechart->raiseStartupEnded();
	statechart->raiseSelfTestPassed();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_SELFTEST));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	statechart->exit();
}
TEST_F(TestClass, pair_10_2_14_2) {
	statechart->enter();
	//// test name test@pair_10_2_14_2
	//// generated for (test goal): startupEnded=true - startVentilation=true
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	statechart->raiseStartVentilation();
	statechart->raiseStartupEnded();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_SELFTEST));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	statechart->exit();
}
TEST_F(TestClass, pair_10_2_15_2) {
	statechart->enter();
	//// test name test@pair_10_2_15_2
	//// generated for (test goal): startupEnded=true - pawGTMaxPinsp=true
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(true);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	statechart->raiseStartupEnded();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_SELFTEST));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	statechart->exit();
}
TEST_F(TestClass, pair_11_2_12_2) {
	statechart->enter();
	//// test name test@pair_11_2_12_2
	//// generated for (test goal): turnOffApneaAlarm=true - exp_pause=true
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(true);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	statechart->exit();
}
TEST_F(TestClass, pair_11_2_13_2) {
	statechart->enter();
	//// test name test@pair_11_2_13_2
	//// generated for (test goal): turnOffApneaAlarm=true - selfTestPassed=true
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	statechart->raiseSelfTestPassed();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	statechart->exit();
}
TEST_F(TestClass, pair_11_2_14_2) {
	statechart->enter();
	//// test name test@pair_11_2_14_2
	//// generated for (test goal): turnOffApneaAlarm=true - startVentilation=true
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	statechart->raiseStartVentilation();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	statechart->exit();
}
TEST_F(TestClass, pair_11_2_15_2) {
	statechart->enter();
	//// test name test@pair_11_2_15_2
	//// generated for (test goal): turnOffApneaAlarm=true - pawGTMaxPinsp=true
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(true);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	statechart->exit();
}
TEST_F(TestClass, pair_12_2_13_2) {
	statechart->enter();
	//// test name test@pair_12_2_13_2
	//// generated for (test goal): exp_pause=true - selfTestPassed=true
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(true);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	statechart->raiseSelfTestPassed();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	statechart->exit();
}
TEST_F(TestClass, pair_12_2_14_2) {
	statechart->enter();
	//// test name test@pair_12_2_14_2
	//// generated for (test goal): exp_pause=true - startVentilation=true
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(true);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	statechart->raiseStartVentilation();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	statechart->exit();
}
TEST_F(TestClass, pair_12_2_15_2) {
	statechart->enter();
	//// test name test@pair_12_2_15_2
	//// generated for (test goal): exp_pause=true - pawGTMaxPinsp=true
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(true);
	statechart->setExp_pause(true);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	statechart->exit();
}
TEST_F(TestClass, pair_13_2_14_2) {
	statechart->enter();
	//// test name test@pair_13_2_14_2
	//// generated for (test goal): selfTestPassed=true - startVentilation=true
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	statechart->raiseStartVentilation();
	statechart->raiseSelfTestPassed();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	statechart->exit();
}
TEST_F(TestClass, pair_13_2_15_2) {
	statechart->enter();
	//// test name test@pair_13_2_15_2
	//// generated for (test goal): selfTestPassed=true - pawGTMaxPinsp=true
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(true);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	statechart->raiseSelfTestPassed();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	statechart->exit();
}
TEST_F(TestClass, pair_14_2_15_2) {
	statechart->enter();
	//// test name test@pair_14_2_15_2
	//// generated for (test goal): startVentilation=true - pawGTMaxPinsp=true
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(true);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	statechart->raiseStartVentilation();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	statechart->exit();
}
TEST_F(TestClass, pair_1_1_11_2) {
	statechart->enter();
	//// test name test@pair_1_1_11_2
	//// generated for (test goal): dropPAW_ITS_PCV=false - turnOffApneaAlarm=true
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	statechart->exit();
}
TEST_F(TestClass, pair_1_2_10_2) {
	statechart->enter();
	//// test name test@pair_1_2_10_2
	//// generated for (test goal): dropPAW_ITS_PCV=true - startupEnded=true
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(true);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	statechart->raiseStartupEnded();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_SELFTEST));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	statechart->exit();
}
TEST_F(TestClass, pair_1_2_11_2) {
	statechart->enter();
	//// test name test@pair_1_2_11_2
	//// generated for (test goal): dropPAW_ITS_PCV=true - turnOffApneaAlarm=true
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(true);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	statechart->exit();
}
TEST_F(TestClass, pair_1_2_12_2) {
	statechart->enter();
	//// test name test@pair_1_2_12_2
	//// generated for (test goal): dropPAW_ITS_PCV=true - exp_pause=true
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(true);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(true);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	statechart->exit();
}
TEST_F(TestClass, pair_1_2_13_2) {
	statechart->enter();
	//// test name test@pair_1_2_13_2
	//// generated for (test goal): dropPAW_ITS_PCV=true - selfTestPassed=true
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(true);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	statechart->raiseSelfTestPassed();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	statechart->exit();
}
TEST_F(TestClass, pair_1_2_14_2) {
	statechart->enter();
	//// test name test@pair_1_2_14_2
	//// generated for (test goal): dropPAW_ITS_PCV=true - startVentilation=true
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(true);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	statechart->raiseStartVentilation();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	statechart->exit();
}
TEST_F(TestClass, pair_1_2_15_2) {
	statechart->enter();
	//// test name test@pair_1_2_15_2
	//// generated for (test goal): dropPAW_ITS_PCV=true - pawGTMaxPinsp=true
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(true);
	pawGTMaxPinspMock->setDefaultBehavior(true);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	statechart->exit();
}
TEST_F(TestClass, pair_1_2_2_1) {
	statechart->enter();
	//// test name test@pair_1_2_2_1
	//// generated for (test goal): dropPAW_ITS_PCV=true - flowDropPSV=false
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(true);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	statechart->exit();
}
TEST_F(TestClass, pair_1_2_2_2) {
	statechart->enter();
	//// test name test@pair_1_2_2_2
	//// generated for (test goal): dropPAW_ITS_PCV=true - flowDropPSV=true
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(true);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(true);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	statechart->exit();
}
TEST_F(TestClass, pair_1_2_3_2) {
	statechart->enter();
	//// test name test@pair_1_2_3_2
	//// generated for (test goal): dropPAW_ITS_PCV=true - resume=true
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(true);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	statechart->raiseResume();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	statechart->exit();
}
TEST_F(TestClass, pair_1_2_4_2) {
	statechart->enter();
	//// test name test@pair_1_2_4_2
	//// generated for (test goal): dropPAW_ITS_PCV=true - poweroff=true
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(true);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	statechart->raisePoweroff();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION__final_));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	statechart->exit();
}
TEST_F(TestClass, pair_1_2_5_2) {
	statechart->enter();
	//// test name test@pair_1_2_5_2
	//// generated for (test goal): dropPAW_ITS_PCV=true - rm_request=true
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(true);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	statechart->exit();
}
TEST_F(TestClass, pair_1_2_6_2) {
	statechart->enter();
	//// test name test@pair_1_2_6_2
	//// generated for (test goal): dropPAW_ITS_PCV=true - stopVentilation=true
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(true);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	statechart->raiseStopVentilation();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	statechart->exit();
}
TEST_F(TestClass, pair_1_2_7_2) {
	statechart->enter();
	//// test name test@pair_1_2_7_2
	//// generated for (test goal): dropPAW_ITS_PCV=true - ins_pause=true
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(true);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(true);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	statechart->exit();
}
TEST_F(TestClass, pair_1_2_8_2) {
	statechart->enter();
	//// test name test@pair_1_2_8_2
	//// generated for (test goal): dropPAW_ITS_PCV=true - dropPAW_ITS_PSV=true
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(true);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(true);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	statechart->exit();
}
TEST_F(TestClass, pair_1_2_9_2) {
	statechart->enter();
	//// test name test@pair_1_2_9_2
	//// generated for (test goal): dropPAW_ITS_PCV=true - mode=PCV
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(true);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	statechart->exit();
}
TEST_F(TestClass, pair_2_2_10_2) {
	statechart->enter();
	//// test name test@pair_2_2_10_2
	//// generated for (test goal): flowDropPSV=true - startupEnded=true
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(true);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	statechart->raiseStartupEnded();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_SELFTEST));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	statechart->exit();
}
TEST_F(TestClass, pair_2_2_11_2) {
	statechart->enter();
	//// test name test@pair_2_2_11_2
	//// generated for (test goal): flowDropPSV=true - turnOffApneaAlarm=true
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(true);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	statechart->exit();
}
TEST_F(TestClass, pair_2_2_13_2) {
	statechart->enter();
	//// test name test@pair_2_2_13_2
	//// generated for (test goal): flowDropPSV=true - selfTestPassed=true
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(true);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	statechart->raiseSelfTestPassed();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	statechart->exit();
}
TEST_F(TestClass, pair_2_2_14_2) {
	statechart->enter();
	//// test name test@pair_2_2_14_2
	//// generated for (test goal): flowDropPSV=true - startVentilation=true
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(true);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	statechart->raiseStartVentilation();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	statechart->exit();
}
TEST_F(TestClass, pair_2_2_3_2) {
	statechart->enter();
	//// test name test@pair_2_2_3_2
	//// generated for (test goal): flowDropPSV=true - resume=true
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(true);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	statechart->raiseResume();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	statechart->exit();
}
TEST_F(TestClass, pair_2_2_4_2) {
	statechart->enter();
	//// test name test@pair_2_2_4_2
	//// generated for (test goal): flowDropPSV=true - poweroff=true
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(true);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	statechart->raisePoweroff();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION__final_));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	statechart->exit();
}
TEST_F(TestClass, pair_3_2_10_2) {
	statechart->enter();
	//// test name test@pair_3_2_10_2
	//// generated for (test goal): resume=true - startupEnded=true
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	statechart->raiseStartupEnded();
	statechart->raiseResume();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_SELFTEST));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	statechart->exit();
}
TEST_F(TestClass, pair_3_2_11_2) {
	statechart->enter();
	//// test name test@pair_3_2_11_2
	//// generated for (test goal): resume=true - turnOffApneaAlarm=true
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	statechart->raiseResume();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	statechart->exit();
}
TEST_F(TestClass, pair_3_2_12_2) {
	statechart->enter();
	//// test name test@pair_3_2_12_2
	//// generated for (test goal): resume=true - exp_pause=true
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(true);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	statechart->raiseResume();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	statechart->exit();
}
TEST_F(TestClass, pair_3_2_13_2) {
	statechart->enter();
	//// test name test@pair_3_2_13_2
	//// generated for (test goal): resume=true - selfTestPassed=true
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	statechart->raiseSelfTestPassed();
	statechart->raiseResume();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	statechart->exit();
}
TEST_F(TestClass, pair_3_2_14_2) {
	statechart->enter();
	//// test name test@pair_3_2_14_2
	//// generated for (test goal): resume=true - startVentilation=true
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	statechart->raiseStartVentilation();
	statechart->raiseResume();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	statechart->exit();
}
TEST_F(TestClass, pair_3_2_15_2) {
	statechart->enter();
	//// test name test@pair_3_2_15_2
	//// generated for (test goal): resume=true - pawGTMaxPinsp=true
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(true);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	statechart->raiseResume();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	statechart->exit();
}
TEST_F(TestClass, pair_3_2_4_2) {
	statechart->enter();
	//// test name test@pair_3_2_4_2
	//// generated for (test goal): resume=true - poweroff=true
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	statechart->raiseResume();
	statechart->raisePoweroff();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION__final_));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	statechart->exit();
}
TEST_F(TestClass, pair_3_2_7_2) {
	statechart->enter();
	//// test name test@pair_3_2_7_2
	//// generated for (test goal): resume=true - ins_pause=true
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(true);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	statechart->raiseResume();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	statechart->exit();
}
TEST_F(TestClass, pair_3_2_8_2) {
	statechart->enter();
	//// test name test@pair_3_2_8_2
	//// generated for (test goal): resume=true - dropPAW_ITS_PSV=true
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(true);
	statechart->setRm_request(false);
	statechart->raiseResume();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	statechart->exit();
}
TEST_F(TestClass, pair_4_2_10_2) {
	statechart->enter();
	//// test name test@pair_4_2_10_2
	//// generated for (test goal): poweroff=true - startupEnded=true
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	statechart->raiseStartupEnded();
	statechart->raisePoweroff();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION__final_));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	statechart->exit();
}
TEST_F(TestClass, pair_4_2_11_2) {
	statechart->enter();
	//// test name test@pair_4_2_11_2
	//// generated for (test goal): poweroff=true - turnOffApneaAlarm=true
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	statechart->raisePoweroff();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION__final_));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	statechart->exit();
}
TEST_F(TestClass, pair_4_2_12_2) {
	statechart->enter();
	//// test name test@pair_4_2_12_2
	//// generated for (test goal): poweroff=true - exp_pause=true
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(true);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	statechart->raisePoweroff();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION__final_));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	statechart->exit();
}
TEST_F(TestClass, pair_4_2_13_2) {
	statechart->enter();
	//// test name test@pair_4_2_13_2
	//// generated for (test goal): poweroff=true - selfTestPassed=true
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	statechart->raiseSelfTestPassed();
	statechart->raisePoweroff();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION__final_));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	statechart->exit();
}
TEST_F(TestClass, pair_4_2_14_2) {
	statechart->enter();
	//// test name test@pair_4_2_14_2
	//// generated for (test goal): poweroff=true - startVentilation=true
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	statechart->raiseStartVentilation();
	statechart->raisePoweroff();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION__final_));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	statechart->exit();
}
TEST_F(TestClass, pair_4_2_15_2) {
	statechart->enter();
	//// test name test@pair_4_2_15_2
	//// generated for (test goal): poweroff=true - pawGTMaxPinsp=true
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(true);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	statechart->raisePoweroff();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION__final_));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	statechart->exit();
}
TEST_F(TestClass, pair_4_2_7_2) {
	statechart->enter();
	//// test name test@pair_4_2_7_2
	//// generated for (test goal): poweroff=true - ins_pause=true
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(true);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	statechart->raisePoweroff();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION__final_));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	statechart->exit();
}
TEST_F(TestClass, pair_4_2_8_2) {
	statechart->enter();
	//// test name test@pair_4_2_8_2
	//// generated for (test goal): poweroff=true - dropPAW_ITS_PSV=true
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(true);
	statechart->setRm_request(false);
	statechart->raisePoweroff();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION__final_));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	statechart->exit();
}
TEST_F(TestClass, pair_5_2_10_2) {
	statechart->enter();
	//// test name test@pair_5_2_10_2
	//// generated for (test goal): rm_request=true - startupEnded=true
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	statechart->raiseStartupEnded();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_SELFTEST));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	statechart->exit();
}
TEST_F(TestClass, pair_5_2_11_2) {
	statechart->enter();
	//// test name test@pair_5_2_11_2
	//// generated for (test goal): rm_request=true - turnOffApneaAlarm=true
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	statechart->exit();
}
TEST_F(TestClass, pair_6_2_10_2) {
	statechart->enter();
	//// test name test@pair_6_2_10_2
	//// generated for (test goal): stopVentilation=true - startupEnded=true
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	statechart->raiseStartupEnded();
	statechart->raiseStopVentilation();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_SELFTEST));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	statechart->exit();
}
TEST_F(TestClass, pair_6_2_11_2) {
	statechart->enter();
	//// test name test@pair_6_2_11_2
	//// generated for (test goal): stopVentilation=true - turnOffApneaAlarm=true
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	statechart->raiseStopVentilation();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	statechart->exit();
}
TEST_F(TestClass, pair_6_2_12_2) {
	statechart->enter();
	//// test name test@pair_6_2_12_2
	//// generated for (test goal): stopVentilation=true - exp_pause=true
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(true);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	statechart->raiseStopVentilation();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	statechart->exit();
}
TEST_F(TestClass, pair_6_2_13_2) {
	statechart->enter();
	//// test name test@pair_6_2_13_2
	//// generated for (test goal): stopVentilation=true - selfTestPassed=true
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	statechart->raiseStopVentilation();
	statechart->raiseSelfTestPassed();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	statechart->exit();
}
TEST_F(TestClass, pair_6_2_7_2) {
	statechart->enter();
	//// test name test@pair_6_2_7_2
	//// generated for (test goal): stopVentilation=true - ins_pause=true
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(true);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	statechart->raiseStopVentilation();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	statechart->exit();
}
TEST_F(TestClass, pair_6_2_8_2) {
	statechart->enter();
	//// test name test@pair_6_2_8_2
	//// generated for (test goal): stopVentilation=true - dropPAW_ITS_PSV=true
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(true);
	statechart->setRm_request(false);
	statechart->raiseStopVentilation();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	statechart->exit();
}
TEST_F(TestClass, pair_7_2_10_2) {
	statechart->enter();
	//// test name test@pair_7_2_10_2
	//// generated for (test goal): ins_pause=true - startupEnded=true
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(true);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	statechart->raiseStartupEnded();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_SELFTEST));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	statechart->exit();
}
TEST_F(TestClass, pair_7_2_11_2) {
	statechart->enter();
	//// test name test@pair_7_2_11_2
	//// generated for (test goal): ins_pause=true - turnOffApneaAlarm=true
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(true);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	statechart->exit();
}
TEST_F(TestClass, pair_7_2_12_2) {
	statechart->enter();
	//// test name test@pair_7_2_12_2
	//// generated for (test goal): ins_pause=true - exp_pause=true
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(true);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(true);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	statechart->exit();
}
TEST_F(TestClass, pair_7_2_13_2) {
	statechart->enter();
	//// test name test@pair_7_2_13_2
	//// generated for (test goal): ins_pause=true - selfTestPassed=true
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(true);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	statechart->raiseSelfTestPassed();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	statechart->exit();
}
TEST_F(TestClass, pair_7_2_14_2) {
	statechart->enter();
	//// test name test@pair_7_2_14_2
	//// generated for (test goal): ins_pause=true - startVentilation=true
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(true);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	statechart->raiseStartVentilation();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	statechart->exit();
}
TEST_F(TestClass, pair_7_2_15_2) {
	statechart->enter();
	//// test name test@pair_7_2_15_2
	//// generated for (test goal): ins_pause=true - pawGTMaxPinsp=true
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(true);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(true);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	statechart->exit();
}
TEST_F(TestClass, pair_7_2_8_2) {
	statechart->enter();
	//// test name test@pair_7_2_8_2
	//// generated for (test goal): ins_pause=true - dropPAW_ITS_PSV=true
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(true);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(true);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	statechart->exit();
}
TEST_F(TestClass, pair_8_2_10_2) {
	statechart->enter();
	//// test name test@pair_8_2_10_2
	//// generated for (test goal): dropPAW_ITS_PSV=true - startupEnded=true
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(true);
	statechart->setRm_request(false);
	statechart->raiseStartupEnded();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_SELFTEST));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	statechart->exit();
}
TEST_F(TestClass, pair_8_2_11_2) {
	statechart->enter();
	//// test name test@pair_8_2_11_2
	//// generated for (test goal): dropPAW_ITS_PSV=true - turnOffApneaAlarm=true
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(true);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	statechart->exit();
}
TEST_F(TestClass, pair_8_2_12_2) {
	statechart->enter();
	//// test name test@pair_8_2_12_2
	//// generated for (test goal): dropPAW_ITS_PSV=true - exp_pause=true
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(true);
	dropPAW_ITS_PSVMock->setDefaultBehavior(true);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	statechart->exit();
}
TEST_F(TestClass, pair_8_2_13_2) {
	statechart->enter();
	//// test name test@pair_8_2_13_2
	//// generated for (test goal): dropPAW_ITS_PSV=true - selfTestPassed=true
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(true);
	statechart->setRm_request(false);
	statechart->raiseSelfTestPassed();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	statechart->exit();
}
TEST_F(TestClass, pair_8_2_14_2) {
	statechart->enter();
	//// test name test@pair_8_2_14_2
	//// generated for (test goal): dropPAW_ITS_PSV=true - startVentilation=true
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(true);
	statechart->setRm_request(false);
	statechart->raiseStartVentilation();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	statechart->exit();
}
TEST_F(TestClass, pair_8_2_15_2) {
	statechart->enter();
	//// test name test@pair_8_2_15_2
	//// generated for (test goal): dropPAW_ITS_PSV=true - pawGTMaxPinsp=true
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(true);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(true);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	statechart->exit();
}
TEST_F(TestClass, pair_9_2_11_2) {
	statechart->enter();
	//// test name test@pair_9_2_11_2
	//// generated for (test goal): mode=PCV - turnOffApneaAlarm=true
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	statechart->exit();
}
TEST_F(TestClass, pair_9_2_12_2) {
	statechart->enter();
	//// test name test@pair_9_2_12_2
	//// generated for (test goal): mode=PCV - exp_pause=true
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(true);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	statechart->exit();
}
TEST_F(TestClass, RG_r_Main_FFFTF12) {
	statechart->enter();
	//// test name test@RG_r_Main_FFFTF12
	//// generated for (test goal): RG_r_Main_FFFTF12: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (apneaAlarm and not((turnOffApneaAlarm = true) and ((state = MAIN_REGION_PCV_R1_INSPIRATION) or (state = MAIN_REGION_PCV_R1_EXPIRATION))))))
	statechart->exit();
}
TEST_F(TestClass, RG_r_Main_FFFTF82) {
	statechart->enter();
	//// test name test@RG_r_Main_FFFTF82
	//// generated for (test goal): RG_r_Main_FFFTF82: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_RM) and not(expiredTIMER_MAX_RM_TIME or rm_request))))
	statechart->exit();
}
TEST_F(TestClass, RG_r_Main_FFFTFF102) {
	statechart->enter();
	//// test name test@RG_r_Main_FFFTFF102
	//// generated for (test goal): RG_r_Main_FFFTFF102: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_EXPIRATORYPAUSE) and (not expiredTIMER_MAX_EXP_PAUSE and exp_pause))))
	statechart->exit();
}
TEST_F(TestClass, RG_r_Main_FFFTFF152) {
	statechart->enter();
	//// test name test@RG_r_Main_FFFTFF152
	//// generated for (test goal): RG_r_Main_FFFTFF152: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PSV_R1_EXPIRATORYPAUSE) and (not expiredTIMER_MAX_EXP_PAUSE and exp_pause))))
	statechart->exit();
}
TEST_F(TestClass, RG_r_Main_FFFTFF72) {
	statechart->enter();
	//// test name test@RG_r_Main_FFFTFF72
	//// generated for (test goal): RG_r_Main_FFFTFF72: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_INSPIRATORYPAUSE) and (not expiredTIMER_MAX_INS_PAUSE and ins_pause))))
	statechart->exit();
}
TEST_F(TestClass, RG_r_Main_FFFTFFFTRG_r_PCVinspOValve_RG2142) {
	statechart->enter();
	//// test name test@RG_r_Main_FFFTFFFTRG_r_PCVinspOValve_RG2142
	//// generated for (test goal): RG_r_Main_FFFTFFFTRG_r_PCVinspOValve_RG2142: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PSV_R1_EXPIRATION) and (not(expiredTIMER_TRIGGERWINDOWDELAY_MS and dropPAW_ITS_PSV) and (not expiredTIMER_APNEALAG and (not((expiredTIMER_MIN_EXP_TIME_PSV and (mode = PSV)) and exp_pause) and ((expiredTIMER_MIN_EXP_TIME_PSV and (mode = PCV)) and (oValve != CLOSED))))))))
	statechart->exit();
}
TEST_F(TestClass, RG_r_Main_FFFTFFFTRG_r_PCVinspOValve_RG_r_PCVinsp_RG11142) {
	statechart->enter();
	//// test name test@RG_r_Main_FFFTFFFTRG_r_PCVinspOValve_RG_r_PCVinsp_RG11142
	//// generated for (test goal): RG_r_Main_FFFTFFFTRG_r_PCVinspOValve_RG_r_PCVinsp_RG11142: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PSV_R1_EXPIRATION) and (not(expiredTIMER_TRIGGERWINDOWDELAY_MS and dropPAW_ITS_PSV) and (not expiredTIMER_APNEALAG and (not((expiredTIMER_MIN_EXP_TIME_PSV and (mode = PSV)) and exp_pause) and ((expiredTIMER_MIN_EXP_TIME_PSV and (mode = PCV)) and (state != MAIN_REGION_PCV_R1_INSPIRATION))))))))
	statechart->exit();
}
TEST_F(TestClass, RG_r_Main_FFFTFFFTRG_r_PCVinspOValve_RG_r_PCVinsp_RG21142) {
	statechart->enter();
	//// test name test@RG_r_Main_FFFTFFFTRG_r_PCVinspOValve_RG_r_PCVinsp_RG21142
	//// generated for (test goal): RG_r_Main_FFFTFFFTRG_r_PCVinspOValve_RG_r_PCVinsp_RG21142: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PSV_R1_EXPIRATION) and (not(expiredTIMER_TRIGGERWINDOWDELAY_MS and dropPAW_ITS_PSV) and (not expiredTIMER_APNEALAG and (not((expiredTIMER_MIN_EXP_TIME_PSV and (mode = PSV)) and exp_pause) and ((expiredTIMER_MIN_EXP_TIME_PSV and (mode = PCV)) and (iValve != OPEN))))))))
	statechart->exit();
}
TEST_F(TestClass, RG_r_Main_FFFTFFTRG_r_PSVexp_pause_RG1142) {
	statechart->enter();
	//// test name test@RG_r_Main_FFFTFFTRG_r_PSVexp_pause_RG1142
	//// generated for (test goal): RG_r_Main_FFFTFFTRG_r_PSVexp_pause_RG1142: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PSV_R1_EXPIRATION) and (not(expiredTIMER_TRIGGERWINDOWDELAY_MS and dropPAW_ITS_PSV) and (not expiredTIMER_APNEALAG and (((expiredTIMER_MIN_EXP_TIME_PSV and (mode = PSV)) and exp_pause) and (state != MAIN_REGION_PSV_R1_EXPIRATORYPAUSE)))))))
	statechart->exit();
}
TEST_F(TestClass, RG_r_Main_FFFTFFTRG_r_PSVexp_pause_RG2142) {
	statechart->enter();
	//// test name test@RG_r_Main_FFFTFFTRG_r_PSVexp_pause_RG2142
	//// generated for (test goal): RG_r_Main_FFFTFFTRG_r_PSVexp_pause_RG2142: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PSV_R1_EXPIRATION) and (not(expiredTIMER_TRIGGERWINDOWDELAY_MS and dropPAW_ITS_PSV) and (not expiredTIMER_APNEALAG and (((expiredTIMER_MIN_EXP_TIME_PSV and (mode = PSV)) and exp_pause) and (oValve != CLOSED)))))))
	statechart->exit();
}
TEST_F(TestClass, RG_r_Main_FFFTFTF92) {
	statechart->enter();
	//// test name test@RG_r_Main_FFFTFTF92
	//// generated for (test goal): RG_r_Main_FFFTFTF92: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_EXPIRATION) and (not expiredTIMER_EXPIRATION_DURATION_MS and (expiredTIMER_TRIGGERWINDOWDELAY_MS and not dropPAW_ITS_PCV)))))
	statechart->exit();
}
TEST_F(TestClass, RG_r_Main_FFFTFTRG_r_PCVexp_RG172) {
	statechart->enter();
	//// test name test@RG_r_Main_FFFTFTRG_r_PCVexp_RG172
	//// generated for (test goal): RG_r_Main_FFFTFTRG_r_PCVexp_RG172: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_INSPIRATORYPAUSE) and (not expiredTIMER_MAX_INS_PAUSE and (not ins_pause and (state != MAIN_REGION_PCV_R1_EXPIRATION))))))
	statechart->exit();
}
TEST_F(TestClass, RG_r_Main_FFFTFTRG_r_PCVexp_RG272) {
	statechart->enter();
	//// test name test@RG_r_Main_FFFTFTRG_r_PCVexp_RG272
	//// generated for (test goal): RG_r_Main_FFFTFTRG_r_PCVexp_RG272: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_INSPIRATORYPAUSE) and (not expiredTIMER_MAX_INS_PAUSE and (not ins_pause and (oValve != OPEN))))))
	statechart->exit();
}
TEST_F(TestClass, RG_r_Main_FFFTFTRG_r_PCVinspApnea_RG2142) {
	statechart->enter();
	//// test name test@RG_r_Main_FFFTFTRG_r_PCVinspApnea_RG2142
	//// generated for (test goal): RG_r_Main_FFFTFTRG_r_PCVinspApnea_RG2142: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PSV_R1_EXPIRATION) and (not(expiredTIMER_TRIGGERWINDOWDELAY_MS and dropPAW_ITS_PSV) and (expiredTIMER_APNEALAG and (apneaAlarm != true))))))
	statechart->exit();
}
TEST_F(TestClass, RG_r_Main_FFFTFTRG_r_PCVinspApnea_RG3142) {
	statechart->enter();
	//// test name test@RG_r_Main_FFFTFTRG_r_PCVinspApnea_RG3142
	//// generated for (test goal): RG_r_Main_FFFTFTRG_r_PCVinspApnea_RG3142: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PSV_R1_EXPIRATION) and (not(expiredTIMER_TRIGGERWINDOWDELAY_MS and dropPAW_ITS_PSV) and (expiredTIMER_APNEALAG and (apnea_backup_mode != true))))))
	statechart->exit();
}
TEST_F(TestClass, RG_r_Main_FFFTFTRG_r_PCVinspApnea_RG_r_PCVinspOValve_RG21142) {
	statechart->enter();
	//// test name test@RG_r_Main_FFFTFTRG_r_PCVinspApnea_RG_r_PCVinspOValve_RG21142
	//// generated for (test goal): RG_r_Main_FFFTFTRG_r_PCVinspApnea_RG_r_PCVinspOValve_RG21142: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PSV_R1_EXPIRATION) and (not(expiredTIMER_TRIGGERWINDOWDELAY_MS and dropPAW_ITS_PSV) and (expiredTIMER_APNEALAG and (oValve != CLOSED))))))
	statechart->exit();
}
TEST_F(TestClass, RG_r_Main_FFFTFTRG_r_PCVinspApnea_RG_r_PCVinspOValve_RG_r_PCVinsp_RG111142) {
	statechart->enter();
	//// test name test@RG_r_Main_FFFTFTRG_r_PCVinspApnea_RG_r_PCVinspOValve_RG_r_PCVinsp_RG111142
	//// generated for (test goal): RG_r_Main_FFFTFTRG_r_PCVinspApnea_RG_r_PCVinspOValve_RG_r_PCVinsp_RG111142: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PSV_R1_EXPIRATION) and (not(expiredTIMER_TRIGGERWINDOWDELAY_MS and dropPAW_ITS_PSV) and (expiredTIMER_APNEALAG and (state != MAIN_REGION_PCV_R1_INSPIRATION))))))
	statechart->exit();
}
TEST_F(TestClass, RG_r_Main_FFFTFTRG_r_PCVinspApnea_RG_r_PCVinspOValve_RG_r_PCVinsp_RG211142) {
	statechart->enter();
	//// test name test@RG_r_Main_FFFTFTRG_r_PCVinspApnea_RG_r_PCVinspOValve_RG_r_PCVinsp_RG211142
	//// generated for (test goal): RG_r_Main_FFFTFTRG_r_PCVinspApnea_RG_r_PCVinspOValve_RG_r_PCVinsp_RG211142: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PSV_R1_EXPIRATION) and (not(expiredTIMER_TRIGGERWINDOWDELAY_MS and dropPAW_ITS_PSV) and (expiredTIMER_APNEALAG and (iValve != OPEN))))))
	statechart->exit();
}
TEST_F(TestClass, RG_r_Main_FFFTFTRG_r_PCVinsp_RG1102) {
	statechart->enter();
	//// test name test@RG_r_Main_FFFTFTRG_r_PCVinsp_RG1102
	//// generated for (test goal): RG_r_Main_FFFTFTRG_r_PCVinsp_RG1102: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_EXPIRATORYPAUSE) and (not expiredTIMER_MAX_EXP_PAUSE and (not exp_pause and (state != MAIN_REGION_PCV_R1_INSPIRATION))))))
	statechart->exit();
}
TEST_F(TestClass, RG_r_Main_FFFTFTRG_r_PCVinsp_RG2102) {
	statechart->enter();
	//// test name test@RG_r_Main_FFFTFTRG_r_PCVinsp_RG2102
	//// generated for (test goal): RG_r_Main_FFFTFTRG_r_PCVinsp_RG2102: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_EXPIRATORYPAUSE) and (not expiredTIMER_MAX_EXP_PAUSE and (not exp_pause and (iValve != OPEN))))))
	statechart->exit();
}
TEST_F(TestClass, RG_r_Main_FFFTFTRG_r_PSVinsp_RG1152) {
	statechart->enter();
	//// test name test@RG_r_Main_FFFTFTRG_r_PSVinsp_RG1152
	//// generated for (test goal): RG_r_Main_FFFTFTRG_r_PSVinsp_RG1152: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PSV_R1_EXPIRATORYPAUSE) and (not expiredTIMER_MAX_EXP_PAUSE and (not exp_pause and (state != MAIN_REGION_PSV_R1_INSPIRATION))))))
	statechart->exit();
}
TEST_F(TestClass, RG_r_Main_FFFTFTRG_r_PSVinsp_RG2152) {
	statechart->enter();
	//// test name test@RG_r_Main_FFFTFTRG_r_PSVinsp_RG2152
	//// generated for (test goal): RG_r_Main_FFFTFTRG_r_PSVinsp_RG2152: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PSV_R1_EXPIRATORYPAUSE) and (not expiredTIMER_MAX_EXP_PAUSE and (not exp_pause and (iValve != OPEN))))))
	statechart->exit();
}
TEST_F(TestClass, RG_r_Main_FFFTFTTRG_r_PCVinspOValve_RG292) {
	statechart->enter();
	//// test name test@RG_r_Main_FFFTFTTRG_r_PCVinspOValve_RG292
	//// generated for (test goal): RG_r_Main_FFFTFTTRG_r_PCVinspOValve_RG292: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_EXPIRATION) and (not expiredTIMER_EXPIRATION_DURATION_MS and (expiredTIMER_TRIGGERWINDOWDELAY_MS and (dropPAW_ITS_PCV and (oValve != CLOSED)))))))
	statechart->exit();
}
TEST_F(TestClass, RG_r_Main_FFFTFTTRG_r_PCVinspOValve_RG_r_PCVinsp_RG1192) {
	statechart->enter();
	//// test name test@RG_r_Main_FFFTFTTRG_r_PCVinspOValve_RG_r_PCVinsp_RG1192
	//// generated for (test goal): RG_r_Main_FFFTFTTRG_r_PCVinspOValve_RG_r_PCVinsp_RG1192: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_EXPIRATION) and (not expiredTIMER_EXPIRATION_DURATION_MS and (expiredTIMER_TRIGGERWINDOWDELAY_MS and (dropPAW_ITS_PCV and (state != MAIN_REGION_PCV_R1_INSPIRATION)))))))
	statechart->exit();
}
TEST_F(TestClass, RG_r_Main_FFFTFTTRG_r_PCVinspOValve_RG_r_PCVinsp_RG2192) {
	statechart->enter();
	//// test name test@RG_r_Main_FFFTFTTRG_r_PCVinspOValve_RG_r_PCVinsp_RG2192
	//// generated for (test goal): RG_r_Main_FFFTFTTRG_r_PCVinspOValve_RG_r_PCVinsp_RG2192: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_EXPIRATION) and (not expiredTIMER_EXPIRATION_DURATION_MS and (expiredTIMER_TRIGGERWINDOWDELAY_MS and (dropPAW_ITS_PCV and (iValve != OPEN)))))))
	statechart->exit();
}
TEST_F(TestClass, RG_r_Main_FFFTTF162) {
	statechart->enter();
	//// test name test@RG_r_Main_FFFTTF162
	//// generated for (test goal): RG_r_Main_FFFTTF162: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_INSPIRATION) and (expiredTIMER_INSPIRATION_DURATION_MS and (mode != PSV)))))
	statechart->exit();
}
TEST_F(TestClass, RG_r_Main_FFFTTF262) {
	statechart->enter();
	//// test name test@RG_r_Main_FFFTTF262
	//// generated for (test goal): RG_r_Main_FFFTTF262: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_INSPIRATION) and (expiredTIMER_INSPIRATION_DURATION_MS and (mode != PCV)))))
	statechart->exit();
}
TEST_F(TestClass, RG_r_Main_FFFTTFRG_r_PCVinspOValve_RG292) {
	statechart->enter();
	//// test name test@RG_r_Main_FFFTTFRG_r_PCVinspOValve_RG292
	//// generated for (test goal): RG_r_Main_FFFTTFRG_r_PCVinspOValve_RG292: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_EXPIRATION) and (expiredTIMER_EXPIRATION_DURATION_MS and (not exp_pause and (oValve != CLOSED))))))
	statechart->exit();
}
TEST_F(TestClass, RG_r_Main_FFFTTFRG_r_PCVinspOValve_RG_r_PCVinsp_RG1192) {
	statechart->enter();
	//// test name test@RG_r_Main_FFFTTFRG_r_PCVinspOValve_RG_r_PCVinsp_RG1192
	//// generated for (test goal): RG_r_Main_FFFTTFRG_r_PCVinspOValve_RG_r_PCVinsp_RG1192: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_EXPIRATION) and (expiredTIMER_EXPIRATION_DURATION_MS and (not exp_pause and (state != MAIN_REGION_PCV_R1_INSPIRATION))))))
	statechart->exit();
}
TEST_F(TestClass, RG_r_Main_FFFTTFRG_r_PCVinspOValve_RG_r_PCVinsp_RG2192) {
	statechart->enter();
	//// test name test@RG_r_Main_FFFTTFRG_r_PCVinspOValve_RG_r_PCVinsp_RG2192
	//// generated for (test goal): RG_r_Main_FFFTTFRG_r_PCVinspOValve_RG_r_PCVinsp_RG2192: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_EXPIRATION) and (expiredTIMER_EXPIRATION_DURATION_MS and (not exp_pause and (iValve != OPEN))))))
	statechart->exit();
}
TEST_F(TestClass, RG_r_Main_FFFTTRG_r_apneaAlarmOFF_RG12) {
	statechart->enter();
	//// test name test@RG_r_Main_FFFTTRG_r_apneaAlarmOFF_RG12
	//// generated for (test goal): RG_r_Main_FFFTTRG_r_apneaAlarmOFF_RG12: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (apneaAlarm and (((turnOffApneaAlarm = true) and ((state = MAIN_REGION_PCV_R1_INSPIRATION) or (state = MAIN_REGION_PCV_R1_EXPIRATION))) and (apneaAlarm != false)))))
	statechart->exit();
}
TEST_F(TestClass, RG_r_Main_FFFTTRG_r_PCVexpIValve_RG282) {
	statechart->enter();
	//// test name test@RG_r_Main_FFFTTRG_r_PCVexpIValve_RG282
	//// generated for (test goal): RG_r_Main_FFFTTRG_r_PCVexpIValve_RG282: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_RM) and ((expiredTIMER_MAX_RM_TIME or rm_request) and (iValve != CLOSED)))))
	statechart->exit();
}
TEST_F(TestClass, RG_r_Main_FFFTTRG_r_PCVexpIValve_RG_r_PCVexp_RG1182) {
	statechart->enter();
	//// test name test@RG_r_Main_FFFTTRG_r_PCVexpIValve_RG_r_PCVexp_RG1182
	//// generated for (test goal): RG_r_Main_FFFTTRG_r_PCVexpIValve_RG_r_PCVexp_RG1182: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_RM) and ((expiredTIMER_MAX_RM_TIME or rm_request) and (state != MAIN_REGION_PCV_R1_EXPIRATION)))))
	statechart->exit();
}
TEST_F(TestClass, RG_r_Main_FFFTTRG_r_PCVexpIValve_RG_r_PCVexp_RG2182) {
	statechart->enter();
	//// test name test@RG_r_Main_FFFTTRG_r_PCVexpIValve_RG_r_PCVexp_RG2182
	//// generated for (test goal): RG_r_Main_FFFTTRG_r_PCVexpIValve_RG_r_PCVexp_RG2182: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_RM) and ((expiredTIMER_MAX_RM_TIME or rm_request) and (oValve != OPEN)))))
	statechart->exit();
}
TEST_F(TestClass, RG_r_Main_FFFTTRG_r_PCVexp_RG172) {
	statechart->enter();
	//// test name test@RG_r_Main_FFFTTRG_r_PCVexp_RG172
	//// generated for (test goal): RG_r_Main_FFFTTRG_r_PCVexp_RG172: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_INSPIRATORYPAUSE) and (expiredTIMER_MAX_INS_PAUSE and (state != MAIN_REGION_PCV_R1_EXPIRATION)))))
	statechart->exit();
}
TEST_F(TestClass, RG_r_Main_FFFTTRG_r_PCVexp_RG272) {
	statechart->enter();
	//// test name test@RG_r_Main_FFFTTRG_r_PCVexp_RG272
	//// generated for (test goal): RG_r_Main_FFFTTRG_r_PCVexp_RG272: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_INSPIRATORYPAUSE) and (expiredTIMER_MAX_INS_PAUSE and (oValve != OPEN)))))
	statechart->exit();
}
TEST_F(TestClass, RG_r_Main_FFFTTRG_r_PCVinsp_RG1102) {
	statechart->enter();
	//// test name test@RG_r_Main_FFFTTRG_r_PCVinsp_RG1102
	//// generated for (test goal): RG_r_Main_FFFTTRG_r_PCVinsp_RG1102: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_EXPIRATORYPAUSE) and (expiredTIMER_MAX_EXP_PAUSE and (state != MAIN_REGION_PCV_R1_INSPIRATION)))))
	statechart->exit();
}
TEST_F(TestClass, RG_r_Main_FFFTTRG_r_PCVinsp_RG2102) {
	statechart->enter();
	//// test name test@RG_r_Main_FFFTTRG_r_PCVinsp_RG2102
	//// generated for (test goal): RG_r_Main_FFFTTRG_r_PCVinsp_RG2102: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_EXPIRATORYPAUSE) and (expiredTIMER_MAX_EXP_PAUSE and (iValve != OPEN)))))
	statechart->exit();
}
TEST_F(TestClass, RG_r_Main_FFFTTRG_r_PSVexp_RG1122) {
	statechart->enter();
	//// test name test@RG_r_Main_FFFTTRG_r_PSVexp_RG1122
	//// generated for (test goal): RG_r_Main_FFFTTRG_r_PSVexp_RG1122: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PSV_R1_INSPIRATORYPAUSE) and (expiredTIMER_MAX_INS_PAUSE and (state != MAIN_REGION_PSV_R1_EXPIRATION)))))
	statechart->exit();
}
TEST_F(TestClass, RG_r_Main_FFFTTRG_r_PSVexp_RG2122) {
	statechart->enter();
	//// test name test@RG_r_Main_FFFTTRG_r_PSVexp_RG2122
	//// generated for (test goal): RG_r_Main_FFFTTRG_r_PSVexp_RG2122: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PSV_R1_INSPIRATORYPAUSE) and (expiredTIMER_MAX_INS_PAUSE and (oValve != OPEN)))))
	statechart->exit();
}
TEST_F(TestClass, RG_r_Main_FFFTTRG_r_PSVinsp_RG1152) {
	statechart->enter();
	//// test name test@RG_r_Main_FFFTTRG_r_PSVinsp_RG1152
	//// generated for (test goal): RG_r_Main_FFFTTRG_r_PSVinsp_RG1152: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PSV_R1_EXPIRATORYPAUSE) and (expiredTIMER_MAX_EXP_PAUSE and (state != MAIN_REGION_PSV_R1_INSPIRATION)))))
	statechart->exit();
}
TEST_F(TestClass, RG_r_Main_FFFTTRG_r_PSVinsp_RG2152) {
	statechart->enter();
	//// test name test@RG_r_Main_FFFTTRG_r_PSVinsp_RG2152
	//// generated for (test goal): RG_r_Main_FFFTTRG_r_PSVinsp_RG2152: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PSV_R1_EXPIRATORYPAUSE) and (expiredTIMER_MAX_EXP_PAUSE and (iValve != OPEN)))))
	statechart->exit();
}
TEST_F(TestClass, RG_r_Main_FFFTTTFFRG_r_PCVexpIValve_RG2262) {
	statechart->enter();
	//// test name test@RG_r_Main_FFFTTTFFRG_r_PCVexpIValve_RG2262
	//// generated for (test goal): RG_r_Main_FFFTTTFFRG_r_PCVexpIValve_RG2262: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_INSPIRATION) and (expiredTIMER_INSPIRATION_DURATION_MS and ((mode = PCV) and (not ins_pause and (not rm_request and (iValve != CLOSED))))))))
	statechart->exit();
}
TEST_F(TestClass, RG_r_Main_FFFTTTFFRG_r_PCVexpIValve_RG_r_PCVexp_RG11262) {
	statechart->enter();
	//// test name test@RG_r_Main_FFFTTTFFRG_r_PCVexpIValve_RG_r_PCVexp_RG11262
	//// generated for (test goal): RG_r_Main_FFFTTTFFRG_r_PCVexpIValve_RG_r_PCVexp_RG11262: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_INSPIRATION) and (expiredTIMER_INSPIRATION_DURATION_MS and ((mode = PCV) and (not ins_pause and (not rm_request and (state != MAIN_REGION_PCV_R1_EXPIRATION))))))))
	statechart->exit();
}
TEST_F(TestClass, RG_r_Main_FFFTTTFFRG_r_PCVexpIValve_RG_r_PCVexp_RG21262) {
	statechart->enter();
	//// test name test@RG_r_Main_FFFTTTFFRG_r_PCVexpIValve_RG_r_PCVexp_RG21262
	//// generated for (test goal): RG_r_Main_FFFTTTFFRG_r_PCVexpIValve_RG_r_PCVexp_RG21262: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_INSPIRATION) and (expiredTIMER_INSPIRATION_DURATION_MS and ((mode = PCV) and (not ins_pause and (not rm_request and (oValve != OPEN))))))))
	statechart->exit();
}
TEST_F(TestClass, RG_r_Main_FFFTTTFFRG_r_PSVexpIValveFromPCV_RG2162) {
	statechart->enter();
	//// test name test@RG_r_Main_FFFTTTFFRG_r_PSVexpIValveFromPCV_RG2162
	//// generated for (test goal): RG_r_Main_FFFTTTFFRG_r_PSVexpIValveFromPCV_RG2162: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_INSPIRATION) and (expiredTIMER_INSPIRATION_DURATION_MS and ((mode = PSV) and (not ins_pause and (not rm_request and (iValve != CLOSED))))))))
	statechart->exit();
}
TEST_F(TestClass, RG_r_Main_FFFTTTFFRG_r_PSVexpIValveFromPCV_RG3162) {
	statechart->enter();
	//// test name test@RG_r_Main_FFFTTTFFRG_r_PSVexpIValveFromPCV_RG3162
	//// generated for (test goal): RG_r_Main_FFFTTTFFRG_r_PSVexpIValveFromPCV_RG3162: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_INSPIRATION) and (expiredTIMER_INSPIRATION_DURATION_MS and ((mode = PSV) and (not ins_pause and (not rm_request and (apnea_backup_mode != false))))))))
	statechart->exit();
}
TEST_F(TestClass, RG_r_Main_FFFTTTFFRG_r_PSVexpIValveFromPCV_RG_r_PSVexp_RG11162) {
	statechart->enter();
	//// test name test@RG_r_Main_FFFTTTFFRG_r_PSVexpIValveFromPCV_RG_r_PSVexp_RG11162
	//// generated for (test goal): RG_r_Main_FFFTTTFFRG_r_PSVexpIValveFromPCV_RG_r_PSVexp_RG11162: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_INSPIRATION) and (expiredTIMER_INSPIRATION_DURATION_MS and ((mode = PSV) and (not ins_pause and (not rm_request and (state != MAIN_REGION_PSV_R1_EXPIRATION))))))))
	statechart->exit();
}
TEST_F(TestClass, RG_r_Main_FFFTTTFFRG_r_PSVexpIValveFromPCV_RG_r_PSVexp_RG21162) {
	statechart->enter();
	//// test name test@RG_r_Main_FFFTTTFFRG_r_PSVexpIValveFromPCV_RG_r_PSVexp_RG21162
	//// generated for (test goal): RG_r_Main_FFFTTTFFRG_r_PSVexpIValveFromPCV_RG_r_PSVexp_RG21162: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_INSPIRATION) and (expiredTIMER_INSPIRATION_DURATION_MS and ((mode = PSV) and (not ins_pause and (not rm_request and (oValve != OPEN))))))))
	statechart->exit();
}
TEST_F(TestClass, RG_r_Main_FFFTTTFTRG_r_PCVrm_RG1262) {
	statechart->enter();
	//// test name test@RG_r_Main_FFFTTTFTRG_r_PCVrm_RG1262
	//// generated for (test goal): RG_r_Main_FFFTTTFTRG_r_PCVrm_RG1262: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_INSPIRATION) and (expiredTIMER_INSPIRATION_DURATION_MS and ((mode = PCV) and (not ins_pause and (rm_request and (state != MAIN_REGION_PCV_R1_RM))))))))
	statechart->exit();
}
TEST_F(TestClass, RG_r_Main_FFFTTTFTRG_r_PSVrmFromPCV_RG1162) {
	statechart->enter();
	//// test name test@RG_r_Main_FFFTTTFTRG_r_PSVrmFromPCV_RG1162
	//// generated for (test goal): RG_r_Main_FFFTTTFTRG_r_PSVrmFromPCV_RG1162: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_INSPIRATION) and (expiredTIMER_INSPIRATION_DURATION_MS and ((mode = PSV) and (not ins_pause and (rm_request and (state != MAIN_REGION_PSV_R1_RM))))))))
	statechart->exit();
}
TEST_F(TestClass, RG_r_Main_FFFTTTFTRG_r_PSVrmFromPCV_RG2162) {
	statechart->enter();
	//// test name test@RG_r_Main_FFFTTTFTRG_r_PSVrmFromPCV_RG2162
	//// generated for (test goal): RG_r_Main_FFFTTTFTRG_r_PSVrmFromPCV_RG2162: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_INSPIRATION) and (expiredTIMER_INSPIRATION_DURATION_MS and ((mode = PSV) and (not ins_pause and (rm_request and (apnea_backup_mode != false))))))))
	statechart->exit();
}
TEST_F(TestClass, RG_r_Main_FFFTTTRG_r_PCVexp_pause_RG192) {
	statechart->enter();
	//// test name test@RG_r_Main_FFFTTTRG_r_PCVexp_pause_RG192
	//// generated for (test goal): RG_r_Main_FFFTTTRG_r_PCVexp_pause_RG192: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_EXPIRATION) and (expiredTIMER_EXPIRATION_DURATION_MS and (exp_pause and (state != MAIN_REGION_PCV_R1_EXPIRATORYPAUSE))))))
	statechart->exit();
}
TEST_F(TestClass, RG_r_Main_FFFTTTRG_r_PCVexp_pause_RG292) {
	statechart->enter();
	//// test name test@RG_r_Main_FFFTTTRG_r_PCVexp_pause_RG292
	//// generated for (test goal): RG_r_Main_FFFTTTRG_r_PCVexp_pause_RG292: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_EXPIRATION) and (expiredTIMER_EXPIRATION_DURATION_MS and (exp_pause and (oValve != CLOSED))))))
	statechart->exit();
}
TEST_F(TestClass, RG_r_Main_FFFTTTTRG_r_PCVins_pause_RG1262) {
	statechart->enter();
	//// test name test@RG_r_Main_FFFTTTTRG_r_PCVins_pause_RG1262
	//// generated for (test goal): RG_r_Main_FFFTTTTRG_r_PCVins_pause_RG1262: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_INSPIRATION) and (expiredTIMER_INSPIRATION_DURATION_MS and ((mode = PCV) and (ins_pause and (state != MAIN_REGION_PCV_R1_INSPIRATORYPAUSE)))))))
	statechart->exit();
}
TEST_F(TestClass, RG_r_Main_FFFTTTTRG_r_PCVins_pause_RG2262) {
	statechart->enter();
	//// test name test@RG_r_Main_FFFTTTTRG_r_PCVins_pause_RG2262
	//// generated for (test goal): RG_r_Main_FFFTTTTRG_r_PCVins_pause_RG2262: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_INSPIRATION) and (expiredTIMER_INSPIRATION_DURATION_MS and ((mode = PCV) and (ins_pause and (iValve != CLOSED)))))))
	statechart->exit();
}
TEST_F(TestClass, RG_r_Main_FFFTTTTRG_r_PSVins_pauseFromPCV_RG1162) {
	statechart->enter();
	//// test name test@RG_r_Main_FFFTTTTRG_r_PSVins_pauseFromPCV_RG1162
	//// generated for (test goal): RG_r_Main_FFFTTTTRG_r_PSVins_pauseFromPCV_RG1162: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_INSPIRATION) and (expiredTIMER_INSPIRATION_DURATION_MS and ((mode = PSV) and (ins_pause and (state != MAIN_REGION_PSV_R1_INSPIRATORYPAUSE)))))))
	statechart->exit();
}
TEST_F(TestClass, RG_r_Main_FFFTTTTRG_r_PSVins_pauseFromPCV_RG2162) {
	statechart->enter();
	//// test name test@RG_r_Main_FFFTTTTRG_r_PSVins_pauseFromPCV_RG2162
	//// generated for (test goal): RG_r_Main_FFFTTTTRG_r_PSVins_pauseFromPCV_RG2162: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_INSPIRATION) and (expiredTIMER_INSPIRATION_DURATION_MS and ((mode = PSV) and (ins_pause and (iValve != CLOSED)))))))
	statechart->exit();
}
TEST_F(TestClass, RG_r_Main_FFFTTTTRG_r_PSVins_pauseFromPCV_RG3162) {
	statechart->enter();
	//// test name test@RG_r_Main_FFFTTTTRG_r_PSVins_pauseFromPCV_RG3162
	//// generated for (test goal): RG_r_Main_FFFTTTTRG_r_PSVins_pauseFromPCV_RG3162: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_INSPIRATION) and (expiredTIMER_INSPIRATION_DURATION_MS and ((mode = PSV) and (ins_pause and (apnea_backup_mode != false)))))))
	statechart->exit();
}
TEST_F(TestClass, RG_r_Main_FTRG_r_ventOffPCV_RG32) {
	statechart->enter();
	//// test name test@RG_r_Main_FTRG_r_ventOffPCV_RG32
	//// generated for (test goal): RG_r_Main_FTRG_r_ventOffPCV_RG32: not poweroff and (((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (apnea_backup_mode != false))
	statechart->exit();
}
TEST_F(TestClass, RG_r_Main_TRG_r_turnOff_RG12) {
	statechart->enter();
	//// test name test@RG_r_Main_TRG_r_turnOff_RG12
	//// generated for (test goal): RG_r_Main_TRG_r_turnOff_RG12: poweroff and (iValve != CLOSED)
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_STARTUP));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	statechart->raiseStartupEnded();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_SELFTEST));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	statechart->raiseSelfTestPassed();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_VENTILATIONOFF));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PSV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	statechart->raiseStartVentilation();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , OPEN);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION_PSV_R1_INSPIRATION));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , CLOSED);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(true);
	statechart->raiseStopVentilation();
	statechart->raisePoweroff();
	runner->proceed_time(100);
	EXPECT_EQ(statechart->getStopVentilationReq() , false);
	EXPECT_EQ(defaultMock->getInValveStatus() , CLOSED);
	EXPECT_EQ((min_exp_time_psvMock->*(min_exp_time_psvMock->getBehavior()))() , 1000);
	EXPECT_EQ(statechart->getApnea_backup_mode() , false);
	EXPECT_EQ(statechart->getExpiration_duration_ms() , 4000);
	EXPECT_EQ(statechart->getMin_insp_time_ms() , 1000);
	EXPECT_EQ(statechart->getMax_rm_time() , 10000);
	EXPECT_EQ(statechart->getMax_ins_pause() , 20000);
	EXPECT_EQ(statechart->getTriggerWindowDelay_ms() , 1000);
	EXPECT_EQ(statechart->getMax_insp_time_psv() , 7000);
	EXPECT_EQ(statechart->getMax_exp_pause() , 20000);
	EXPECT_TRUE(statechart->isStateActive(MVMStateMachineCore::MVMStateMachineCoreStates::MAIN_REGION__final_));
	EXPECT_EQ(statechart->getApnealag() , 10000);
	EXPECT_EQ(defaultMock->getOutValveStatus() , OPEN);
	EXPECT_EQ(statechart->getInspiration_duration_ms() , 2000);
	flowDropPSVMock->setDefaultBehavior(false);
	statechart->setIns_pause(false);
	statechart->setMode(PCV);
	dropPAW_ITS_PCVMock->setDefaultBehavior(false);
	pawGTMaxPinspMock->setDefaultBehavior(false);
	statechart->setExp_pause(false);
	dropPAW_ITS_PSVMock->setDefaultBehavior(false);
	statechart->setRm_request(false);
	runner->proceed_time(100);
	statechart->exit();
}
TEST_F(TestClass, UR_r_Main_FFFTFFFTUR_r_PCVinspOValve_2142) {
	statechart->enter();
	//// test name test@UR_r_Main_FFFTFFFTUR_r_PCVinspOValve_2142
	//// generated for (test goal): UR_r_Main_FFFTFFFTUR_r_PCVinspOValve_2142: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PSV_R1_EXPIRATION) and (not(expiredTIMER_TRIGGERWINDOWDELAY_MS and dropPAW_ITS_PSV) and (not expiredTIMER_APNEALAG and (not((expiredTIMER_MIN_EXP_TIME_PSV and (mode = PSV)) and exp_pause) and ((expiredTIMER_MIN_EXP_TIME_PSV and (mode = PCV)) and (oValve != CLOSED))))))))
	statechart->exit();
}
TEST_F(TestClass, UR_r_Main_FFFTFFFTUR_r_PCVinspOValve_UR_r_PCVinsp_11142) {
	statechart->enter();
	//// test name test@UR_r_Main_FFFTFFFTUR_r_PCVinspOValve_UR_r_PCVinsp_11142
	//// generated for (test goal): UR_r_Main_FFFTFFFTUR_r_PCVinspOValve_UR_r_PCVinsp_11142: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PSV_R1_EXPIRATION) and (not(expiredTIMER_TRIGGERWINDOWDELAY_MS and dropPAW_ITS_PSV) and (not expiredTIMER_APNEALAG and (not((expiredTIMER_MIN_EXP_TIME_PSV and (mode = PSV)) and exp_pause) and ((expiredTIMER_MIN_EXP_TIME_PSV and (mode = PCV)) and (state != MAIN_REGION_PCV_R1_INSPIRATION))))))))
	statechart->exit();
}
TEST_F(TestClass, UR_r_Main_FFFTFFFTUR_r_PCVinspOValve_UR_r_PCVinsp_21142) {
	statechart->enter();
	//// test name test@UR_r_Main_FFFTFFFTUR_r_PCVinspOValve_UR_r_PCVinsp_21142
	//// generated for (test goal): UR_r_Main_FFFTFFFTUR_r_PCVinspOValve_UR_r_PCVinsp_21142: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PSV_R1_EXPIRATION) and (not(expiredTIMER_TRIGGERWINDOWDELAY_MS and dropPAW_ITS_PSV) and (not expiredTIMER_APNEALAG and (not((expiredTIMER_MIN_EXP_TIME_PSV and (mode = PSV)) and exp_pause) and ((expiredTIMER_MIN_EXP_TIME_PSV and (mode = PCV)) and (iValve != OPEN))))))))
	statechart->exit();
}
TEST_F(TestClass, UR_r_Main_FFFTFFTUR_r_PSVexp_pause_1142) {
	statechart->enter();
	//// test name test@UR_r_Main_FFFTFFTUR_r_PSVexp_pause_1142
	//// generated for (test goal): UR_r_Main_FFFTFFTUR_r_PSVexp_pause_1142: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PSV_R1_EXPIRATION) and (not(expiredTIMER_TRIGGERWINDOWDELAY_MS and dropPAW_ITS_PSV) and (not expiredTIMER_APNEALAG and (((expiredTIMER_MIN_EXP_TIME_PSV and (mode = PSV)) and exp_pause) and (state != MAIN_REGION_PSV_R1_EXPIRATORYPAUSE)))))))
	statechart->exit();
}
TEST_F(TestClass, UR_r_Main_FFFTFFTUR_r_PSVexp_pause_2142) {
	statechart->enter();
	//// test name test@UR_r_Main_FFFTFFTUR_r_PSVexp_pause_2142
	//// generated for (test goal): UR_r_Main_FFFTFFTUR_r_PSVexp_pause_2142: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PSV_R1_EXPIRATION) and (not(expiredTIMER_TRIGGERWINDOWDELAY_MS and dropPAW_ITS_PSV) and (not expiredTIMER_APNEALAG and (((expiredTIMER_MIN_EXP_TIME_PSV and (mode = PSV)) and exp_pause) and (oValve != CLOSED)))))))
	statechart->exit();
}
TEST_F(TestClass, UR_r_Main_FFFTFTTUR_r_PCVinspOValve_292) {
	statechart->enter();
	//// test name test@UR_r_Main_FFFTFTTUR_r_PCVinspOValve_292
	//// generated for (test goal): UR_r_Main_FFFTFTTUR_r_PCVinspOValve_292: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_EXPIRATION) and (not expiredTIMER_EXPIRATION_DURATION_MS and (expiredTIMER_TRIGGERWINDOWDELAY_MS and (dropPAW_ITS_PCV and (oValve != CLOSED)))))))
	statechart->exit();
}
TEST_F(TestClass, UR_r_Main_FFFTFTTUR_r_PCVinspOValve_UR_r_PCVinsp_1192) {
	statechart->enter();
	//// test name test@UR_r_Main_FFFTFTTUR_r_PCVinspOValve_UR_r_PCVinsp_1192
	//// generated for (test goal): UR_r_Main_FFFTFTTUR_r_PCVinspOValve_UR_r_PCVinsp_1192: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_EXPIRATION) and (not expiredTIMER_EXPIRATION_DURATION_MS and (expiredTIMER_TRIGGERWINDOWDELAY_MS and (dropPAW_ITS_PCV and (state != MAIN_REGION_PCV_R1_INSPIRATION)))))))
	statechart->exit();
}
TEST_F(TestClass, UR_r_Main_FFFTFTTUR_r_PCVinspOValve_UR_r_PCVinsp_2192) {
	statechart->enter();
	//// test name test@UR_r_Main_FFFTFTTUR_r_PCVinspOValve_UR_r_PCVinsp_2192
	//// generated for (test goal): UR_r_Main_FFFTFTTUR_r_PCVinspOValve_UR_r_PCVinsp_2192: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_EXPIRATION) and (not expiredTIMER_EXPIRATION_DURATION_MS and (expiredTIMER_TRIGGERWINDOWDELAY_MS and (dropPAW_ITS_PCV and (iValve != OPEN)))))))
	statechart->exit();
}
TEST_F(TestClass, UR_r_Main_FFFTFTUR_r_PCVexp_172) {
	statechart->enter();
	//// test name test@UR_r_Main_FFFTFTUR_r_PCVexp_172
	//// generated for (test goal): UR_r_Main_FFFTFTUR_r_PCVexp_172: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_INSPIRATORYPAUSE) and (not expiredTIMER_MAX_INS_PAUSE and (not ins_pause and (state != MAIN_REGION_PCV_R1_EXPIRATION))))))
	statechart->exit();
}
TEST_F(TestClass, UR_r_Main_FFFTFTUR_r_PCVexp_272) {
	statechart->enter();
	//// test name test@UR_r_Main_FFFTFTUR_r_PCVexp_272
	//// generated for (test goal): UR_r_Main_FFFTFTUR_r_PCVexp_272: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_INSPIRATORYPAUSE) and (not expiredTIMER_MAX_INS_PAUSE and (not ins_pause and (oValve != OPEN))))))
	statechart->exit();
}
TEST_F(TestClass, UR_r_Main_FFFTFTUR_r_PCVinspApnea_2142) {
	statechart->enter();
	//// test name test@UR_r_Main_FFFTFTUR_r_PCVinspApnea_2142
	//// generated for (test goal): UR_r_Main_FFFTFTUR_r_PCVinspApnea_2142: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PSV_R1_EXPIRATION) and (not(expiredTIMER_TRIGGERWINDOWDELAY_MS and dropPAW_ITS_PSV) and (expiredTIMER_APNEALAG and (apneaAlarm != true))))))
	statechart->exit();
}
TEST_F(TestClass, UR_r_Main_FFFTFTUR_r_PCVinspApnea_3142) {
	statechart->enter();
	//// test name test@UR_r_Main_FFFTFTUR_r_PCVinspApnea_3142
	//// generated for (test goal): UR_r_Main_FFFTFTUR_r_PCVinspApnea_3142: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PSV_R1_EXPIRATION) and (not(expiredTIMER_TRIGGERWINDOWDELAY_MS and dropPAW_ITS_PSV) and (expiredTIMER_APNEALAG and (apnea_backup_mode != true))))))
	statechart->exit();
}
TEST_F(TestClass, UR_r_Main_FFFTFTUR_r_PCVinspApnea_UR_r_PCVinspOValve_21142) {
	statechart->enter();
	//// test name test@UR_r_Main_FFFTFTUR_r_PCVinspApnea_UR_r_PCVinspOValve_21142
	//// generated for (test goal): UR_r_Main_FFFTFTUR_r_PCVinspApnea_UR_r_PCVinspOValve_21142: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PSV_R1_EXPIRATION) and (not(expiredTIMER_TRIGGERWINDOWDELAY_MS and dropPAW_ITS_PSV) and (expiredTIMER_APNEALAG and (oValve != CLOSED))))))
	statechart->exit();
}
TEST_F(TestClass, UR_r_Main_FFFTFTUR_r_PCVinspApnea_UR_r_PCVinspOValve_UR_r_PCVinsp_111142) {
	statechart->enter();
	//// test name test@UR_r_Main_FFFTFTUR_r_PCVinspApnea_UR_r_PCVinspOValve_UR_r_PCVinsp_111142
	//// generated for (test goal): UR_r_Main_FFFTFTUR_r_PCVinspApnea_UR_r_PCVinspOValve_UR_r_PCVinsp_111142: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PSV_R1_EXPIRATION) and (not(expiredTIMER_TRIGGERWINDOWDELAY_MS and dropPAW_ITS_PSV) and (expiredTIMER_APNEALAG and (state != MAIN_REGION_PCV_R1_INSPIRATION))))))
	statechart->exit();
}
TEST_F(TestClass, UR_r_Main_FFFTFTUR_r_PCVinspApnea_UR_r_PCVinspOValve_UR_r_PCVinsp_211142) {
	statechart->enter();
	//// test name test@UR_r_Main_FFFTFTUR_r_PCVinspApnea_UR_r_PCVinspOValve_UR_r_PCVinsp_211142
	//// generated for (test goal): UR_r_Main_FFFTFTUR_r_PCVinspApnea_UR_r_PCVinspOValve_UR_r_PCVinsp_211142: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PSV_R1_EXPIRATION) and (not(expiredTIMER_TRIGGERWINDOWDELAY_MS and dropPAW_ITS_PSV) and (expiredTIMER_APNEALAG and (iValve != OPEN))))))
	statechart->exit();
}
TEST_F(TestClass, UR_r_Main_FFFTFTUR_r_PCVinsp_1102) {
	statechart->enter();
	//// test name test@UR_r_Main_FFFTFTUR_r_PCVinsp_1102
	//// generated for (test goal): UR_r_Main_FFFTFTUR_r_PCVinsp_1102: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_EXPIRATORYPAUSE) and (not expiredTIMER_MAX_EXP_PAUSE and (not exp_pause and (state != MAIN_REGION_PCV_R1_INSPIRATION))))))
	statechart->exit();
}
TEST_F(TestClass, UR_r_Main_FFFTFTUR_r_PCVinsp_2102) {
	statechart->enter();
	//// test name test@UR_r_Main_FFFTFTUR_r_PCVinsp_2102
	//// generated for (test goal): UR_r_Main_FFFTFTUR_r_PCVinsp_2102: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_EXPIRATORYPAUSE) and (not expiredTIMER_MAX_EXP_PAUSE and (not exp_pause and (iValve != OPEN))))))
	statechart->exit();
}
TEST_F(TestClass, UR_r_Main_FFFTFTUR_r_PSVinsp_1152) {
	statechart->enter();
	//// test name test@UR_r_Main_FFFTFTUR_r_PSVinsp_1152
	//// generated for (test goal): UR_r_Main_FFFTFTUR_r_PSVinsp_1152: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PSV_R1_EXPIRATORYPAUSE) and (not expiredTIMER_MAX_EXP_PAUSE and (not exp_pause and (state != MAIN_REGION_PSV_R1_INSPIRATION))))))
	statechart->exit();
}
TEST_F(TestClass, UR_r_Main_FFFTFTUR_r_PSVinsp_2152) {
	statechart->enter();
	//// test name test@UR_r_Main_FFFTFTUR_r_PSVinsp_2152
	//// generated for (test goal): UR_r_Main_FFFTFTUR_r_PSVinsp_2152: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PSV_R1_EXPIRATORYPAUSE) and (not expiredTIMER_MAX_EXP_PAUSE and (not exp_pause and (iValve != OPEN))))))
	statechart->exit();
}
TEST_F(TestClass, UR_r_Main_FFFTTFUR_r_PCVinspOValve_292) {
	statechart->enter();
	//// test name test@UR_r_Main_FFFTTFUR_r_PCVinspOValve_292
	//// generated for (test goal): UR_r_Main_FFFTTFUR_r_PCVinspOValve_292: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_EXPIRATION) and (expiredTIMER_EXPIRATION_DURATION_MS and (not exp_pause and (oValve != CLOSED))))))
	statechart->exit();
}
TEST_F(TestClass, UR_r_Main_FFFTTFUR_r_PCVinspOValve_UR_r_PCVinsp_1192) {
	statechart->enter();
	//// test name test@UR_r_Main_FFFTTFUR_r_PCVinspOValve_UR_r_PCVinsp_1192
	//// generated for (test goal): UR_r_Main_FFFTTFUR_r_PCVinspOValve_UR_r_PCVinsp_1192: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_EXPIRATION) and (expiredTIMER_EXPIRATION_DURATION_MS and (not exp_pause and (state != MAIN_REGION_PCV_R1_INSPIRATION))))))
	statechart->exit();
}
TEST_F(TestClass, UR_r_Main_FFFTTFUR_r_PCVinspOValve_UR_r_PCVinsp_2192) {
	statechart->enter();
	//// test name test@UR_r_Main_FFFTTFUR_r_PCVinspOValve_UR_r_PCVinsp_2192
	//// generated for (test goal): UR_r_Main_FFFTTFUR_r_PCVinspOValve_UR_r_PCVinsp_2192: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_EXPIRATION) and (expiredTIMER_EXPIRATION_DURATION_MS and (not exp_pause and (iValve != OPEN))))))
	statechart->exit();
}
TEST_F(TestClass, UR_r_Main_FFFTTTFFUR_r_PCVexpIValve_2262) {
	statechart->enter();
	//// test name test@UR_r_Main_FFFTTTFFUR_r_PCVexpIValve_2262
	//// generated for (test goal): UR_r_Main_FFFTTTFFUR_r_PCVexpIValve_2262: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_INSPIRATION) and (expiredTIMER_INSPIRATION_DURATION_MS and ((mode = PCV) and (not ins_pause and (not rm_request and (iValve != CLOSED))))))))
	statechart->exit();
}
TEST_F(TestClass, UR_r_Main_FFFTTTFFUR_r_PCVexpIValve_UR_r_PCVexp_11262) {
	statechart->enter();
	//// test name test@UR_r_Main_FFFTTTFFUR_r_PCVexpIValve_UR_r_PCVexp_11262
	//// generated for (test goal): UR_r_Main_FFFTTTFFUR_r_PCVexpIValve_UR_r_PCVexp_11262: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_INSPIRATION) and (expiredTIMER_INSPIRATION_DURATION_MS and ((mode = PCV) and (not ins_pause and (not rm_request and (state != MAIN_REGION_PCV_R1_EXPIRATION))))))))
	statechart->exit();
}
TEST_F(TestClass, UR_r_Main_FFFTTTFFUR_r_PCVexpIValve_UR_r_PCVexp_21262) {
	statechart->enter();
	//// test name test@UR_r_Main_FFFTTTFFUR_r_PCVexpIValve_UR_r_PCVexp_21262
	//// generated for (test goal): UR_r_Main_FFFTTTFFUR_r_PCVexpIValve_UR_r_PCVexp_21262: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_INSPIRATION) and (expiredTIMER_INSPIRATION_DURATION_MS and ((mode = PCV) and (not ins_pause and (not rm_request and (oValve != OPEN))))))))
	statechart->exit();
}
TEST_F(TestClass, UR_r_Main_FFFTTTFFUR_r_PSVexpIValveFromPCV_2162) {
	statechart->enter();
	//// test name test@UR_r_Main_FFFTTTFFUR_r_PSVexpIValveFromPCV_2162
	//// generated for (test goal): UR_r_Main_FFFTTTFFUR_r_PSVexpIValveFromPCV_2162: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_INSPIRATION) and (expiredTIMER_INSPIRATION_DURATION_MS and ((mode = PSV) and (not ins_pause and (not rm_request and (iValve != CLOSED))))))))
	statechart->exit();
}
TEST_F(TestClass, UR_r_Main_FFFTTTFFUR_r_PSVexpIValveFromPCV_3162) {
	statechart->enter();
	//// test name test@UR_r_Main_FFFTTTFFUR_r_PSVexpIValveFromPCV_3162
	//// generated for (test goal): UR_r_Main_FFFTTTFFUR_r_PSVexpIValveFromPCV_3162: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_INSPIRATION) and (expiredTIMER_INSPIRATION_DURATION_MS and ((mode = PSV) and (not ins_pause and (not rm_request and (apnea_backup_mode != false))))))))
	statechart->exit();
}
TEST_F(TestClass, UR_r_Main_FFFTTTFFUR_r_PSVexpIValveFromPCV_UR_r_PSVexp_11162) {
	statechart->enter();
	//// test name test@UR_r_Main_FFFTTTFFUR_r_PSVexpIValveFromPCV_UR_r_PSVexp_11162
	//// generated for (test goal): UR_r_Main_FFFTTTFFUR_r_PSVexpIValveFromPCV_UR_r_PSVexp_11162: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_INSPIRATION) and (expiredTIMER_INSPIRATION_DURATION_MS and ((mode = PSV) and (not ins_pause and (not rm_request and (state != MAIN_REGION_PSV_R1_EXPIRATION))))))))
	statechart->exit();
}
TEST_F(TestClass, UR_r_Main_FFFTTTFFUR_r_PSVexpIValveFromPCV_UR_r_PSVexp_21162) {
	statechart->enter();
	//// test name test@UR_r_Main_FFFTTTFFUR_r_PSVexpIValveFromPCV_UR_r_PSVexp_21162
	//// generated for (test goal): UR_r_Main_FFFTTTFFUR_r_PSVexpIValveFromPCV_UR_r_PSVexp_21162: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_INSPIRATION) and (expiredTIMER_INSPIRATION_DURATION_MS and ((mode = PSV) and (not ins_pause and (not rm_request and (oValve != OPEN))))))))
	statechart->exit();
}
TEST_F(TestClass, UR_r_Main_FFFTTTFTUR_r_PCVrm_1262) {
	statechart->enter();
	//// test name test@UR_r_Main_FFFTTTFTUR_r_PCVrm_1262
	//// generated for (test goal): UR_r_Main_FFFTTTFTUR_r_PCVrm_1262: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_INSPIRATION) and (expiredTIMER_INSPIRATION_DURATION_MS and ((mode = PCV) and (not ins_pause and (rm_request and (state != MAIN_REGION_PCV_R1_RM))))))))
	statechart->exit();
}
TEST_F(TestClass, UR_r_Main_FFFTTTFTUR_r_PSVrmFromPCV_1162) {
	statechart->enter();
	//// test name test@UR_r_Main_FFFTTTFTUR_r_PSVrmFromPCV_1162
	//// generated for (test goal): UR_r_Main_FFFTTTFTUR_r_PSVrmFromPCV_1162: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_INSPIRATION) and (expiredTIMER_INSPIRATION_DURATION_MS and ((mode = PSV) and (not ins_pause and (rm_request and (state != MAIN_REGION_PSV_R1_RM))))))))
	statechart->exit();
}
TEST_F(TestClass, UR_r_Main_FFFTTTFTUR_r_PSVrmFromPCV_2162) {
	statechart->enter();
	//// test name test@UR_r_Main_FFFTTTFTUR_r_PSVrmFromPCV_2162
	//// generated for (test goal): UR_r_Main_FFFTTTFTUR_r_PSVrmFromPCV_2162: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_INSPIRATION) and (expiredTIMER_INSPIRATION_DURATION_MS and ((mode = PSV) and (not ins_pause and (rm_request and (apnea_backup_mode != false))))))))
	statechart->exit();
}
TEST_F(TestClass, UR_r_Main_FFFTTTTUR_r_PCVins_pause_1262) {
	statechart->enter();
	//// test name test@UR_r_Main_FFFTTTTUR_r_PCVins_pause_1262
	//// generated for (test goal): UR_r_Main_FFFTTTTUR_r_PCVins_pause_1262: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_INSPIRATION) and (expiredTIMER_INSPIRATION_DURATION_MS and ((mode = PCV) and (ins_pause and (state != MAIN_REGION_PCV_R1_INSPIRATORYPAUSE)))))))
	statechart->exit();
}
TEST_F(TestClass, UR_r_Main_FFFTTTTUR_r_PCVins_pause_2262) {
	statechart->enter();
	//// test name test@UR_r_Main_FFFTTTTUR_r_PCVins_pause_2262
	//// generated for (test goal): UR_r_Main_FFFTTTTUR_r_PCVins_pause_2262: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_INSPIRATION) and (expiredTIMER_INSPIRATION_DURATION_MS and ((mode = PCV) and (ins_pause and (iValve != CLOSED)))))))
	statechart->exit();
}
TEST_F(TestClass, UR_r_Main_FFFTTTTUR_r_PSVins_pauseFromPCV_1162) {
	statechart->enter();
	//// test name test@UR_r_Main_FFFTTTTUR_r_PSVins_pauseFromPCV_1162
	//// generated for (test goal): UR_r_Main_FFFTTTTUR_r_PSVins_pauseFromPCV_1162: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_INSPIRATION) and (expiredTIMER_INSPIRATION_DURATION_MS and ((mode = PSV) and (ins_pause and (state != MAIN_REGION_PSV_R1_INSPIRATORYPAUSE)))))))
	statechart->exit();
}
TEST_F(TestClass, UR_r_Main_FFFTTTTUR_r_PSVins_pauseFromPCV_2162) {
	statechart->enter();
	//// test name test@UR_r_Main_FFFTTTTUR_r_PSVins_pauseFromPCV_2162
	//// generated for (test goal): UR_r_Main_FFFTTTTUR_r_PSVins_pauseFromPCV_2162: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_INSPIRATION) and (expiredTIMER_INSPIRATION_DURATION_MS and ((mode = PSV) and (ins_pause and (iValve != CLOSED)))))))
	statechart->exit();
}
TEST_F(TestClass, UR_r_Main_FFFTTTTUR_r_PSVins_pauseFromPCV_3162) {
	statechart->enter();
	//// test name test@UR_r_Main_FFFTTTTUR_r_PSVins_pauseFromPCV_3162
	//// generated for (test goal): UR_r_Main_FFFTTTTUR_r_PSVins_pauseFromPCV_3162: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_INSPIRATION) and (expiredTIMER_INSPIRATION_DURATION_MS and ((mode = PSV) and (ins_pause and (apnea_backup_mode != false)))))))
	statechart->exit();
}
TEST_F(TestClass, UR_r_Main_FFFTTTUR_r_PCVexp_pause_192) {
	statechart->enter();
	//// test name test@UR_r_Main_FFFTTTUR_r_PCVexp_pause_192
	//// generated for (test goal): UR_r_Main_FFFTTTUR_r_PCVexp_pause_192: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_EXPIRATION) and (expiredTIMER_EXPIRATION_DURATION_MS and (exp_pause and (state != MAIN_REGION_PCV_R1_EXPIRATORYPAUSE))))))
	statechart->exit();
}
TEST_F(TestClass, UR_r_Main_FFFTTTUR_r_PCVexp_pause_292) {
	statechart->enter();
	//// test name test@UR_r_Main_FFFTTTUR_r_PCVexp_pause_292
	//// generated for (test goal): UR_r_Main_FFFTTTUR_r_PCVexp_pause_292: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_EXPIRATION) and (expiredTIMER_EXPIRATION_DURATION_MS and (exp_pause and (oValve != CLOSED))))))
	statechart->exit();
}
TEST_F(TestClass, UR_r_Main_FFFTTUR_r_apneaAlarmOFF_12) {
	statechart->enter();
	//// test name test@UR_r_Main_FFFTTUR_r_apneaAlarmOFF_12
	//// generated for (test goal): UR_r_Main_FFFTTUR_r_apneaAlarmOFF_12: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (apneaAlarm and (((turnOffApneaAlarm = true) and ((state = MAIN_REGION_PCV_R1_INSPIRATION) or (state = MAIN_REGION_PCV_R1_EXPIRATION))) and (apneaAlarm != false)))))
	statechart->exit();
}
TEST_F(TestClass, UR_r_Main_FFFTTUR_r_PCVexpIValve_282) {
	statechart->enter();
	//// test name test@UR_r_Main_FFFTTUR_r_PCVexpIValve_282
	//// generated for (test goal): UR_r_Main_FFFTTUR_r_PCVexpIValve_282: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_RM) and ((expiredTIMER_MAX_RM_TIME or rm_request) and (iValve != CLOSED)))))
	statechart->exit();
}
TEST_F(TestClass, UR_r_Main_FFFTTUR_r_PCVexpIValve_UR_r_PCVexp_1182) {
	statechart->enter();
	//// test name test@UR_r_Main_FFFTTUR_r_PCVexpIValve_UR_r_PCVexp_1182
	//// generated for (test goal): UR_r_Main_FFFTTUR_r_PCVexpIValve_UR_r_PCVexp_1182: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_RM) and ((expiredTIMER_MAX_RM_TIME or rm_request) and (state != MAIN_REGION_PCV_R1_EXPIRATION)))))
	statechart->exit();
}
TEST_F(TestClass, UR_r_Main_FFFTTUR_r_PCVexpIValve_UR_r_PCVexp_2182) {
	statechart->enter();
	//// test name test@UR_r_Main_FFFTTUR_r_PCVexpIValve_UR_r_PCVexp_2182
	//// generated for (test goal): UR_r_Main_FFFTTUR_r_PCVexpIValve_UR_r_PCVexp_2182: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_RM) and ((expiredTIMER_MAX_RM_TIME or rm_request) and (oValve != OPEN)))))
	statechart->exit();
}
TEST_F(TestClass, UR_r_Main_FFFTTUR_r_PCVexp_172) {
	statechart->enter();
	//// test name test@UR_r_Main_FFFTTUR_r_PCVexp_172
	//// generated for (test goal): UR_r_Main_FFFTTUR_r_PCVexp_172: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_INSPIRATORYPAUSE) and (expiredTIMER_MAX_INS_PAUSE and (state != MAIN_REGION_PCV_R1_EXPIRATION)))))
	statechart->exit();
}
TEST_F(TestClass, UR_r_Main_FFFTTUR_r_PCVexp_272) {
	statechart->enter();
	//// test name test@UR_r_Main_FFFTTUR_r_PCVexp_272
	//// generated for (test goal): UR_r_Main_FFFTTUR_r_PCVexp_272: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_INSPIRATORYPAUSE) and (expiredTIMER_MAX_INS_PAUSE and (oValve != OPEN)))))
	statechart->exit();
}
TEST_F(TestClass, UR_r_Main_FFFTTUR_r_PCVinsp_1102) {
	statechart->enter();
	//// test name test@UR_r_Main_FFFTTUR_r_PCVinsp_1102
	//// generated for (test goal): UR_r_Main_FFFTTUR_r_PCVinsp_1102: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_EXPIRATORYPAUSE) and (expiredTIMER_MAX_EXP_PAUSE and (state != MAIN_REGION_PCV_R1_INSPIRATION)))))
	statechart->exit();
}
TEST_F(TestClass, UR_r_Main_FFFTTUR_r_PCVinsp_2102) {
	statechart->enter();
	//// test name test@UR_r_Main_FFFTTUR_r_PCVinsp_2102
	//// generated for (test goal): UR_r_Main_FFFTTUR_r_PCVinsp_2102: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PCV_R1_EXPIRATORYPAUSE) and (expiredTIMER_MAX_EXP_PAUSE and (iValve != OPEN)))))
	statechart->exit();
}
TEST_F(TestClass, UR_r_Main_FFFTTUR_r_PSVexp_1122) {
	statechart->enter();
	//// test name test@UR_r_Main_FFFTTUR_r_PSVexp_1122
	//// generated for (test goal): UR_r_Main_FFFTTUR_r_PSVexp_1122: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PSV_R1_INSPIRATORYPAUSE) and (expiredTIMER_MAX_INS_PAUSE and (state != MAIN_REGION_PSV_R1_EXPIRATION)))))
	statechart->exit();
}
TEST_F(TestClass, UR_r_Main_FFFTTUR_r_PSVexp_2122) {
	statechart->enter();
	//// test name test@UR_r_Main_FFFTTUR_r_PSVexp_2122
	//// generated for (test goal): UR_r_Main_FFFTTUR_r_PSVexp_2122: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PSV_R1_INSPIRATORYPAUSE) and (expiredTIMER_MAX_INS_PAUSE and (oValve != OPEN)))))
	statechart->exit();
}
TEST_F(TestClass, UR_r_Main_FFFTTUR_r_PSVinsp_1152) {
	statechart->enter();
	//// test name test@UR_r_Main_FFFTTUR_r_PSVinsp_1152
	//// generated for (test goal): UR_r_Main_FFFTTUR_r_PSVinsp_1152: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PSV_R1_EXPIRATORYPAUSE) and (expiredTIMER_MAX_EXP_PAUSE and (state != MAIN_REGION_PSV_R1_INSPIRATION)))))
	statechart->exit();
}
TEST_F(TestClass, UR_r_Main_FFFTTUR_r_PSVinsp_2152) {
	statechart->enter();
	//// test name test@UR_r_Main_FFFTTUR_r_PSVinsp_2152
	//// generated for (test goal): UR_r_Main_FFFTTUR_r_PSVinsp_2152: not poweroff and (not((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (not((state = MAIN_REGION_PSV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and ((state = MAIN_REGION_PSV_R1_EXPIRATORYPAUSE) and (expiredTIMER_MAX_EXP_PAUSE and (iValve != OPEN)))))
	statechart->exit();
}
TEST_F(TestClass, UR_r_Main_FTUR_r_ventOffPCV_32) {
	statechart->enter();
	//// test name test@UR_r_Main_FTUR_r_ventOffPCV_32
	//// generated for (test goal): UR_r_Main_FTUR_r_ventOffPCV_32: not poweroff and (((state = MAIN_REGION_PCV_R1_EXPIRATION) and (stopVentilationRequested or stopVentilation)) and (apnea_backup_mode != false))
	statechart->exit();
}
