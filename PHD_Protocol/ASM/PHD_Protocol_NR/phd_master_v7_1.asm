asm phd_master_v7_1

import ../StandardLibrary

signature:
	// DOMAINS
	enum domain Status = {UNASSOCIATED | WAITING_FOR_CONFIG | CHECKING_CONFIG | OPERATING | DISASSOCIATING | ASSOCIATING}
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
					 	r_setStatusMsg[UNASSOCIATED,MSG_NO_RESPONSE] //2.6
					endif
					if (status = ASSOCIATING) then
						r_setStatusMsg[UNASSOCIATED,MSG_RX_ABRT] //3.6
					endif
					if status = WAITING_FOR_CONFIG or status = CHECKING_CONFIG or status = OPERATING then
						r_setStatusMsg[DISASSOCIATING,MSG_RX_RLRQ] //WAIT 6.6 CHECK 7.6 OP 12.6
					endif
					if (status = DISASSOCIATING) then
						r_setStatusMsg[DISASSOCIATING,MSG_NO_RESPONSE] //13.6
					endif
				endpar
			endif 
			if transition = REQ_ASSOC_ABORT or transition = RX_AARE then
				if status = UNASSOCIATED or status = ASSOCIATING or status = WAITING_FOR_CONFIG or status = CHECKING_CONFIG or status = OPERATING or status = DISASSOCIATING then
				 	r_setStatusMsg[UNASSOCIATED,MSG_RX_ABRT]//UNASS-REQ_ASSOC_ABORT 2.7 UNASS-RX_AARE 2.12
				 											//ASS-REQ_ASSOC_ABORT 3.7 ASS-RX_AARE 3.12
				 											//WAIT-REQ_ASSOC_ABORT 6.7 WAIT-RX_AARE 6.12
				 											//CHECK-REQ_ASSOC_ABORT 7.7 CHECK-RX_AARE 7.12
				 											//OP-REQ_ASSOC_ABORT 12.7 OP-RX_AARE 12.12
				 											//DIS-REQ_ASSOC_ABORT 13.7 DIS-RX_AARE 13.12
				endif
			endif
			if transition = RX_ABRT then
				if status = UNASSOCIATED or status = ASSOCIATING or status = WAITING_FOR_CONFIG or status = CHECKING_CONFIG or status = OPERATING or status = DISASSOCIATING then
					r_setStatusMsg[UNASSOCIATED,MSG_NO_RESPONSE]//UNASS 2.18 ASS 3.18 WAIT 6.18 CHECK 7.18 OP 12.18 DIS 13.18
				endif
			endif
			if (transition = RX_AARQ_ACCEPTABLE_AND_KNOWN_CONFIGURATION) then
				if status = ASSOCIATING then
				 	r_setStatusMsg[OPERATING,MSG_RX_AARE] //3.9
				else
					r_setStatusMsg[UNASSOCIATED,MSG_RX_ABRT]//2.19 6.19 7.19 12.19 13.19
				endif
			endif
			if (transition = RX_AARQ_ACCEPTABLE_AND_UNKNOWN_CONFIGURATION) then
				if status = ASSOCIATING then
				 	r_setStatusMsg[WAITING_FOR_CONFIG,MSG_RX_AARE] //3.10
				 else
					r_setStatusMsg[UNASSOCIATED,MSG_RX_ABRT] //2.19 6.19 7.19 12.19 13.19
				endif
			endif
			if transition = RX_AARQ_UNACCEPTABLE_CONFIGURATION or transition = RX_AARQ_INVALID or transition = RX_AARQ_EXTERNAL then
				if status = ASSOCIATING then
				 	r_setStatusMsg[UNASSOCIATED,MSG_RX_AARE] //3.11
				else
					r_setStatusMsg[UNASSOCIATED,MSG_RX_ABRT] //2.19 6.19 7.19 12.19 13.19
				endif
			endif
			if (transition = RX_RLRQ) then
				par
					if status = UNASSOCIATED or status = ASSOCIATING then
					 	r_setStatusMsg[UNASSOCIATED,MSG_RX_ABRT] //UNASS 2.16 ASS 3.16
					endif
					if status = WAITING_FOR_CONFIG or status = CHECKING_CONFIG or status = OPERATING then
						r_setStatusMsg[UNASSOCIATED,MSG_RX_RLRE] //WAIT 6.16 CHECK 7.16 OP 12.16
					endif
					if (status = DISASSOCIATING) then
						r_setStatusMsg[DISASSOCIATING,MSG_RX_RLRE] //13.16
					endif
				endpar
			endif
			if (transition = RX_RLRE) then
				par
					if status = UNASSOCIATED or status = DISASSOCIATING then
					 	r_setStatusMsg[UNASSOCIATED, MSG_NO_RESPONSE]//UNASS 2.17 DIS 13.17
					endif
					if status = ASSOCIATING or status = WAITING_FOR_CONFIG or status = CHECKING_CONFIG or status = OPERATING then
						r_setStatusMsg[UNASSOCIATED, MSG_RX_ABRT] //ASS 3.17 WAIT 6.17 CHECK 7.17 OP 12.17
					endif
				endpar
			endif
			if (transition = RX_ROIV_CONFIRMED_EVENT_REPORT) then
				par
					if status = UNASSOCIATED or status = ASSOCIATING then
					 	r_setStatusMsg[UNASSOCIATED, MSG_RX_ABRT] //UNASS 2.19 ASS 3.19
					endif
					if status = WAITING_FOR_CONFIG then
						r_setStatusMsg[CHECKING_CONFIG, MSG_NO_RESPONSE] //6.44
					endif
					if status = CHECKING_CONFIG then
						r_setStatusMsg[UNASSOCIATED, MSG_RX_ABRT] //7.24
					endif
					if status = OPERATING then
						r_setStatusMsg[UNASSOCIATED, MSG_RX_ABRT] //12.44
					endif
					/*if status = CHECKING_CONFIG then
						r_setStatusMsg[CHECKING_CONFIG, MSG_RX_ABRT]
					endif
					if status = OPERATING then
						r_setStatusMsg[OPERATING, MSG_RX_PRST]
					endif*/
					if status = DISASSOCIATING then
						r_setStatusMsg[DISASSOCIATING, MSG_NO_RESPONSE] //13.21
					endif
				endpar
			endif
			if transition = RX_AARQ then
				if status = UNASSOCIATED then
				 	r_setStatusMsg[ASSOCIATING,MSG_NO_RESPONSE] //2.8
				else
					r_setStatusMsg[UNASSOCIATED,MSG_RX_ABRT] //ASS 3.8 WAIT 6.8 CHECK 7.8 OP 12.8 DIS 13.8
				endif
			endif
			if transition = RX_ROIV then
				par
					if status = UNASSOCIATED or status = ASSOCIATING then
					 	r_setStatusMsg[UNASSOCIATED, MSG_RX_ABRT] //UNASS 2.19 ASS 3.19
					endif
					if status = WAITING_FOR_CONFIG then
						 r_setStatusMsg[UNASSOCIATED, MSG_RX_ABRT]//6.45
					endif
					if status = CHECKING_CONFIG then
						 r_setStatusMsg[UNASSOCIATED, MSG_RX_ABRT]//7.25
					endif
					/*if status = WAITING_FOR_CONFIG then
						 r_setStatusMsg[WAITING_FOR_CONFIG, MSG_RX_PRST]
					endif
					if status = CHECKING_CONFIG then
						r_setStatusMsg[UNASSOCIATED, MSG_RX_PRST]
					endif*/
					if status = OPERATING then
						r_setStatusMsg[OPERATING, MSG_RX_PRST] //12.45
					endif
					if status = DISASSOCIATING then
						r_setStatusMsg[DISASSOCIATING, MSG_NO_RESPONSE] //13.21
					endif
				endpar
			endif
			if (transition = RX_ROER or transition = RX_RORJ or transition = RX_RORS or transition = RX_RORS_GET or transition = RX_RORS_CONFIRMED_SET or transition = RX_RORS_CONFIRMED_ACTION) then
				par
					if status = WAITING_FOR_CONFIG then
						 r_InvokeId[WAITING_FOR_CONFIG,MSG_NO_RESPONSE,UNASSOCIATED,MSG_RX_ABRT] //6.26
					endif
					if status = CHECKING_CONFIG then
						r_setStatusMsg[CHECKING_CONFIG, MSG_NO_RESPONSE] //7.26
					endif
					if status = OPERATING then
						r_InvokeId[OPERATING, MSG_NO_RESPONSE,UNASSOCIATED,MSG_RX_ABRT] //12.26
					endif
					if status = DISASSOCIATING or status = UNASSOCIATED or status = ASSOCIATING then
						r_setStatusMsg[UNASSOCIATED, MSG_RX_ABRT] //DIS 13.26 UNASS 2.19 ASS 3.19
					endif
				endpar
			endif
			if transition = REQ_AGENT_SUPPLIED_UNKNOWN_CONFIGURATION then
				if status = CHECKING_CONFIG then
					//r_setStatusMsg[CHECKING_CONFIG, MSG_RX_PRST]
					r_setStatusMsg[WAITING_FOR_CONFIG, MSG_RX_PRST] //7.31
				else
					r_setStatusMsg[UNASSOCIATED, MSG_RX_ABRT] //2.19 3.19 6.19 12.19 13.19
				endif
			endif
			if transition = REQ_AGENT_SUPPLIED_KNOWN_CONFIGURATION then 
				if status = CHECKING_CONFIG then
					r_setStatusMsg[OPERATING, MSG_RX_PRST]//7.32
				else
					r_setStatusMsg[UNASSOCIATED, MSG_RX_ABRT] //2.19 3.19 6.19 12.19 13.19
				endif
			endif
		endpar
		
		
		
// INITIAL STATE
default init s0:
	function status = UNASSOCIATED
	function transition = RX_ABRT
