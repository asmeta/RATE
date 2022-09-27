package org.asmeta.atgt.generator.coverage;

import static org.junit.Assert.*;

import java.io.File;

import org.apache.log4j.Level;
import org.apache.log4j.Logger;
import org.asmeta.atgt.generator.AsmTestGenerator;
import org.asmeta.atgt.generator.ConverterCounterExample;
import org.asmeta.atgt.generator.NuSMVtestGenerator;
import org.asmeta.atgt.generator.TestGenerationWithNuSMV;
import org.asmeta.parser.ASMParser;
import org.junit.BeforeClass;
import org.junit.Test;

import atgt.coverage.AsmCoverage;

public class AsmetaBasicRuleVisitorTest {

	@Test
	public void testGetTPTreePillBox() throws Exception {
		generateCoverageFor("../Case_studies/PillBox/ASM/Level_2/pillbox_2.asm");
	}
	/**
	 * @param ex
	 * @throws Exception
	 */
	private void generateCoverageFor(String ex) throws Exception {
		asmeta.AsmCollection asms = ASMParser.setUpReadAsm(new File(ex));
		
		
		AsmetaBasicRuleVisitor tpbuilder = new AsmetaBasicRuleVisitor();
		AsmCoverage tp = tpbuilder.getTPTree(new AsmetaAsSpec(asms));
		tp.allTPs().forEach(x -> System.out.println(x.getCondition()));
	}

	@BeforeClass
	public static void setup() {
		Logger.getLogger(AsmTestGenerator.class).setLevel(Level.DEBUG);
		Logger.getLogger(TestGenerationWithNuSMV.class).setLevel(Level.DEBUG);
		Logger.getLogger(NuSMVtestGenerator.class).setLevel(Level.DEBUG);
		Logger.getLogger(ConverterCounterExample.class).setLevel(Level.DEBUG);
		Logger.getLogger("org.asmeta.parser").setLevel(Level.OFF);
		TestGenerationWithNuSMV.useLTLandBMC = true;
	}

}
