asm phd_master_v7

import StandardLibrary

signature:
	// DOMAINS
	enum domain Status = {UNASSOCIATED | WAITING_FOR_CONFIG | CHECKING_CONFIG | OPERATING | DISASSOCIATING}
	enum domain Transition = {REQ_ASSOC_REL | REQ_ASSOC_ABORT | RX_AARQ_ACCEPTABLE_AND_KNOWN_CONFIGURATION | RX_AARQ_ACCEPTABLE_AND_UNKNOWN_CONFIGURATION | RX_AARQ_UNACCEPTABLE_CONFIGURATION | RX_AARE | RX_RLRQ | RX_RLRE | RX_ABRT | RX_ROIV_CONFIRMED_EVENT_REPORT | RX_AARQ | RX_ROIV | RX_ROER | RX_RORJ | REQ_AGENT_SUPPLIED_UNKNOWN_CONFIGURATION | REQ_AGENT_SUPPLIED_KNOWN_CONFIGURATION | RX_RORS | RX_RORS_CONFIRMED_ACTION | RX_RORS_CONFIRMED_SET | RX_RORS_GET | RX_AARQ_INVALID | RX_AARQ_EXTERNAL}
	enum domain Message = {MSG_NO_RESPONSE | MSG_RX_AARE | MSG_RX_ABRT | MSG_RX_RLRQ | MSG_RX_RLRE | MSG_RX_PRST | MSG_RX_AARQ_UNACCEPTABLE_CONFIGURATION}
	// FUNCTIONS
	controlled status: Status
	monitored transition: Transition //row chosen by the user
	controlled message: Message
	monitored invokeIdValid: Boolean

	
