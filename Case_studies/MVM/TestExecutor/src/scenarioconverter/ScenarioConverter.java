package scenarioconverter;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;
import java.lang.reflect.Type;
import java.nio.file.Files;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import org.apache.commons.lang3.StringUtils;
import org.asmeta.atgt.generator.CriteriaEnum;

import com.google.gson.Gson;
import com.google.gson.GsonBuilder;
import com.google.gson.reflect.TypeToken;
import com.google.gson.stream.JsonReader;

import scenarioconverter.util.Command;
import scenarioconverter.util.Configuration;
import scenarioconverter.util.FunctionType;
import scenarioconverter.util.UtilityFunctions;

public class ScenarioConverter {

	public String stateName = "";
	public static String CONFIG_PATH = "config/config.json";
	public static int STEP_TIME = 100;
	public String stateMachineName = "";
	public String statesEnumName = "";
	public String stateMachineVarName = "";
	public String finalStateName = "";
	public PrintWriter out;
	private List<Command> loadedFunctionNames;

	/**
	 * ScenarioConverter constructor. It builds GTest test suites starting from
	 * AValLa scenarios
	 * 
	 * @param stateName           the name used to indicate the machine state in the
	 *                            avalla scenario
	 * @param stateMachineName    the name used for the class implementing the state
	 *                            machine in the C/C++ code
	 * @param statesEnumName      the name used for the enum describing the machine
	 *                            states in the C/C++ code
	 * @param stateMachineVarName the name used for variable implementing the state
	 *                            machine in the mock file
	 * @param finalStateName      the name used for indicating the final state
	 * @param out                 the out stream where the output of the scenario
	 *                            conversion must be printed
	 */
	public ScenarioConverter(String stateName, String stateMachineName, String statesEnumName,
			String stateMachineVarName, String finalStateName, PrintWriter out) {
		this.stateName = stateName;
		this.stateMachineName = stateMachineName;
		this.statesEnumName = statesEnumName;
		this.stateMachineVarName = stateMachineVarName;
		this.out = out;
		this.finalStateName = finalStateName;
	}

	/**
	 * Extracts the info about the an asm command/instruction from the mapping
	 * 
	 * @param asmName the asmName of the function to be searched
	 * @return the Command containing all the configuration info about the function
	 */
	private Command getMappedFunction(String asmName) {
		for (Command c : loadedFunctionNames) {
			if (c.asmName.equals(asmName))
				return c;
		}

		throw new RuntimeException("Function [" + asmName + "] has not been found in mapping json file");
	}

	/**
	 * Prints the includes directives in the outout stream
	 */
	public void printIncludes() {
		// Function printing the includes directives
		String imports = "#include <string>\r\n" + "#include \"gtest/gtest.h\"\r\n" + "#define private public\r\n" + "#include \"" + stateMachineName
				+ ".h\"\r\n" + "#undef private\r\n" + "#include \"sc_runner_timed.h\"\r\n" + "#include \"sc_types.h\"\n"
				+ "using namespace std;\n";
		out.println(imports);
	}

	/**
	 * Prints the content of the mock file inside the output test file
	 * 
	 * @param mockFilePath the path of the file to be printed inside the test file
	 * @throws IOException
	 */
	public void printMock(String mockFilePath) throws IOException {
		// Read the content of the file
		BufferedReader read = new BufferedReader(new FileReader(new File(mockFilePath)));
		String line = "";

		while ((line = read.readLine()) != null) {
			out.println(line);
		}

		out.println("\n\n");

		read.close();
	}

