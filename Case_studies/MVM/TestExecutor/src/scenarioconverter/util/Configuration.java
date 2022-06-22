package scenarioconverter.util;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.stream.Collectors;

import org.asmeta.atgt.generator.CriteriaEnum;

public class Configuration {

	public List<CriteriaEnum> criteria;
	public boolean useMonitoring;
	public String level;
	public String configPath;

	public Configuration(String level, String configPath,List<CriteriaEnum> criteria) {
		this.criteria = criteria;
		this.useMonitoring = true;
		this.level = level;
		this.configPath = configPath;
	}

	public Configuration(boolean useMonitoring, List<CriteriaEnum> criteria, String level, String configPath) {
		this.criteria = criteria;
		this.useMonitoring = useMonitoring;
		this.level = level;
		this.configPath = configPath;
	}

//	public Configuration(String level, String configPath, CriteriaEnum... criteria) {
//		this(Arrays.asList(criteria), level, configPath);
//	}

	public Configuration(boolean useMonitoring, String level, String configPath, CriteriaEnum... criteria) {
		this(useMonitoring, Arrays.asList(criteria), level, configPath);
	}

	@Override
	public String toString() {
		String cri;
		if (this.criteria == ALL_CRITERIA) cri = "ALL_CRITERIA";
		else if (this.criteria == MANUAL_TEST) cri = "MAN";
		else cri =criteria.stream().map(x -> x.getAbbrvName()).collect(Collectors.joining("_"));
		return level + "_" + cri;
	}

	public static final List<CriteriaEnum> ALL_CRITERIA = Arrays.asList(
			CriteriaEnum.BASIC_RULE,
			CriteriaEnum.COMBINATORIAL_MON, 
			CriteriaEnum.COMPLETE_RULE, 
			CriteriaEnum.MCDC, 
			CriteriaEnum.RULE_GUARD,
			CriteriaEnum.RULE_UPDATE, 
			CriteriaEnum.THREEWISE_MON);
	
	public static final List<CriteriaEnum> MANUAL_TEST = new ArrayList<>();
	
	@Override
	public int hashCode() {
		return super.hashCode();
	}
			
}