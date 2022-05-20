package generator;

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

public class AbstractTestsGenerator {

	class Configuration {
		String asmPath;
		List<CriteriaEnum> criteria;
		boolean useMonitoring;
		String level;
		
		public Configuration(String asmPath, List<CriteriaEnum> criteria, String level) {
			this.asmPath = asmPath;
			this.criteria = criteria;
			this.useMonitoring = true;
			this.level = level;
		}
		
		public Configuration(String asmPath, boolean useMonitoring, List<CriteriaEnum> criteria, String level) {
			this.asmPath = asmPath;
			this.criteria = criteria;
			this.useMonitoring = useMonitoring;
			this.level = level;
		}
		
		public Configuration(String asmPath, String level, CriteriaEnum ... criteria) {
			this(asmPath,Arrays.asList(criteria), level);
		}
		
		public Configuration(String asmPath, boolean useMonitoring, String level, CriteriaEnum ... criteria) {
			this(asmPath,useMonitoring,Arrays.asList(criteria), level);
		}
	}

	static String ASM_FOLDER = "../ASM/TrafficLight_";
	static String ABSTRACT_TESTS_FOLDER = "../abstract_tests/TrafficLight_";

	List<Configuration> filesToProcess = new ArrayList<Configuration>() {
		
		private static final long serialVersionUID = 5014712752302397500L;
		{
			add(new Configuration(ASM_FOLDER + "0/TrafficLight_0.asm", "0", CriteriaEnum.BASIC_RULE));
			add(new Configuration(ASM_FOLDER + "0/TrafficLight_0.asm", "0", CriteriaEnum.COMBINATORIAL_ALL));
			add(new Configuration(ASM_FOLDER + "0/TrafficLight_0.asm", "0", CriteriaEnum.COMPLETE_RULE));
			add(new Configuration(ASM_FOLDER + "0/TrafficLight_0.asm", "0", CriteriaEnum.MCDC));
			add(new Configuration(ASM_FOLDER + "0/TrafficLight_0.asm", "0", CriteriaEnum.RULE_GUARD));
			add(new Configuration(ASM_FOLDER + "0/TrafficLight_0.asm", "0", CriteriaEnum.RULE_UPDATE));
			add(new Configuration(ASM_FOLDER + "0/TrafficLight_0.asm", "0", CriteriaEnum.THREEWISE));
			add(new Configuration(ASM_FOLDER + "0/TrafficLight_0.asm", "0", CriteriaEnum.BASIC_RULE, CriteriaEnum.COMBINATORIAL_ALL,
					CriteriaEnum.COMPLETE_RULE, CriteriaEnum.MCDC, CriteriaEnum.RULE_GUARD, CriteriaEnum.RULE_UPDATE, CriteriaEnum.THREEWISE));
			
			add(new Configuration(ASM_FOLDER + "1/TrafficLight_1.asm", "1", CriteriaEnum.BASIC_RULE));
			add(new Configuration(ASM_FOLDER + "1/TrafficLight_1.asm", "1", CriteriaEnum.COMBINATORIAL_ALL));
			add(new Configuration(ASM_FOLDER + "1/TrafficLight_1.asm", "1", CriteriaEnum.COMPLETE_RULE));
			add(new Configuration(ASM_FOLDER + "1/TrafficLight_1.asm", "1", CriteriaEnum.MCDC));
			add(new Configuration(ASM_FOLDER + "1/TrafficLight_1.asm", "1", CriteriaEnum.RULE_GUARD));
			add(new Configuration(ASM_FOLDER + "1/TrafficLight_1.asm", "1", CriteriaEnum.RULE_UPDATE));
			add(new Configuration(ASM_FOLDER + "1/TrafficLight_1.asm", "1", CriteriaEnum.THREEWISE));
			add(new Configuration(ASM_FOLDER + "1/TrafficLight_1.asm", "1", CriteriaEnum.BASIC_RULE, CriteriaEnum.COMBINATORIAL_ALL,
					CriteriaEnum.COMPLETE_RULE, CriteriaEnum.MCDC, CriteriaEnum.RULE_GUARD, CriteriaEnum.RULE_UPDATE, CriteriaEnum.THREEWISE));
			
			add(new Configuration(ASM_FOLDER + "2/TrafficLight_2.asm", "2", CriteriaEnum.BASIC_RULE));
			add(new Configuration(ASM_FOLDER + "2/TrafficLight_2.asm", "2", CriteriaEnum.COMBINATORIAL_ALL));
			add(new Configuration(ASM_FOLDER + "2/TrafficLight_2.asm", "2", CriteriaEnum.COMPLETE_RULE));
			add(new Configuration(ASM_FOLDER + "2/TrafficLight_2.asm", "2", CriteriaEnum.MCDC));
			add(new Configuration(ASM_FOLDER + "2/TrafficLight_2.asm", "2", CriteriaEnum.RULE_GUARD));
			add(new Configuration(ASM_FOLDER + "2/TrafficLight_2.asm", "2", CriteriaEnum.RULE_UPDATE));
			add(new Configuration(ASM_FOLDER + "2/TrafficLight_2.asm", "2", CriteriaEnum.THREEWISE));
			add(new Configuration(ASM_FOLDER + "2/TrafficLight_2.asm", "2", CriteriaEnum.BASIC_RULE, CriteriaEnum.COMBINATORIAL_ALL,
					CriteriaEnum.COMPLETE_RULE, CriteriaEnum.MCDC, CriteriaEnum.RULE_GUARD, CriteriaEnum.RULE_UPDATE, CriteriaEnum.THREEWISE));
						
			add(new Configuration(ASM_FOLDER + "3/TrafficLight_3.asm", "3", CriteriaEnum.BASIC_RULE));
			add(new Configuration(ASM_FOLDER + "3/TrafficLight_3.asm", "3", CriteriaEnum.COMBINATORIAL_ALL));
			add(new Configuration(ASM_FOLDER + "3/TrafficLight_3.asm", "3", CriteriaEnum.COMPLETE_RULE));
			add(new Configuration(ASM_FOLDER + "3/TrafficLight_3.asm", "3", CriteriaEnum.MCDC));
			add(new Configuration(ASM_FOLDER + "3/TrafficLight_3.asm", "3", CriteriaEnum.RULE_GUARD));
			add(new Configuration(ASM_FOLDER + "3/TrafficLight_3.asm", "3", CriteriaEnum.RULE_UPDATE));
			add(new Configuration(ASM_FOLDER + "3/TrafficLight_3.asm", "3", CriteriaEnum.THREEWISE));
			add(new Configuration(ASM_FOLDER + "3/TrafficLight_3.asm", "3", CriteriaEnum.BASIC_RULE, CriteriaEnum.COMBINATORIAL_ALL,
					CriteriaEnum.COMPLETE_RULE, CriteriaEnum.MCDC, CriteriaEnum.RULE_GUARD, CriteriaEnum.RULE_UPDATE, CriteriaEnum.THREEWISE));
			
			add(new Configuration(ASM_FOLDER + "NR/TrafficLight_4.asm", "NR", CriteriaEnum.BASIC_RULE));
			add(new Configuration(ASM_FOLDER + "NR/TrafficLight_4.asm", "NR", CriteriaEnum.COMBINATORIAL_ALL));
			add(new Configuration(ASM_FOLDER + "NR/TrafficLight_4.asm", "NR", CriteriaEnum.COMPLETE_RULE));
			add(new Configuration(ASM_FOLDER + "NR/TrafficLight_4.asm", "NR", CriteriaEnum.MCDC));
			add(new Configuration(ASM_FOLDER + "NR/TrafficLight_4.asm", "NR", CriteriaEnum.RULE_GUARD));
			add(new Configuration(ASM_FOLDER + "NR/TrafficLight_4.asm", "NR", CriteriaEnum.RULE_UPDATE));
			add(new Configuration(ASM_FOLDER + "NR/TrafficLight_4.asm", "NR", CriteriaEnum.THREEWISE));
			add(new Configuration(ASM_FOLDER + "NR/TrafficLight_4.asm", "NR", CriteriaEnum.BASIC_RULE, CriteriaEnum.COMBINATORIAL_ALL,
					CriteriaEnum.COMPLETE_RULE, CriteriaEnum.MCDC, CriteriaEnum.RULE_GUARD, CriteriaEnum.RULE_UPDATE, CriteriaEnum.THREEWISE));
		}
	};	
	
	@Test
	public void generateAbstractTests() throws Exception {
		// XXX select configuration to execute (only one at a time!!)
		Logger.getLogger(StdPairwiseCovBuild.class).setLevel(Level.ALL);
		generateAbstractTests(filesToProcess.get(1));	
	}
	
	/**
	 * Generates all the abstract tests
	 * @throws Exception
	 */
	@Test
	public void generateAllAbstractTests() throws Exception {
		Logger.getLogger(StdPairwiseCovBuild.class).setLevel(Level.ALL);
		for (Configuration c : filesToProcess) {
			generateAbstractTests(c);
		}	
	}

	
	public void generateAbstractTests(Configuration c) throws Exception {
		String modelName = "";
		
		// Generate the abstract tests
		modelName = c.asmPath.substring(0, c.asmPath.lastIndexOf("."));
		new GenerateTestsFromFSM().saveAbstractTests(modelName, c.asmPath, c.useMonitoring, c.criteria, ABSTRACT_TESTS_FOLDER + c.level + "/");	
	}
	
}