	/**
	 * Creates the function mapping JSON file, for each command in the scenario
	 * 
	 * @param fileName  the fileName of the scenario to be analyzed
	 * @param overwrite true if the JSON file has to be overwritten, false otherwise
	 * @throws Exception
	 */
	public void createFunctionMappings(String fileName, boolean overwrite) throws Exception {
		assert new File(fileName).exists();
		assert fileName.endsWith(".avalla") : "The file must be an avalla scenario";

		// Hashset storing the names of the functions
		ArrayList<String> names = new ArrayList<>();

		loadedFunctionNames = new ArrayList<Command>();
		// If the JSON file must not be overwritten, then load its current state
		if (!overwrite && new File(CONFIG_PATH).exists()) {
			loadFunctionMappings();
			loadedFunctionNames.forEach(f -> names.add(f.asmName));
		}

		// Read the JSON file
		BufferedReader read = new BufferedReader(new FileReader(new File(fileName)));
		String line = "", result = "";

		while ((line = read.readLine()) != null) {
			if (line.startsWith("check ") || line.startsWith("set ")) {
				result = getFunctionName(line);
				if (names.indexOf(result) < 0) {
					loadedFunctionNames.add(new Command(result, result, FunctionType.TBD, "", "", false));
					names.add(result);
				}
			}
		}

		// Store the resulting hashset in a json file
		Gson gson = new GsonBuilder().setPrettyPrinting().serializeNulls().create();
		List<Object> functionNamesList = Arrays.asList(loadedFunctionNames.toArray());
		FileWriter writer = new FileWriter(new File(CONFIG_PATH));
		gson.toJson(functionNamesList, writer);

		read.close();
		writer.close();

		System.out.println("JSON mapping file created");
	}

	/**
	 * Function to be used for creating the JSON file starting from a bunch of
	 * scenarios
	 * 
	 * @param folderName the name of the parent folder containing the scenarios
	 * @param c 		 the configuration the user want to execute (i.e., basically the criteria)
	 * @param overwrite  equals to true if the existing file has to be deleted,
	 *                   false otherwise
	 * @throws Exception
	 */
	public void createFunctionMappingsFromFolder(String folderName, Configuration c, boolean overwrite) throws Exception {
		assert new File(folderName).exists();

		if (overwrite && new File(CONFIG_PATH).exists())
			new File(CONFIG_PATH).delete();

		Files.walk(new File(folderName + c.level + "/").toPath()).filter(f -> f.getFileName().toString().endsWith(".avalla"))
				.filter(f -> startsWithAtLeasOneCriteria(f.getFileName().toString(), c))
				.forEach(f -> {
					try {
						createFunctionMappings(f.toAbsolutePath().toString(), false);
					} catch (Exception e) {
						e.printStackTrace();
					}
				});
	}

	/**
	 * Checks whether the filename starts with the name of at least one of the criteria contained in the
	 * configuration to be executed
	 * @param fileName	the file name
	 * @param c			the configuration to be executed
	 * @return			true if the filename starts with the name of at least one of the criteria contained in the
	 * 					configuration to be executed, false otherwise
	 */
	private boolean startsWithAtLeasOneCriteria(String fileName, Configuration c) {
		for (CriteriaEnum ce : c.criteria) {
			String abbrvName = ce.getAbbrvName();
			
			if (abbrvName.equals("2WISE"))
				abbrvName = "pair";
			if (abbrvName.equals("3-WISEw all") || abbrvName.equals("3-WISEmon"))
				abbrvName = "3-wise";
			
			if (fileName.startsWith("test" + abbrvName) || fileName.startsWith(abbrvName + "_") || fileName.startsWith("test" + abbrvName.toLowerCase()) || fileName.startsWith(abbrvName.toLowerCase() + "_"))
				return true;
		}
		return false;
	}

	/**
	 * Extracts from an avalla line the name of the function to be checked/set
	 * 
	 * @param line the avalla instruction
	 * @return the name of the function to be checked/set
	 */
	private String getFunctionName(String line) {
		String cleanString = "";

		if (line.startsWith("set ")) {
			cleanString = line.replace("set ", "");
			return cleanString.split(":=")[0].trim();
		} else {
			cleanString = line.replace("check ", "");
			return cleanString.split("=")[0].trim();
		}

	}

