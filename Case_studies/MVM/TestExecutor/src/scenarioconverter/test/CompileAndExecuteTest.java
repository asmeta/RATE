package scenarioconverter.test;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.nio.file.Files;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

import scenarioconverter.ScenarioConverter;
import scenarioconverter.util.Configuration;
import scenarioconverter.util.CriteriaEnum;

public class CompileAndExecuteTest {

	private static String DESTINATION_PATH = "../SUT/src-gen-test/";
	private static String DEBUG_PATH = "../SUT/Debug/";
	private static boolean WRITE_JSON = false;

	public static List<Configuration> filesToProcess = new ArrayList<Configuration>() {
		
		private static final long serialVersionUID = 5014712752302397500L;
		{		
			// level 0
//			add(new Configuration("00", "config/config3.json", Configuration.MANUAL_TEST));
//			add(new Configuration("00", "config/config3.json", Configuration.ALL_CRITERIA));
//			// level 1
//			add(new Configuration("01", "config/config3.json", Configuration.MANUAL_TEST));
//			add(new Configuration("01", "config/config3.json", Configuration.ALL_CRITERIA));
//			// level 2
//			add(new Configuration("02", "config/config3.json", Configuration.MANUAL_TEST));
//			add(new Configuration("02", "config/config3.json", Configuration.ALL_CRITERIA));
//			// level 3
//			add(new Configuration("03", "config/config3.json", Configuration.MANUAL_TEST));
//			add(new Configuration("03", "config/config3.json", Configuration.ALL_CRITERIA));
			
			add(new Configuration("NR", "config/config3.json", Configuration.ALL_CRITERIA));
		}
	};	
	
	public static void main(String[] args) throws Exception {
		// Now compile the MVM code with the GoogleTests
		// executeBat("build_gtest.bat");
		// executeBat("build_MVM.bat");
		// XXX Select the configuration you want to execute
		//Configuration c = filesToProcess.get(0);
		List<Boolean> failures = new ArrayList<>();
		for(Configuration c: filesToProcess) {
			failures.add(executeTest(c));
			//break;
		}
		System.out.println(failures);
	}

	static boolean executeTest(Configuration c) throws FileNotFoundException, Exception, IOException, InterruptedException {
		
		// The path of abstract tests
		String PATH_AT;
		
		PrintWriter out = new PrintWriter(new File(DESTINATION_PATH + "test.cpp"));
		
		ScenarioConverter sc = new ScenarioConverter("state", "MVMStateMachineCore", "MVMStateMachineCoreStates",
				"statechart", "MAIN_REGION__final_", out);
		try {
			ScenarioConverter.CONFIG_PATH = c.configPath;
			if (c.criteria == Configuration.MANUAL_TEST) {
				// Manual scenarios
				PATH_AT = "../ASM/MVM_" + c.level + "/scenarios/";
				
				// Prints the includes and the mock instructions
				sc.printIncludes();
				sc.printMock("./additional_files/mock_simple.c");

				assert new File(PATH_AT).exists();

				Files.walk(new File(PATH_AT).toPath()).filter(f -> (f.getFileName().toString().endsWith(".avalla")))
						.forEach(f -> {
							try {
								sc.convert(f.toAbsolutePath().toString());
							} catch (IOException e) {
								e.printStackTrace();
							}
						});
			} else {
				// Abstract tests
				PATH_AT = "../abstract_tests/MVM_" + c.level;
				assert new File(PATH_AT).exists();
				assert new File(PATH_AT).isDirectory();
				// in futuro potrebbe essere anche un solo criterio o cose simili
				assert c.criteria == Configuration.ALL_CRITERIA;
				// If JSON file must be written
				if (WRITE_JSON) {
					sc.createFunctionMappingsFromFolder(PATH_AT, c, false);
				} else {
					// Prints the includes and the mock instructions
					sc.printIncludes();
					sc.printMock("./additional_files/mock_simple.c");
					// Convert all the files
					sc.convertFromFolder(PATH_AT, c);
				}
			}

		} catch (IOException e) {
			e.printStackTrace();
		}
		out.close();
		
//		boolean failed = executeBat("build_and_exec_test.bat");
		boolean failed = executeBat("commands_for_building_MVM.bat");
		System.err.println("+++ " + failed);
		return failed;
	}

	static boolean executeBat(String batfile) {
		System.out.println("executing bath file " + batfile);
		ProcessBuilder builder = new ProcessBuilder();
		builder.command("cmd.exe", "/c",
				new File("./additional_files/"+batfile).getAbsolutePath());
		builder.directory(new File(DEBUG_PATH));
		// comment this to have a separate IO for this process
		//builder.inheritIO();
		boolean failed = false;
		try {
			Process process = builder.start();
			BufferedReader buf = new BufferedReader(new InputStreamReader(process.getInputStream()));
			String line = "";		
			while ((line = buf.readLine()) != null) {
				System.out.println(line);
				// [  FAILED  ]
				if (line.startsWith("[  FAILED  ]")) failed = true;
			}
			process.waitFor();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}		
		return failed;
	}
}
