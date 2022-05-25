package gui;

import java.io.IOException;
import java.io.OutputStream;
import java.io.PrintStream;
import org.eclipse.swt.widgets.Display;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Event;
import org.eclipse.swt.widgets.MessageBox;
import org.eclipse.swt.widgets.Shell;

public class TestWithoutGUIInteraction {

	static String ASM_PHD_FOLDER = "F:\\Dati-Andrea\\GitHub\\ProTestUTA\\ASM\\newPHD\\";

	public static int computeCoverage(String fileName, boolean useArera) {
		int failed = 0;
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
			if (display.readAndDispatch())
				continue;
			display.sleep();
		}

		// record the number of failed tests
		failed = MainWin.undoneSeqsNum;

		display.dispose();
		return failed;
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
