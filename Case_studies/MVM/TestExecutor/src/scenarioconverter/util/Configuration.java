package scenarioconverter.util;

import java.util.Arrays;
import java.util.List;
import java.util.stream.Collectors;

import org.asmeta.atgt.generator.CriteriaEnum;

public class Configuration {
	
	public List<CriteriaEnum> criteria;
	public boolean useMonitoring;
	public String level;
	public String configPath;
	
	public Configuration(List<CriteriaEnum> criteria, String level, String configPath) {
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
	
	public Configuration(String level, String configPath, CriteriaEnum ... criteria) {
		this(Arrays.asList(criteria), level, configPath);
	}
	
	public Configuration(boolean useMonitoring, String level, String configPath, CriteriaEnum ... criteria) {
		this(useMonitoring,Arrays.asList(criteria), level, configPath);
	}
	
	@Override
	public String toString() {
		return criteria.stream().map(x -> x.getAbbrvName()).collect(Collectors.joining("_"));
	}
	
	
}