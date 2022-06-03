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

	static String ASM_FOLDER_TLCS = "../Case_studies/Traffic_Light_Control_System/ASM/TrafficLight_";
	static String ABSTRACT_TESTS_FOLDER_TLCS = "../Case_studies/Traffic_Light_Control_System/abstract_tests/TrafficLight_";
	static String ASM_FOLDER_PHD = "../Case_studies/PHD_Protocol/ASM/PHD_Protocol_";
	static String ABSTRACT_TESTS_FOLDER_PHD = "../Case_studies/PHD_Protocol/abstract_tests/PHD_Protocol_";
	static String ASM_FOLDER_MVM = "../Case_studies/MVM/ASM/MVM_";
	static String ABSTRACT_TESTS_FOLDER_MVM = "../Case_studies/MVM/abstract_tests/MVM_";

	List<Configuration> filesToProcess = new ArrayList<Configuration>() {
		
		private static final long serialVersionUID = 5014712752302397500L;
		{
			//----------------------------
			// TLCS Case Study
			//----------------------------
			/*add(new Configuration(ASM_FOLDER_TLCS + "0/TrafficLight_0.asm", "0", CriteriaEnum.BASIC_RULE));
			add(new Configuration(ASM_FOLDER_TLCS + "0/TrafficLight_0.asm", "0", CriteriaEnum.COMBINATORIAL_ALL));
			add(new Configuration(ASM_FOLDER_TLCS + "0/TrafficLight_0.asm", "0", CriteriaEnum.COMPLETE_RULE));
			add(new Configuration(ASM_FOLDER_TLCS + "0/TrafficLight_0.asm", "0", CriteriaEnum.MCDC));
			add(new Configuration(ASM_FOLDER_TLCS + "0/TrafficLight_0.asm", "0", CriteriaEnum.RULE_GUARD));
			add(new Configuration(ASM_FOLDER_TLCS + "0/TrafficLight_0.asm", "0", CriteriaEnum.RULE_UPDATE));
			add(new Configuration(ASM_FOLDER_TLCS + "0/TrafficLight_0.asm", "0", CriteriaEnum.THREEWISE));
			add(new Configuration(ASM_FOLDER_TLCS + "0/TrafficLight_0.asm", "0", CriteriaEnum.BASIC_RULE, CriteriaEnum.COMBINATORIAL_ALL,
					CriteriaEnum.COMPLETE_RULE, CriteriaEnum.MCDC, CriteriaEnum.RULE_GUARD, CriteriaEnum.RULE_UPDATE, CriteriaEnum.THREEWISE));
			
			add(new Configuration(ASM_FOLDER_TLCS + "1/TrafficLight_1.asm", "1", CriteriaEnum.BASIC_RULE));
			add(new Configuration(ASM_FOLDER_TLCS + "1/TrafficLight_1.asm", "1", CriteriaEnum.COMBINATORIAL_ALL));
			add(new Configuration(ASM_FOLDER_TLCS + "1/TrafficLight_1.asm", "1", CriteriaEnum.COMPLETE_RULE));
			add(new Configuration(ASM_FOLDER_TLCS + "1/TrafficLight_1.asm", "1", CriteriaEnum.MCDC));
			add(new Configuration(ASM_FOLDER_TLCS + "1/TrafficLight_1.asm", "1", CriteriaEnum.RULE_GUARD));
			add(new Configuration(ASM_FOLDER_TLCS + "1/TrafficLight_1.asm", "1", CriteriaEnum.RULE_UPDATE));
			add(new Configuration(ASM_FOLDER_TLCS + "1/TrafficLight_1.asm", "1", CriteriaEnum.THREEWISE));
			add(new Configuration(ASM_FOLDER_TLCS + "1/TrafficLight_1.asm", "1", CriteriaEnum.BASIC_RULE, CriteriaEnum.COMBINATORIAL_ALL,
					CriteriaEnum.COMPLETE_RULE, CriteriaEnum.MCDC, CriteriaEnum.RULE_GUARD, CriteriaEnum.RULE_UPDATE, CriteriaEnum.THREEWISE));
			
			add(new Configuration(ASM_FOLDER_TLCS + "2/TrafficLight_2.asm", "2", CriteriaEnum.BASIC_RULE));
			add(new Configuration(ASM_FOLDER_TLCS + "2/TrafficLight_2.asm", "2", CriteriaEnum.COMBINATORIAL_ALL));
			add(new Configuration(ASM_FOLDER_TLCS + "2/TrafficLight_2.asm", "2", CriteriaEnum.COMPLETE_RULE));
			add(new Configuration(ASM_FOLDER_TLCS + "2/TrafficLight_2.asm", "2", CriteriaEnum.MCDC));
			add(new Configuration(ASM_FOLDER_TLCS + "2/TrafficLight_2.asm", "2", CriteriaEnum.RULE_GUARD));
			add(new Configuration(ASM_FOLDER_TLCS + "2/TrafficLight_2.asm", "2", CriteriaEnum.RULE_UPDATE));
			add(new Configuration(ASM_FOLDER_TLCS + "2/TrafficLight_2.asm", "2", CriteriaEnum.THREEWISE));
			add(new Configuration(ASM_FOLDER_TLCS + "2/TrafficLight_2.asm", "2", CriteriaEnum.BASIC_RULE, CriteriaEnum.COMBINATORIAL_ALL,
					CriteriaEnum.COMPLETE_RULE, CriteriaEnum.MCDC, CriteriaEnum.RULE_GUARD, CriteriaEnum.RULE_UPDATE, CriteriaEnum.THREEWISE));
						
			add(new Configuration(ASM_FOLDER_TLCS + "3/TrafficLight_3.asm", "3", CriteriaEnum.BASIC_RULE));
			add(new Configuration(ASM_FOLDER_TLCS + "3/TrafficLight_3.asm", "3", CriteriaEnum.COMBINATORIAL_ALL));
			add(new Configuration(ASM_FOLDER_TLCS + "3/TrafficLight_3.asm", "3", CriteriaEnum.COMPLETE_RULE));
			add(new Configuration(ASM_FOLDER_TLCS + "3/TrafficLight_3.asm", "3", CriteriaEnum.MCDC));
			add(new Configuration(ASM_FOLDER_TLCS + "3/TrafficLight_3.asm", "3", CriteriaEnum.RULE_GUARD));
			add(new Configuration(ASM_FOLDER_TLCS + "3/TrafficLight_3.asm", "3", CriteriaEnum.RULE_UPDATE));
			add(new Configuration(ASM_FOLDER_TLCS + "3/TrafficLight_3.asm", "3", CriteriaEnum.THREEWISE));
			add(new Configuration(ASM_FOLDER_TLCS + "3/TrafficLight_3.asm", "3", CriteriaEnum.BASIC_RULE, CriteriaEnum.COMBINATORIAL_ALL,
					CriteriaEnum.COMPLETE_RULE, CriteriaEnum.MCDC, CriteriaEnum.RULE_GUARD, CriteriaEnum.RULE_UPDATE, CriteriaEnum.THREEWISE));
			
			add(new Configuration(ASM_FOLDER_TLCS + "NR/TrafficLight_4.asm", "NR", CriteriaEnum.BASIC_RULE));
			add(new Configuration(ASM_FOLDER_TLCS + "NR/TrafficLight_4.asm", "NR", CriteriaEnum.COMBINATORIAL_ALL));
			add(new Configuration(ASM_FOLDER_TLCS + "NR/TrafficLight_4.asm", "NR", CriteriaEnum.COMPLETE_RULE));
			add(new Configuration(ASM_FOLDER_TLCS + "NR/TrafficLight_4.asm", "NR", CriteriaEnum.MCDC));
			add(new Configuration(ASM_FOLDER_TLCS + "NR/TrafficLight_4.asm", "NR", CriteriaEnum.RULE_GUARD));
			add(new Configuration(ASM_FOLDER_TLCS + "NR/TrafficLight_4.asm", "NR", CriteriaEnum.RULE_UPDATE));
			add(new Configuration(ASM_FOLDER_TLCS + "NR/TrafficLight_4.asm", "NR", CriteriaEnum.THREEWISE));
			add(new Configuration(ASM_FOLDER_TLCS + "NR/TrafficLight_4.asm", "NR", CriteriaEnum.BASIC_RULE, CriteriaEnum.COMBINATORIAL_ALL,
					CriteriaEnum.COMPLETE_RULE, CriteriaEnum.MCDC, CriteriaEnum.RULE_GUARD, CriteriaEnum.RULE_UPDATE, CriteriaEnum.THREEWISE));*/
			
			//----------------------------
			// PHD Case Study
			//----------------------------		
			/*add(new Configuration(ASM_FOLDER_PHD + "0/phd_master_flat2_v0.asm", "0", CriteriaEnum.BASIC_RULE));
			add(new Configuration(ASM_FOLDER_PHD + "1/phd_master_flat2_v1.asm", "1", CriteriaEnum.BASIC_RULE));
			add(new Configuration(ASM_FOLDER_PHD + "2/phd_master_flat2_v2.asm", "2", CriteriaEnum.BASIC_RULE));
			add(new Configuration(ASM_FOLDER_PHD + "3/phd_master_flat2_v3.asm", "3", CriteriaEnum.BASIC_RULE));
			add(new Configuration(ASM_FOLDER_PHD + "4/phd_master_flat2_v4.asm", "4", CriteriaEnum.BASIC_RULE));
			add(new Configuration(ASM_FOLDER_PHD + "5/phd_master_flat2_v5.asm", "5", CriteriaEnum.BASIC_RULE));
			add(new Configuration(ASM_FOLDER_PHD + "6/phd_master_flat2_v6.asm", "6", CriteriaEnum.BASIC_RULE));
			add(new Configuration(ASM_FOLDER_PHD + "NR/phd_master_v7_1.asm", "NR", CriteriaEnum.BASIC_RULE));

			add(new Configuration(ASM_FOLDER_PHD + "0/phd_master_flat2_v0.asm", "0", CriteriaEnum.COMBINATORIAL_ALL));
			add(new Configuration(ASM_FOLDER_PHD + "1/phd_master_flat2_v1.asm", "1", CriteriaEnum.COMBINATORIAL_ALL));
			add(new Configuration(ASM_FOLDER_PHD + "2/phd_master_flat2_v2.asm", "2", CriteriaEnum.COMBINATORIAL_ALL));
			add(new Configuration(ASM_FOLDER_PHD + "3/phd_master_flat2_v3.asm", "3", CriteriaEnum.COMBINATORIAL_ALL));
			add(new Configuration(ASM_FOLDER_PHD + "4/phd_master_flat2_v4.asm", "4", CriteriaEnum.COMBINATORIAL_ALL));
			add(new Configuration(ASM_FOLDER_PHD + "5/phd_master_flat2_v5.asm", "5", CriteriaEnum.COMBINATORIAL_ALL));
			add(new Configuration(ASM_FOLDER_PHD + "6/phd_master_flat2_v6.asm", "6", CriteriaEnum.COMBINATORIAL_ALL));
			add(new Configuration(ASM_FOLDER_PHD + "NR/phd_master_v7_1.asm", "NR", CriteriaEnum.COMBINATORIAL_ALL));

			add(new Configuration(ASM_FOLDER_PHD + "0/phd_master_flat2_v0.asm", "0", CriteriaEnum.COMPLETE_RULE));
			add(new Configuration(ASM_FOLDER_PHD + "1/phd_master_flat2_v1.asm", "1", CriteriaEnum.COMPLETE_RULE));
			add(new Configuration(ASM_FOLDER_PHD + "2/phd_master_flat2_v2.asm", "2", CriteriaEnum.COMPLETE_RULE));
			add(new Configuration(ASM_FOLDER_PHD + "3/phd_master_flat2_v3.asm", "3", CriteriaEnum.COMPLETE_RULE));
			add(new Configuration(ASM_FOLDER_PHD + "4/phd_master_flat2_v4.asm", "4", CriteriaEnum.COMPLETE_RULE));
			add(new Configuration(ASM_FOLDER_PHD + "5/phd_master_flat2_v5.asm", "5", CriteriaEnum.COMPLETE_RULE));
			add(new Configuration(ASM_FOLDER_PHD + "6/phd_master_flat2_v6.asm", "6", CriteriaEnum.COMPLETE_RULE));
			add(new Configuration(ASM_FOLDER_PHD + "NR/phd_master_v7_1.asm", "NR", CriteriaEnum.COMPLETE_RULE));

			add(new Configuration(ASM_FOLDER_PHD + "0/phd_master_flat2_v0.asm", "0", CriteriaEnum.MCDC));
			add(new Configuration(ASM_FOLDER_PHD + "1/phd_master_flat2_v1.asm", "1", CriteriaEnum.MCDC));
			add(new Configuration(ASM_FOLDER_PHD + "2/phd_master_flat2_v2.asm", "2", CriteriaEnum.MCDC));
			add(new Configuration(ASM_FOLDER_PHD + "3/phd_master_flat2_v3.asm", "3", CriteriaEnum.MCDC));
			add(new Configuration(ASM_FOLDER_PHD + "4/phd_master_flat2_v4.asm", "4", CriteriaEnum.MCDC));
			add(new Configuration(ASM_FOLDER_PHD + "5/phd_master_flat2_v5.asm", "5", CriteriaEnum.MCDC));
			add(new Configuration(ASM_FOLDER_PHD + "6/phd_master_flat2_v6.asm", "6", CriteriaEnum.MCDC));
			add(new Configuration(ASM_FOLDER_PHD + "NR/phd_master_v7_1.asm", "NR", CriteriaEnum.MCDC));

			add(new Configuration(ASM_FOLDER_PHD + "0/phd_master_flat2_v0.asm", "0", CriteriaEnum.RULE_GUARD));
			add(new Configuration(ASM_FOLDER_PHD + "1/phd_master_flat2_v1.asm", "1", CriteriaEnum.RULE_GUARD));
			add(new Configuration(ASM_FOLDER_PHD + "2/phd_master_flat2_v2.asm", "2", CriteriaEnum.RULE_GUARD));
			add(new Configuration(ASM_FOLDER_PHD + "3/phd_master_flat2_v3.asm", "3", CriteriaEnum.RULE_GUARD));
			add(new Configuration(ASM_FOLDER_PHD + "4/phd_master_flat2_v4.asm", "4", CriteriaEnum.RULE_GUARD));
			add(new Configuration(ASM_FOLDER_PHD + "5/phd_master_flat2_v5.asm", "5", CriteriaEnum.RULE_GUARD));
			add(new Configuration(ASM_FOLDER_PHD + "6/phd_master_flat2_v6.asm", "6", CriteriaEnum.RULE_GUARD));
			add(new Configuration(ASM_FOLDER_PHD + "NR/phd_master_v7_1.asm", "NR", CriteriaEnum.RULE_GUARD));

			add(new Configuration(ASM_FOLDER_PHD + "0/phd_master_flat2_v0.asm", "0", CriteriaEnum.RULE_UPDATE));
			add(new Configuration(ASM_FOLDER_PHD + "1/phd_master_flat2_v1.asm", "1", CriteriaEnum.RULE_UPDATE));
			add(new Configuration(ASM_FOLDER_PHD + "2/phd_master_flat2_v2.asm", "2", CriteriaEnum.RULE_UPDATE));
			add(new Configuration(ASM_FOLDER_PHD + "3/phd_master_flat2_v3.asm", "3", CriteriaEnum.RULE_UPDATE));
			add(new Configuration(ASM_FOLDER_PHD + "4/phd_master_flat2_v4.asm", "4", CriteriaEnum.RULE_UPDATE));
			add(new Configuration(ASM_FOLDER_PHD + "5/phd_master_flat2_v5.asm", "5", CriteriaEnum.RULE_UPDATE));
			add(new Configuration(ASM_FOLDER_PHD + "6/phd_master_flat2_v6.asm", "6", CriteriaEnum.RULE_UPDATE));
			add(new Configuration(ASM_FOLDER_PHD + "NR/phd_master_v7_1.asm", "NR", CriteriaEnum.RULE_UPDATE));
			
			add(new Configuration(ASM_FOLDER_PHD + "0/phd_master_flat2_v0.asm", "0", CriteriaEnum.THREEWISE));
			add(new Configuration(ASM_FOLDER_PHD + "1/phd_master_flat2_v1.asm", "1", CriteriaEnum.THREEWISE));
			add(new Configuration(ASM_FOLDER_PHD + "2/phd_master_flat2_v2.asm", "2", CriteriaEnum.THREEWISE));
			add(new Configuration(ASM_FOLDER_PHD + "3/phd_master_flat2_v3.asm", "3", CriteriaEnum.THREEWISE));
			add(new Configuration(ASM_FOLDER_PHD + "4/phd_master_flat2_v4.asm", "4", CriteriaEnum.THREEWISE));
			add(new Configuration(ASM_FOLDER_PHD + "5/phd_master_flat2_v5.asm", "5", CriteriaEnum.THREEWISE));
			add(new Configuration(ASM_FOLDER_PHD + "6/phd_master_flat2_v6.asm", "6", CriteriaEnum.THREEWISE));
			add(new Configuration(ASM_FOLDER_PHD + "NR/phd_master_v7_1.asm", "NR", CriteriaEnum.THREEWISE));
			
			add(new Configuration(ASM_FOLDER_PHD + "0/phd_master_flat2_v0.asm", "0", CriteriaEnum.BASIC_RULE, CriteriaEnum.COMBINATORIAL_ALL, CriteriaEnum.COMPLETE_RULE, CriteriaEnum.MCDC, CriteriaEnum.RULE_GUARD, CriteriaEnum.RULE_UPDATE, CriteriaEnum.THREEWISE));
			add(new Configuration(ASM_FOLDER_PHD + "1/phd_master_flat2_v1.asm", "1", CriteriaEnum.BASIC_RULE, CriteriaEnum.COMBINATORIAL_ALL, CriteriaEnum.COMPLETE_RULE, CriteriaEnum.MCDC, CriteriaEnum.RULE_GUARD, CriteriaEnum.RULE_UPDATE, CriteriaEnum.THREEWISE));
			add(new Configuration(ASM_FOLDER_PHD + "2/phd_master_flat2_v2.asm", "2", CriteriaEnum.BASIC_RULE, CriteriaEnum.COMBINATORIAL_ALL, CriteriaEnum.COMPLETE_RULE, CriteriaEnum.MCDC, CriteriaEnum.RULE_GUARD, CriteriaEnum.RULE_UPDATE, CriteriaEnum.THREEWISE));
			add(new Configuration(ASM_FOLDER_PHD + "3/phd_master_flat2_v3.asm", "3", CriteriaEnum.BASIC_RULE, CriteriaEnum.COMBINATORIAL_ALL, CriteriaEnum.COMPLETE_RULE, CriteriaEnum.MCDC, CriteriaEnum.RULE_GUARD, CriteriaEnum.RULE_UPDATE, CriteriaEnum.THREEWISE));
			add(new Configuration(ASM_FOLDER_PHD + "4/phd_master_flat2_v4.asm", "4", CriteriaEnum.BASIC_RULE, CriteriaEnum.COMBINATORIAL_ALL, CriteriaEnum.COMPLETE_RULE, CriteriaEnum.MCDC, CriteriaEnum.RULE_GUARD, CriteriaEnum.RULE_UPDATE, CriteriaEnum.THREEWISE));
			add(new Configuration(ASM_FOLDER_PHD + "5/phd_master_flat2_v5.asm", "5", CriteriaEnum.BASIC_RULE, CriteriaEnum.COMBINATORIAL_ALL, CriteriaEnum.COMPLETE_RULE, CriteriaEnum.MCDC, CriteriaEnum.RULE_GUARD, CriteriaEnum.RULE_UPDATE, CriteriaEnum.THREEWISE));
			add(new Configuration(ASM_FOLDER_PHD + "6/phd_master_flat2_v6.asm", "6", CriteriaEnum.BASIC_RULE, CriteriaEnum.COMBINATORIAL_ALL, CriteriaEnum.COMPLETE_RULE, CriteriaEnum.MCDC, CriteriaEnum.RULE_GUARD, CriteriaEnum.RULE_UPDATE, CriteriaEnum.THREEWISE));
			add(new Configuration(ASM_FOLDER_PHD + "NR/phd_master_v7_1.asm", "NR", CriteriaEnum.BASIC_RULE, CriteriaEnum.COMBINATORIAL_ALL, CriteriaEnum.COMPLETE_RULE, CriteriaEnum.MCDC, CriteriaEnum.RULE_GUARD, CriteriaEnum.RULE_UPDATE, CriteriaEnum.THREEWISE));*/
			
			//----------------------------
			// MVM Case Study
			//----------------------------
			/*add(new Configuration(ASM_FOLDER_MVM + "0/Ventilatore0.asm", "0", CriteriaEnum.BASIC_RULE));
			add(new Configuration(ASM_FOLDER_MVM + "0/Ventilatore0.asm", "0", CriteriaEnum.COMBINATORIAL_ALL));
			add(new Configuration(ASM_FOLDER_MVM + "0/Ventilatore0.asm", "0", CriteriaEnum.COMPLETE_RULE));
			add(new Configuration(ASM_FOLDER_MVM + "0/Ventilatore0.asm", "0", CriteriaEnum.MCDC));
			add(new Configuration(ASM_FOLDER_MVM + "0/Ventilatore0.asm", "0", CriteriaEnum.RULE_GUARD));
			add(new Configuration(ASM_FOLDER_MVM + "0/Ventilatore0.asm", "0", CriteriaEnum.RULE_UPDATE));
			add(new Configuration(ASM_FOLDER_MVM + "0/Ventilatore0.asm", "0", CriteriaEnum.THREEWISE));
			add(new Configuration(ASM_FOLDER_MVM + "0/Ventilatore0.asm", "0", CriteriaEnum.BASIC_RULE, CriteriaEnum.COMBINATORIAL_ALL,
					CriteriaEnum.COMPLETE_RULE, CriteriaEnum.MCDC, CriteriaEnum.RULE_GUARD, CriteriaEnum.RULE_UPDATE, CriteriaEnum.THREEWISE));
			
			*/add(new Configuration(ASM_FOLDER_MVM + "1/Ventilatore1.asm", "1", CriteriaEnum.BASIC_RULE));
			add(new Configuration(ASM_FOLDER_MVM + "1/Ventilatore1.asm", "1", CriteriaEnum.COMBINATORIAL_ALL));
			add(new Configuration(ASM_FOLDER_MVM + "1/Ventilatore1.asm", "1", CriteriaEnum.COMPLETE_RULE));
			add(new Configuration(ASM_FOLDER_MVM + "1/Ventilatore1.asm", "1", CriteriaEnum.MCDC));
			add(new Configuration(ASM_FOLDER_MVM + "1/Ventilatore1.asm", "1", CriteriaEnum.RULE_GUARD));
			add(new Configuration(ASM_FOLDER_MVM + "1/Ventilatore1.asm", "1", CriteriaEnum.RULE_UPDATE));
			add(new Configuration(ASM_FOLDER_MVM + "1/Ventilatore1.asm", "1", CriteriaEnum.THREEWISE));
			/*add(new Configuration(ASM_FOLDER_MVM + "1/Ventilatore1.asm", "1", CriteriaEnum.BASIC_RULE, CriteriaEnum.COMBINATORIAL_ALL,
					CriteriaEnum.COMPLETE_RULE, CriteriaEnum.MCDC, CriteriaEnum.RULE_GUARD, CriteriaEnum.RULE_UPDATE, CriteriaEnum.THREEWISE));
			
			add(new Configuration(ASM_FOLDER_MVM + "2/Ventilatore2.asm", "2", CriteriaEnum.BASIC_RULE));
			add(new Configuration(ASM_FOLDER_MVM + "2/Ventilatore2.asm", "2", CriteriaEnum.COMBINATORIAL_ALL));
			add(new Configuration(ASM_FOLDER_MVM + "2/Ventilatore2.asm", "2", CriteriaEnum.COMPLETE_RULE));
			add(new Configuration(ASM_FOLDER_MVM + "2/Ventilatore2.asm", "2", CriteriaEnum.MCDC));
			add(new Configuration(ASM_FOLDER_MVM + "2/Ventilatore2.asm", "2", CriteriaEnum.RULE_GUARD));
			add(new Configuration(ASM_FOLDER_MVM + "2/Ventilatore2.asm", "2", CriteriaEnum.RULE_UPDATE));
			add(new Configuration(ASM_FOLDER_MVM + "2/Ventilatore2.asm", "2", CriteriaEnum.THREEWISE));
			add(new Configuration(ASM_FOLDER_MVM + "2/Ventilatore2.asm", "2", CriteriaEnum.BASIC_RULE, CriteriaEnum.COMBINATORIAL_ALL,
					CriteriaEnum.COMPLETE_RULE, CriteriaEnum.MCDC, CriteriaEnum.RULE_GUARD, CriteriaEnum.RULE_UPDATE, CriteriaEnum.THREEWISE));
						
			add(new Configuration(ASM_FOLDER_MVM + "3/Ventilatore4SimpleTimeLtdY.asm", "3", CriteriaEnum.BASIC_RULE));
			add(new Configuration(ASM_FOLDER_MVM + "3/Ventilatore4SimpleTimeLtdY.asm", "3", CriteriaEnum.COMBINATORIAL_ALL));
			add(new Configuration(ASM_FOLDER_MVM + "3/Ventilatore4SimpleTimeLtdY.asm", "3", CriteriaEnum.COMPLETE_RULE));
			add(new Configuration(ASM_FOLDER_MVM + "3/Ventilatore4SimpleTimeLtdY.asm", "3", CriteriaEnum.MCDC));
			add(new Configuration(ASM_FOLDER_MVM + "3/Ventilatore4SimpleTimeLtdY.asm", "3", CriteriaEnum.RULE_GUARD));
			add(new Configuration(ASM_FOLDER_MVM + "3/Ventilatore4SimpleTimeLtdY.asm", "3", CriteriaEnum.RULE_UPDATE));*/
			//add(new Configuration(ASM_FOLDER_MVM + "3/Ventilatore4SimpleTimeLtdY.asm", "3", CriteriaEnum.THREEWISE));
			/*add(new Configuration(ASM_FOLDER_MVM + "3/Ventilatore4SimpleTimeLtdY.asm", "3", CriteriaEnum.BASIC_RULE, CriteriaEnum.COMBINATORIAL_ALL,
					CriteriaEnum.COMPLETE_RULE, CriteriaEnum.MCDC, CriteriaEnum.RULE_GUARD, CriteriaEnum.RULE_UPDATE, CriteriaEnum.THREEWISE));*/
			
		}
	};	
	
	@Test
	public void generateAbstractTests() throws Exception {
		// XXX select configuration to execute (only one at a time!!)
		Logger.getLogger(StdPairwiseCovBuild.class).setLevel(Level.ALL);
		Configuration c = filesToProcess.get(1);
		String abstract_test_folder = "";
		
		if (c.asmPath.contains(ASM_FOLDER_TLCS))
			abstract_test_folder = ABSTRACT_TESTS_FOLDER_TLCS;
		else if (c.asmPath.contains(ASM_FOLDER_PHD))
			abstract_test_folder = ABSTRACT_TESTS_FOLDER_PHD;
		else if (c.asmPath.contains(ASM_FOLDER_MVM))
			abstract_test_folder = ABSTRACT_TESTS_FOLDER_MVM;
		
		generateAbstractTests(c, abstract_test_folder);	
	}
	
	/**
	 * Generates all the abstract tests
	 * @throws Exception
	 */
	@Test
	public void generateAllAbstractTests() throws Exception {
		Logger.getLogger(StdPairwiseCovBuild.class).setLevel(Level.ALL);
		String abstract_test_folder = "";
		
		for (Configuration c : filesToProcess) {
			if (c.asmPath.contains(ASM_FOLDER_TLCS))
				abstract_test_folder = ABSTRACT_TESTS_FOLDER_TLCS;
			else if (c.asmPath.contains(ASM_FOLDER_PHD))
				abstract_test_folder = ABSTRACT_TESTS_FOLDER_PHD;
			else if (c.asmPath.contains(ASM_FOLDER_MVM))
				abstract_test_folder = ABSTRACT_TESTS_FOLDER_MVM;
			
			generateAbstractTests(c, abstract_test_folder);	
		}	
	}

	
	public void generateAbstractTests(Configuration c, String abstract_test_folder) throws Exception {
		String modelName = "";
		
		// Generate the abstract tests
		modelName = c.asmPath.substring(0, c.asmPath.lastIndexOf("."));
		new GenerateTestsFromFSM().saveAbstractTests(modelName, c.asmPath, c.useMonitoring, c.criteria, abstract_test_folder + c.level + "/");	
	}
	
}
