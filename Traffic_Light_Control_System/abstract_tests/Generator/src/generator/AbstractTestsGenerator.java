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

	class TLCSConfiguration {
		String asmPath;
		List<CriteriaEnum> criteria;
		boolean useMonitoring;
		
		public TLCSConfiguration(String asmPath, List<CriteriaEnum> criteria) {
			this.asmPath = asmPath;
			this.criteria = criteria;
			this.useMonitoring = true;
		}
		
		public TLCSConfiguration(String asmPath, boolean useMonitoring, List<CriteriaEnum> criteria) {
			this.asmPath = asmPath;
			this.criteria = criteria;
			this.useMonitoring = useMonitoring;
		}
		
		public TLCSConfiguration(String asmPath, CriteriaEnum ... criteria) {
			this(asmPath,Arrays.asList(criteria));
		}
		
		public TLCSConfiguration(String asmPath, boolean useMonitoring, CriteriaEnum ... criteria) {
			this(asmPath,useMonitoring,Arrays.asList(criteria));
		}
	}

	static String ASM_PHD_FOLDER = "../../ASM/TrafficLight_";

	List<TLCSConfiguration> filesToProcess = new ArrayList<TLCSConfiguration>() {
		
		private static final long serialVersionUID = 5014712752302397500L;
		{
			add(new TLCSConfiguration(ASM_PHD_FOLDER + "0/TrafficLight_0.asm", CriteriaEnum.BASIC_RULE));
			add(new TLCSConfiguration(ASM_PHD_FOLDER + "0/TrafficLight_0.asm", CriteriaEnum.COMBINATORIAL_ALL));
			add(new TLCSConfiguration(ASM_PHD_FOLDER + "0/TrafficLight_0.asm", CriteriaEnum.COMPLETE_RULE));
			add(new TLCSConfiguration(ASM_PHD_FOLDER + "0/TrafficLight_0.asm", CriteriaEnum.MCDC));
			add(new TLCSConfiguration(ASM_PHD_FOLDER + "0/TrafficLight_0.asm", CriteriaEnum.RULE_GUARD));
			add(new TLCSConfiguration(ASM_PHD_FOLDER + "0/TrafficLight_0.asm", CriteriaEnum.RULE_UPDATE));
			add(new TLCSConfiguration(ASM_PHD_FOLDER + "0/TrafficLight_0.asm", CriteriaEnum.THREEWISE));
			add(new TLCSConfiguration(ASM_PHD_FOLDER + "0/TrafficLight_0.asm", CriteriaEnum.BASIC_RULE, CriteriaEnum.COMBINATORIAL_ALL,
					CriteriaEnum.COMPLETE_RULE, CriteriaEnum.MCDC, CriteriaEnum.RULE_GUARD, CriteriaEnum.RULE_UPDATE, CriteriaEnum.THREEWISE));
			
			add(new TLCSConfiguration(ASM_PHD_FOLDER + "1/TrafficLight_1.asm", CriteriaEnum.BASIC_RULE));
			add(new TLCSConfiguration(ASM_PHD_FOLDER + "1/TrafficLight_1.asm", CriteriaEnum.COMBINATORIAL_ALL));
			add(new TLCSConfiguration(ASM_PHD_FOLDER + "1/TrafficLight_1.asm", CriteriaEnum.COMPLETE_RULE));
			add(new TLCSConfiguration(ASM_PHD_FOLDER + "1/TrafficLight_1.asm", CriteriaEnum.MCDC));
			add(new TLCSConfiguration(ASM_PHD_FOLDER + "1/TrafficLight_1.asm", CriteriaEnum.RULE_GUARD));
			add(new TLCSConfiguration(ASM_PHD_FOLDER + "1/TrafficLight_1.asm", CriteriaEnum.RULE_UPDATE));
			add(new TLCSConfiguration(ASM_PHD_FOLDER + "1/TrafficLight_1.asm", CriteriaEnum.THREEWISE));
			add(new TLCSConfiguration(ASM_PHD_FOLDER + "1/TrafficLight_1.asm", CriteriaEnum.BASIC_RULE, CriteriaEnum.COMBINATORIAL_ALL,
					CriteriaEnum.COMPLETE_RULE, CriteriaEnum.MCDC, CriteriaEnum.RULE_GUARD, CriteriaEnum.RULE_UPDATE, CriteriaEnum.THREEWISE));
			
			add(new TLCSConfiguration(ASM_PHD_FOLDER + "2/TrafficLight_2.asm", CriteriaEnum.BASIC_RULE));
			add(new TLCSConfiguration(ASM_PHD_FOLDER + "2/TrafficLight_2.asm", CriteriaEnum.COMBINATORIAL_ALL));
			add(new TLCSConfiguration(ASM_PHD_FOLDER + "2/TrafficLight_2.asm", CriteriaEnum.COMPLETE_RULE));
			add(new TLCSConfiguration(ASM_PHD_FOLDER + "2/TrafficLight_2.asm", CriteriaEnum.MCDC));
			add(new TLCSConfiguration(ASM_PHD_FOLDER + "2/TrafficLight_2.asm", CriteriaEnum.RULE_GUARD));
			add(new TLCSConfiguration(ASM_PHD_FOLDER + "2/TrafficLight_2.asm", CriteriaEnum.RULE_UPDATE));
			add(new TLCSConfiguration(ASM_PHD_FOLDER + "2/TrafficLight_2.asm", CriteriaEnum.THREEWISE));
			add(new TLCSConfiguration(ASM_PHD_FOLDER + "2/TrafficLight_2.asm", CriteriaEnum.BASIC_RULE, CriteriaEnum.COMBINATORIAL_ALL,
					CriteriaEnum.COMPLETE_RULE, CriteriaEnum.MCDC, CriteriaEnum.RULE_GUARD, CriteriaEnum.RULE_UPDATE, CriteriaEnum.THREEWISE));
						
			add(new TLCSConfiguration(ASM_PHD_FOLDER + "3/TrafficLight_3.asm", CriteriaEnum.BASIC_RULE));
			add(new TLCSConfiguration(ASM_PHD_FOLDER + "3/TrafficLight_3.asm", CriteriaEnum.COMBINATORIAL_ALL));
			add(new TLCSConfiguration(ASM_PHD_FOLDER + "3/TrafficLight_3.asm", CriteriaEnum.COMPLETE_RULE));
			add(new TLCSConfiguration(ASM_PHD_FOLDER + "3/TrafficLight_3.asm", CriteriaEnum.MCDC));
			add(new TLCSConfiguration(ASM_PHD_FOLDER + "3/TrafficLight_3.asm", CriteriaEnum.RULE_GUARD));
			add(new TLCSConfiguration(ASM_PHD_FOLDER + "3/TrafficLight_3.asm", CriteriaEnum.RULE_UPDATE));
			add(new TLCSConfiguration(ASM_PHD_FOLDER + "3/TrafficLight_3.asm", CriteriaEnum.THREEWISE));
			add(new TLCSConfiguration(ASM_PHD_FOLDER + "3/TrafficLight_3.asm", CriteriaEnum.BASIC_RULE, CriteriaEnum.COMBINATORIAL_ALL,
					CriteriaEnum.COMPLETE_RULE, CriteriaEnum.MCDC, CriteriaEnum.RULE_GUARD, CriteriaEnum.RULE_UPDATE, CriteriaEnum.THREEWISE));
			
			add(new TLCSConfiguration(ASM_PHD_FOLDER + "NR/TrafficLight_4.asm", CriteriaEnum.BASIC_RULE));
			add(new TLCSConfiguration(ASM_PHD_FOLDER + "NR/TrafficLight_4.asm", CriteriaEnum.COMBINATORIAL_ALL));
			add(new TLCSConfiguration(ASM_PHD_FOLDER + "NR/TrafficLight_4.asm", CriteriaEnum.COMPLETE_RULE));
			add(new TLCSConfiguration(ASM_PHD_FOLDER + "NR/TrafficLight_4.asm", CriteriaEnum.MCDC));
			add(new TLCSConfiguration(ASM_PHD_FOLDER + "NR/TrafficLight_4.asm", CriteriaEnum.RULE_GUARD));
			add(new TLCSConfiguration(ASM_PHD_FOLDER + "NR/TrafficLight_4.asm", CriteriaEnum.RULE_UPDATE));
			add(new TLCSConfiguration(ASM_PHD_FOLDER + "NR/TrafficLight_4.asm", CriteriaEnum.THREEWISE));
			add(new TLCSConfiguration(ASM_PHD_FOLDER + "NR/TrafficLight_4.asm", CriteriaEnum.BASIC_RULE, CriteriaEnum.COMBINATORIAL_ALL,
					CriteriaEnum.COMPLETE_RULE, CriteriaEnum.MCDC, CriteriaEnum.RULE_GUARD, CriteriaEnum.RULE_UPDATE, CriteriaEnum.THREEWISE));
		}
	};	
	
	@Test
	public void generateAbstractTests() throws Exception {
		// XXX select configuration to execute (only one at a time!!)
		Logger.getLogger(StdPairwiseCovBuild.class).setLevel(Level.ALL);
		generateAbstractTests(filesToProcess.get(0));	
	}
	
	/**
	 * Generates all the abstract tests
	 * @throws Exception
	 */
	@Test
	public void generateAllAbstractTests() throws Exception {
		Logger.getLogger(StdPairwiseCovBuild.class).setLevel(Level.ALL);
		for (TLCSConfiguration c : filesToProcess) {
			generateAbstractTests(c);
		}	
	}

	
	public void generateAbstractTests(TLCSConfiguration c) throws Exception {
		String outputFileName = null;
		String[] outputFileNames = null;
		String modelName = "";
		
		// Generate the abstract tests
		modelName = c.asmPath.substring(0, c.asmPath.lastIndexOf("."));
		outputFileNames = new GenerateTestsFromFSM().saveFSMWithAvallaTests(modelName, c.asmPath, c.useMonitoring, c.criteria);	
	}
	
}
