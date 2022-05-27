package scenarioconverter.test;

import java.io.BufferedReader;
import java.io.File;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.List;

import org.asmeta.atgt.generator.CriteriaEnum;

import jodd.io.StreamGobbler;
import scenarioconverter.ScenarioConverter;
import scenarioconverter.util.Configuration;

public class Test {

	private static String DESTINATION_PATH = "../SUT/src-gen-test/";
	private static String DEBUG_PATH = "../SUT/Debug/";
	private static boolean WRITE_JSON = false;

	// The path of abstract tests
	private static String PATH_AT = "../abstract_tests/MVM_";

	public static List<Configuration> filesToProcess = new ArrayList<Configuration>() {
		
		private static final long serialVersionUID = 5014712752302397500L;
		{			
			add(new Configuration("0", CriteriaEnum.BASIC_RULE));
			add(new Configuration("1", CriteriaEnum.BASIC_RULE));
			add(new Configuration("2", CriteriaEnum.BASIC_RULE));
			add(new Configuration("3", CriteriaEnum.BASIC_RULE));
			add(new Configuration("NR", CriteriaEnum.BASIC_RULE));
			
			add(new Configuration("0", CriteriaEnum.COMBINATORIAL_ALL));
			add(new Configuration("1", CriteriaEnum.COMBINATORIAL_ALL));
			add(new Configuration("2", CriteriaEnum.COMBINATORIAL_ALL));
			add(new Configuration("3", CriteriaEnum.COMBINATORIAL_ALL));
			add(new Configuration("NR", CriteriaEnum.COMBINATORIAL_ALL));
			
			add(new Configuration("0", CriteriaEnum.COMPLETE_RULE));
			add(new Configuration("1", CriteriaEnum.COMPLETE_RULE));
			add(new Configuration("2", CriteriaEnum.COMPLETE_RULE));
			add(new Configuration("3", CriteriaEnum.COMPLETE_RULE));
			add(new Configuration("NR", CriteriaEnum.COMPLETE_RULE));
			
			add(new Configuration("0", CriteriaEnum.MCDC));
			add(new Configuration("1", CriteriaEnum.MCDC));
			add(new Configuration("2", CriteriaEnum.MCDC));
			add(new Configuration("3", CriteriaEnum.MCDC));
			add(new Configuration("NR", CriteriaEnum.MCDC));
			
			add(new Configuration("0", CriteriaEnum.RULE_GUARD));
			add(new Configuration("1", CriteriaEnum.RULE_GUARD));
			add(new Configuration("2", CriteriaEnum.RULE_GUARD));
			add(new Configuration("3", CriteriaEnum.RULE_GUARD));
			add(new Configuration("NR", CriteriaEnum.RULE_GUARD));
			
			add(new Configuration("0", CriteriaEnum.RULE_UPDATE));
			add(new Configuration("1", CriteriaEnum.RULE_UPDATE));
			add(new Configuration("2", CriteriaEnum.RULE_UPDATE));
			add(new Configuration("3", CriteriaEnum.RULE_UPDATE));
			add(new Configuration("NR", CriteriaEnum.RULE_UPDATE));
			
			add(new Configuration("0", CriteriaEnum.THREEWISE));
			add(new Configuration("1", CriteriaEnum.THREEWISE));
			add(new Configuration("2", CriteriaEnum.THREEWISE));
			add(new Configuration("3", CriteriaEnum.THREEWISE));
			add(new Configuration("NR", CriteriaEnum.THREEWISE));
			
			add(new Configuration("0", CriteriaEnum.BASIC_RULE, CriteriaEnum.COMBINATORIAL_ALL, CriteriaEnum.COMPLETE_RULE, CriteriaEnum.MCDC, CriteriaEnum.RULE_GUARD, CriteriaEnum.RULE_UPDATE, CriteriaEnum.THREEWISE));
			add(new Configuration("1", CriteriaEnum.BASIC_RULE, CriteriaEnum.COMBINATORIAL_ALL, CriteriaEnum.COMPLETE_RULE, CriteriaEnum.MCDC, CriteriaEnum.RULE_GUARD, CriteriaEnum.RULE_UPDATE, CriteriaEnum.THREEWISE));
			add(new Configuration("2", CriteriaEnum.BASIC_RULE, CriteriaEnum.COMBINATORIAL_ALL, CriteriaEnum.COMPLETE_RULE, CriteriaEnum.MCDC, CriteriaEnum.RULE_GUARD, CriteriaEnum.RULE_UPDATE, CriteriaEnum.THREEWISE));
			add(new Configuration("3", CriteriaEnum.BASIC_RULE, CriteriaEnum.COMBINATORIAL_ALL, CriteriaEnum.COMPLETE_RULE, CriteriaEnum.MCDC, CriteriaEnum.RULE_GUARD, CriteriaEnum.RULE_UPDATE, CriteriaEnum.THREEWISE));
			add(new Configuration("NR", CriteriaEnum.BASIC_RULE, CriteriaEnum.COMBINATORIAL_ALL, CriteriaEnum.COMPLETE_RULE, CriteriaEnum.MCDC, CriteriaEnum.RULE_GUARD, CriteriaEnum.RULE_UPDATE, CriteriaEnum.THREEWISE));
		}
	};	

	public static void main(String[] args) throws Exception {
		PrintWriter out = new PrintWriter(new File(DESTINATION_PATH + "test.cpp"));
		ScenarioConverter sc = new ScenarioConverter("state", "MVMStateMachineCore", "MVMStateMachineCoreStates",
				"statechart", "MAIN_REGION__final_", out);
		
		// XXX Select the configuration you want to execute
		Configuration c = filesToProcess.get(3);
		
		try {

			// If JSON file must be written
			if (WRITE_JSON) {
				sc.createFunctionMappingsFromFolder(PATH_AT, c, false);
			}
			else {
				// Prints the includes and the mock instructions
				sc.printIncludes();
				sc.printMock("./additional_files/mock_simple.c");
	
				// Convert all the files
				sc.convertFromFolder(PATH_AT, c);
			}

		} catch (IOException e) {
			e.printStackTrace();
		}
		out.close();
		
		// Now compile the MVM code with the GoogleTests
		ProcessBuilder builder = new ProcessBuilder();
		builder.command("cmd.exe", "/c",
				new File("./additional_files/commands_for_building_MVM.bat").getAbsolutePath());
		builder.directory(new File(DEBUG_PATH));
		builder.inheritIO();
		Process process = builder.start();
		
		BufferedReader buf = new BufferedReader(new InputStreamReader(process.getInputStream()));
		String line = "";
		while ((line = buf.readLine()) != null) {
			System.out.println(line);
		}
		process.waitFor();
	}
}