	/**
	 * Converts an avalla scenario file (whose name is given as parameter) in a
	 * GTest test case
	 * 
	 * @param fileName the avalla scenario file to be converted
	 * @throws IOException
	 */
	public void convert(String fileName) throws IOException {
		assert new File(fileName).exists();
		assert !stateName.equals("") : "Static field STATE_NAME must be set";
		assert !CONFIG_PATH.equals("") : "Static field CONFIG_PATH must be set";
		assert !stateMachineName.equals("") : "Static field STATE_MACHINE_NAME must be set";
		assert !statesEnumName.equals("") : "Static field STATES_ENUM_NAME must be set";

		// Read the content of the function mapping file
		loadedFunctionNames = new ArrayList<Command>();
		loadFunctionMappings();

		// Read the content of the file
		BufferedReader read = new BufferedReader(new FileReader(new File(fileName)));
		String line = "";
		String result = "";

		out.println("TEST_F(TestClass, " + new File(fileName).getName().substring(0, new File(fileName).getName().length() - 7).replace("-", "_") + ") {");
		out.println("\t" + stateMachineVarName + "->enter();");

		while (true) {
			line = read.readLine();
			if (line == null)
				break;

			result = convertLine(line);

			out.println("\t" + "//" + line);
			if (!result.equals("")) {
				out.println("\t" + result);
			}
		}

		out.println("\t" + stateMachineVarName + "->exit();");
		out.println("}");

		read.close();
	}

	/**
	 * Loads the function mapping from the JSON file
	 * 
	 * @throws FileNotFoundException
	 */
	private void loadFunctionMappings() throws FileNotFoundException {
		Gson gson = new Gson();
		Type listType = new TypeToken<ArrayList<Command>>() {
		}.getType();
		this.loadedFunctionNames = gson.fromJson(new JsonReader(new FileReader(new File(CONFIG_PATH))), listType);
	}

	/**
	 * Function converting a generic avalla instruction
	 * 
	 * @param line the avalla instruction
	 * @return the equivalent GTest test instruction
	 */
	private String convertLine(String line) {

		// If it is a comment, write it in the resulting file
		if (line.startsWith("//"))
			return line;

		// Only set and check lines must be considered
		if (line.startsWith("scenario ") || line.startsWith("load ") || line.equals(""))
			return "";

		// Message to be used to check the current state of the machine
		if (line.startsWith("check " + stateName + " ="))
			return convertCheckState(line);

		// Message to be used to check the other values of the machine
		if (line.startsWith("check "))
			return convertGeneralCheck(line);

		// Message to be used to set the values of the machine
		if (line.startsWith("set "))
			return convertSet(line);

		if (line.equals("step"))
			return convertStep();

		throw new RuntimeException("line [" + line + "] cannot be converted");
	}

	/**
	 * Converts the set step instruction
	 * 
	 * @return the GTest equivalent string
	 */
	private String convertStep() {
		return "runner->proceed_time(" + STEP_TIME + ");";
	}

	/**
	 * Converts the set instruction, depending on the type of the function
	 * 
	 * @param line the line containing the set instruction
	 * @return the GTest equivalent string
	 */
	private String convertSet(String line) {
		String setString = line.replace("set ", "");
		String[] splitCommand = setString.split(":=");
		assert splitCommand.length == 2 : "Set line [" + line + "] cannot be converted";
		Command c = getMappedFunction(splitCommand[0].trim());
		String strValue = splitCommand[1].trim().replace(";", "");

		if (c.ignore)
			return "";

		switch (c.commandType) {
		case IN_EVENT:
			// In_Events must be called using the raise + [eventName] function, only when
			// value true is assigned
			if (strValue.equals("true"))
				if (c.cName.contains("(") && c.cName.contains(")"))
					return stateMachineVarName + "->raise" + StringUtils.capitalize(c.cName) + ";";
				else
					return stateMachineVarName + "->raise" + StringUtils.capitalize(c.cName) + "();";
			else
				return "";
		case VAR:
			return stateMachineVarName + "->set" + StringUtils.capitalize(c.cName) + "("
					+ (UtilityFunctions.isInteger(strValue) ? Integer.parseInt(strValue) * STEP_TIME : strValue) + ");";
		case OPERATION:
			return c.cName + "(" + (UtilityFunctions.isInteger(strValue) ? Integer.parseInt(strValue) * STEP_TIME : strValue)
					+ ");";
		case BOOL_OPERATION:
			if (strValue.toLowerCase().equals("true"))
				return c.cName + "(" + c.whenTrue + ");";
			else
				return c.cName + "(" + c.whenFalse + ");";
		default:
			// Extract the information from the check operation and return the string
			return stateMachineVarName + "->" + c.cName + " = "
					+ (UtilityFunctions.isInteger(strValue) ? Integer.parseInt(strValue) * STEP_TIME : strValue) + ";";
		}
	}

