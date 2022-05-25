package gui;

import java.io.File;
import java.io.IOException;
import java.io.OutputStream;
import java.io.PrintStream;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import org.apache.log4j.Level;
import org.apache.log4j.Logger;
import org.asmeta.atgt.generator.CriteriaEnum;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Display;
import org.eclipse.swt.widgets.Event;
import org.eclipse.swt.widgets.MessageBox;
import org.eclipse.swt.widgets.Shell;
import org.junit.Test;

import extgt.coverage.combinatorial.StdPairwiseCovBuild;
import service.FSMCreator;

public class TestWithoutGUIInteraction {

	/** Is the PHD Manager running on the "arera" server? */
	public static boolean useArera = true;

	class Configuration {
		boolean useArera;
		List<CriteriaEnum> criteria;
		boolean useMonitoring;
		String level;
		
		public Configuration(boolean useArera, List<CriteriaEnum> criteria, String level) {
			this.useArera = useArera;
			this.criteria = criteria;
			this.useMonitoring = true;
			this.level = level;
		}
		
		public Configuration(boolean useArera, boolean useMonitoring, List<CriteriaEnum> criteria, String level) {
			this.useArera = useArera;
			this.criteria = criteria;
			this.useMonitoring = useMonitoring;
			this.level = level;
		}
		
		public Configuration(boolean useArera, String level, CriteriaEnum ... criteria) {
			this(useArera,Arrays.asList(criteria), level);
		}
		
		public Configuration(boolean useArera, boolean useMonitoring, String level, CriteriaEnum ... criteria) {
			this(useArera,useMonitoring,Arrays.asList(criteria), level);
		}
	}

	// The path of abstract tests
	private static String PATH_AT = "../abstract_tests/PHD_Protocol_";

