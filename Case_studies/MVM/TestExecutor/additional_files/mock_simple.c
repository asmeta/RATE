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
