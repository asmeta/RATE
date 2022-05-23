package util;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;

public class DataMaker {
	
	public static ArrayList<String[]> readSqsFromFile(String sequenceFile) throws IOException {
		String readLn;
		File f = new File(sequenceFile);
		FileReader fin = new FileReader(f.getAbsolutePath());
		BufferedReader sequencesFile = new BufferedReader(fin);
		ArrayList<String[]> sequences = new ArrayList<>();

		// Read the content of the file
		while (true) {

			// Read the Sequences File
			readLn = null;
			try {
				readLn = sequencesFile.readLine();
			} catch (IOException e) {
				break;
			}
			if (readLn == null)
				break;
			if (readLn.startsWith("Finite State Machine :"))
				continue;
			if (readLn.startsWith("\t"))
				continue;
			if (readLn.startsWith("Set of Sequences :"))
				continue;
			// Extract information
			sequences.add(readLn.split(" "));
		}

		// Close all the streams
		sequencesFile.close();
		try {
			fin.close();
		} catch (Exception e) {
			e.printStackTrace();
		}

		// Return the built automaton
		return sequences;
	}
	
	public static ArrayList<FSMEntry> readEntriesFromFile(String sequenceFile) throws IOException {
		String readLn;
		File f = new File(sequenceFile);
		FileReader fin = new FileReader(f.getAbsolutePath());
		BufferedReader sequencesFile = new BufferedReader(fin);
		ArrayList<FSMEntry> entries = new ArrayList<>();

		// Read the content of the file
		while (true) {

			// Read the Sequences File
			readLn = null;
			try {
				readLn = sequencesFile.readLine();
			} catch (IOException e) {
				break;
			}
			if (readLn == null)
				break;
			if (readLn.startsWith("Finite State Machine :"))
				continue;
			if (readLn.startsWith("\t")) {
				readLn = readLn.replace("\t", "");
				FSMEntry entry = new FSMEntry(readLn.split(",")[0], readLn.split(",")[1], readLn.split(",")[2], readLn.split(",")[3]);
				entries.add(entry);
			}
			if (readLn.startsWith("Set of Sequences :"))
				break;
		}

		// Close all the streams
		sequencesFile.close();
		try {
			fin.close();
		} catch (Exception e) {
			e.printStackTrace();
		}

		// Return the built information
		return entries;
	}
}
