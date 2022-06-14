package generator;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.stream.Collectors;

import org.apache.log4j.Level;
import org.apache.log4j.Logger;
import org.asmeta.atgt.generator.CriteriaEnum;
import org.asmeta.atgt.generator.GenerateTestsFromFSM;
import org.asmeta.atgt.generator.NuSMVtestGenerator;
import org.asmeta.atgt.generator.AsmTestGenerator.MBTCoverage;
import org.asmeta.parser.ASMParser;
import org.junit.Test;

import atgt.coverage.AsmCoverage;
import atgt.coverage.AsmCoverageBuilder;
import atgt.parser.asmeta.AsmetaLLoader;
import atgt.parser.asmgofer.ASMParserTest;
import atgt.specification.ASMSpecification;
import extgt.coverage.combinatorial.StdPairwiseCovBuild;
import tgtlib.specification.ParseException;

public class AbstractTestsGenerator {

	class Configuration {
		String asmPath;
		List<CriteriaEnum> criteria;
		boolean useMonitoring;
		String level;
		boolean removeUnChangedControlles;
		boolean removeUnaskedChanges;
		
		public Configuration(String asmPath, List<CriteriaEnum> criteria, String level, boolean removeUnChangedControlles, boolean removeUnaskedChanges) {
			this.asmPath = asmPath;
			this.criteria = criteria;
			this.useMonitoring = true;
			this.level = level;
			this.removeUnChangedControlles = removeUnChangedControlles;
			this.removeUnaskedChanges = removeUnaskedChanges;
		}
		
		public Configuration(String asmPath, boolean useMonitoring, List<CriteriaEnum> criteria, String level, boolean removeUnChangedControlles, boolean removeUnaskedChanges) {
			this.asmPath = asmPath;
			this.criteria = criteria;
			this.useMonitoring = useMonitoring;
			this.level = level;
			this.removeUnChangedControlles = removeUnChangedControlles;
			this.removeUnaskedChanges = removeUnaskedChanges;
		}
		
		public Configuration(String asmPath, String level, boolean removeUnaskedChanges, boolean removeUnChangedControlles, CriteriaEnum ... criteria) {
			this(asmPath,Arrays.asList(criteria), level, removeUnaskedChanges, removeUnChangedControlles);
		}
		
