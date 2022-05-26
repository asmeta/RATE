package scenarioconverter.util;

import com.google.gson.annotations.SerializedName;

public class Command {

	@SerializedName("asmName")
	public String asmName;
	@SerializedName("cName")
	public String cName;
	@SerializedName("commandType")
	public FunctionType commandType;
	@SerializedName("ignore")
	public boolean ignore;
	@SerializedName("whenTrue")
	public String whenTrue;
	@SerializedName("whenFalse")
	public String whenFalse;

	public Command(String asmName, String cName, FunctionType commandType, String whenTrue, String whenFalse,
			boolean ignore) {
		this.asmName = asmName;
		this.cName = cName;
		this.commandType = commandType;
		this.ignore = ignore;
		this.whenTrue = whenTrue;
		this.whenFalse = whenFalse;
	}

	public String getAsmName() {
		return asmName;
	}

	public void setAsmName(String asmName) {
		this.asmName = asmName;
	}

	public String getcName() {
		return cName;
	}

	public FunctionType getCommandType() {
		return commandType;
	}

	public void setCommandType(FunctionType commandType) {
		this.commandType = commandType;
	}

	public void setcName(String cName) {
		this.cName = cName;
	}

	public boolean isIgnore() {
		return ignore;
	}

	public void setIgnore(boolean ignore) {
		this.ignore = ignore;
	}

	public String getWhenTrue() {
		return whenTrue;
	}

	public void setWhenTrue(String whenTrue) {
		this.whenTrue = whenTrue;
	}

	public String getWhenFalse() {
		return whenFalse;
	}

	public void setWhenFalse(String whenFalse) {
		this.whenFalse = whenFalse;
	}

	@Override
	public boolean equals(Object obj) {
		if (obj instanceof Command)
			return this.asmName.equals(((Command) obj).asmName) && this.cName.equals(((Command) obj).cName);

		return false;
	}

}
