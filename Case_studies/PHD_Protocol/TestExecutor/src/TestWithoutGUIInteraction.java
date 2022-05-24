package gui;

import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.io.OutputStream;
import java.io.PrintStream;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import org.apache.log4j.Level;
import org.apache.log4j.Logger;
import org.asmeta.atgt.generator.AsmTestGenerator;
import org.asmeta.atgt.generator.CriteriaEnum;
import org.asmeta.atgt.generator.GenerateTestsFromFSM;
import org.asmeta.atgt.generator.NuSMVtestGenerator;
import org.asmeta.atgt.generator.TestGenerationWithNuSMV;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Display;
import org.eclipse.swt.widgets.Event;
import org.eclipse.swt.widgets.MessageBox;
import org.eclipse.swt.widgets.Shell;
import org.junit.Test;

import atgt.coverage.AsmCoverageBuilder;
import atgt.coverage.AsmTestSeqContent;
import atgt.coverage.AsmTestSuite;
import extgt.coverage.combinatorial.StdPairwiseCovBuild;
import service.FSMCreator;

public class TestWithoutGUIInteraction {

	/** if it is not arera, it is speedy */
	public static boolean useArera = true;

	class Configuration {
		String fsmPath;
		String asmPath;
		boolean useArera;
		List<CriteriaEnum> criteria;
		boolean useMonitoring;
		
		public Configuration(String fsmPath, String asmPath, boolean useArera, List<CriteriaEnum> criteria) {
			this.fsmPath = fsmPath;
			this.asmPath = asmPath;
			this.useArera = useArera;
			this.criteria = criteria;
			this.useMonitoring = true;
		}
		
		public Configuration(String fsmPath, String asmPath, boolean useArera, boolean useMonitoring, List<CriteriaEnum> criteria) {
			this.fsmPath = fsmPath;
			this.asmPath = asmPath;
			this.useArera = useArera;
			this.criteria = criteria;
			this.useMonitoring = useMonitoring;
		}
		
		public Configuration(String fsmPath, String asmPath, boolean useArera, CriteriaEnum ... criteria) {
			this(fsmPath,asmPath,useArera,Arrays.asList(criteria));
		}
		
		public Configuration(String fsmPath, String asmPath, boolean useArera, boolean useMonitoring, CriteriaEnum ... criteria) {
			this(fsmPath,asmPath,useArera,useMonitoring,Arrays.asList(criteria));
		}
	}

	static String ASM_PHD_FOLDER = "F:\\Dati-Andrea\\GitHub\\ProTestUTA\\ASM\\newPHD\\";

