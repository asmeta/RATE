package main;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Scanner;

import org.apache.commons.io.FilenameUtils;
import org.asmeta.atgt.generator.CriteriaEnum;

import com.jcraft.jsch.JSchException;
import com.jcraft.jsch.Session;
import com.jcraft.jsch.UserInfo;

import gui.TestWithoutGUIInteraction;
import service.FSMCreator;
import util.MyUserInfo;
import util.SCPManager;
import util.SSHManager;

public class TestExecutor {

	// List of criteria for ALL Criteria
	static CriteriaEnum[] allCriteria = {CriteriaEnum.BASIC_RULE, CriteriaEnum.COMBINATORIAL_ALL,
			CriteriaEnum.COMPLETE_RULE, CriteriaEnum.MCDC, CriteriaEnum.RULE_GUARD, CriteriaEnum.RULE_UPDATE, CriteriaEnum.THREEWISE_ALL};
	
	static String testPath = "../abstract_tests/PHD_Protocol_";
	static String testPathManual= "../ASM/PHD_Protocol_";
	static String level = "NR";
	static Boolean useManualScenarios = false;

	// Connection data
	static String userName = "bombarda";
	static String password = "andrea";
	static String connectionIP = "arera.unibg.it";

	// Fsm files
	static String fsmPath = ".\\FSM\\manager_fsm.c";
	static String outFsmPath = ".\\FSM\\manager_fsm_muted.c";

	static String mutations[][] = { 
			{"{fsm_state_operating,		fsm_evt_req_assoc_rel,						fsm_state_disassociating,	&operating_assoc_release_req_tx},",
			 "{fsm_state_operating,		fsm_evt_req_assoc_rel,						fsm_state_disassociating,	NULL}," },
			{"{fsm_state_checking_config,	fsm_evt_ind_transport_disconnect,				fsm_state_disconnected,		&communication_disconnect_tx},",
			 "{fsm_state_checking_config,	fsm_evt_ind_transport_disconnect,				fsm_state_disconnected,		NULL}," },
			{"{fsm_state_disassociating,	fsm_evt_rx_rlre,						fsm_state_unassociated,		&disassociating_release_proccess_completed},",
			 "{fsm_state_disassociating,	fsm_evt_rx_rlre,						fsm_state_unassociated,		NULL}," }, 
			{"{fsm_state_unassociated,	fsm_evt_rx_aare,						fsm_state_unassociated,		&communication_abort_undefined_reason_tx},",
			 "{fsm_state_unassociated,	fsm_evt_rx_aare,						fsm_state_unassociated,		NULL}," },
			{"{fsm_state_unassociated,	fsm_evt_rx_abrt,						fsm_state_unassociated,		NULL},",
			 "{fsm_state_unassociated,	fsm_evt_rx_abrt,						fsm_state_waiting_for_config,		NULL}," },
			{"{fsm_state_waiting_for_config,	fsm_evt_rx_rlrq,						fsm_state_unassociated,		&disassociating_release_response_tx},",
			 "{fsm_state_waiting_for_config,	fsm_evt_rx_rlrq,						fsm_state_unassociated,		&communication_abort_undefined_reason_tx}," }, 
			{"{fsm_state_disconnected,	fsm_evt_ind_transport_connection,				fsm_state_unassociated,		NULL},",
			 "{fsm_state_disconnected,	fsm_evt_ind_transport_connection,				fsm_state_unassociated,		&communication_disconnect_tx}," },
			{"{fsm_state_unassociated,	fsm_evt_rx_rlre,						fsm_state_unassociated,		NULL},",
			 "{fsm_state_unassociated,	fsm_evt_rx_rlre,						fsm_state_unassociated,		&disassociating_release_response_tx}," },
			{"{fsm_state_unassociated,	fsm_evt_rx_aarq_acceptable_and_unknown_configuration,		fsm_state_waiting_for_config,	&configuring_transition_waiting_for_config},",
			 "{fsm_state_unassociated,	fsm_evt_rx_aarq_acceptable_and_unknown_configuration,		fsm_state_waiting_for_config,	&association_accept_config_tx}," }, 
			{"{fsm_state_waiting_for_config,	fsm_evt_rx_abrt,						fsm_state_unassociated,		NULL},",
			 "{fsm_state_waiting_for_config,	fsm_evt_rx_abrt,						fsm_state_unassociated,		&communication_abort_undefined_reason_tx}," },
			{"{fsm_state_checking_config,	fsm_evt_rx_aare,						fsm_state_unassociated,		&communication_abort_undefined_reason_tx},",
			 "{fsm_state_checking_config,	fsm_evt_rx_aare,						fsm_state_waiting_for_config,		&communication_abort_undefined_reason_tx}," },
			{"{fsm_state_operating,		fsm_evt_rx_rlre,						fsm_state_unassociated,		&communication_abort_undefined_reason_tx},",
			 "{fsm_state_operating,		fsm_evt_rx_rlre,						fsm_state_unassociated,		NULL}," },
			{"{fsm_state_disassociating,	fsm_evt_rx_rorj,						fsm_state_unassociated,		&communication_abort_undefined_reason_tx},",
			 "{fsm_state_disassociating,	fsm_evt_rx_rorj,						fsm_state_unassociated,		NULL}," },
			{"{fsm_state_checking_config,	fsm_evt_rx_rorj,						fsm_state_checking_config,	NULL},",
			 "{fsm_state_checking_config,	fsm_evt_rx_rorj,						fsm_state_checking_config,	&disassociating_release_proccess_completed}," },
			{"{fsm_state_waiting_for_config,	fsm_evt_rx_rors,						fsm_state_waiting_for_config,	&communication_check_invoke_id_abort_tx},",
			 "{fsm_state_waiting_for_config,	fsm_evt_rx_rors,						fsm_state_waiting_for_config,	&communication_roer_tx}," } };

