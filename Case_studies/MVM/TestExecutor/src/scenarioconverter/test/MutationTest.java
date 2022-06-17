package scenarioconverter.test;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.URI;
import java.nio.charset.Charset;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;

import org.asmeta.atgt.generator.CriteriaEnum;
import org.junit.Test;

import scenarioconverter.ScenarioConverter;
import scenarioconverter.util.Configuration;

public class MutationTest {

	private static final String path = "../SUT/src-gen/MVMStateMachineCore.cpp";
	static final String backupCpp = "../SUT/src-gen/MVMStateMachineCore_backup_cpp";
	static Charset charset = StandardCharsets.UTF_8;

	@Test
	public void mutate() throws FileNotFoundException, IOException, InterruptedException, Exception {
		// init the map
		Map<Configuration, Integer> mutationKilled = new HashMap<>();
		for (Configuration c : CompileAndExecuteTest.filesToProcess) {
			mutationKilled.put(c, 0);
		}

		// Now compile the MVM code with the GoogleTests
		// CompileAndExecuteTest.executeBat("build_gtest.bat");
		// XXX Select the configuration you want to execute
		// Configuration c = filesToProcess.get(0);
		// contenuto originale come Strring
		File original = new File(backupCpp);
		assert original.exists();
		String content = new String(Files.readAllBytes(original.toPath()), charset);
		// applica una mutazione alla volta
		int pos = 0;
		int numMutation = 0;
		for(;;){
			StringBuffer sb = new StringBuffer(content);
			pos = MutationTest.insertComment("\texseq_", sb, pos);
			if (pos == -1) {
				System.out.println("no more mutations");
				break;
			}
			else {
				System.out.println("commenting around " + sb.toString().substring(pos - 10, pos + 10));
				pos++;
			}
			if (++numMutation > 5) {
				System.out.println("mutations max number reached");
				break;
			}
			// save to file
			Files.write(Paths.get(path), sb.toString().getBytes());
			// build the mutated machine
			CompileAndExecuteTest.executeBat("build_MVM.bat");
			// run the tests
			Map<Configuration, Boolean> failures = runTests();
			for (Entry<Configuration, Boolean> e : failures.entrySet()) {
				if (e.getValue()) {
					mutationKilled.put(e.getKey(), mutationKilled.get(e.getKey()) + 1);
				}
			}
		} 
		//
		System.out.println(mutationKilled);
	}

	// return whcih configurations kill the mutant
	private static Map<Configuration, Boolean> runTests()
			throws FileNotFoundException, IOException, InterruptedException, Exception {
		Map<Configuration, Boolean> failures = new HashMap<>();
		for (Configuration c : CompileAndExecuteTest.filesToProcess) {
			// compile
			boolean executeTest = CompileAndExecuteTest.executeTest(c);
			failures.put(c, executeTest);
		}
		return failures;
	}

	// sostuiscie una stringa con un'altra
	static int insertComment(String search, StringBuffer in, int pos) {
		int index = in.indexOf(search, pos);
		if (index == -1)
			return -1;
		in.insert(index, "//");
		return index;
	}

}
