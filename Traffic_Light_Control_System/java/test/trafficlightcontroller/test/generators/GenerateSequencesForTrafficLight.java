package trafficlightcontroller.test.generators;
import java.io.File;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import org.apache.log4j.Level;
import org.apache.log4j.Logger;
import org.asmeta.atgt.generator.CriteriaEnum;
import org.asmeta.atgt.generator.GenerateTestsFromFSM;
import org.junit.Test;

import extgt.coverage.combinatorial.StdPairwiseCovBuild;
import trafficlightcontroller.test.util.FSMCreator;

public class GenerateSequencesForTrafficLight {

	class ConfigurationNoGUI {
		String fsmPath;
		String asmPath;
		int nStatuses;
		int nTransitions;
		int nMessages;
		List<CriteriaEnum> criteria;
		boolean useMonitoring;
		
		public ConfigurationNoGUI(String fsmPath, String asmPath, int nStatuses, int nTransitions, int nMessages, List<CriteriaEnum> criteria) {
			this.fsmPath = fsmPath;
			this.asmPath = asmPath;
			this.criteria = criteria;
			this.useMonitoring = true;
			this.nMessages = nMessages;
			this.nTransitions = nTransitions;
			this.nStatuses = nStatuses;
		}
		
		public ConfigurationNoGUI(String fsmPath, String asmPath, int nStatuses, int nTransitions, int nMessages, boolean useMonitoring, List<CriteriaEnum> criteria) {
			this.fsmPath = fsmPath;
			this.asmPath = asmPath;
			this.criteria = criteria;
			this.useMonitoring = useMonitoring;
			this.nMessages = nMessages;
			this.nTransitions = nTransitions;
			this.nStatuses = nStatuses;
		}
		
		public ConfigurationNoGUI(String fsmPath, String asmPath, int nStatuses, int nTransitions, int nMessages, CriteriaEnum ... criteria) {
			this(fsmPath,asmPath,nStatuses,nTransitions,nMessages,Arrays.asList(criteria));
		}
		
		public ConfigurationNoGUI(String fsmPath, String asmPath, int nStatuses, int nTransitions, int nMessages, boolean useMonitoring, CriteriaEnum ... criteria) {
			this(fsmPath,asmPath,nStatuses,nTransitions,nMessages,useMonitoring,Arrays.asList(criteria));
		}
	}

	static String ASM_TRAFFICLIGHT_FOLDER = "F:\\Dati-Andrea\\GitHub\\ProTestUTA\\trafficlightcontroller\\ASM\\";