	/**
	 * Executes the commands used to compile the PHD protocol on arera server
	 */
	public static void compilePHD() {
		String command = "cd health; cd antidote-2.1.0; echo " + password + " | sudo -S ./autoMakeAndReinstall.sh";
		SSHManager instance = new SSHManager(userName, password, connectionIP, "");
		String errorMessage = instance.connect();

		if (errorMessage != null) {
			System.out.println(errorMessage);
			throw new RuntimeException("Connection refused");
		}

		String result = instance.sendCommand(command);
		System.out.println(result);

		instance.close();
	}

	/**
	 * Reads the FSM file (original file) to be modified.
	 * 
	 * @return the string representing the file lines containing the definition of
	 *         the FSM
	 * @throws IOException
	 */
	public static String readFSM() throws IOException {
		File f = new File(fsmPath);
		Boolean save = false;
		String line = "";
		String fsm = "";
		assert f.exists();
		Scanner myReader = new Scanner(f);
		while (myReader.hasNextLine()) {
			line = myReader.nextLine().trim();

			// The labels "stop here" and "start here" are used to delimit the lines of the
			// file in which the fsm is described
			if (line.equals("// STOP HERE"))
				save = false;

			if (save)
				fsm += "\n" + line;

			if (line.equals("// START HERE"))
				save = true;
		}

		myReader.close();

		return fsm;
	}

	/**
	 * Applies the mutations required
	 * 
	 * @param fsm the fsm to be muted
	 * @return the muted fst
	 */
	public static String applyMutations(String fsm) {
		for (String mutant[] : mutations) {
			fsm = fsm.replace(mutant[0], mutant[1]);
		}
		return fsm;
	}
	
	/**
	 * Applies the mutation required
	 * 
	 * @param fsm the fsm to be muted
	 * @param i the index of the mutation to be applied
	 * @return the muted fsm
	 */
	public static String applyMutations(String fsm, int i) {
		fsm = fsm.replace(mutations[i][0], mutations[i][1]);
		return fsm;
	}

