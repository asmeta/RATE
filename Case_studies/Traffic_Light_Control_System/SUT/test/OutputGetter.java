
import java.util.ArrayList;

public class OutputGetter {

	public static Boolean[] getOutput(String initialState, String msg, ArrayList<trafficlightcontroller.test.util.FSMEntry> entries) {
		
		Boolean[] output = new Boolean[6];
		String msgOut;
		
		for (trafficlightcontroller.test.util.FSMEntry entry : entries) {
			if (entry.getStateFrom().equals(initialState) && entry.getReceivedMessage().equals(msg)) {
				msgOut = entry.getSentMessage();
				if (msgOut.contains("|") && !msgOut.contains("none") && msgOut != null && !msgOut.equals("|")) {
					output[0] = (msgOut.split("\\|")[0].equals("red")) ? true : false;
					output[1] = (msgOut.split("\\|")[0].equals("green")) ? true : false;
					output[2] = (msgOut.split("\\|")[0].equals("yellow")) ? true : false;
					output[3] = (msgOut.split("\\|")[1].equals("red")) ? true : false;
					output[4] = (msgOut.split("\\|")[1].equals("green")) ? true : false;
					output[5] = (msgOut.split("\\|")[1].equals("yellow")) ? true : false;
					return output;
				} else {
					return null;
				}
			}
		}
		
		return null;
	}
	
}