	List<ConfigurationNoGUI> filesToProcess = new ArrayList<ConfigurationNoGUI>() {
		
		private static final long serialVersionUID = 5014712752302397500L;
		{
			add(new ConfigurationNoGUI("", ASM_TRAFFICLIGHT_FOLDER + "TrafficLight0//TrafficLight_0.asm", 3, 1, 2, CriteriaEnum.BASIC_RULE));
			add(new ConfigurationNoGUI("", ASM_TRAFFICLIGHT_FOLDER + "TrafficLight0//TrafficLight_0.asm", 3, 1, 2, CriteriaEnum.COMBINATORIAL_ALL));
			add(new ConfigurationNoGUI("", ASM_TRAFFICLIGHT_FOLDER + "TrafficLight0//TrafficLight_0.asm", 3, 1, 2, CriteriaEnum.COMPLETE_RULE));
			add(new ConfigurationNoGUI("", ASM_TRAFFICLIGHT_FOLDER + "TrafficLight0//TrafficLight_0.asm", 3, 1, 2, CriteriaEnum.MCDC));
			add(new ConfigurationNoGUI("", ASM_TRAFFICLIGHT_FOLDER + "TrafficLight0//TrafficLight_0.asm", 3, 1, 2, CriteriaEnum.RULE_GUARD));
			add(new ConfigurationNoGUI("", ASM_TRAFFICLIGHT_FOLDER + "TrafficLight0//TrafficLight_0.asm", 3, 1, 2, CriteriaEnum.RULE_UPDATE));
			add(new ConfigurationNoGUI("", ASM_TRAFFICLIGHT_FOLDER + "TrafficLight0//TrafficLight_0.asm", 3, 1, 2, CriteriaEnum.THREEWISE));
			add(new ConfigurationNoGUI("", ASM_TRAFFICLIGHT_FOLDER + "TrafficLight0//TrafficLight_0.asm", 3, 1, 2, CriteriaEnum.BASIC_RULE, CriteriaEnum.COMBINATORIAL_ALL,
					CriteriaEnum.COMPLETE_RULE, CriteriaEnum.MCDC, CriteriaEnum.RULE_GUARD, CriteriaEnum.RULE_UPDATE, CriteriaEnum.THREEWISE));
			
			add(new ConfigurationNoGUI("", ASM_TRAFFICLIGHT_FOLDER + "TrafficLight1//TrafficLight_1.asm", 4, 2, 2, CriteriaEnum.BASIC_RULE));
			add(new ConfigurationNoGUI("", ASM_TRAFFICLIGHT_FOLDER + "TrafficLight1//TrafficLight_1.asm", 4, 2, 2, CriteriaEnum.COMBINATORIAL_ALL));
			add(new ConfigurationNoGUI("", ASM_TRAFFICLIGHT_FOLDER + "TrafficLight1//TrafficLight_1.asm", 4, 2, 2, CriteriaEnum.COMPLETE_RULE));
			add(new ConfigurationNoGUI("", ASM_TRAFFICLIGHT_FOLDER + "TrafficLight1//TrafficLight_1.asm", 4, 2, 2, CriteriaEnum.MCDC));
			add(new ConfigurationNoGUI("", ASM_TRAFFICLIGHT_FOLDER + "TrafficLight1//TrafficLight_1.asm", 4, 2, 2, CriteriaEnum.RULE_GUARD));
			add(new ConfigurationNoGUI("", ASM_TRAFFICLIGHT_FOLDER + "TrafficLight1//TrafficLight_1.asm", 4, 2, 2, CriteriaEnum.RULE_UPDATE));
			add(new ConfigurationNoGUI("", ASM_TRAFFICLIGHT_FOLDER + "TrafficLight1//TrafficLight_1.asm", 4, 2, 2, CriteriaEnum.THREEWISE));
			add(new ConfigurationNoGUI("", ASM_TRAFFICLIGHT_FOLDER + "TrafficLight1//TrafficLight_1.asm", 4, 2, 2, CriteriaEnum.BASIC_RULE, CriteriaEnum.COMBINATORIAL_ALL,
					CriteriaEnum.COMPLETE_RULE, CriteriaEnum.MCDC, CriteriaEnum.RULE_GUARD, CriteriaEnum.RULE_UPDATE, CriteriaEnum.THREEWISE));
						
			add(new ConfigurationNoGUI("", ASM_TRAFFICLIGHT_FOLDER + "TrafficLight2//TrafficLight_2.asm", 4, 3, 2, CriteriaEnum.BASIC_RULE));
			add(new ConfigurationNoGUI("", ASM_TRAFFICLIGHT_FOLDER + "TrafficLight2//TrafficLight_2.asm", 4, 3, 2, CriteriaEnum.COMBINATORIAL_ALL));
			add(new ConfigurationNoGUI("", ASM_TRAFFICLIGHT_FOLDER + "TrafficLight2//TrafficLight_2.asm", 4, 3, 2, CriteriaEnum.COMPLETE_RULE));
			add(new ConfigurationNoGUI("", ASM_TRAFFICLIGHT_FOLDER + "TrafficLight2//TrafficLight_2.asm", 4, 3, 2, CriteriaEnum.MCDC));
			add(new ConfigurationNoGUI("", ASM_TRAFFICLIGHT_FOLDER + "TrafficLight2//TrafficLight_2.asm", 4, 3, 2, CriteriaEnum.RULE_GUARD));
			add(new ConfigurationNoGUI("", ASM_TRAFFICLIGHT_FOLDER + "TrafficLight2//TrafficLight_2.asm", 4, 3, 2, CriteriaEnum.RULE_UPDATE));
			add(new ConfigurationNoGUI("", ASM_TRAFFICLIGHT_FOLDER + "TrafficLight2//TrafficLight_2.asm", 4, 3, 2, CriteriaEnum.THREEWISE));
			add(new ConfigurationNoGUI("", ASM_TRAFFICLIGHT_FOLDER + "TrafficLight2//TrafficLight_2.asm", 4, 3, 2, CriteriaEnum.BASIC_RULE, CriteriaEnum.COMBINATORIAL_ALL,
					CriteriaEnum.COMPLETE_RULE, CriteriaEnum.MCDC, CriteriaEnum.RULE_GUARD, CriteriaEnum.RULE_UPDATE, CriteriaEnum.THREEWISE));
			
			add(new ConfigurationNoGUI("", ASM_TRAFFICLIGHT_FOLDER + "TrafficLight3//TrafficLight_3.asm", 4, 3, 2, CriteriaEnum.BASIC_RULE));
			add(new ConfigurationNoGUI("", ASM_TRAFFICLIGHT_FOLDER + "TrafficLight3//TrafficLight_3.asm", 4, 3, 2, CriteriaEnum.COMBINATORIAL_ALL));
			add(new ConfigurationNoGUI("", ASM_TRAFFICLIGHT_FOLDER + "TrafficLight3//TrafficLight_3.asm", 4, 3, 2, CriteriaEnum.COMPLETE_RULE));
			add(new ConfigurationNoGUI("", ASM_TRAFFICLIGHT_FOLDER + "TrafficLight3//TrafficLight_3.asm", 4, 3, 2, CriteriaEnum.MCDC));
			add(new ConfigurationNoGUI("", ASM_TRAFFICLIGHT_FOLDER + "TrafficLight3//TrafficLight_3.asm", 4, 3, 2, CriteriaEnum.RULE_GUARD));
			add(new ConfigurationNoGUI("", ASM_TRAFFICLIGHT_FOLDER + "TrafficLight3//TrafficLight_3.asm", 4, 3, 2, CriteriaEnum.RULE_UPDATE));
			add(new ConfigurationNoGUI("", ASM_TRAFFICLIGHT_FOLDER + "TrafficLight3//TrafficLight_3.asm", 4, 3, 2, CriteriaEnum.THREEWISE));
			add(new ConfigurationNoGUI("", ASM_TRAFFICLIGHT_FOLDER + "TrafficLight3//TrafficLight_3.asm", 4, 3, 2, CriteriaEnum.BASIC_RULE, CriteriaEnum.COMBINATORIAL_ALL,
					CriteriaEnum.COMPLETE_RULE, CriteriaEnum.MCDC, CriteriaEnum.RULE_GUARD, CriteriaEnum.RULE_UPDATE, CriteriaEnum.THREEWISE));
			
		}
	};	
	
