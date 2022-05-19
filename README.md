# RATE: Refinement and Test Execution 

RATE is a model-based testing approach that combines model Refinement and Test Execution.

### Repository content

The repository contains all the results we have obtained by applying RATE to two case studies

* PHD_Protocol: the PHD Protocol
  * ASM: the project containing ASMETA files
	* ./PHD_Protocol_X/: the ASMETA specifications for each level X
	* ./PHD_Protocol_X/scenarios: the scenarios used for manual test on the specification of level X
  * Crossvalidation_PHD: the java project used for crossvalidating the ASM specification written with and without refinement
  * Results: the coverage results and data analysis
  * StateMachines and Sequences: all the state machines that are given as inputs to ProTest, and the corresponging test sequences   

* Traffic_Light_Control_System: the Traffic Light Control System, taken from the examples proposed by Yakindu SCT 
  * ASM: all the asmeta specications
  * Avalla: the scenarios used for manual test
  * java: all the java code of the TLCS (as given by the example of Yakindu SCT)
	* Test/TestExecutor.java: the test executor to be used for applying RATE to the TLCS case study
  * Results: the coverage results and data analysis
  * StateMachines and Sequences: all the state machines that are given as inputs to the test executor, and the corresponging test sequences 
