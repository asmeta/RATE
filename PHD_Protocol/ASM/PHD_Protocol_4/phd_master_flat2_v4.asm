// a simple example with a tic tac toe game

asm phd_master_flat2_v4

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

	
definitions:
	// DOMAIN DEFINITIONS
	
	rule r_Unassociated =
		switch transition
			case REQ_ASSOC_REL:
				par
					status := UNASSOCIATED 
					message := MSG_NO_RESPONSE
				endpar
			case REQ_ASSOC_ABORT:
				par
					status := UNASSOCIATED 
					message := MSG_RX_ABRT
				endpar
			case RX_AARQ_ACCEPTABLE_AND_KNOWN_CONFIGURATION:
				par
					status := OPERATING 
					message := MSG_RX_AARE
				endpar
			case RX_AARQ_ACCEPTABLE_AND_UNKNOWN_CONFIGURATION:
				par
					status := WAITING_FOR_CONFIG 
					message := MSG_RX_AARE 
				endpar
			case RX_AARQ_UNACCEPTABLE_CONFIGURATION:
				par
					status := UNASSOCIATED 
					message := MSG_RX_AARE
				endpar
			case RX_AARE:
				par
					status := UNASSOCIATED 
					message := MSG_RX_ABRT
				endpar
			case RX_RLRQ:
				par
					status := UNASSOCIATED 
					message := MSG_RX_ABRT
				endpar
			case RX_RLRE:
				par
					status := UNASSOCIATED 
					message := MSG_NO_RESPONSE
				endpar
			case RX_ABRT:
				par
					status := UNASSOCIATED 
					message := MSG_NO_RESPONSE
				endpar
			case RX_ROIV_CONFIRMED_EVENT_REPORT:
				par
					status := UNASSOCIATED 
					message := MSG_RX_ABRT
				endpar
			case RX_AARQ_INVALID:
				par
					status := UNASSOCIATED 
					message := MSG_RX_AARE
				endpar
			case RX_AARQ_EXTERNAL:
				par
					status := UNASSOCIATED 
					message := MSG_RX_AARE
				endpar
		endswitch 


	rule r_Waiting_For_Config = 
		switch transition
			case REQ_ASSOC_REL:
				par
					status := DISASSOCIATING 
					message := MSG_RX_RLRQ
				endpar
			case REQ_ASSOC_ABORT:
				par
					status := UNASSOCIATED 
					message := MSG_RX_ABRT
				endpar
			case RX_AARQ:
				par
					status := UNASSOCIATED 
					message := MSG_RX_ABRT
				endpar
			case RX_AARE:
				par
					status := UNASSOCIATED 
					message := MSG_RX_ABRT
				endpar
			case RX_RLRQ:
				par
					status := UNASSOCIATED 
					message := MSG_RX_RLRE
				endpar
			case RX_RLRE:
				par
					status := UNASSOCIATED 
					message := MSG_RX_ABRT
				endpar
			case RX_ABRT:
				par
					status := UNASSOCIATED 
					message := MSG_NO_RESPONSE
				endpar
			case RX_ROIV_CONFIRMED_EVENT_REPORT:
				par
					status := CHECKING_CONFIG 
					message := MSG_NO_RESPONSE
				endpar
			case RX_ROIV:
				par
					status := WAITING_FOR_CONFIG 
					message := MSG_RX_PRST
				endpar
			case RX_ROER:
				par
					status := WAITING_FOR_CONFIG 
					message := MSG_NO_RESPONSE
				endpar
			case RX_RORJ:
				par
					status := WAITING_FOR_CONFIG 
					message := MSG_NO_RESPONSE
				endpar
			case RX_RORS:
				par
					status := WAITING_FOR_CONFIG 
					message := MSG_NO_RESPONSE
				endpar
			case RX_RORS_GET:
				par
					status := WAITING_FOR_CONFIG
					message := MSG_NO_RESPONSE
				endpar
			case RX_RORS_CONFIRMED_SET:
				par
					status := WAITING_FOR_CONFIG
					message := MSG_NO_RESPONSE
				endpar
			case RX_RORS_CONFIRMED_ACTION:
				par
					status := WAITING_FOR_CONFIG
					message := MSG_NO_RESPONSE
				endpar
		endswitch

	rule r_Checking_Config = 
		switch transition
			case REQ_ASSOC_REL:
				par
					status := DISASSOCIATING 
					message := MSG_RX_RLRQ 
				endpar
			case REQ_ASSOC_ABORT:
				par
					status := UNASSOCIATED 
					message := MSG_RX_ABRT 
				endpar
			case RX_AARQ:
				par
					status := UNASSOCIATED 
					message := MSG_RX_ABRT 
				endpar
			case RX_AARE:
				par
					status := UNASSOCIATED 
					message := MSG_RX_ABRT 
				endpar
			case RX_RLRQ:
				par
					status := UNASSOCIATED 
					message := MSG_RX_RLRE 
				endpar
			case RX_RLRE:
				par
					status := UNASSOCIATED 
					message := MSG_RX_ABRT 
				endpar
			case RX_ABRT:
				par
					status := UNASSOCIATED 
					message := MSG_NO_RESPONSE 
				endpar
			case RX_ROIV_CONFIRMED_EVENT_REPORT:
				par
					status := CHECKING_CONFIG 
					message := MSG_RX_ABRT 
				endpar
			case RX_ROIV:
				par
					status := UNASSOCIATED 
					message := MSG_RX_PRST 
				endpar
			case RX_ROER:
				par
					status := CHECKING_CONFIG 
					message := MSG_NO_RESPONSE 
				endpar
			case RX_RORJ:
				par
					status := CHECKING_CONFIG 
					message := MSG_NO_RESPONSE 
				endpar
			case REQ_AGENT_SUPPLIED_UNKNOWN_CONFIGURATION:
				par
					status := WAITING_FOR_CONFIG
					message := MSG_RX_PRST 
				endpar
			case REQ_AGENT_SUPPLIED_KNOWN_CONFIGURATION:
				par
					status := OPERATING 
					message := MSG_RX_PRST 
				endpar
			case RX_RORS:
				par
					status := CHECKING_CONFIG 
					message := MSG_NO_RESPONSE
				endpar
			case RX_RORS_CONFIRMED_ACTION:
				par
					status := CHECKING_CONFIG 
					message := MSG_NO_RESPONSE
				endpar
			case RX_RORS_CONFIRMED_SET:
				par
					status := CHECKING_CONFIG 
					message := MSG_NO_RESPONSE
				endpar
			case RX_RORS_GET:
				par
					status := CHECKING_CONFIG 
					message := MSG_NO_RESPONSE
				endpar
		endswitch

		
	rule r_Operating =
		switch transition
			case REQ_ASSOC_REL:
				par
					status := DISASSOCIATING 
					message := MSG_RX_RLRQ
				endpar
			case REQ_ASSOC_ABORT:
				par
					status := UNASSOCIATED
					message := MSG_RX_ABRT
				endpar
			case RX_AARQ:
				par
					status := UNASSOCIATED 
					message := MSG_RX_ABRT
				endpar
			case RX_AARE:
				par
					status := UNASSOCIATED 
					message := MSG_RX_ABRT
				endpar
			case RX_RLRQ:
				par
					status := UNASSOCIATED
					message := MSG_RX_RLRE
				endpar
			case RX_RLRE:
				par
					status := UNASSOCIATED
					message := MSG_RX_ABRT
				endpar
			case RX_ABRT:
				par
					status := UNASSOCIATED
					message := MSG_NO_RESPONSE
				endpar
			case RX_ROER:
				par
					status := OPERATING
					message := MSG_NO_RESPONSE
				endpar
			case RX_RORJ:
				par
					status := OPERATING 
					message := MSG_NO_RESPONSE
				endpar
			case RX_RORS:
				par
					status := OPERATING
					message := MSG_NO_RESPONSE
				endpar
			case RX_RORS_CONFIRMED_ACTION:
				par
					status := OPERATING 
					message := MSG_NO_RESPONSE
				endpar
			case RX_RORS_CONFIRMED_SET:
				par
					status := OPERATING 
					message := MSG_NO_RESPONSE
				endpar
			case RX_RORS_GET:
				par
					status := OPERATING 
					message := MSG_NO_RESPONSE
				endpar
			case RX_ROIV:
				par
					status := OPERATING
					message := MSG_RX_PRST
				endpar
			case RX_ROIV_CONFIRMED_EVENT_REPORT:
				par
					status := OPERATING
					message := MSG_RX_PRST
				endpar
		endswitch

	rule r_Disassociating =
		switch transition
			case REQ_ASSOC_REL:
				par
					status := DISASSOCIATING 
					message := MSG_NO_RESPONSE
				endpar
			case REQ_ASSOC_ABORT:
				par
					status := UNASSOCIATED 
					message := MSG_RX_ABRT
				endpar
			case RX_AARQ:
				par
					status := UNASSOCIATED 
					message := MSG_RX_ABRT
				endpar
			case RX_AARE:
				par
					status := UNASSOCIATED 
					message := MSG_RX_ABRT
				endpar
			case RX_RLRQ:
				par
					status := DISASSOCIATING 
					message := MSG_RX_RLRE
				endpar
			case RX_RLRE:
				par
					status := UNASSOCIATED 
					message := MSG_NO_RESPONSE
				endpar
			case RX_ABRT:
				par
					status := UNASSOCIATED 
					message := MSG_NO_RESPONSE
				endpar
			case RX_ROIV:
				par
					status := DISASSOCIATING 
					message := MSG_NO_RESPONSE
				endpar
			case RX_ROER:
				par
					status := UNASSOCIATED 
					message := MSG_RX_ABRT
				endpar
			case RX_RORJ:
				par
					status := UNASSOCIATED 
					message := MSG_RX_ABRT
				endpar
			case RX_RORS:
				par
					status := UNASSOCIATED 
					message := MSG_RX_ABRT
				endpar
		endswitch

	INVAR (status = DISASSOCIATING) implies (transition = REQ_ASSOC_REL or transition = REQ_ASSOC_ABORT
		or transition = RX_AARQ or transition = RX_AARE or transition = RX_RLRQ or transition = RX_RLRE
		or transition = RX_ABRT or transition = RX_ROIV or transition = RX_ROER or transition = RX_RORJ
		or transition = RX_RORS)

	INVAR (status = OPERATING) implies (transition = REQ_ASSOC_REL or transition = REQ_ASSOC_ABORT
		or transition = RX_AARQ or transition = RX_AARE or transition = RX_RLRQ or transition = RX_RLRE
		or transition = RX_ABRT or transition = RX_ROER or transition = RX_RORJ or transition = RX_RORS
		or transition = RX_RORS_CONFIRMED_ACTION or transition = RX_RORS_CONFIRMED_SET
		or transition = RX_RORS_GET or transition = RX_ROIV or transition = RX_ROIV_CONFIRMED_EVENT_REPORT)

	INVAR (status = UNASSOCIATED) implies (transition = REQ_ASSOC_REL or transition = REQ_ASSOC_ABORT
		or transition = RX_AARE or transition = RX_RLRQ or transition = RX_RLRE
		or transition = RX_ABRT or transition = RX_AARQ_ACCEPTABLE_AND_KNOWN_CONFIGURATION
		or transition = RX_AARQ_ACCEPTABLE_AND_UNKNOWN_CONFIGURATION
		or transition = RX_AARQ_UNACCEPTABLE_CONFIGURATION or transition = RX_ROIV_CONFIRMED_EVENT_REPORT
		or transition = RX_ROIV_CONFIRMED_EVENT_REPORT or transition = RX_AARQ_INVALID
		or transition = RX_AARQ_EXTERNAL)

	INVAR (status = WAITING_FOR_CONFIG) implies (transition = REQ_ASSOC_REL
		or transition = REQ_ASSOC_ABORT or transition = RX_RLRQ
		or transition = RX_AARE or transition = RX_RLRE or transition = RX_ABRT
		or transition = RX_ROIV_CONFIRMED_EVENT_REPORT or transition = RX_ROIV
		or transition = RX_ROER or transition = RX_RORJ or transition = RX_RORS
		or transition = RX_RORS_GET or transition = RX_RORS_CONFIRMED_SET
		or transition = RX_RORS_CONFIRMED_ACTION)

	INVAR (status = CHECKING_CONFIG) implies (transition = REQ_ASSOC_REL or transition = REQ_ASSOC_ABORT
		or transition = RX_AARQ or transition = RX_AARE or transition = RX_RLRQ
		or transition = RX_RLRE or transition = RX_ABRT or transition = RX_ROER
		or transition = RX_ROIV_CONFIRMED_EVENT_REPORT or transition = RX_ROIV
		or transition = RX_RORJ or transition = REQ_AGENT_SUPPLIED_UNKNOWN_CONFIGURATION
		or transition = REQ_AGENT_SUPPLIED_KNOWN_CONFIGURATION or transition = RX_RORS
		or transition = RX_RORS_CONFIRMED_ACTION or transition = RX_RORS_CONFIRMED_SET
		or transition = RX_RORS_GET)

	main rule r_Main = 
		par
			if (status = UNASSOCIATED) then
				r_Unassociated[]
			endif 
			if (status = WAITING_FOR_CONFIG) then
				r_Waiting_For_Config[] 
			endif
			if (status = CHECKING_CONFIG) then
				r_Checking_Config[]
			endif
			if (status = OPERATING) then
				r_Operating[] 
			endif
			if (status = DISASSOCIATING) then
				r_Disassociating[] 
			endif 
		endpar
		
		
// INITIAL STATE
default init s0:
	function status = UNASSOCIATED
	function transition = RX_ABRT