	List<Configuration> filesToProcess = new ArrayList<Configuration>() {
		
		private static final long serialVersionUID = 5014712752302397500L;
		{
			add(new Configuration("", ASM_PHD_FOLDER + "phd_master_v7.asm", useArera, CriteriaEnum.BASIC_RULE, CriteriaEnum.COMBINATORIAL_ALL, CriteriaEnum.COMPLETE_RULE, CriteriaEnum.MCDC, CriteriaEnum.RULE_GUARD, CriteriaEnum.RULE_UPDATE, CriteriaEnum.THREEWISE));
			add(new Configuration("", ASM_PHD_FOLDER + "phd_master_v7.asm", useArera, CriteriaEnum.THREEWISE));
			
			add(new Configuration("", ASM_PHD_FOLDER + "phd_master_flat2_v0.asm", useArera, CriteriaEnum.BASIC_RULE));
			add(new Configuration("", ASM_PHD_FOLDER + "phd_master_flat2_v1.asm", useArera, CriteriaEnum.BASIC_RULE));
			add(new Configuration("", ASM_PHD_FOLDER + "phd_master_flat2_v2.asm", useArera, CriteriaEnum.BASIC_RULE));
			add(new Configuration("", ASM_PHD_FOLDER + "phd_master_flat2_v3.asm", useArera, CriteriaEnum.BASIC_RULE));
			add(new Configuration("", ASM_PHD_FOLDER + "phd_master_flat2_v4.asm", useArera, CriteriaEnum.BASIC_RULE));
			add(new Configuration("", ASM_PHD_FOLDER + "phd_master_flat2_v5.asm", useArera, CriteriaEnum.BASIC_RULE));
			add(new Configuration("", ASM_PHD_FOLDER + "phd_master_flat2_v6.asm", useArera, CriteriaEnum.BASIC_RULE));
			add(new Configuration("", ASM_PHD_FOLDER + "phd_master_v7_1.asm", useArera, CriteriaEnum.BASIC_RULE));

			add(new Configuration("", ASM_PHD_FOLDER + "phd_master_flat2_v0.asm", useArera, CriteriaEnum.COMBINATORIAL_ALL));
			add(new Configuration("", ASM_PHD_FOLDER + "phd_master_flat2_v1.asm", useArera, CriteriaEnum.COMBINATORIAL_ALL));
			add(new Configuration("", ASM_PHD_FOLDER + "phd_master_flat2_v2.asm", useArera, CriteriaEnum.COMBINATORIAL_ALL));
			add(new Configuration("", ASM_PHD_FOLDER + "phd_master_flat2_v3.asm", useArera, CriteriaEnum.COMBINATORIAL_ALL));
			add(new Configuration("", ASM_PHD_FOLDER + "phd_master_flat2_v4.asm", useArera, CriteriaEnum.COMBINATORIAL_ALL));
			add(new Configuration("", ASM_PHD_FOLDER + "phd_master_flat2_v5.asm", useArera, CriteriaEnum.COMBINATORIAL_ALL));
			add(new Configuration("", ASM_PHD_FOLDER + "phd_master_flat2_v6.asm", useArera, CriteriaEnum.COMBINATORIAL_ALL));
			add(new Configuration("", ASM_PHD_FOLDER + "phd_master_v7_1.asm", useArera, CriteriaEnum.COMBINATORIAL_ALL));

			add(new Configuration("", ASM_PHD_FOLDER + "phd_master_flat2_v0.asm", useArera, CriteriaEnum.COMPLETE_RULE));
			add(new Configuration("", ASM_PHD_FOLDER + "phd_master_flat2_v1.asm", useArera, CriteriaEnum.COMPLETE_RULE));
			add(new Configuration("", ASM_PHD_FOLDER + "phd_master_flat2_v2.asm", useArera, CriteriaEnum.COMPLETE_RULE));
			add(new Configuration("", ASM_PHD_FOLDER + "phd_master_flat2_v3.asm", useArera, CriteriaEnum.COMPLETE_RULE));
			add(new Configuration("", ASM_PHD_FOLDER + "phd_master_flat2_v4.asm", useArera, CriteriaEnum.COMPLETE_RULE));
			add(new Configuration("", ASM_PHD_FOLDER + "phd_master_flat2_v5.asm", useArera, CriteriaEnum.COMPLETE_RULE));
			add(new Configuration("", ASM_PHD_FOLDER + "phd_master_flat2_v6.asm", useArera, CriteriaEnum.COMPLETE_RULE));
			add(new Configuration("", ASM_PHD_FOLDER + "phd_master_v7b.asm", useArera, CriteriaEnum.COMPLETE_RULE));

			add(new Configuration("", ASM_PHD_FOLDER + "phd_master_flat2_v0.asm", useArera, CriteriaEnum.MCDC));
			add(new Configuration("", ASM_PHD_FOLDER + "phd_master_flat2_v1.asm", useArera, CriteriaEnum.MCDC));
			add(new Configuration("", ASM_PHD_FOLDER + "phd_master_flat2_v2.asm", useArera, CriteriaEnum.MCDC));
			add(new Configuration("", ASM_PHD_FOLDER + "phd_master_flat2_v3.asm", useArera, CriteriaEnum.MCDC));
			add(new Configuration("", ASM_PHD_FOLDER + "phd_master_flat2_v4.asm", useArera, CriteriaEnum.MCDC));
			add(new Configuration("", ASM_PHD_FOLDER + "phd_master_flat2_v5.asm", useArera, CriteriaEnum.MCDC));
			add(new Configuration("", ASM_PHD_FOLDER + "phd_master_flat2_v6.asm", useArera, CriteriaEnum.MCDC));
			add(new Configuration("", ASM_PHD_FOLDER + "phd_master_v7_1.asm", useArera, CriteriaEnum.MCDC));

			add(new Configuration("", ASM_PHD_FOLDER + "phd_master_flat2_v0.asm", useArera, CriteriaEnum.RULE_GUARD));
			add(new Configuration("", ASM_PHD_FOLDER + "phd_master_flat2_v1.asm", useArera, CriteriaEnum.RULE_GUARD));
			add(new Configuration("", ASM_PHD_FOLDER + "phd_master_flat2_v2.asm", useArera, CriteriaEnum.RULE_GUARD));
			add(new Configuration("", ASM_PHD_FOLDER + "phd_master_flat2_v3.asm", useArera, CriteriaEnum.RULE_GUARD));
			add(new Configuration("", ASM_PHD_FOLDER + "phd_master_flat2_v4.asm", useArera, CriteriaEnum.RULE_GUARD));
			add(new Configuration("", ASM_PHD_FOLDER + "phd_master_flat2_v5.asm", useArera, CriteriaEnum.RULE_GUARD));
			add(new Configuration("", ASM_PHD_FOLDER + "phd_master_flat2_v6.asm", useArera, CriteriaEnum.RULE_GUARD));
			add(new Configuration("", ASM_PHD_FOLDER + "phd_master_v7_1.asm", useArera, CriteriaEnum.RULE_GUARD));

			add(new Configuration("", ASM_PHD_FOLDER + "phd_master_flat2_v0.asm", useArera, CriteriaEnum.RULE_UPDATE));
			add(new Configuration("", ASM_PHD_FOLDER + "phd_master_flat2_v1.asm", useArera, CriteriaEnum.RULE_UPDATE));
			add(new Configuration("", ASM_PHD_FOLDER + "phd_master_flat2_v2.asm", useArera, CriteriaEnum.RULE_UPDATE));
			add(new Configuration("", ASM_PHD_FOLDER + "phd_master_flat2_v3.asm", useArera, CriteriaEnum.RULE_UPDATE));
			add(new Configuration("", ASM_PHD_FOLDER + "phd_master_flat2_v4.asm", useArera, CriteriaEnum.RULE_UPDATE));
			add(new Configuration("", ASM_PHD_FOLDER + "phd_master_flat2_v5.asm", useArera, CriteriaEnum.RULE_UPDATE));
			add(new Configuration("", ASM_PHD_FOLDER + "phd_master_flat2_v6.asm", useArera, CriteriaEnum.RULE_UPDATE));
			add(new Configuration("", ASM_PHD_FOLDER + "phd_master_v7b.asm", useArera, CriteriaEnum.RULE_UPDATE));
			
			add(new Configuration("", ASM_PHD_FOLDER + "phd_master_flat2_v0.asm", useArera, CriteriaEnum.BASIC_RULE, CriteriaEnum.COMBINATORIAL_ALL, CriteriaEnum.COMPLETE_RULE, CriteriaEnum.MCDC, CriteriaEnum.RULE_GUARD, CriteriaEnum.RULE_UPDATE, CriteriaEnum.THREEWISE));
			add(new Configuration("", ASM_PHD_FOLDER + "phd_master_flat2_v1.asm", useArera, CriteriaEnum.BASIC_RULE, CriteriaEnum.COMBINATORIAL_ALL, CriteriaEnum.COMPLETE_RULE, CriteriaEnum.MCDC, CriteriaEnum.RULE_GUARD, CriteriaEnum.RULE_UPDATE, CriteriaEnum.THREEWISE));
			add(new Configuration("", ASM_PHD_FOLDER + "phd_master_flat2_v2.asm", useArera, CriteriaEnum.BASIC_RULE, CriteriaEnum.COMBINATORIAL_ALL, CriteriaEnum.COMPLETE_RULE, CriteriaEnum.MCDC, CriteriaEnum.RULE_GUARD, CriteriaEnum.RULE_UPDATE, CriteriaEnum.THREEWISE));
			add(new Configuration("", ASM_PHD_FOLDER + "phd_master_flat2_v3.asm", useArera, CriteriaEnum.BASIC_RULE, CriteriaEnum.COMBINATORIAL_ALL, CriteriaEnum.COMPLETE_RULE, CriteriaEnum.MCDC, CriteriaEnum.RULE_GUARD, CriteriaEnum.RULE_UPDATE, CriteriaEnum.THREEWISE));
			add(new Configuration("", ASM_PHD_FOLDER + "phd_master_flat2_v4.asm", useArera, CriteriaEnum.BASIC_RULE, CriteriaEnum.COMBINATORIAL_ALL, CriteriaEnum.COMPLETE_RULE, CriteriaEnum.MCDC, CriteriaEnum.RULE_GUARD, CriteriaEnum.RULE_UPDATE, CriteriaEnum.THREEWISE));
			add(new Configuration("", ASM_PHD_FOLDER + "phd_master_flat2_v5.asm", useArera, CriteriaEnum.BASIC_RULE, CriteriaEnum.COMBINATORIAL_ALL, CriteriaEnum.COMPLETE_RULE, CriteriaEnum.MCDC, CriteriaEnum.RULE_GUARD, CriteriaEnum.RULE_UPDATE, CriteriaEnum.THREEWISE));
			add(new Configuration("", ASM_PHD_FOLDER + "phd_master_flat2_v6.asm", useArera, CriteriaEnum.BASIC_RULE, CriteriaEnum.COMBINATORIAL_ALL, CriteriaEnum.COMPLETE_RULE, CriteriaEnum.MCDC, CriteriaEnum.RULE_GUARD, CriteriaEnum.RULE_UPDATE, CriteriaEnum.THREEWISE));
			add(new Configuration("", ASM_PHD_FOLDER + "phd_master_v7_1.asm", useArera, CriteriaEnum.BASIC_RULE, CriteriaEnum.COMBINATORIAL_ALL, CriteriaEnum.COMPLETE_RULE, CriteriaEnum.MCDC, CriteriaEnum.RULE_GUARD, CriteriaEnum.RULE_UPDATE, CriteriaEnum.THREEWISE));
			
			add(new Configuration("", ASM_PHD_FOLDER + "phd_master_flat2_v0.asm", useArera, false, CriteriaEnum.BASIC_RULE, CriteriaEnum.COMBINATORIAL_ALL, CriteriaEnum.COMPLETE_RULE, CriteriaEnum.MCDC, CriteriaEnum.RULE_GUARD, CriteriaEnum.RULE_UPDATE, CriteriaEnum.THREEWISE));
			add(new Configuration("", ASM_PHD_FOLDER + "phd_master_flat2_v1.asm", useArera, false, CriteriaEnum.BASIC_RULE, CriteriaEnum.COMBINATORIAL_ALL, CriteriaEnum.COMPLETE_RULE, CriteriaEnum.MCDC, CriteriaEnum.RULE_GUARD, CriteriaEnum.RULE_UPDATE, CriteriaEnum.THREEWISE));
			add(new Configuration("", ASM_PHD_FOLDER + "phd_master_flat2_v2.asm", useArera, false, CriteriaEnum.BASIC_RULE, CriteriaEnum.COMBINATORIAL_ALL, CriteriaEnum.COMPLETE_RULE, CriteriaEnum.MCDC, CriteriaEnum.RULE_GUARD, CriteriaEnum.RULE_UPDATE, CriteriaEnum.THREEWISE));
			add(new Configuration("", ASM_PHD_FOLDER + "phd_master_flat2_v3.asm", useArera, false, CriteriaEnum.BASIC_RULE, CriteriaEnum.COMBINATORIAL_ALL, CriteriaEnum.COMPLETE_RULE, CriteriaEnum.MCDC, CriteriaEnum.RULE_GUARD, CriteriaEnum.RULE_UPDATE, CriteriaEnum.THREEWISE));
			add(new Configuration("", ASM_PHD_FOLDER + "phd_master_flat2_v4.asm", useArera, false, CriteriaEnum.BASIC_RULE, CriteriaEnum.COMBINATORIAL_ALL, CriteriaEnum.COMPLETE_RULE, CriteriaEnum.MCDC, CriteriaEnum.RULE_GUARD, CriteriaEnum.RULE_UPDATE, CriteriaEnum.THREEWISE));
			add(new Configuration("", ASM_PHD_FOLDER + "phd_master_flat2_v5.asm", useArera, false, CriteriaEnum.BASIC_RULE, CriteriaEnum.COMBINATORIAL_ALL, CriteriaEnum.COMPLETE_RULE, CriteriaEnum.MCDC, CriteriaEnum.RULE_GUARD, CriteriaEnum.RULE_UPDATE, CriteriaEnum.THREEWISE));
			add(new Configuration("", ASM_PHD_FOLDER + "phd_master_flat2_v6.asm", useArera, false, CriteriaEnum.BASIC_RULE, CriteriaEnum.COMBINATORIAL_ALL, CriteriaEnum.COMPLETE_RULE, CriteriaEnum.MCDC, CriteriaEnum.RULE_GUARD, CriteriaEnum.RULE_UPDATE, CriteriaEnum.THREEWISE));
			add(new Configuration("", ASM_PHD_FOLDER + "phd_master_v7b.asm", useArera, false, CriteriaEnum.BASIC_RULE, CriteriaEnum.COMBINATORIAL_ALL, CriteriaEnum.COMPLETE_RULE, CriteriaEnum.MCDC, CriteriaEnum.RULE_GUARD, CriteriaEnum.RULE_UPDATE, CriteriaEnum.THREEWISE));
			
			add(new Configuration("", ASM_PHD_FOLDER + "phd_master_flat2_v0.asm", useArera, CriteriaEnum.THREEWISE));
			add(new Configuration("", ASM_PHD_FOLDER + "phd_master_flat2_v1.asm", useArera, CriteriaEnum.THREEWISE));
			add(new Configuration("", ASM_PHD_FOLDER + "phd_master_flat2_v2.asm", useArera, CriteriaEnum.THREEWISE));
			add(new Configuration("", ASM_PHD_FOLDER + "phd_master_flat2_v3.asm", useArera, CriteriaEnum.THREEWISE));
			add(new Configuration("", ASM_PHD_FOLDER + "phd_master_flat2_v4.asm", useArera, CriteriaEnum.THREEWISE));
			add(new Configuration("", ASM_PHD_FOLDER + "phd_master_flat2_v5.asm", useArera, CriteriaEnum.THREEWISE));
			add(new Configuration("", ASM_PHD_FOLDER + "phd_master_flat2_v6.asm", useArera, CriteriaEnum.THREEWISE));
			add(new Configuration("", ASM_PHD_FOLDER + "phd_master_v7_1.asm", useArera, CriteriaEnum.THREEWISE));
			
/*			add(new Configuration("testFSM//sequences_asm_v00_correct.txt", null, useArera, false));
			add(new Configuration("testFSM//sequences_asm_v00_correct.txt", null, useArera, true));
			add(new Configuration("testFSM//sequences_asm_v0_correct.txt", null, useArera, false));
			add(new Configuration("testFSM//sequences_asm_v0_correct.txt", null, useArera, true));
			add(new Configuration("testFSM//sequences_asm_v1_correct.txt", null, useArera, false));
			add(new Configuration("testFSM//sequences_asm_v1_correct.txt", null, useArera, true));
			add(new Configuration("testFSM//sequences_asm_v2_correct.txt", null, useArera, false));
			add(new Configuration("testFSM//sequences_asm_v2_correct.txt", null, useArera, true));
			add(new Configuration("testFSM//sequences_asm_v3_correct.txt", null, useArera, false));
			add(new Configuration("testFSM//sequences_asm_v3_correct.txt", null, useArera, true));
			add(new Configuration("testFSM//sequences_asm_v4_correct.txt", null, useArera, false));
			add(new Configuration("testFSM//sequences_asm_v4_correct.txt", null, useArera, true));
			
			/*add(new Configuration("testFSM//sequences_asm_v0_correct.txt", null, useArera, false));
			add(new Configuration("testFSM//sequences_asm_v1_BasicRule_correct.txt", null, useArera, false));
			add(new Configuration("testFSM//sequences_asm_v2_BasicRule_correct.txt", null, useArera, false));
			add(new Configuration("testFSM//sequences_asm_v4.txt", null, useArera, false));
			
			add(new Configuration("testFSM//sequences_asm_v0_BasicRule.txt", "testFSM//phd_master_flat2_v0.asm", useArera, false));
			add(new Configuration("testFSM//sequences_asm_v1_BasicRule.txt", "testFSM//phd_master_flat2.asm", useArera, false));
			add(new Configuration("testFSM//sequences_asm_v2_BasicRule.txt", "testFSM//phd_master_flat2_v2.asm", useArera, true));
			add(new Configuration("testFSM//sequences_asm_v3_BasicRule.txt", "testFSM//phd_master_flat2_v3.asm", useArera, true));
			add(new Configuration("testFSM//sequences_asm_v0_correct.txt", "testFSM//sequences_asm_v0_correct.asm", useArera, false));*/
		}
	};	
	