		public Configuration(String asmPath, boolean useMonitoring, String level, boolean removeUnaskedChanges, boolean removeUnChangedControlles, CriteriaEnum ... criteria) {
			this(asmPath,useMonitoring,Arrays.asList(criteria), level, removeUnaskedChanges, removeUnChangedControlles);
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
//			add(new Configuration(ASM_FOLDER_TLCS + "0/TrafficLight_0.asm", "0", false, false, CriteriaEnum.BASIC_RULE));
//			add(new Configuration(ASM_FOLDER_TLCS + "0/TrafficLight_0.asm", "0", false, false, CriteriaEnum.COMBINATORIAL_ALL));
//			add(new Configuration(ASM_FOLDER_TLCS + "0/TrafficLight_0.asm", "0", false, false, CriteriaEnum.COMPLETE_RULE));
//			add(new Configuration(ASM_FOLDER_TLCS + "0/TrafficLight_0.asm", "0", false, false, CriteriaEnum.MCDC));
//			add(new Configuration(ASM_FOLDER_TLCS + "0/TrafficLight_0.asm", "0", false, false, CriteriaEnum.RULE_GUARD));
//			add(new Configuration(ASM_FOLDER_TLCS + "0/TrafficLight_0.asm", "0", false, false, CriteriaEnum.RULE_UPDATE));
//			add(new Configuration(ASM_FOLDER_TLCS + "0/TrafficLight_0.asm", "0", false, false, CriteriaEnum.THREEWISE));
//			add(new Configuration(ASM_FOLDER_TLCS + "0/TrafficLight_0.asm", "0", false, false, CriteriaEnum.BASIC_RULE, CriteriaEnum.COMBINATORIAL_MON,
//					CriteriaEnum.COMPLETE_RULE, CriteriaEnum.MCDC, CriteriaEnum.RULE_GUARD, CriteriaEnum.RULE_UPDATE, CriteriaEnum.THREEWISE_MON));
//			
//			add(new Configuration(ASM_FOLDER_TLCS + "1/TrafficLight_1.asm", "1", false, false, CriteriaEnum.BASIC_RULE));
//			add(new Configuration(ASM_FOLDER_TLCS + "1/TrafficLight_1.asm", "1", false, false, CriteriaEnum.COMBINATORIAL_ALL));
//			add(new Configuration(ASM_FOLDER_TLCS + "1/TrafficLight_1.asm", "1", false, false, CriteriaEnum.COMPLETE_RULE));
//			add(new Configuration(ASM_FOLDER_TLCS + "1/TrafficLight_1.asm", "1", false, false, CriteriaEnum.MCDC));
//			add(new Configuration(ASM_FOLDER_TLCS + "1/TrafficLight_1.asm", "1", false, false, CriteriaEnum.RULE_GUARD));
//			add(new Configuration(ASM_FOLDER_TLCS + "1/TrafficLight_1.asm", "1", false, false, CriteriaEnum.RULE_UPDATE));
//			add(new Configuration(ASM_FOLDER_TLCS + "1/TrafficLight_1.asm", "1", false, false, CriteriaEnum.THREEWISE));
//			add(new Configuration(ASM_FOLDER_TLCS + "1/TrafficLight_1.asm", "1", false, false, CriteriaEnum.BASIC_RULE, CriteriaEnum.COMBINATORIAL_MON,
//					CriteriaEnum.COMPLETE_RULE, CriteriaEnum.MCDC, CriteriaEnum.RULE_GUARD, CriteriaEnum.RULE_UPDATE, CriteriaEnum.THREEWISE_MON));
//			
//			add(new Configuration(ASM_FOLDER_TLCS + "2/TrafficLight_2.asm", "2", false, false, CriteriaEnum.BASIC_RULE));
//			add(new Configuration(ASM_FOLDER_TLCS + "2/TrafficLight_2.asm", "2", false, false, CriteriaEnum.COMBINATORIAL_ALL));
//			add(new Configuration(ASM_FOLDER_TLCS + "2/TrafficLight_2.asm", "2", false, false, CriteriaEnum.COMPLETE_RULE));
//			add(new Configuration(ASM_FOLDER_TLCS + "2/TrafficLight_2.asm", "2", false, false, CriteriaEnum.MCDC));
//			add(new Configuration(ASM_FOLDER_TLCS + "2/TrafficLight_2.asm", "2", false, false, CriteriaEnum.RULE_GUARD));
//			add(new Configuration(ASM_FOLDER_TLCS + "2/TrafficLight_2.asm", "2", false, false, CriteriaEnum.RULE_UPDATE));
//			add(new Configuration(ASM_FOLDER_TLCS + "2/TrafficLight_2.asm", "2", false, false, CriteriaEnum.THREEWISE));
//			add(new Configuration(ASM_FOLDER_TLCS + "2/TrafficLight_2.asm", "2", false, false, CriteriaEnum.BASIC_RULE, CriteriaEnum.COMBINATORIAL_MON,
//					CriteriaEnum.COMPLETE_RULE, CriteriaEnum.MCDC, CriteriaEnum.RULE_GUARD, CriteriaEnum.RULE_UPDATE, CriteriaEnum.THREEWISE_MON));
//						
//			add(new Configuration(ASM_FOLDER_TLCS + "3/TrafficLight_3.asm", "3", false, false, CriteriaEnum.BASIC_RULE));
//			add(new Configuration(ASM_FOLDER_TLCS + "3/TrafficLight_3.asm", "3", false, false, CriteriaEnum.COMBINATORIAL_ALL));
//			add(new Configuration(ASM_FOLDER_TLCS + "3/TrafficLight_3.asm", "3", false, false, CriteriaEnum.COMPLETE_RULE));
//			add(new Configuration(ASM_FOLDER_TLCS + "3/TrafficLight_3.asm", "3", false, false, CriteriaEnum.MCDC));
//			add(new Configuration(ASM_FOLDER_TLCS + "3/TrafficLight_3.asm", "3", false, false, CriteriaEnum.RULE_GUARD));
//			add(new Configuration(ASM_FOLDER_TLCS + "3/TrafficLight_3.asm", "3", false, false, CriteriaEnum.RULE_UPDATE));
//			add(new Configuration(ASM_FOLDER_TLCS + "3/TrafficLight_3.asm", "3", false, false, CriteriaEnum.THREEWISE));
//			add(new Configuration(ASM_FOLDER_TLCS + "3/TrafficLight_3.asm", "3", false, false, CriteriaEnum.BASIC_RULE, CriteriaEnum.COMBINATORIAL_MON,
//					CriteriaEnum.COMPLETE_RULE, CriteriaEnum.MCDC, CriteriaEnum.RULE_GUARD, CriteriaEnum.RULE_UPDATE, CriteriaEnum.THREEWISE_MON));
//			
//			add(new Configuration(ASM_FOLDER_TLCS + "NR/TrafficLight_4.asm", "NR", false, false, CriteriaEnum.BASIC_RULE));
//			add(new Configuration(ASM_FOLDER_TLCS + "NR/TrafficLight_4.asm", "NR", false, false, CriteriaEnum.COMBINATORIAL_ALL));
//			add(new Configuration(ASM_FOLDER_TLCS + "NR/TrafficLight_4.asm", "NR", false, false, CriteriaEnum.COMPLETE_RULE));
//			add(new Configuration(ASM_FOLDER_TLCS + "NR/TrafficLight_4.asm", "NR", false, false, CriteriaEnum.MCDC));
//			add(new Configuration(ASM_FOLDER_TLCS + "NR/TrafficLight_4.asm", "NR", false, false, CriteriaEnum.RULE_GUARD));
//			add(new Configuration(ASM_FOLDER_TLCS + "NR/TrafficLight_4.asm", "NR", false, false, CriteriaEnum.RULE_UPDATE));
//			add(new Configuration(ASM_FOLDER_TLCS + "NR/TrafficLight_4.asm", "NR", false, false, CriteriaEnum.THREEWISE));
//			add(new Configuration(ASM_FOLDER_TLCS + "NR/TrafficLight_4.asm", "NR", false, false, CriteriaEnum.BASIC_RULE, CriteriaEnum.COMBINATORIAL_MON,
//					CriteriaEnum.COMPLETE_RULE, CriteriaEnum.MCDC, CriteriaEnum.RULE_GUARD, CriteriaEnum.RULE_UPDATE, CriteriaEnum.THREEWISE_MON));
			
			//----------------------------
			// PHD Case Study
			//----------------------------		
//			add(new Configuration(ASM_FOLDER_PHD + "0/phd_master_flat2_v0.asm", "0", false, false, CriteriaEnum.BASIC_RULE));
//			add(new Configuration(ASM_FOLDER_PHD + "1/phd_master_flat2_v1.asm", "1", false, false, CriteriaEnum.BASIC_RULE));
//			add(new Configuration(ASM_FOLDER_PHD + "2/phd_master_flat2_v2.asm", "2", false, false, CriteriaEnum.BASIC_RULE));
//			add(new Configuration(ASM_FOLDER_PHD + "3/phd_master_flat2_v3.asm", "3", false, false, CriteriaEnum.BASIC_RULE));
//			add(new Configuration(ASM_FOLDER_PHD + "4/phd_master_flat2_v4.asm", "4", false, false, CriteriaEnum.BASIC_RULE));
//			add(new Configuration(ASM_FOLDER_PHD + "5/phd_master_flat2_v5.asm", "5", false, false, CriteriaEnum.BASIC_RULE));
//			add(new Configuration(ASM_FOLDER_PHD + "6/phd_master_flat2_v6.asm", "6", false, false, CriteriaEnum.BASIC_RULE));
//			add(new Configuration(ASM_FOLDER_PHD + "NR/phd_master_v7_1.asm", "NR", false, false, CriteriaEnum.BASIC_RULE));
//
//			add(new Configuration(ASM_FOLDER_PHD + "0/phd_master_flat2_v0.asm", "0", false, false, CriteriaEnum.COMBINATORIAL_ALL));
//			add(new Configuration(ASM_FOLDER_PHD + "1/phd_master_flat2_v1.asm", "1", false, false, CriteriaEnum.COMBINATORIAL_ALL));
//			add(new Configuration(ASM_FOLDER_PHD + "2/phd_master_flat2_v2.asm", "2", false, false, CriteriaEnum.COMBINATORIAL_ALL));
//			add(new Configuration(ASM_FOLDER_PHD + "3/phd_master_flat2_v3.asm", "3", false, false, CriteriaEnum.COMBINATORIAL_ALL));
//			add(new Configuration(ASM_FOLDER_PHD + "4/phd_master_flat2_v4.asm", "4", false, false, CriteriaEnum.COMBINATORIAL_ALL));
//			add(new Configuration(ASM_FOLDER_PHD + "5/phd_master_flat2_v5.asm", "5", false, false, CriteriaEnum.COMBINATORIAL_ALL));
//			add(new Configuration(ASM_FOLDER_PHD + "6/phd_master_flat2_v6.asm", "6", false, false, CriteriaEnum.COMBINATORIAL_ALL));
//			add(new Configuration(ASM_FOLDER_PHD + "NR/phd_master_v7_1.asm", "NR", false, false, CriteriaEnum.COMBINATORIAL_ALL));
//
//			add(new Configuration(ASM_FOLDER_PHD + "0/phd_master_flat2_v0.asm", "0", false, false, CriteriaEnum.COMPLETE_RULE));
//			add(new Configuration(ASM_FOLDER_PHD + "1/phd_master_flat2_v1.asm", "1", false, false, CriteriaEnum.COMPLETE_RULE));
//			add(new Configuration(ASM_FOLDER_PHD + "2/phd_master_flat2_v2.asm", "2", false, false, CriteriaEnum.COMPLETE_RULE));
//			add(new Configuration(ASM_FOLDER_PHD + "3/phd_master_flat2_v3.asm", "3", false, false, CriteriaEnum.COMPLETE_RULE));
//			add(new Configuration(ASM_FOLDER_PHD + "4/phd_master_flat2_v4.asm", "4", false, false, CriteriaEnum.COMPLETE_RULE));
//			add(new Configuration(ASM_FOLDER_PHD + "5/phd_master_flat2_v5.asm", "5", false, false, CriteriaEnum.COMPLETE_RULE));
//			add(new Configuration(ASM_FOLDER_PHD + "6/phd_master_flat2_v6.asm", "6", false, false, CriteriaEnum.COMPLETE_RULE));
//			add(new Configuration(ASM_FOLDER_PHD + "NR/phd_master_v7_1.asm", "NR", false, false, CriteriaEnum.COMPLETE_RULE));
//
//			add(new Configuration(ASM_FOLDER_PHD + "0/phd_master_flat2_v0.asm", "0", false, false, CriteriaEnum.MCDC));
//			add(new Configuration(ASM_FOLDER_PHD + "1/phd_master_flat2_v1.asm", "1", false, false, CriteriaEnum.MCDC));
//			add(new Configuration(ASM_FOLDER_PHD + "2/phd_master_flat2_v2.asm", "2", false, false, CriteriaEnum.MCDC));
//			add(new Configuration(ASM_FOLDER_PHD + "3/phd_master_flat2_v3.asm", "3", false, false, CriteriaEnum.MCDC));
//			add(new Configuration(ASM_FOLDER_PHD + "4/phd_master_flat2_v4.asm", "4", false, false, CriteriaEnum.MCDC));
//			add(new Configuration(ASM_FOLDER_PHD + "5/phd_master_flat2_v5.asm", "5", false, false, CriteriaEnum.MCDC));
//			add(new Configuration(ASM_FOLDER_PHD + "6/phd_master_flat2_v6.asm", "6", false, false, CriteriaEnum.MCDC));
//			add(new Configuration(ASM_FOLDER_PHD + "NR/phd_master_v7_1.asm", "NR", false, false, CriteriaEnum.MCDC));
//
//			add(new Configuration(ASM_FOLDER_PHD + "0/phd_master_flat2_v0.asm", "0", false, false, CriteriaEnum.RULE_GUARD));
//			add(new Configuration(ASM_FOLDER_PHD + "1/phd_master_flat2_v1.asm", "1", false, false, CriteriaEnum.RULE_GUARD));
//			add(new Configuration(ASM_FOLDER_PHD + "2/phd_master_flat2_v2.asm", "2", false, false, CriteriaEnum.RULE_GUARD));
//			add(new Configuration(ASM_FOLDER_PHD + "3/phd_master_flat2_v3.asm", "3", false, false, CriteriaEnum.RULE_GUARD));
//			add(new Configuration(ASM_FOLDER_PHD + "4/phd_master_flat2_v4.asm", "4", false, false, CriteriaEnum.RULE_GUARD));
//			add(new Configuration(ASM_FOLDER_PHD + "5/phd_master_flat2_v5.asm", "5", false, false, CriteriaEnum.RULE_GUARD));
//			add(new Configuration(ASM_FOLDER_PHD + "6/phd_master_flat2_v6.asm", "6", false, false, CriteriaEnum.RULE_GUARD));
//			add(new Configuration(ASM_FOLDER_PHD + "NR/phd_master_v7_1.asm", "NR", false, false, CriteriaEnum.RULE_GUARD));
//
//			add(new Configuration(ASM_FOLDER_PHD + "0/phd_master_flat2_v0.asm", "0", false, false, CriteriaEnum.RULE_UPDATE));
//			add(new Configuration(ASM_FOLDER_PHD + "1/phd_master_flat2_v1.asm", "1", false, false, CriteriaEnum.RULE_UPDATE));
//			add(new Configuration(ASM_FOLDER_PHD + "2/phd_master_flat2_v2.asm", "2", false, false, CriteriaEnum.RULE_UPDATE));
//			add(new Configuration(ASM_FOLDER_PHD + "3/phd_master_flat2_v3.asm", "3", false, false, CriteriaEnum.RULE_UPDATE));
//			add(new Configuration(ASM_FOLDER_PHD + "4/phd_master_flat2_v4.asm", "4", false, false, CriteriaEnum.RULE_UPDATE));
//			add(new Configuration(ASM_FOLDER_PHD + "5/phd_master_flat2_v5.asm", "5", false, false, CriteriaEnum.RULE_UPDATE));
//			add(new Configuration(ASM_FOLDER_PHD + "6/phd_master_flat2_v6.asm", "6", false, false, CriteriaEnum.RULE_UPDATE));
//			add(new Configuration(ASM_FOLDER_PHD + "NR/phd_master_v7_1.asm", "NR", false, false, CriteriaEnum.RULE_UPDATE));
//			
//			add(new Configuration(ASM_FOLDER_PHD + "0/phd_master_flat2_v0.asm", "0", false, false, CriteriaEnum.THREEWISE));
//			add(new Configuration(ASM_FOLDER_PHD + "1/phd_master_flat2_v1.asm", "1", false, false, CriteriaEnum.THREEWISE));
//			add(new Configuration(ASM_FOLDER_PHD + "2/phd_master_flat2_v2.asm", "2", false, false, CriteriaEnum.THREEWISE));
//			add(new Configuration(ASM_FOLDER_PHD + "3/phd_master_flat2_v3.asm", "3", false, false, CriteriaEnum.THREEWISE));
//			add(new Configuration(ASM_FOLDER_PHD + "4/phd_master_flat2_v4.asm", "4", false, false, CriteriaEnum.THREEWISE));
//			add(new Configuration(ASM_FOLDER_PHD + "5/phd_master_flat2_v5.asm", "5", false, false, CriteriaEnum.THREEWISE));
//			add(new Configuration(ASM_FOLDER_PHD + "6/phd_master_flat2_v6.asm", "6", false, false, CriteriaEnum.THREEWISE));
//			add(new Configuration(ASM_FOLDER_PHD + "NR/phd_master_v7_1.asm", "NR", false, false, CriteriaEnum.THREEWISE));
//			
//			add(new Configuration(ASM_FOLDER_PHD + "0/phd_master_flat2_v0.asm", "0", false, false, CriteriaEnum.BASIC_RULE, CriteriaEnum.COMBINATORIAL_MON, CriteriaEnum.COMPLETE_RULE, CriteriaEnum.MCDC, CriteriaEnum.RULE_GUARD, CriteriaEnum.RULE_UPDATE, CriteriaEnum.THREEWISE_MON));
//			add(new Configuration(ASM_FOLDER_PHD + "1/phd_master_flat2_v1.asm", "1", false, false, CriteriaEnum.BASIC_RULE, CriteriaEnum.COMBINATORIAL_MON, CriteriaEnum.COMPLETE_RULE, CriteriaEnum.MCDC, CriteriaEnum.RULE_GUARD, CriteriaEnum.RULE_UPDATE, CriteriaEnum.THREEWISE_MON));
//			add(new Configuration(ASM_FOLDER_PHD + "2/phd_master_flat2_v2.asm", "2", false, false, CriteriaEnum.BASIC_RULE, CriteriaEnum.COMBINATORIAL_MON, CriteriaEnum.COMPLETE_RULE, CriteriaEnum.MCDC, CriteriaEnum.RULE_GUARD, CriteriaEnum.RULE_UPDATE, CriteriaEnum.THREEWISE_MON));
//			add(new Configuration(ASM_FOLDER_PHD + "3/phd_master_flat2_v3.asm", "3", false, false, CriteriaEnum.BASIC_RULE, CriteriaEnum.COMBINATORIAL_MON, CriteriaEnum.COMPLETE_RULE, CriteriaEnum.MCDC, CriteriaEnum.RULE_GUARD, CriteriaEnum.RULE_UPDATE, CriteriaEnum.THREEWISE_MON));
//			add(new Configuration(ASM_FOLDER_PHD + "4/phd_master_flat2_v4.asm", "4", false, false, CriteriaEnum.BASIC_RULE, CriteriaEnum.COMBINATORIAL_MON, CriteriaEnum.COMPLETE_RULE, CriteriaEnum.MCDC, CriteriaEnum.RULE_GUARD, CriteriaEnum.RULE_UPDATE, CriteriaEnum.THREEWISE_MON));
//			add(new Configuration(ASM_FOLDER_PHD + "5/phd_master_flat2_v5.asm", "5", false, false, CriteriaEnum.BASIC_RULE, CriteriaEnum.COMBINATORIAL_MON, CriteriaEnum.COMPLETE_RULE, CriteriaEnum.MCDC, CriteriaEnum.RULE_GUARD, CriteriaEnum.RULE_UPDATE, CriteriaEnum.THREEWISE_MON));
//			add(new Configuration(ASM_FOLDER_PHD + "6/phd_master_flat2_v6.asm", "6", false, false, CriteriaEnum.BASIC_RULE, CriteriaEnum.COMBINATORIAL_MON, CriteriaEnum.COMPLETE_RULE, CriteriaEnum.MCDC, CriteriaEnum.RULE_GUARD, CriteriaEnum.RULE_UPDATE, CriteriaEnum.THREEWISE_MON));
//			add(new Configuration(ASM_FOLDER_PHD + "NR/phd_master_v7_1.asm", "NR", false, false, CriteriaEnum.BASIC_RULE, CriteriaEnum.COMBINATORIAL_MON, CriteriaEnum.COMPLETE_RULE, CriteriaEnum.MCDC, CriteriaEnum.RULE_GUARD, CriteriaEnum.RULE_UPDATE, CriteriaEnum.THREEWISE_MON));
			
			//----------------------------
			// MVM Case Study
			//----------------------------
//			add(new Configuration(ASM_FOLDER_MVM + "0/Ventilatore0.asm", "0", true, true, CriteriaEnum.BASIC_RULE));
//			add(new Configuration(ASM_FOLDER_MVM + "0/Ventilatore0.asm", "0", true, true, CriteriaEnum.COMBINATORIAL_MON));
//			add(new Configuration(ASM_FOLDER_MVM + "0/Ventilatore0.asm", "0", true, true, CriteriaEnum.COMPLETE_RULE));
//			add(new Configuration(ASM_FOLDER_MVM + "0/Ventilatore0.asm", "0", true, true, CriteriaEnum.MCDC));
//			add(new Configuration(ASM_FOLDER_MVM + "0/Ventilatore0.asm", "0", true, true, CriteriaEnum.RULE_GUARD));
//			add(new Configuration(ASM_FOLDER_MVM + "0/Ventilatore0.asm", "0", true, true, CriteriaEnum.RULE_UPDATE));
//			add(new Configuration(ASM_FOLDER_MVM + "0/Ventilatore0.asm", "0", true, true, CriteriaEnum.THREEWISE_MON));
//			add(new Configuration(ASM_FOLDER_MVM + "0/Ventilatore0.asm", "0", true, true, CriteriaEnum.BASIC_RULE, CriteriaEnum.COMBINATORIAL_ALL,
//					CriteriaEnum.COMPLETE_RULE, CriteriaEnum.MCDC, CriteriaEnum.RULE_GUARD, CriteriaEnum.RULE_UPDATE, CriteriaEnum.THREEWISE_MON));
//			
			add(new Configuration(ASM_FOLDER_MVM + "00/Ventilatore0.asm", "00", true, true, CriteriaEnum.BASIC_RULE));
			add(new Configuration(ASM_FOLDER_MVM + "00/Ventilatore0.asm", "00", true, true, CriteriaEnum.COMBINATORIAL_MON));
			add(new Configuration(ASM_FOLDER_MVM + "00/Ventilatore0.asm", "00", true, true, CriteriaEnum.COMPLETE_RULE));
			add(new Configuration(ASM_FOLDER_MVM + "00/Ventilatore0.asm", "00", true, true, CriteriaEnum.MCDC));
			add(new Configuration(ASM_FOLDER_MVM + "00/Ventilatore0.asm", "00", true, true, CriteriaEnum.RULE_GUARD));
			add(new Configuration(ASM_FOLDER_MVM + "00/Ventilatore0.asm", "00", true, true, CriteriaEnum.RULE_UPDATE));
			add(new Configuration(ASM_FOLDER_MVM + "00/Ventilatore0.asm", "00", true, true, CriteriaEnum.THREEWISE_MON));
			
			add(new Configuration(ASM_FOLDER_MVM + "01/Ventilatore01.asm", "01", true, true, CriteriaEnum.BASIC_RULE));
			add(new Configuration(ASM_FOLDER_MVM + "01/Ventilatore01.asm", "01", true, true, CriteriaEnum.COMBINATORIAL_MON));
			add(new Configuration(ASM_FOLDER_MVM + "01/Ventilatore01.asm", "01", true, true, CriteriaEnum.COMPLETE_RULE));
			add(new Configuration(ASM_FOLDER_MVM + "01/Ventilatore01.asm", "01", true, true, CriteriaEnum.MCDC));
			add(new Configuration(ASM_FOLDER_MVM + "01/Ventilatore01.asm", "01", true, true, CriteriaEnum.RULE_GUARD));
			add(new Configuration(ASM_FOLDER_MVM + "01/Ventilatore01.asm", "01", true, true, CriteriaEnum.RULE_UPDATE));
			add(new Configuration(ASM_FOLDER_MVM + "01/Ventilatore01.asm", "01", true, true, CriteriaEnum.THREEWISE_MON));
			
			add(new Configuration(ASM_FOLDER_MVM + "02/Ventilatore02.asm", "02", true, true, CriteriaEnum.BASIC_RULE));
			add(new Configuration(ASM_FOLDER_MVM + "02/Ventilatore02.asm", "02", true, true, CriteriaEnum.COMBINATORIAL_MON));
			add(new Configuration(ASM_FOLDER_MVM + "02/Ventilatore02.asm", "02", true, true, CriteriaEnum.COMPLETE_RULE));
			add(new Configuration(ASM_FOLDER_MVM + "02/Ventilatore02.asm", "02", false, false, CriteriaEnum.MCDC));
			add(new Configuration(ASM_FOLDER_MVM + "02/Ventilatore02.asm", "02", true, true, CriteriaEnum.RULE_GUARD));
			add(new Configuration(ASM_FOLDER_MVM + "02/Ventilatore02.asm", "02", true, true, CriteriaEnum.RULE_UPDATE));
			add(new Configuration(ASM_FOLDER_MVM + "02/Ventilatore02.asm", "02", true, true, CriteriaEnum.THREEWISE_MON));
//			
//			add(new Configuration(ASM_FOLDER_MVM + "02/Ventilatore02.asm", "02", true, true, CriteriaEnum.BASIC_RULE, CriteriaEnum.MCDC, CriteriaEnum.RULE_GUARD, CriteriaEnum.RULE_UPDATE));
//			add(new Configuration(ASM_FOLDER_MVM + "01/Ventilatore01.asm", "01", true, true, CriteriaEnum.THREEWISE_MON));
//			add(new Configuration(ASM_FOLDER_MVM + "02/Ventilatore02.asm", "02", true, true, CriteriaEnum.THREEWISE_MON));
//			
//			add(new Configuration(ASM_FOLDER_MVM + "1/Ventilatore1.asm", "1", true, true, CriteriaEnum.BASIC_RULE));
//			add(new Configuration(ASM_FOLDER_MVM + "1/Ventilatore1.asm", "1", true, true, CriteriaEnum.COMBINATORIAL_ALL));
//			add(new Configuration(ASM_FOLDER_MVM + "1/Ventilatore1.asm", "1", true, true, CriteriaEnum.COMPLETE_RULE));
//			add(new Configuration(ASM_FOLDER_MVM + "1/Ventilatore1.asm", "1", true, true, CriteriaEnum.MCDC));
//			add(new Configuration(ASM_FOLDER_MVM + "1/Ventilatore1.asm", "1", true, true, CriteriaEnum.RULE_GUARD));
//			add(new Configuration(ASM_FOLDER_MVM + "1/Ventilatore1.asm", "1", true, true, CriteriaEnum.RULE_UPDATE));
//			add(new Configuration(ASM_FOLDER_MVM + "1/Ventilatore1.asm", "1", true, true, CriteriaEnum.THREEWISE_MON));
//			add(new Configuration(ASM_FOLDER_MVM + "1/Ventilatore1.asm", "1", true, true, CriteriaEnum.BASIC_RULE, CriteriaEnum.COMBINATORIAL_ALL,
//					CriteriaEnum.COMPLETE_RULE, CriteriaEnum.MCDC, CriteriaEnum.RULE_GUARD, CriteriaEnum.RULE_UPDATE, CriteriaEnum.THREEWISE_MON));
//			
//			add(new Configuration(ASM_FOLDER_MVM + "2/Ventilatore2.asm", "2", true, true, CriteriaEnum.BASIC_RULE));
//			add(new Configuration(ASM_FOLDER_MVM + "2/Ventilatore2.asm", "2", true, true, CriteriaEnum.COMBINATORIAL_ALL));
//			add(new Configuration(ASM_FOLDER_MVM + "2/Ventilatore2.asm", "2", true, true, CriteriaEnum.COMPLETE_RULE));
//			add(new Configuration(ASM_FOLDER_MVM + "2/Ventilatore2.asm", "2", true, true, CriteriaEnum.MCDC));
//			add(new Configuration(ASM_FOLDER_MVM + "2/Ventilatore2.asm", "2", true, true, CriteriaEnum.RULE_GUARD));
//			add(new Configuration(ASM_FOLDER_MVM + "2/Ventilatore2.asm", "2", true, true, CriteriaEnum.RULE_UPDATE));
//			add(new Configuration(ASM_FOLDER_MVM + "2/Ventilatore2.asm", "2", true, true, CriteriaEnum.THREEWISE_MON));
//			add(new Configuration(ASM_FOLDER_MVM + "2/Ventilatore2.asm", "2", true, true, CriteriaEnum.BASIC_RULE, CriteriaEnum.COMBINATORIAL_ALL,
//					CriteriaEnum.COMPLETE_RULE, CriteriaEnum.MCDC, CriteriaEnum.RULE_GUARD, CriteriaEnum.RULE_UPDATE, CriteriaEnum.THREEWISE_MON));
//						
//			add(new Configuration(ASM_FOLDER_MVM + "3/Ventilatore4SimpleTimeLtdY.asm", "3", true, true, CriteriaEnum.BASIC_RULE));
//			add(new Configuration(ASM_FOLDER_MVM + "3/Ventilatore4SimpleTimeLtdY.asm", "3", true, true, CriteriaEnum.COMBINATORIAL_ALL));
//			add(new Configuration(ASM_FOLDER_MVM + "3/Ventilatore4SimpleTimeLtdY.asm", "3", true, true, CriteriaEnum.COMPLETE_RULE));
//			add(new Configuration(ASM_FOLDER_MVM + "3/Ventilatore4SimpleTimeLtdY.asm", "3", true, true, CriteriaEnum.MCDC));
//			add(new Configuration(ASM_FOLDER_MVM + "3/Ventilatore4SimpleTimeLtdY.asm", "3", true, true, CriteriaEnum.RULE_GUARD));
//			add(new Configuration(ASM_FOLDER_MVM + "3/Ventilatore4SimpleTimeLtdY.asm", "3", true, true, CriteriaEnum.RULE_UPDATE));
//			add(new Configuration(ASM_FOLDER_MVM + "3/Ventilatore4SimpleTimeLtdY.asm", "3", true, true, CriteriaEnum.THREEWISE_MON));
//			add(new Configuration(ASM_FOLDER_MVM + "3/Ventilatore4SimpleTimeLtdY.asm", "3", true, true, CriteriaEnum.BASIC_RULE, CriteriaEnum.COMBINATORIAL_ALL,
//					CriteriaEnum.COMPLETE_RULE, CriteriaEnum.MCDC, CriteriaEnum.RULE_GUARD, CriteriaEnum.RULE_UPDATE, CriteriaEnum.THREEWISE));
			
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
		Logger.getLogger(NuSMVtestGenerator.class).setLevel(Level.ALL);
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
	
	@Test
	public void counttp() throws Exception {
//		String ex = ASM_FOLDER_MVM + "00/Ventilatore0.asm";
//		String ex = ASM_FOLDER_MVM + "01/Ventilatore01.asm";
//		String ex = ASM_FOLDER_MVM + "02/Ventilatore02.asm";
		String ex = ASM_FOLDER_MVM + "3/Ventilatore4SimpleTimeLtdY.asm";
		AsmCoverage ct = buildCT(ex);
		for (int i = 0; i < 9; i++) {
			System.out.println(ct.getChildAt(i).getName() + ": " + ((AsmCoverage) ct.getChildAt(i)).getNumberofTPs());
		}
	}
	
	private AsmCoverage buildCT(String ex) throws Exception, ParseException {
		// Build all the criteria needed for the experiment
		List<AsmCoverageBuilder> coverageCriteria = new ArrayList<>();
		for (CriteriaEnum c : CriteriaEnum.values()) {
			coverageCriteria.add(c.criteria);
		}
		// Read and transform the spec
		ASMSpecification spec = new AsmetaLLoader().read(new File(ex));
		// Build the generator and the tree depending on the criteria
		AsmCoverage ct = new MBTCoverage(coverageCriteria).getTPTree(spec);
		return ct;
	}

	public void generateAbstractTests(Configuration c, String abstract_test_folder) throws Exception {
		String modelName = "";
		
		// Generate the abstract tests
		modelName = c.asmPath.substring(0, c.asmPath.lastIndexOf("."));
		new GenerateTestsFromFSM().saveAbstractTests(modelName, c.asmPath, c.useMonitoring, c.criteria, abstract_test_folder + c.level + "/", c.removeUnaskedChanges, c.removeUnChangedControlles);	
	}
	
	@Test
	public void computeStatistics() throws Exception {
		String abstract_test_folder = "";
		
		for (Configuration c : filesToProcess) {
			if (c.asmPath.contains(ASM_FOLDER_TLCS))
				abstract_test_folder = ABSTRACT_TESTS_FOLDER_TLCS;
			else if (c.asmPath.contains(ASM_FOLDER_PHD))
				abstract_test_folder = ABSTRACT_TESTS_FOLDER_PHD;
			else if (c.asmPath.contains(ASM_FOLDER_MVM))
				abstract_test_folder = ABSTRACT_TESTS_FOLDER_MVM;
			
			// Find the name of the files to be checked
			String criteria = c.criteria.stream().map(n -> n.getAbbrvName()).collect(Collectors.joining("_"));
			String fileOutputName = "test"
					+ criteria;
			// Summary data
			int numSeqs = 0;
			float totSteps = 0;
			int minSteps = 0;
			int maxSteps = 0;
			
			System.out.println("***** CRITERION: " + fileOutputName + " *****");
			System.out.println("***** LEVEL: " + c.level + " *****");
			
			Object[] files = Files.walk(new File(abstract_test_folder + c.level + "/").toPath()).filter(
					f -> (f.getFileName().toString().startsWith(fileOutputName) || f.getFileName().toString().startsWith(criteria))).toArray();
			
			for (Object o : files) {
				Path f = (Path)o;
				int steps = countSteps(f);
				if (steps > 0) {
					numSeqs++;
					totSteps += steps;
					
					if (steps > maxSteps)
						maxSteps = steps;
					
					if (steps < minSteps || minSteps == 0)
						minSteps = steps;
				}
			}
			
			System.out.println("# Seq: " + numSeqs);
			System.out.println("Min steps: " + minSteps);
			System.out.println("Max steps: " + maxSteps);
			System.out.println("Tot steps: " + totSteps);
			System.out.println("Avg steps: " + (totSteps / numSeqs));
		}	
	}
	
	@Test
	public void computeStatisticsForManualScenarios() throws Exception {
		String abstract_test_folder = "";
		
		for (Configuration c : filesToProcess) {
			if (c.asmPath.contains(ASM_FOLDER_TLCS))
				abstract_test_folder = ASM_FOLDER_TLCS + c.level + "/scenarios/";
			else if (c.asmPath.contains(ASM_FOLDER_PHD))
				abstract_test_folder = ASM_FOLDER_PHD + c.level + "/scenarios/";
			else if (c.asmPath.contains(ASM_FOLDER_MVM))
				abstract_test_folder = ASM_FOLDER_MVM + c.level + "/scenarios/";
			
			// Find the name of the files to be checked
			String criteria = c.criteria.stream().map(n -> n.getAbbrvName()).collect(Collectors.joining("_"));
			String fileOutputName = "test"
					+ criteria;
			// Summary data
			int numSeqs = 0;
			float totSteps = 0;
			int minSteps = 0;
			int maxSteps = 0;
			
			System.out.println("***** CRITERION: " + fileOutputName + " *****");
			System.out.println("***** LEVEL: " + c.level + " *****");
			
			Object[] files = Files.walk(new File(abstract_test_folder).toPath()).filter(
					f -> (f.getFileName().toString().startsWith("scenario") && f.getFileName().toString().endsWith(".avalla"))).toArray();
			
			for (Object o : files) {
				Path f = (Path)o;
				int steps = countSteps(f);
				if (steps > 0) {
					numSeqs++;
					totSteps += steps;
					
					if (steps > maxSteps)
						maxSteps = steps;
					
					if (steps < minSteps || minSteps == 0)
						minSteps = steps;
				}
			}
			
			System.out.println("# Seq: " + numSeqs);
			System.out.println("Min steps: " + minSteps);
			System.out.println("Max steps: " + maxSteps);
			System.out.println("Tot steps: " + totSteps);
			System.out.println("Avg steps: " + (totSteps / numSeqs));
		}	
	}

	private int countSteps(Path f) {
		int stepsCount = 0;
		boolean foundSet = false;

		try (BufferedReader br = new BufferedReader(new FileReader(f.toAbsolutePath().toString()))) 
		{
		    String sCurrentLine;
		    while ((sCurrentLine = br.readLine()) != null) 
		    {
		    	if (sCurrentLine.startsWith("set "))
		    		foundSet = true;
		    	
		    	if (sCurrentLine.startsWith("step ") || sCurrentLine.equals("step"))
		    		stepsCount++;
		    }
		} 
		catch (IOException e) 
		{
		    e.printStackTrace();
		}
		
		return foundSet ? (1+stepsCount) : 0;
	}
	
}