	List<Configuration> filesToProcess = new ArrayList<Configuration>() {
		
		private static final long serialVersionUID = 5014712752302397500L;
		{			
			add(new Configuration(useArera, "0", CriteriaEnum.BASIC_RULE));
			add(new Configuration(useArera, "1", CriteriaEnum.BASIC_RULE));
			add(new Configuration(useArera, "2", CriteriaEnum.BASIC_RULE));
			add(new Configuration(useArera, "3", CriteriaEnum.BASIC_RULE));
			add(new Configuration(useArera, "4", CriteriaEnum.BASIC_RULE));
			add(new Configuration(useArera, "5", CriteriaEnum.BASIC_RULE));
			add(new Configuration(useArera, "6", CriteriaEnum.BASIC_RULE));
			add(new Configuration(useArera, "NR", CriteriaEnum.BASIC_RULE));
			
			add(new Configuration(useArera, "0", CriteriaEnum.COMBINATORIAL_ALL));
			add(new Configuration(useArera, "1", CriteriaEnum.COMBINATORIAL_ALL));
			add(new Configuration(useArera, "2", CriteriaEnum.COMBINATORIAL_ALL));
			add(new Configuration(useArera, "3", CriteriaEnum.COMBINATORIAL_ALL));
			add(new Configuration(useArera, "4", CriteriaEnum.COMBINATORIAL_ALL));
			add(new Configuration(useArera, "5", CriteriaEnum.COMBINATORIAL_ALL));
			add(new Configuration(useArera, "6", CriteriaEnum.COMBINATORIAL_ALL));
			add(new Configuration(useArera, "NR", CriteriaEnum.COMBINATORIAL_ALL));
			
			add(new Configuration(useArera, "0", CriteriaEnum.COMPLETE_RULE));
			add(new Configuration(useArera, "1", CriteriaEnum.COMPLETE_RULE));
			add(new Configuration(useArera, "2", CriteriaEnum.COMPLETE_RULE));
			add(new Configuration(useArera, "3", CriteriaEnum.COMPLETE_RULE));
			add(new Configuration(useArera, "4", CriteriaEnum.COMPLETE_RULE));
			add(new Configuration(useArera, "5", CriteriaEnum.COMPLETE_RULE));
			add(new Configuration(useArera, "6", CriteriaEnum.COMPLETE_RULE));
			add(new Configuration(useArera, "NR", CriteriaEnum.COMPLETE_RULE));
			
			add(new Configuration(useArera, "0", CriteriaEnum.MCDC));
			add(new Configuration(useArera, "1", CriteriaEnum.MCDC));
			add(new Configuration(useArera, "2", CriteriaEnum.MCDC));
			add(new Configuration(useArera, "3", CriteriaEnum.MCDC));
			add(new Configuration(useArera, "4", CriteriaEnum.MCDC));
			add(new Configuration(useArera, "5", CriteriaEnum.MCDC));
			add(new Configuration(useArera, "6", CriteriaEnum.MCDC));
			add(new Configuration(useArera, "NR", CriteriaEnum.MCDC));
			
			add(new Configuration(useArera, "0", CriteriaEnum.RULE_GUARD));
			add(new Configuration(useArera, "1", CriteriaEnum.RULE_GUARD));
			add(new Configuration(useArera, "2", CriteriaEnum.RULE_GUARD));
			add(new Configuration(useArera, "3", CriteriaEnum.RULE_GUARD));
			add(new Configuration(useArera, "4", CriteriaEnum.RULE_GUARD));
			add(new Configuration(useArera, "5", CriteriaEnum.RULE_GUARD));
			add(new Configuration(useArera, "6", CriteriaEnum.RULE_GUARD));
			add(new Configuration(useArera, "NR", CriteriaEnum.RULE_GUARD));
			
			add(new Configuration(useArera, "0", CriteriaEnum.RULE_UPDATE));
			add(new Configuration(useArera, "1", CriteriaEnum.RULE_UPDATE));
			add(new Configuration(useArera, "2", CriteriaEnum.RULE_UPDATE));
			add(new Configuration(useArera, "3", CriteriaEnum.RULE_UPDATE));
			add(new Configuration(useArera, "4", CriteriaEnum.RULE_UPDATE));
			add(new Configuration(useArera, "5", CriteriaEnum.RULE_UPDATE));
			add(new Configuration(useArera, "6", CriteriaEnum.RULE_UPDATE));
			add(new Configuration(useArera, "NR", CriteriaEnum.RULE_UPDATE));
			
			add(new Configuration(useArera, "0", CriteriaEnum.THREEWISE));
			add(new Configuration(useArera, "1", CriteriaEnum.THREEWISE));
			add(new Configuration(useArera, "2", CriteriaEnum.THREEWISE));
			add(new Configuration(useArera, "3", CriteriaEnum.THREEWISE));
			add(new Configuration(useArera, "4", CriteriaEnum.THREEWISE));
			add(new Configuration(useArera, "5", CriteriaEnum.THREEWISE));
			add(new Configuration(useArera, "6", CriteriaEnum.THREEWISE));
			add(new Configuration(useArera, "NR", CriteriaEnum.THREEWISE));
			
			add(new Configuration(useArera, "0", CriteriaEnum.BASIC_RULE, CriteriaEnum.COMBINATORIAL_ALL, CriteriaEnum.COMPLETE_RULE, CriteriaEnum.MCDC, CriteriaEnum.RULE_GUARD, CriteriaEnum.RULE_UPDATE, CriteriaEnum.THREEWISE));
			add(new Configuration(useArera, "1", CriteriaEnum.BASIC_RULE, CriteriaEnum.COMBINATORIAL_ALL, CriteriaEnum.COMPLETE_RULE, CriteriaEnum.MCDC, CriteriaEnum.RULE_GUARD, CriteriaEnum.RULE_UPDATE, CriteriaEnum.THREEWISE));
			add(new Configuration(useArera, "2", CriteriaEnum.BASIC_RULE, CriteriaEnum.COMBINATORIAL_ALL, CriteriaEnum.COMPLETE_RULE, CriteriaEnum.MCDC, CriteriaEnum.RULE_GUARD, CriteriaEnum.RULE_UPDATE, CriteriaEnum.THREEWISE));
			add(new Configuration(useArera, "3", CriteriaEnum.BASIC_RULE, CriteriaEnum.COMBINATORIAL_ALL, CriteriaEnum.COMPLETE_RULE, CriteriaEnum.MCDC, CriteriaEnum.RULE_GUARD, CriteriaEnum.RULE_UPDATE, CriteriaEnum.THREEWISE));
			add(new Configuration(useArera, "4", CriteriaEnum.BASIC_RULE, CriteriaEnum.COMBINATORIAL_ALL, CriteriaEnum.COMPLETE_RULE, CriteriaEnum.MCDC, CriteriaEnum.RULE_GUARD, CriteriaEnum.RULE_UPDATE, CriteriaEnum.THREEWISE));
			add(new Configuration(useArera, "5", CriteriaEnum.BASIC_RULE, CriteriaEnum.COMBINATORIAL_ALL, CriteriaEnum.COMPLETE_RULE, CriteriaEnum.MCDC, CriteriaEnum.RULE_GUARD, CriteriaEnum.RULE_UPDATE, CriteriaEnum.THREEWISE));
			add(new Configuration(useArera, "6", CriteriaEnum.BASIC_RULE, CriteriaEnum.COMBINATORIAL_ALL, CriteriaEnum.COMPLETE_RULE, CriteriaEnum.MCDC, CriteriaEnum.RULE_GUARD, CriteriaEnum.RULE_UPDATE, CriteriaEnum.THREEWISE));
			add(new Configuration(useArera, "NR", CriteriaEnum.BASIC_RULE, CriteriaEnum.COMBINATORIAL_ALL, CriteriaEnum.COMPLETE_RULE, CriteriaEnum.MCDC, CriteriaEnum.RULE_GUARD, CriteriaEnum.RULE_UPDATE, CriteriaEnum.THREEWISE));
		}
	};	
	
