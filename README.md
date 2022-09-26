# RATE: Refinement and Test Execution 

RATE is a model-based testing approach that combines model Refinement and Test Execution.

### Repository content

The repository contains all the results we have obtained by applying RATE to three case studies

* AbstractTestGenerator: the project generating the abstract tests (in avalla format) from the ASMs
* Crossvalidation: the project allowing to crossvalidate the specification written without refinement with the one obtained with the last refinement level
* Case_studies: the files specific for each case study
  * PHD_Protocol: the PHD Protocol
  	* ASM: the project containing ASMETA files
		* ./PHD_Protocol_X/: the ASMETA specifications for each level X
		* ./PHD_Protocol_X/scenarios: the scenarios used for manual test on the specification of level X
  	* SUT: the submodule containing the source file for the PHD Protocol
  	* abstract_tests: the folder containing the abstract tests generated by ATGT
		* ./PHD_Protocol_X/: the abstract tests for each level X
  	* Results: the coverage results and data analysis
  	* TestExecutor: the test executor. It receives as input the abstract tests in avalla format and executes them on the SUT by using the ProTest tool (developed by the University of Texas at Arlington)
  	* mutation_test: the script simulating mutation in the Finite State Machine of the PHD manager. It exploits the ProTest tool (developed by the University of Texas at Arlington) for executing the tests

  * Traffic_Light_Control_System: the Traffic Light Control System, taken from the examples proposed by Yakindu SCT 
  	* ASM: the project containing ASMETA files
		* ./TrafficLight_X/: the ASMETA specifications for each level X
		* ./TrafficLight_X/scenarios: the scenarios used for manual test on the specification of level X
  	* SUT: all the java code of the TLCS (as given by the example of Yakindu SCT)
  		* ./test/TestExecutorForPit.java: the script that performs mutation testing. To be executed with Maven, using the goal ```org.pitest:pitest-maven:mutationCoverage```
  	* abstract_tests: the folder containing the abstract tests generated by ATGT
		* ./TrafficLight_X/: the abstract tests for each level X
  	* Results: the coverage results and data analysis
  	* TestExecutor: the test executor. It receives as input the abstract tests in avalla format and executes them on the SUT

  * MVM: The [Milano Ventilatore Meccanico](http://mvm.care/it/home-it/) case study. The SUT is not publicly available. If you need it for your experiments, please contact us.
  	* ASM: the project containing ASMETA files
		* ./MVM_X/: the ASMETA specifications for each level X
		* ./MVM_X/scenarios: the scenarios used for manual test on the specification of level X
	* astract_tests: the folder containing the abstract tests generated by ATGT
		* ./MVM_X/: the abstract tests for each level X
		
Many of the Eclipse projects contained into this repository depend on [atgt](https://github.com/asmeta/asmeta/tree/master/code/extensions/asmeta.atgt).
  