	@Test
	public void testComputeCoverage() throws Exception {
		// XXX select configuration to execute (only one at a time!!)
		Logger.getLogger(StdPairwiseCovBuild.class).setLevel(Level.ALL);

		testComputeCoverage(filesToProcess.get(0));
	}
	
	@Test
	public void testComputeTestPredicates() throws Exception {
		
		FileWriter file = new FileWriter(new File("logFile.txt"));
		
		for (Configuration c : filesToProcess) {
			Logger.getLogger(NuSMVtestGenerator.class).setLevel(Level.OFF);
			Logger.getLogger(AsmTestGenerator.class).setLevel(Level.OFF);
			Logger.getLogger(AsmTestSeqContent.class).setLevel(Level.OFF);
			Logger.getLogger(StdPairwiseCovBuild.class).setLevel(Level.OFF);
			List<AsmCoverageBuilder> coverageCriteria = CriteriaEnum.getCoverageCriteria(c.criteria);
			NuSMVtestGenerator nuSMVtestGenerator = new NuSMVtestGenerator(c.asmPath, true, coverageCriteria);
			TestGenerationWithNuSMV.useLTLandBMC = true;
			AsmTestSuite result = nuSMVtestGenerator.generateAbstractTests(Integer.MAX_VALUE, "abcdef.*");
			/*
			file.write(c.asmPath + "_"
					+ c.criteria.stream().map(n -> n.getAbbrvName()).collect(Collectors.joining("_")) + "\n");
			file.write("TPS: "  + nuSMVtestGenerator.getTPSNumber() + "\n");
			*/
		}
		
		file.close();
	}