	/**
	 * Converts the check instruction
	 * 
	 * @param line the line containing the check instruction
	 * @return the GTest equivalent string
	 */
	private String convertGeneralCheck(String line) {
		String checkString = line.replace("check ", "");
		String[] splitCommand = checkString.split("=");
		assert splitCommand.length == 2 : "Check line [" + line + "] cannot be converted";
		Command c = getMappedFunction(splitCommand[0].trim());
		String strValue = splitCommand[1].trim().replace(";", "");

		if (c.ignore)
			return "";

		switch (c.commandType) {
		case VAR:
			return "EXPECT_EQ(" + stateMachineVarName + "->get" + StringUtils.capitalize(c.cName) + "() , "
					+ (UtilityFunctions.isInteger(strValue) ? Integer.parseInt(strValue) * STEP_TIME : strValue) + ");";
		case OPERATION:
			return "EXPECT_EQ(" + c.cName + "() , "
					+ (UtilityFunctions.isInteger(strValue) ? Integer.parseInt(strValue) * STEP_TIME : strValue) + ");";
		default:
			// Extract the information from the check operation and return the string
			return "EXPECT_EQ(" + stateMachineVarName + "->" + c.cName + " , "
					+ (UtilityFunctions.isInteger(strValue) ? Integer.parseInt(strValue) * STEP_TIME : strValue) + ");";
		}
	}

	/**
	 * Converts the check state instruction
	 * 
	 * @param line the line containing the check state instruction
	 * @return the GTest equivalent string
	 */
	private String convertCheckState(String line) {
		// Extract the name of the state to be checked
		String thisStateName = line.split("=")[1].trim().replace(";", "");

		if (thisStateName.equals("OFF")) {
			assert !finalStateName
					.equals("") : "Final state reached. To proceed you need to set a valid final state name mapping";
			return "EXPECT_TRUE(" + stateMachineVarName + "->isStateActive(" + stateMachineName + "::" + statesEnumName
					+ "::" + finalStateName + "));";
		}
		if (thisStateName.length() > 0) {
			return "EXPECT_TRUE(" + stateMachineVarName + "->isStateActive(" + stateMachineName + "::" + statesEnumName
					+ "::" + thisStateName + "));";
		}

		return null;
	}

	/**
	 * Function to be used for converting a bunch of scenarios
	 * 
	 * @param folderName 	the name of the parent folder containing the scenarios
	 * @param c				the configuration to be executed
	 * @throws Exception
	 */
	public void convertFromFolder(String folderName, Configuration c) throws Exception {
		assert new File(folderName + c.level + "/").exists() : folderName + " does not exist";

		Files.walk(new File(folderName + c.level + "/").toPath()).filter(f -> (f.getFileName().toString().endsWith(".avalla") 
				&& startsWithAtLeasOneCriteria(f.getFileName().toString(), c)))
				.forEach(f -> {
					try {
						convert(f.toAbsolutePath().toString());
					} catch (IOException e) {
						e.printStackTrace();
					}
				});
	}

}
