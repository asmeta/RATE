package scenarioconverter.util;

public enum CriteriaEnum{
	BASIC_RULE("BasicRule","BR"), 
	COMPLETE_RULE("CompleteRule","CR"),
	RULE_GUARD("RuleGuard","RG"),
	RULE_UPDATE("RuleUpdate","UR"),
	MCDC("MCDC","MCDC"),
	// combinatorial
	COMBINATORIAL_MON("pairwise monitored", "pair"),
	COMBINATORIAL_ALL("pairwise all", "pair"),
	
	THREEWISE_ALL("3wise", "3-wise"),
	THREEWISE_MON("3wise", "3-wise");
	
	
	private String prefix;

	CriteriaEnum(String name, String prefix) {
		this.prefix = prefix;
	}

	public String getAbbrvName() {
		return prefix;
	}
	
}