	@Test
	public void testComputeCoverageOnlyRunProTest() throws Exception {
		// XXX select configuration to execute (only one at a time!!)
		/*computeCoverage(new File("testFSM//sequences_asm_v0_correct.txt").getAbsolutePath(), useArera);
		computeCoverage(new File("testFSM//sequences_asm_v1_correct.txt").getAbsolutePath(), useArera);
		computeCoverage(new File("testFSM//sequences_asm_v2_correct.txt").getAbsolutePath(), useArera);*/
		computeCoverage(new File(ASM_PHD_FOLDER + "fsm//phd_master_flat2_v5_BR_2WISE_CR_MCDC_RG_UR0_Merged.txt").getAbsolutePath(), useArera);
	}
	
	public void testComputeCoverage(Configuration c) throws Exception {
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
			GenerateTestsFromFSM generator = new GenerateTestsFromFSM();
			outputFileNames = generator.saveFSMWithAvallaTests(modelName, c.asmPath, c.useMonitoring, c.criteria);	
			// Geneate an unique FSM 			
			outputFileName = FSMCreator.createFSMFromMultipleAvalla(outputFileNames, c.useMonitoring);
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
		
		// Open the FSM file
		String path = new File(outputFileName).getAbsolutePath();
		computeCoverage(path, c.useArera);
	}
	
