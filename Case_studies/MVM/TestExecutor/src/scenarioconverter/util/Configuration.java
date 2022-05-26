package scenarioconverter.util;

import java.util.Arrays;
import java.util.List;

import org.asmeta.atgt.generator.CriteriaEnum;

public class Configuration {
	public List<CriteriaEnum> criteria;
	public boolean useMonitoring;
	public String level;
	
	public Configuration(List<CriteriaEnum> criteria, String level) {
		this.criteria = criteria;
		this.useMonitoring = true;
		this.level = level;
	}
	
	public Configuration(boolean useMonitoring, List<CriteriaEnum> criteria, String level) {
		this.criteria = criteria;
		this.useMonitoring = useMonitoring;
		this.level = level;
	}
	
	public Configuration(String level, CriteriaEnum ... criteria) {
		this(Arrays.asList(criteria), level);
	}
	
	public Configuration(boolean useMonitoring, String level, CriteriaEnum ... criteria) {
		this(useMonitoring,Arrays.asList(criteria), level);
	}
}