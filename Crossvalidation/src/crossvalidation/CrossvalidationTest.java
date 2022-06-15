package crossvalidation;

import java.io.File;
import java.util.Collection;
import java.util.Collections;

import org.asmeta.atgt.generator.CriteriaEnum;
import org.asmeta.atgt.generator.NuSMVtestGenerator;
import org.asmeta.atgt.generator.TestGenerationWithNuSMV;
import org.asmeta.xt.validator.AsmetaV;

import atgt.coverage.AsmTestSequence;
import atgt.coverage.AsmTestSuite;
import atgt.testseqexport.toAvalla;

public class CrossvalidationTest {

	public static void main(String[] args) throws Exception {
		// XXX: fill the two following variables for checking their equivalence 
		String manualASM = "E:\\WorkSpaces\\MVM\\MVM_0\\VentilatoreNR.asm";
		String lastRefinedASM = "F:\\Dati-Andrea\\GitHub\\RATE\\Case_studies\\MVM\\ASM\\MVM_3\\Ventilatore4SimpleTimeLtdY.asm";
		NuSMVtestGenerator nuSMVtestGenerator = new NuSMVtestGenerator(manualASM);
		TestGenerationWithNuSMV.useLTLandBMC = true;
		NuSMVtestGenerator.removeUnChangedControlles = false;
		NuSMVtestGenerator.removeUnaskedChanges = false;
		Collection<CriteriaEnum> criteria = Collections.singletonList(CriteriaEnum.COMBINATORIAL_MON);
		AsmTestSuite result = nuSMVtestGenerator.generateAbstractTests(CriteriaEnum.getCoverageCriteria(criteria), Integer.MAX_VALUE, ".*");
		// Export in avalla
		int counter = 0;
		for (AsmTestSequence tc : result.getTests()) {
			if (tc.numberOfStates() ==0) {
				System.err.println(" Empty scenario !!!");
				continue;
			}
			// Create the new avalla file
			new toAvalla(new File("temp/scenario" + counter + ".avalla"), tc, lastRefinedASM).save();
			counter++;
		}
		// Executes all the scenarios
		AsmetaV.execValidation("temp", false);
	}
	
}