	@Test
	public void showStats() {
		
		String seqsFile = "F:\\Dati-Andrea\\GitHub\\ProTestUTA\\trafficlightcontroller\\ASM\\TrafficLight2\\scenario21_Merged.txt";

		showStatsCoverage(seqsFile);
		
	}
	
	public static void showStatsCoverage(String fileName) {
		initializeGUI();
		
		// open file
		if (MainWin.openFile(fileName)) {
            MainWin.disposeAllMSC();
            MainWin.disposeStatistic();
            MainWin.disposeCodeCoverage();
            MainWin.resetFSMViewer();
            MainWin.showSequencesInTree();
            MainWin.showStatistic();
        }
		
		int maxLen, minLen, totLen;
		float avgLen;
		maxLen = minLen = totLen = MainWin.sequences.get(0).length;
		for (int i = 1; i < MainWin.sequences.size(); i++) {
			int curLen = MainWin.sequences.get(i).length;
			maxLen = Math.max(maxLen, curLen);
			minLen = Math.min(minLen, curLen);
			totLen = totLen + curLen;
		}
		avgLen = (float) totLen / MainWin.sequences.size();
		
		System.out.println(MainWin.sequences.size() + "\t" + minLen + "\t" + maxLen + "\t" + totLen + "\t" + avgLen);
		
		display.dispose();
	}
	
