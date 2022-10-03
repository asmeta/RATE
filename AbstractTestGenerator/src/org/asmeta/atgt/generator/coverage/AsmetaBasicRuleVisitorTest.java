package org.asmeta.atgt.generator.coverage;

import static org.junit.Assert.*;

import java.io.File;
import java.util.ArrayList;
import java.util.List;

import org.apache.log4j.Level;
import org.apache.log4j.Logger;
import org.asmeta.atgt.generator.AsmTestGenerator;
import org.asmeta.atgt.generator.ConverterCounterExample;
import org.asmeta.atgt.generator.CriteriaEnum;
import org.asmeta.atgt.generator.NuSMVtestGenerator;
import org.asmeta.atgt.generator.TestGenerationWithNuSMV;
import org.asmeta.atgt.generator.AsmTestGenerator.MBTCoverage;
import org.asmeta.parser.ASMParser;
import org.junit.BeforeClass;
import org.junit.Test;

import atgt.coverage.AsmCoverage;
import atgt.coverage.AsmCoverageBuilder;
import atgt.parser.asmeta.AsmetaLLoader;
import atgt.specification.ASMSpecification;

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
		List<AsmCoverageBuilder> coverageCriteria = new ArrayList<>();
		for (CriteriaEnum c : CriteriaEnum.values()) {
			coverageCriteria.add(c.criteria);
		}
		
		ASMSpecification spec = new AsmetaLLoader().read(new File(ex));
		// Build the generator and the tree depending on the criteria
		AsmCoverage ct = new MBTCoverage(coverageCriteria).getTPTree(spec);
		ct.allTPs().forEach(x -> System.out.println(x.getCondition()));
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