	@Test
	public void testComputeCoverage() throws Exception {
		// XXX select configuration to execute (only one at a time!!)
		Logger.getLogger(StdPairwiseCovBuild.class).setLevel(Level.ALL);
		Configuration c = filesToProcess.get(0);
		String path = PATH_AT + c.level + "/";		
		testComputeCoverage(c, path);
	}
	
	@Test
	public void testAllComputeCoverage() throws Exception {
		Logger.getLogger(StdPairwiseCovBuild.class).setLevel(Level.ALL);
		for (Configuration c : filesToProcess) {
			String path = PATH_AT + c.level + "/";		
			testComputeCoverage(c, path);
		}
	}
	
	@Test
	public void testAllLevel1() throws Exception {
		testAllLevelX("1");
	}
	
	@Test
	public void testAllLevel2() throws Exception {
		testAllLevelX("2");
	}
	
	@Test
	public void testAllLevel3() throws Exception {
		testAllLevelX("3");
	}
	
	@Test
	public void testAllLevel4() throws Exception {
		testAllLevelX("4");
	}
	
	@Test
	public void testAllLevel5() throws Exception {
		testAllLevelX("5");
	}
	
	@Test
	public void testAllLevel6() throws Exception {
		testAllLevelX("6");
	}
	
	@Test
	public void testAllLevelNR() throws Exception {
		testAllLevelX("NR");
	}
	
	public void testAllLevelX(String level) throws Exception {
		Logger.getLogger(StdPairwiseCovBuild.class).setLevel(Level.ALL);
		for (Configuration c : filesToProcess) {
			if (c.level.equals(level)) {
				String path = PATH_AT + c.level + "/";
				
				// Execute tests
				testComputeCoverage(c, path);
			}
		}
	}
	
	public void testComputeCoverage(Configuration c, String path) throws Exception {
		String fsmPath = "";
		
		// Build the FSM from multiple Avallas
		File folder = new File(path);
		File[] listOfFiles = folder.listFiles();
		ArrayList<String> avallaFiles = new ArrayList<String>();
		for (File f:listOfFiles) {
			for (CriteriaEnum ce : c.criteria) {
				if (f.getName().startsWith("test" + ce.getAbbrvName())) {
					avallaFiles.add(f.getAbsolutePath());
					break;
				}
			}
			
		}
		String[] avallaFilesList = new String[avallaFiles.size()]; 
		avallaFilesList = avallaFiles.toArray(avallaFilesList);
		fsmPath = FSMCreator.createFSMFromMultipleAvalla(avallaFilesList, true);
		
		// Open the FSM file with ProTest and run the tests
		fsmPath = new File(fsmPath).getAbsolutePath();
		computeCoverage(fsmPath, c.useArera);
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
//        MainWin.showCodeCoverage();
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