	@Test
	public void testComputeCoverage() throws Exception {
		// XXX select configuration to execute (only one at a time!!)
		Logger.getLogger(StdPairwiseCovBuild.class).setLevel(Level.ALL);
		for (int i=0; i<filesToProcess.size(); i++)
			createFSMSequencesTest(filesToProcess.get(i));	
	}
	
	@Test
	public void testSingleComputeCoverage() throws Exception {
		// XXX select configuration to execute (only one at a time!!)
		Logger.getLogger(StdPairwiseCovBuild.class).setLevel(Level.ALL);
		createFSMSequencesTest(filesToProcess.get(23));	
	}
	
	@Test
	public void createFSMSequencesFromManualAvalla_Lev0() throws Exception {
		// Generate an unique FSM 		
		String[] outputFileNames = {"F:\\Dati-Andrea\\GitHub\\ProTestUTA\\trafficlightcontroller\\ASM\\TrafficLight0\\scenario01.avalla"};
		FSMCreator.createFSMFromMultipleAvalla(outputFileNames, true, 2, 1, 3);
		// Remove all the temporary files, if any
		for(String filePath : outputFileNames) {
			new File(filePath.replaceAll("avalla", "txt")).delete();
		}
	}
	
	@Test
	public void createFSMSequencesFromManualAvalla_Lev1() throws Exception {
		// Generate an unique FSM 		
		String[] outputFileNames = {"F:\\Dati-Andrea\\GitHub\\ProTestUTA\\trafficlightcontroller\\ASM\\TrafficLight1\\scenario11.avalla",
				"F:\\Dati-Andrea\\GitHub\\ProTestUTA\\trafficlightcontroller\\ASM\\TrafficLight1\\scenario12.avalla"};
		FSMCreator.createFSMFromMultipleAvalla(outputFileNames, true, 2, 2, 4);
		// Remove all the temporary files, if any
		for(String filePath : outputFileNames) {
			new File(filePath.replaceAll("avalla", "txt")).delete();
		}
	}
	