	public static void computeCoverage(String fileName, boolean useArera) {
		initializeGUI();
		
		// open file
		if (MainWin.openFile(fileName)) {
            MainWin.disposeAllMSC();
            MainWin.disposeStatistic();
            MainWin.disposeCodeCoverage();
            MainWin.resetFSMViewer();
            MainWin.showSequencesInTree();
            MainWin.showStatistic();
        }
		
		// configure manager
		MainWin.serverIP = useArera ? "193.204.253.35" : "193.204.253.59";
		MainWin.sshPort = 22;
		MainWin.sshUser = useArera ? "bombarda" : "marcoradavelli";
		MainWin.sshPassword = useArera ? "andrea" : "Gargantini01";
		MainWin.sshDirectory = useArera ? "~/health/antidote-2.1.0/" : "~/health/antidote-2.0.0/";
		MainWin.managerName = "ieee_manager";
		MainWin.managerPort = 6024;
		MainWin.sshTimeout = 10000;
		MainWin.codeCoverageShellScript = "cov.sh";
		MainWin.codeCoverageReportLocation = "http://"+MainWin.serverIP+"/~"+MainWin.sshUser+"/cov/";
		MainWin.prvkey = "";
		MainWin.passphrase = "";
		MainWin.killMgrCommand = "pkill " + MainWin.managerName;
		MainWin.checkMgrCommand = "ps -ef | grep " + MainWin.managerName + " | grep -v grep";
		MainWin.startMgrCommand = useArera ? "src/../../../../../usr/local/bin/" + MainWin.managerName + " --tcp" : "src/" + MainWin.managerName + " --tcp";
		MainWin.sshDirectoryCdCommand = "cd " + MainWin.sshDirectory;
		MainWin.codeCoverageClearCommand = "bash " + MainWin.codeCoverageShellScript + " " + "clear";
		MainWin.configurationFlag = true;
		MainWin.changeItemsEnabled();
		
		// start manager
		MainWin.managerStartProcess();
        MainWin.changeItemsEnabled();
        
        // start testing
        MainWin.disposeAllMSC();
        startTestingProcessWithoutAskingForCoverageClear(0, MainWin.sequences.size());
        MainWin.changeItemsEnabled();
        
        while (!MainWin.shell.isDisposed()) {
        	if (display.readAndDispatch()) continue;
        	display.sleep();
        }
        display.dispose();
        
        // show code coverage (the user does it when test is finishedI)
        //MainWin.showCodeCoverage();
        //MainWin.changeItemsEnabled();
	}
	