	/**
	 * Save the new muted file and load it to arera server
	 * 
	 * @param fsm the muted fsm
	 * @throws IOException
	 * @throws JSchException
	 */
	public static void loadFile(String fsm) throws IOException, JSchException {
		// Save the muted file
		saveFile(fsm);

		// load the file
		SCPManager mgr = new SCPManager();
		File f = new File(outFsmPath);
		File f1 = new File(fsmPath);
		Session s = mgr.createSession(userName, connectionIP, 22, password);

		UserInfo ui = new MyUserInfo();
		s.setUserInfo(ui);
		s.connect();

		mgr.copyLocalToRemote(s, f.getAbsolutePath(), "/home/bombarda/health/antidote-2.1.0/src/communication/manager/",
				FilenameUtils.getName(f1.getAbsolutePath()));
	}

	/**
	 * Save the new muted file
	 * 
	 * @param fsm the muted fsm
	 * @throws IOException
	 */
	private static void saveFile(String fsm) throws FileNotFoundException, IOException {
		File f = new File(fsmPath);
		File fOut = new File(outFsmPath);
		Boolean started = false;
		Boolean finished = false;
		String line = "";
		assert f.exists();
		Scanner myReader = new Scanner(f);
		BufferedWriter fWriterOut = new BufferedWriter(new FileWriter(fOut));
		while (myReader.hasNextLine()) {
			line = myReader.nextLine().trim();

			// The labels "stop here" and "start here" are used to delimit the lines of the
			// file in which the fsm is described
			if (line.equals("// STOP HERE"))
				finished = true;

			if ((started && finished) || (!started && !finished))
				fWriterOut.append("\n" + line);

			if (line.equals("// START HERE")) {
				started = true;
				fWriterOut.append("\n" + fsm);
			}
		}

		myReader.close();
		fWriterOut.close();
	}
	
	
	static int computeCoverage(String fsmPath) throws Exception {	
		// Open the FSM file with ProTest and run the tests
		fsmPath = new File(fsmPath).getAbsolutePath();
		return TestWithoutGUIInteraction.computeCoverage(fsmPath, true);
	}

	private static String buidTestCases() throws Exception {
		String fsmPath = "";
		String currentTestPath = "";
		
		if (!useManualScenarios) {
			currentTestPath = testPath + level + "/";
		} else {
			currentTestPath = testPathManual + level + "/scenarios/"; 
		}
		
		// Build the FSM from multiple Avallas
		File folder = new File(currentTestPath);
		File[] listOfFiles = folder.listFiles();
		ArrayList<String> avallaFiles = new ArrayList<String>();
		for (File f:listOfFiles) {
			if (!useManualScenarios) {
				for (CriteriaEnum ce : allCriteria) {
					if (f.getName().startsWith("test" + ce.getAbbrvName())) {
						avallaFiles.add(f.getAbsolutePath());
						break;
					}
				}
			} else {
				avallaFiles.add(f.getAbsolutePath());
			}
		}
		String[] avallaFilesList = new String[avallaFiles.size()]; 
		avallaFilesList = avallaFiles.toArray(avallaFilesList);
		fsmPath = FSMCreator.createFSMFromMultipleAvalla(avallaFilesList, true);
		return fsmPath;
	}

	public static void main(String[] args) throws Exception {
		int testFailures = 0;
		String fsmPath = "";
		fsmPath = buidTestCases();
		
		int[] toCheck = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 14};
		
		for (int i : toCheck) {
			// Read the FSM from the original file
			String fsm = readFSM();

			// Apply the mutations
			fsm = applyMutations(fsm, i);

			// Save the new file and load it on arera server
			loadFile(fsm);

			// Compile the new version of the PHD protocol
			compilePHD();
				
			// Open protest in order to perform tests loading the all rule file, connect it
			// to arera server and launch tests 
			testFailures = computeCoverage(fsmPath);
			System.out.println("Mutation " + (i+1) + " - Failed?: " + testFailures);
		}
	}
}