definitions:
	// DOMAIN DEFINITIONS
	rule r_setStatusMsg($s in Status, $m in Message)=
		par
			status := $s 
			message := $m
		endpar
	
	rule r_InvokeId($s1 in Status, $m1 in Message, $s2 in Status, $m2 in Message)=
		if invokeIdValid = true then
			r_setStatusMsg[$s1, $m1]
		else
			r_setStatusMsg[$s2, $m2]
		endif

	INVAR (invokeIdValid = false) implies ((status = WAITING_FOR_CONFIG and (transition = RX_RORS or transition = RX_ROER
		or transition = RX_RORJ or transition = RX_RORS_CONFIRMED_ACTION or transition = RX_RORS_CONFIRMED_SET
		or transition = RX_RORS_GET)) or (status = OPERATING and (transition = RX_RORS or transition = RX_ROER
		or transition = RX_RORJ or transition = RX_RORS_CONFIRMED_ACTION or transition = RX_RORS_CONFIRMED_SET
		or transition = RX_RORS_GET)))

	main rule r_Main = 
		par
			if (transition = REQ_ASSOC_REL) then
				par
					if status = UNASSOCIATED then
					 	r_setStatusMsg[UNASSOCIATED,MSG_NO_RESPONSE]
					endif
					if status = WAITING_FOR_CONFIG or status = CHECKING_CONFIG or status = OPERATING then
						r_setStatusMsg[DISASSOCIATING,MSG_RX_RLRQ]
					endif
					if (status = DISASSOCIATING) then
						r_setStatusMsg[DISASSOCIATING,MSG_NO_RESPONSE]
					endif
				endpar
			endif 
			if transition = REQ_ASSOC_ABORT or transition = RX_AARE or transition = RX_ABRT then
				if status = UNASSOCIATED or status = WAITING_FOR_CONFIG or status = CHECKING_CONFIG or status = OPERATING or status = DISASSOCIATING then
				 	r_setStatusMsg[UNASSOCIATED,MSG_RX_ABRT]
				endif
			endif
			if (transition = RX_AARQ_ACCEPTABLE_AND_KNOWN_CONFIGURATION) then
				if status = UNASSOCIATED then
				 	r_setStatusMsg[OPERATING,MSG_RX_AARE]
				else
					r_setStatusMsg[UNASSOCIATED,MSG_RX_ABRT]
				endif
			endif
			if (transition = RX_AARQ_ACCEPTABLE_AND_UNKNOWN_CONFIGURATION) then
				if status = UNASSOCIATED then
				 	r_setStatusMsg[WAITING_FOR_CONFIG,MSG_RX_AARE]
				 else
					r_setStatusMsg[UNASSOCIATED,MSG_RX_ABRT]
				endif
			endif
			if transition = RX_AARQ_UNACCEPTABLE_CONFIGURATION or transition = RX_AARQ_INVALID or transition = RX_AARQ_EXTERNAL then
				if status = UNASSOCIATED then
				 	r_setStatusMsg[UNASSOCIATED,MSG_RX_AARE]
				else
					r_setStatusMsg[UNASSOCIATED,MSG_RX_ABRT]
				endif
			endif
			if (transition = RX_RLRQ) then
				par
					if status = UNASSOCIATED then
					 	r_setStatusMsg[UNASSOCIATED,MSG_RX_ABRT]
					endif
					if status = WAITING_FOR_CONFIG or status = CHECKING_CONFIG or status = OPERATING then
						r_setStatusMsg[UNASSOCIATED,MSG_RX_RLRE]
					endif
					if (status = DISASSOCIATING) then
						r_setStatusMsg[DISASSOCIATING,MSG_RX_RLRE]
					endif
				endpar
			endif
			if (transition = RX_RLRE) then
				par
					if status = UNASSOCIATED then
					 	r_setStatusMsg[UNASSOCIATED, MSG_NO_RESPONSE]
					endif
					if status = WAITING_FOR_CONFIG or status = CHECKING_CONFIG or status = OPERATING then
						r_setStatusMsg[UNASSOCIATED, MSG_RX_ABRT]
					endif
					if (status = DISASSOCIATING) then
						r_setStatusMsg[UNASSOCIATED, MSG_NO_RESPONSE]
					endif
				endpar
			endif
			if (transition = RX_ROIV_CONFIRMED_EVENT_REPORT) then
				par
					if status = UNASSOCIATED then
					 	r_setStatusMsg[UNASSOCIATED, MSG_RX_ABRT]
					endif
					if status = WAITING_FOR_CONFIG then
						r_setStatusMsg[CHECKING_CONFIG, MSG_NO_RESPONSE]
					endif
					if status = CHECKING_CONFIG then
						r_setStatusMsg[CHECKING_CONFIG, MSG_RX_ABRT]
					endif
					if status = OPERATING then
						r_setStatusMsg[OPERATING, MSG_RX_PRST]
					endif
					if status = DISASSOCIATING then
						r_setStatusMsg[DISASSOCIATING, MSG_NO_RESPONSE]
					endif
				endpar
			endif
			if transition = RX_AARQ then
				if status = WAITING_FOR_CONFIG or status = CHECKING_CONFIG or status = OPERATING or status = DISASSOCIATING then
					 r_setStatusMsg[UNASSOCIATED, MSG_RX_ABRT]
				else
					r_setStatusMsg[UNASSOCIATED,MSG_RX_ABRT]
				endif
			endif
			if transition = RX_ROIV then
				par
					if status = UNASSOCIATED then
					 	r_setStatusMsg[UNASSOCIATED, MSG_RX_ABRT]
					endif
					if status = WAITING_FOR_CONFIG then
						 r_setStatusMsg[WAITING_FOR_CONFIG, MSG_RX_PRST]
					endif
					if status = CHECKING_CONFIG then
						r_setStatusMsg[UNASSOCIATED, MSG_RX_PRST]
					endif
					if status = OPERATING then
						r_setStatusMsg[OPERATING, MSG_RX_PRST]
					endif
					if status = DISASSOCIATING then
						r_setStatusMsg[DISASSOCIATING, MSG_NO_RESPONSE]
					endif
				endpar
			endif
			if (transition = RX_ROER or transition = RX_RORJ or transition = RX_RORS or transition = RX_RORS_GET or transition = RX_RORS_CONFIRMED_SET or transition = RX_RORS_CONFIRMED_ACTION) then
				par
					if status = WAITING_FOR_CONFIG then
						 r_InvokeId[WAITING_FOR_CONFIG,MSG_NO_RESPONSE,UNASSOCIATED,MSG_RX_ABRT]
					endif
					if status = CHECKING_CONFIG then
						r_setStatusMsg[CHECKING_CONFIG, MSG_NO_RESPONSE]
					endif
					if status = OPERATING then
						r_InvokeId[OPERATING, MSG_NO_RESPONSE,UNASSOCIATED,MSG_RX_ABRT]
					endif
					if status = DISASSOCIATING or status = UNASSOCIATED then
						r_setStatusMsg[UNASSOCIATED, MSG_RX_ABRT]
					endif
				endpar
			endif
			if transition = REQ_AGENT_SUPPLIED_UNKNOWN_CONFIGURATION then
				if status = CHECKING_CONFIG then
					r_setStatusMsg[CHECKING_CONFIG, MSG_RX_PRST]
				endif
			endif
			if transition = REQ_AGENT_SUPPLIED_KNOWN_CONFIGURATION then
				if status = CHECKING_CONFIG then
					r_setStatusMsg[OPERATING, MSG_RX_PRST]
				endif
			endif
		endpar
		
		
		
// INITIAL STATE
default init s0:
	function status = UNASSOCIATED
	function transition = RX_ABRT
