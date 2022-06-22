package scenarioconverter.test;

import static org.junit.Assert.*;

import org.junit.Test;

public class MutationTestTest {

	@Test
	public void testReplaceNext() {
		String s = "pippo piero e topolino pilastro";
		int pos = 0;
		do {
			StringBuffer sb = new StringBuffer(s);
			pos = MutationTest.insertComment("pi", sb, pos);
			System.out.println(pos  + " " + sb.toString());
			if (pos == -1) break;
			else pos ++;
		} while (pos >= 0);
	}

}