	@Test
	public void createFSMSequencesFromManualAvalla_Lev2() throws Exception {
		// Generate an unique FSM 		
		String[] outputFileNames = {"F:\\Dati-Andrea\\GitHub\\ProTestUTA\\trafficlightcontroller\\ASM\\TrafficLight2\\scenario21.avalla",
				"F:\\Dati-Andrea\\GitHub\\ProTestUTA\\trafficlightcontroller\\ASM\\TrafficLight2\\scenario22.avalla"};
		FSMCreator.createFSMFromMultipleAvalla(outputFileNames, true, 2, 3, 4);
		// Remove all the temporary files, if any
		for(String filePath : outputFileNames) {
			new File(filePath.replaceAll("avalla", "txt")).delete();
		}
	}
	
	@Test
	public void createFSMSequencesFromManualAvalla_Lev3() throws Exception {
		// Generate an unique FSM 		
		String[] outputFileNames = {"F:\\Dati-Andrea\\GitHub\\ProTestUTA\\trafficlightcontroller\\ASM\\TrafficLight3\\scenario31.avalla",
				"F:\\Dati-Andrea\\GitHub\\ProTestUTA\\trafficlightcontroller\\ASM\\TrafficLight3\\scenario32.avalla"};
		FSMCreator.createFSMFromMultipleAvalla(outputFileNames, true, 2, 3, 4);
		// Remove all the temporary files, if any
		for(String filePath : outputFileNames) {
			new File(filePath.replaceAll("avalla", "txt")).delete();
		}
	}
	
	public void createFSMSequencesTest(ConfigurationNoGUI c) throws Exception {
		String outputFileName = null;
		String[] outputFileNames = null;
		String modelName = "";
		
		if (c.asmPath==null) {
			// Only FSM
			modelName = c.fsmPath.substring(0, c.fsmPath.lastIndexOf("."));
			outputFileName = new GenerateTestsFromFSM().saveFSMWithTests(modelName, c.criteria);	
		} else if (c.fsmPath.equals("")){
			// Generate the FSM starting only from the ASM
			modelName = c.asmPath.substring(0, c.asmPath.lastIndexOf("."));
			outputFileNames = new GenerateTestsFromFSM().saveFSMWithAvallaTests(modelName, c.asmPath, c.useMonitoring, c.criteria);	
			// Generate an unique FSM 			
			outputFileName = FSMCreator.createFSMFromMultipleAvalla(outputFileNames, c.useMonitoring, c.nMessages, c.nTransitions, c.nStatuses);
			// Remove all the temporary files, if any
			for(String filePath : outputFileNames) {
				new File(filePath).delete();
				new File(filePath.replaceAll("avalla", "txt")).delete();
			}
		} else {
			// Both ASM and FSM
			modelName =  c.fsmPath.substring(0, c.fsmPath.lastIndexOf("."));
			outputFileName = new GenerateTestsFromFSM().saveFSMWithTests(modelName, c.fsmPath, c.asmPath, c.criteria);			
		}
	}
	
}
