package scenarioconverter.util;

public class UtilityFunctions {

	/**
	 * Method used to check whether a string represents an integer nuumber
	 * 
	 * @param s the string to be checked
	 * @return true if s is an integer, false otherwise
	 */
	public static boolean isInteger(String s) {
		try {
			Integer.parseInt(s);
		} catch (NumberFormatException e) {
			return false;
		} catch (NullPointerException e) {
			return false;
		}
		return true;
	}
}
