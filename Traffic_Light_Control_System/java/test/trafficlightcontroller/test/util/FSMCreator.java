package trafficlightcontroller.test.util;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.HashSet;
import java.util.stream.Collectors;

import org.hamcrest.Condition.Step;

/**
 * Class that creates the FSM starting from an Avalla Scenario File
 * 
 * 
 * @author Andrea Bombarda
 */

public class FSMCreator {

	public static String createFSMFromAvalla(String avallaPath, int nMessages, int nTransitions, int nStatuses) throws Exception {
		String fileContent = "";
		// Check if the Avalla file actually exists and create the name of the resulting
		// file
		assert new File(avallaPath).exists();
		String fsmPath = new StringBuilder(
				new StringBuilder(new File(avallaPath).getAbsolutePath()).reverse().toString().split("\\.", 2)[1])
						.reverse().toString()
				+ ".txt";
		// Compute the content of the file
		fileContent = getContentFSMFromAvalla(avallaPath, nMessages, nTransitions, nStatuses);
		// Write the file
		PrintWriter fout = new PrintWriter(new FileWriter(fsmPath));
		fout.write(fileContent);
		fout.close();
		return fsmPath;
	} 

	private static String getContentFSMFromAvalla(String avallaPath, int nMessages, int nTransitions, int nStatuses) throws Exception {
		String content = "Finite State Machine :\n";
		HashSet<FSMEntry> entries = new HashSet<FSMEntry>();
		BufferedReader fin = new BufferedReader(new FileReader(avallaPath));
		int nMessagesTest = 0, nTransitionsTest = 0, nStatusesFromTest = 0, nStatusesToTest = 0;
		int nStep = 0;
		
		// Temp messages
		String lightsA = "", lightsB = "";
		String transitionA = "", transitionB = "", transitionC = "";
		String statusA_pre = "", statusB_pre = "", statusC_pre = "", statusCOperate_pre = "";
		String statusA_post = "", statusB_post = "", statusC_post = "", statusCOperate_post = "";
		
		String line = "";
		String msgsList = "";
		String current_state = "";
		String next_state = "";
		String receivedMessage = "";
		String sentMessage = "";

		// Read the Avalla file
		while ((line = fin.readLine()) != null) {
			if (line.startsWith("scenario"))
				continue;
			if (line.startsWith("//"))
				continue;
			if (line.startsWith("load"))
				continue;
			if (line.startsWith("step")) {
				if (nStep == 0)
					nMessagesTest = 0;
				nStep ++;
				continue;
			}
			if (line.equals(""))
				continue;
			if (line.startsWith("check") && !line.endsWith("undef;")) {
				String splittedLine[] = line.split(" ");
				switch (splittedLine[1]) {
				case "lightsA":
					// Message
					if (nStep != 0) {
						lightsA = splittedLine[3].toLowerCase().replace(";", "");
						nMessagesTest++;
					}
					break;
				case "lightsB":
					// Message
					if (nStep != 0) {
						lightsB = splittedLine[3].toLowerCase().replace(";", "");
						nMessagesTest++;
					}
					break;
				case "statusA":
					// Status
					if (statusA_pre.equals("")) {
						statusA_pre = splittedLine[3].toLowerCase().replace(";", "");
						nStatusesFromTest++;
					} else {
						statusA_post = splittedLine[3].toLowerCase().replace(";", "");
						nStatusesToTest++;
					}
					break;					
				case "statusB":
					// Status
					if (statusB_pre.equals("")) {
						statusB_pre = splittedLine[3].toLowerCase().replace(";", "");
						nStatusesFromTest++;
					} else {
						statusB_post = splittedLine[3].toLowerCase().replace(";", "");
						nStatusesToTest++;
					}
					break;					
				case "statusC":
					// Status
					if (statusC_pre.equals("")) {
						statusC_pre = splittedLine[3].toLowerCase().replace(";", "");
						nStatusesFromTest++;
					} else {
						statusC_post = splittedLine[3].toLowerCase().replace(";", "");
						nStatusesToTest++;
					}
					break;
				case "statusCOperate":
					// Status
					if (statusCOperate_pre.equals("")) {
						statusCOperate_pre = splittedLine[3].toLowerCase().replace(";", "");
						nStatusesFromTest++;
					} else {
						statusCOperate_post = splittedLine[3].toLowerCase().replace(";", "");
						nStatusesToTest++;
					}
					break;
				default:
					throw new Exception("unsupported function type");
				}
			}
			if (line.startsWith("set")) {
				String splittedLine[] = line.split(" ");
				switch (splittedLine[1]) {
				case "transitionA":
					transitionA = splittedLine[3].toLowerCase().replace(";", "");
					if (transitionA.equals("undef")) transitionA = "";
					nTransitionsTest++;
					break;
				case "transitionB":
					transitionB = splittedLine[3].toLowerCase().replace(";", "");
					if (transitionB.equals("undef")) transitionB = "";
					nTransitionsTest++;
					break;
				case "transitionC":
					transitionC = splittedLine[3].toLowerCase().replace(";", "");
					if (transitionC.equals("undef")) transitionC = "";
					nTransitionsTest++;
					break;
				default:
					throw new Exception("unsupported function type");
				}
			}
			
			
			if(nStatusesToTest==nStatuses)  
				next_state = statusA_post + "|" + statusB_post + "|" + statusC_post + "|" + statusCOperate_post;
			if(nStatusesFromTest==nStatuses) 
				current_state = statusA_pre + "|" + statusB_pre + "|" + statusC_pre + "|" + statusCOperate_pre;
			if(nTransitions == nTransitionsTest) 
				receivedMessage = transitionA + "|" + transitionB + "|" + transitionC;
			if(nMessages == nMessagesTest) 
				sentMessage = lightsA + "|" + lightsB;			
					
			if (!current_state.equals("") && !next_state.equals("") && !receivedMessage.equals("")
					&& !sentMessage.equals("")) {	
				
				// Add the new entry to the table
				entries.add(new FSMEntry(current_state, receivedMessage, next_state, sentMessage));
				// Update the test sequence
				msgsList += receivedMessage + " ";
				// Reset all the string values
				current_state = next_state;
				next_state = "";
				receivedMessage = "";
				sentMessage = "";
				nMessagesTest = 0;
				nTransitionsTest = 0;
				nStatusesToTest = 0;
				lightsA = lightsB = transitionA = transitionB = transitionC = "";
				statusA_pre = statusA_post;
				statusB_pre = statusB_post;
				statusC_pre = statusC_post;
				statusCOperate_pre = statusCOperate_post;
				statusA_post = statusB_post = statusC_post = statusCOperate_post = "";
			}
		}
		ArrayList<FSMEntry> list = new ArrayList<FSMEntry>(entries);
		content += printFSMTable(list) + "Set of Sequences :\n" + msgsList + "\nInformation of Sequences :\n0";
		fin.close();
		return content;
	}

