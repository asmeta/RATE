package test_conf_ref_phd;

import java.io.File;

import org.asmeta.atgt.generator.NuSMVtestGenerator;
import org.asmeta.atgt.generator.TestGenerationWithNuSMV;
import org.asmeta.xt.validator.AsmetaV;

import atgt.coverage.AsmTestSequence;
import atgt.coverage.AsmTestSuite;
import atgt.testseqexport.toAvalla;

public class TestPHD {

	public static void main(String[] args) throws Exception {
		// genera i tests per PHD
		String asmPath = "../ASM/Level7/phd_master_v7.asm";
		String asmPath2 = "../ASM/Level6/phd_master_flat2_v6.asm";
		NuSMVtestGenerator nuSMVtestGenerator = new NuSMVtestGenerator(asmPath);
		TestGenerationWithNuSMV.useLTLandBMC = true;
		AsmTestSuite result = nuSMVtestGenerator.generateAbstractTests(Integer.MAX_VALUE, ".*");
		// export ain avalla
		int counter = 0;
		for (AsmTestSequence tc : result.getTests()) {
			if (tc.numberOfStates() ==0) {
				System.err.println(" scenario vuopto !!!");
				continue;
			}
			// Create the new avalla file
			new toAvalla(new File("temp/scenario" + counter + ".avalla"), tc, asmPath2).save();
			counter++;
		}
		// esegui tutti gli scenari
		AsmetaV.execValidation("temp", false);

	}
	
}