	static Display display;
	
	public static void initializeGUI() {
		display = new Display();
        MainWin.shell = new Shell(display);
        MainWin.shell.setText(MainWin.title);
        MainWin.shell.setSize(900, 690);
        GridLayout layout = new GridLayout();
        layout.numColumns = 4;
        layout.makeColumnsEqualWidth = true;
        MainWin.shell.setLayout(layout);
        MainWin.shell.addListener(21, new org.eclipse.swt.widgets.Listener(){

            @Override
            public void handleEvent(Event e) {
                MessageBox messageBox = new MessageBox(MainWin.shell, 65732);
                messageBox.setText("Exit");
                messageBox.setMessage("Exit the program?");
                if (messageBox.open() == 64) {
                    e.doit = true;
                    MainWin.exitProgram();
                } else {
                    e.doit = false;
                }
            }
        });
        MainWin.initializeImages();
        MainWin.initializeMenu();
        MainWin.initializeToolBar();
        MainWin.initializeTree();
        MainWin.initializeGraphViewer();
        MainWin.initializeText();
        MainWin.printStream = new PrintStream(new OutputStream(){

            @Override
            public void write(int b) throws IOException {
                final String toAppend = String.valueOf((char)b);
                Display.getDefault().asyncExec(new Runnable(){

                    @Override
                    public void run() {
                        if (MainWin.serverText.isDisposed()) {
                            return;
                        }
                        MainWin.serverText.append(toAppend);
                    }
                });
            }

        });
        MainWin.changeItemsEnabled();
        MainWin.shell.setImage(MainWin.shellImage);
        MainWin.shell.open();
        MainWin.shell.setLocation(Display.getCurrent().getClientArea().width / 2 - MainWin.shell.getShell().getSize().x / 2, Display.getCurrent().getClientArea().height / 2 - MainWin.shell.getSize().y / 2);
        
	}
	
	public static void startTestingProcessWithoutAskingForCoverageClear(int startPoint, int size) {
		// clear code coverage
		MainWin.covClearedFlag = false;
        CovClearProgressBar covClearProgressBar = new CovClearProgressBar(MainWin.shell);
        covClearProgressBar.initGuage();
        covClearProgressBar.open();
        MainWin.covDataExistFlag = false;
        
        // start testing
        int i = 0;
        while (i < MainWin.sequences.size()) {
            MainWin.completeFlags[i] = false;
         //   MainWin.rootTreeItem.getItem(i).setImage(MainWin.level1TreeItemImage);
            MainWin.disposeAllMSC();
            MainWin.disposeCodeCoverage();
            ++i;
        }
        
        if (MainWin.threadStatus == 1) {
            MainWin.pauseTestingThread();
        }
        MainWin.threadStatus = 0;
        if (!(MainWin.networkFlag && MainWin.managerRunningFlag || MainWin.managerCheckProcess())) {
            return;
        }
        MainWin.resumePoint = startPoint;
        MainWin.runTestingProcess(MainWin.resumePoint, size);
    }
}
