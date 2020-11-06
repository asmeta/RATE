package trafficlightcontroller.test.util;

import java.awt.List;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Objects;

public class FSMEntry {
	private String stateFrom;
	private String receivedMessage;
	private String stateTo;
	private String sentMessage;

	public String getStateFrom() {
		return stateFrom;
	}

	public void setStateFrom(String stateFrom) {
		this.stateFrom = stateFrom;
	}

	public String getReceivedMessage() {
		return receivedMessage;
	}

	public void setReceivedMessage(String receivedMessage) {
		this.receivedMessage = receivedMessage.equals("no_response") ? "no response" : receivedMessage;
	}

	public String getStateTo() {
		return stateTo;
	}

	public void setStateTo(String stateTo) {
		this.stateTo = stateTo;
	}

	public String getSentMessage() {
		return sentMessage;
	}

	public void setSentMessage(String sentMessage) {
		this.sentMessage = sentMessage.equals("no_response") ? "no response" : sentMessage;
	}

	public FSMEntry(String stateFrom, String receivedMessage, String stateTo, String sentMessage, boolean invalidId) {
		super();
		this.stateFrom = stateFrom;
		this.receivedMessage = receivedMessage.equals("no_response") ? "no response" : receivedMessage;
		this.stateTo = stateTo;
		this.sentMessage = sentMessage.equals("no_response") ? "no response" : sentMessage;
		if (invalidId) 
			this.receivedMessage = this.receivedMessage + "_invalidId";
	}
	
	public FSMEntry(String stateFrom, String receivedMessage, String stateTo, String sentMessage) {
		super();
		this.stateFrom = stateFrom;
		this.receivedMessage = receivedMessage.equals("no_response") ? "no response" : receivedMessage;
		this.stateTo = stateTo;
		this.sentMessage = sentMessage.equals("no_response") ? "no response" : sentMessage;
	}

	@Override
	public boolean equals(Object obj) {
		if (obj instanceof FSMEntry) {
			return this.stateFrom.equals(((FSMEntry) obj).stateFrom) && this.stateTo.equals(((FSMEntry) obj).stateTo)
					&& this.sentMessage.equals(((FSMEntry) obj).sentMessage)
					&& this.receivedMessage.equals(((FSMEntry) obj).receivedMessage);
		}
		return false;
	}
	
	@Override
	public int hashCode() {
		return Objects.hash(stateFrom + stateTo + receivedMessage + sentMessage);
	}
}