	private static String printFSMTable(ArrayList<FSMEntry> entries) {
		String ret = "";
		for (FSMEntry entry : entries) {
			ret += "\t" + entry.getStateFrom() + ",\t" + entry.getReceivedMessage() + ",\t" + entry.getStateTo() + ",\t"
					+ entry.getSentMessage() + "\r\n";
		}
		return ret;
	}

	public static String createFSMFromMultipleAvalla(String[] fileNames, boolean useMonitoring, int nMessages, int nTransitions, int nStatuses) throws Exception {
		ArrayList<String> paths = new ArrayList<String>();
		HashSet<FSMEntry> entries = new HashSet<FSMEntry>();
		HashSet<String> sequenceList = new HashSet<String>();
		String content = "Finite State Machine :\n";

		// Compose the name of the FSM
		String fsmPath = new StringBuilder(
				new StringBuilder(new File(fileNames[0]).getAbsolutePath()).reverse().toString().split("\\.", 2)[1])
						.reverse().toString()
				+ "_Merged" + (!useMonitoring ? "_NoMonitoring" : "") + ".txt";

		// Create all the single files
		for (String file : fileNames) {
			paths.add(createFSMFromAvalla(file, nMessages, nTransitions, nStatuses));
		}

		// Merge all the sequences and all the state tables
		for (String file : paths) {
			FileReader reader = new FileReader(file);
			BufferedReader fin = new BufferedReader(reader);
			String line = "";
			while ((line = fin.readLine()) != null) {
				if (line.startsWith("Finite State Machine"))
					continue;
				if (line.startsWith("Set of Sequences :"))
					continue;
				if (line.startsWith("Information of Sequences :"))
					break;
				if (line.startsWith("\t")) {
					// state table row
					String[] splittedLine = line.split("\t");
					entries.add(new FSMEntry(splittedLine[1].replace(",", ""), splittedLine[2].replace(",", ""),
							splittedLine[3].replace(",", ""), splittedLine[4].replace(",", "")));
				} else if (!line.equals("")) {
					// sequence row
					sequenceList.add(line + "\n");
				}
			}
			fin.close();
			reader.close();
		}
		ArrayList<FSMEntry> list = new ArrayList<FSMEntry>(entries);
		content += printFSMTable(list) + "Set of Sequences :\n" + sequenceList.stream().collect(Collectors.joining(""))
				+ "\nInformation of Sequences :\n0";

		PrintWriter fout = new PrintWriter(new FileWriter(fsmPath));
		fout.write(content);
		fout.close();
		return fsmPath;
	}

}
