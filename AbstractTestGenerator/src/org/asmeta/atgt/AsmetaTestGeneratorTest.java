package org.asmeta.atgt;

import java.io.File;
import java.util.Collections;
import java.util.List;

import org.asmeta.atgt.generator.NuSMVtestGenerator;
import org.asmeta.atgt.generator.TestGenerationWithNuSMV;
import org.asmeta.atgt.generator.coverage.AsmetaBasicRuleCoverageBuilder;
import org.asmeta.parser.ASMParser;
import org.junit.Test;

import atgt.coverage.AsmCoverageBuilder;
import atgt.coverage.AsmTestSuite;

/**
 * tests for the asmeta generator
 *
 */
public class AsmetaTestGeneratorTest {

	public static final String FILE_BASE = "../../../../asm_examples/";

	@Test
	public void generate() throws Exception {
		String asmPath = "../Case_studies/PillBox/ASM/Level_2/pillbox_2.asm";
		NuSMVtestGenerator nuSMVtestGenerator = new NuSMVtestGenerator(asmPath);
		TestGenerationWithNuSMV.useLTLandBMC = true;
		AsmetaBasicRuleCoverageBuilder tpbuilder = new AsmetaBasicRuleCoverageBuilder();		
		List<AsmCoverageBuilder> cov = Collections.singletonList(tpbuilder);
		AsmTestSuite result = nuSMVtestGenerator.generateAbstractTests(cov, 10, ".*");
	}
	
}
