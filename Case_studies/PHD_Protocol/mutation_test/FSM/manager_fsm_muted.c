
#include <stdlib.h>
#include <stdio.h>
#include "src/communication/common/fsm.h"
#include "src/communication/manager/manager_fsm.h"
#include "src/communication/manager/manager_association.h"
#include "src/communication/common/communication.h"
#include "src/communication/manager/manager_disassociating.h"
#include "src/communication/manager/manager_configuring.h"
#include "src/communication/manager/manager_operating.h"
#include "src/util/log.h"

// START HERE

static FsmTransitionRule IEEE11073_20601_manager_state_table[] = {
{fsm_state_disconnected,	fsm_evt_ind_transport_connection,				fsm_state_unassociated,		&communication_disconnect_tx},
{fsm_state_unassociated,	fsm_evt_ind_transport_disconnect,				fsm_state_disconnected,		&communication_disconnect_tx},
{fsm_state_unassociated,	fsm_evt_req_assoc_rel,						fsm_state_unassociated,		NULL},
{fsm_state_unassociated,	fsm_evt_req_assoc_abort,					fsm_state_unassociated,         &communication_abort_undefined_reason_tx},
{fsm_state_unassociated,	fsm_evt_rx_aarq_acceptable_and_known_configuration,		fsm_state_operating,		&association_accept_config_tx},
{fsm_state_unassociated,	fsm_evt_rx_aarq_acceptable_and_unknown_configuration,		fsm_state_waiting_for_config,	&configuring_transition_waiting_for_config},
{fsm_state_unassociated,	fsm_evt_rx_aarq_unacceptable_configuration,			fsm_state_unassociated,		&association_unaccept_config_tx},
{fsm_state_unassociated,	fsm_evt_rx_aare,						fsm_state_unassociated,		&communication_abort_undefined_reason_tx},
{fsm_state_unassociated,	fsm_evt_rx_rlrq,						fsm_state_unassociated,		&communication_abort_undefined_reason_tx},
{fsm_state_unassociated,	fsm_evt_rx_rlre,						fsm_state_unassociated,		NULL},
{fsm_state_unassociated,	fsm_evt_rx_abrt,						fsm_state_unassociated,		NULL},
{fsm_state_unassociated,	fsm_evt_rx_prst,						fsm_state_unassociated,		&communication_abort_undefined_reason_tx},
{fsm_state_unassociated,	fsm_evt_req_agent_supplied_known_configuration,		fsm_state_unassociated,	&communication_abort_undefined_reason_tx},
{fsm_state_unassociated,	fsm_evt_req_agent_supplied_unknown_configuration,		fsm_state_unassociated,	&communication_abort_undefined_reason_tx},
{fsm_state_waiting_for_config,	fsm_evt_ind_transport_disconnect,				fsm_state_disconnected,		&communication_disconnect_tx},
{fsm_state_waiting_for_config,	fsm_evt_ind_timeout,						fsm_state_unassociated,		&communication_abort_undefined_reason_tx},
{fsm_state_waiting_for_config,	fsm_evt_req_assoc_rel,						fsm_state_disassociating,	&configuring_association_release_request_tx},
{fsm_state_waiting_for_config,	fsm_evt_req_assoc_abort,					fsm_state_unassociated,		&communication_abort_undefined_reason_tx},
{fsm_state_waiting_for_config,	fsm_evt_rx_aarq,						fsm_state_unassociated,		&communication_abort_undefined_reason_tx},
{fsm_state_waiting_for_config,	fsm_evt_rx_aare,						fsm_state_unassociated,		&communication_abort_undefined_reason_tx},
{fsm_state_waiting_for_config,	fsm_evt_rx_rlrq,						fsm_state_unassociated,		&disassociating_release_response_tx},
{fsm_state_waiting_for_config,	fsm_evt_rx_rlre,						fsm_state_unassociated,		&communication_abort_undefined_reason_tx},
{fsm_state_waiting_for_config,	fsm_evt_rx_abrt,						fsm_state_unassociated,		NULL},
{fsm_state_waiting_for_config,	fsm_evt_rx_roiv_confirmed_event_report,				fsm_state_checking_config,	&configuring_perform_configuration},
{fsm_state_waiting_for_config,	fsm_evt_rx_roiv_event_report,					fsm_state_waiting_for_config,	&communication_roer_tx},
{fsm_state_waiting_for_config,	fsm_evt_rx_roiv_get,						fsm_state_waiting_for_config,	NULL},
{fsm_state_waiting_for_config,	fsm_evt_rx_roiv_set,						fsm_state_waiting_for_config,	NULL},
{fsm_state_waiting_for_config,	fsm_evt_rx_roiv_confirmed_set,					fsm_state_waiting_for_config,	NULL},
{fsm_state_waiting_for_config,	fsm_evt_rx_roiv_action,						fsm_state_waiting_for_config,	NULL},
{fsm_state_waiting_for_config,	fsm_evt_rx_roiv_confirmed_action,				fsm_state_waiting_for_config,	NULL},
{fsm_state_waiting_for_config,	fsm_evt_rx_rors,						fsm_state_waiting_for_config,	&communication_check_invoke_id_abort_tx},
{fsm_state_waiting_for_config,	fsm_evt_rx_roer,						fsm_state_waiting_for_config,	&communication_check_invoke_id_abort_tx},
{fsm_state_waiting_for_config,	fsm_evt_rx_rorj,						fsm_state_waiting_for_config,	&communication_check_invoke_id_abort_tx},
{fsm_state_waiting_for_config,	fsm_evt_req_agent_supplied_unknown_configuration,		fsm_state_unassociated,		&communication_abort_undefined_reason_tx},
{fsm_state_waiting_for_config,	fsm_evt_req_agent_supplied_known_configuration,			fsm_state_unassociated,		&communication_abort_undefined_reason_tx},
{fsm_state_checking_config,	fsm_evt_ind_transport_disconnect,				fsm_state_disconnected,		&communication_disconnect_tx},
{fsm_state_checking_config,	fsm_evt_ind_timeout,						fsm_state_unassociated,		&communication_abort_undefined_reason_tx},
{fsm_state_checking_config,	fsm_evt_req_assoc_rel,						fsm_state_disassociating,	&disassociating_release_request_tx},
{fsm_state_checking_config,	fsm_evt_req_assoc_abort,					fsm_state_unassociated,		&communication_abort_undefined_reason_tx},
{fsm_state_checking_config,	fsm_evt_rx_aarq,		fsm_state_unassociated,		&communication_abort_undefined_reason_tx},
{fsm_state_checking_config,	fsm_evt_rx_aarq_acceptable_and_known_configuration,		fsm_state_unassociated,		&communication_abort_undefined_reason_tx},
{fsm_state_checking_config,	fsm_evt_rx_aarq_acceptable_and_unknown_configuration,		fsm_state_unassociated,		&communication_abort_undefined_reason_tx},
{fsm_state_checking_config,	fsm_evt_rx_aarq_unacceptable_configuration,			fsm_state_unassociated,		&communication_abort_undefined_reason_tx},
{fsm_state_checking_config,	fsm_evt_rx_aare,						fsm_state_unassociated,		&communication_abort_undefined_reason_tx},
{fsm_state_checking_config,	fsm_evt_rx_rlrq,						fsm_state_unassociated,		&disassociating_release_response_tx},
{fsm_state_checking_config,	fsm_evt_rx_rlre,						fsm_state_unassociated,		&communication_abort_undefined_reason_tx},
{fsm_state_checking_config,	fsm_evt_rx_abrt,						fsm_state_unassociated,		NULL},
{fsm_state_checking_config,	fsm_evt_rx_roiv_confirmed_event_report,				fsm_state_checking_config,	&configuring_new_measurements_response_tx},
{fsm_state_checking_config,	fsm_evt_rx_roiv_all_except_confirmed_event_report,		fsm_state_unassociated,		&communication_roer_tx},
{fsm_state_checking_config,	fsm_evt_rx_rors_confirmed_event_report,				fsm_state_checking_config,	NULL},
{fsm_state_checking_config,	fsm_evt_rx_rors_get,						fsm_state_checking_config,	NULL},
{fsm_state_checking_config,	fsm_evt_rx_rors_confirmed_set,					fsm_state_checking_config,	NULL},
{fsm_state_checking_config,	fsm_evt_rx_rors_confirmed_action,				fsm_state_checking_config,	NULL},
{fsm_state_checking_config,	fsm_evt_rx_roer,						fsm_state_checking_config,	NULL},
{fsm_state_checking_config,	fsm_evt_rx_rorj,						fsm_state_checking_config,	NULL},
{fsm_state_checking_config,	fsm_evt_req_agent_supplied_unknown_configuration,		fsm_state_waiting_for_config,	&configuring_configuration_response_tx},
{fsm_state_checking_config,	fsm_evt_req_agent_supplied_known_configuration,			fsm_state_operating,		&configuring_configuration_response_tx},
{fsm_state_checking_config,	fsm_evt_req_agent_supplied_bad_configuration,			fsm_state_waiting_for_config,	&configuring_configuration_rorj_tx},
{fsm_state_operating,		fsm_evt_ind_transport_disconnect,				fsm_state_disconnected,		NULL},
{fsm_state_operating,		fsm_evt_ind_timeout,						fsm_state_unassociated,		&communication_abort_undefined_reason_tx},
{fsm_state_operating,		fsm_evt_req_assoc_rel,						fsm_state_disassociating,	&operating_assoc_release_req_tx},
{fsm_state_operating,		fsm_evt_req_assoc_abort,					fsm_state_unassociated,		&communication_abort_undefined_reason_tx},
{fsm_state_operating,		fsm_evt_rx_aarq,						fsm_state_unassociated,		&communication_abort_undefined_reason_tx},
{fsm_state_operating,		fsm_evt_rx_aare,						fsm_state_unassociated,		&communication_abort_undefined_reason_tx},
{fsm_state_operating,		fsm_evt_rx_rlrq,						fsm_state_unassociated,		&disassociating_release_response_tx},
{fsm_state_operating,		fsm_evt_rx_rlre,						fsm_state_unassociated,		&communication_abort_undefined_reason_tx},
{fsm_state_operating,		fsm_evt_rx_abrt,						fsm_state_unassociated,		NULL},
{fsm_state_operating,		fsm_evt_rx_roiv_confirmed_event_report,				fsm_state_operating,		&operating_event_report},
{fsm_state_operating,		fsm_evt_rx_roiv_event_report,					fsm_state_operating,		&operating_event_report},
{fsm_state_operating,		fsm_evt_rx_roiv_all_except_confirmed_event_report,		fsm_state_operating,		&operating_roiv_non_event_report},
{fsm_state_operating,		fsm_evt_rx_rors_confirmed_event_report,				fsm_state_operating,		NULL},
{fsm_state_operating,		fsm_evt_rx_rors_get,						fsm_state_operating,		&operating_get_response},
{fsm_state_operating,		fsm_evt_rx_rors_confirmed_set,					fsm_state_operating,		&operating_set_scanner_response},
{fsm_state_operating,		fsm_evt_rx_rors_confirmed_action,				fsm_state_operating,		&operating_rors_confirmed_action_tx},
{fsm_state_operating,		fsm_evt_rx_roer,						fsm_state_operating,		&operating_roer_confirmed_action_tx},
{fsm_state_operating,		fsm_evt_rx_rorj,						fsm_state_operating,		&operating_rorj_confirmed_action_tx},
{fsm_state_disassociating,	fsm_evt_ind_transport_disconnect,				fsm_state_disconnected,		NULL},
{fsm_state_disassociating,	fsm_evt_ind_timeout,						fsm_state_unassociated,		&communication_abort_undefined_reason_tx},
{fsm_state_disassociating,	fsm_evt_req_assoc_rel,						fsm_state_disassociating,	NULL},
{fsm_state_disassociating,	fsm_evt_req_assoc_abort,					fsm_state_unassociated,		&communication_abort_undefined_reason_tx},
{fsm_state_disassociating,	fsm_evt_rx_aarq,						fsm_state_unassociated,		&communication_abort_undefined_reason_tx},
{fsm_state_disassociating,	fsm_evt_rx_aare,						fsm_state_unassociated,		&communication_abort_undefined_reason_tx},
{fsm_state_disassociating,	fsm_evt_rx_rlrq,						fsm_state_disassociating,	&disassociating_release_response_tx},
{fsm_state_disassociating,	fsm_evt_rx_rlre,						fsm_state_unassociated,		&disassociating_release_proccess_completed},
{fsm_state_disassociating,	fsm_evt_rx_abrt,						fsm_state_unassociated,		NULL},
{fsm_state_disassociating,	fsm_evt_rx_roiv,						fsm_state_disassociating,	NULL},
{fsm_state_disassociating,	fsm_evt_rx_rors,						fsm_state_unassociated,		&communication_abort_undefined_reason_tx},
{fsm_state_disassociating,	fsm_evt_rx_roer,						fsm_state_unassociated,		&communication_abort_undefined_reason_tx},
{fsm_state_disassociating,	fsm_evt_rx_rorj,						fsm_state_unassociated,		&communication_abort_undefined_reason_tx},
{fsm_state_disassociating,	fsm_evt_req_agent_supplied_unknown_configuration,		fsm_state_unassociated,		&communication_abort_undefined_reason_tx},
{fsm_state_disassociating,	fsm_evt_req_agent_supplied_known_configuration,		fsm_state_unassociated,		&communication_abort_undefined_reason_tx},
};
// STOP HERE

/**
* Initialize fsm with the states and transition rules of
* IEEE 11073-20601 for Manager
*
* @param fsm
*/
void fsm_set_manager_state_table(FSM *fsm)
{
int transition_table_size = sizeof(IEEE11073_20601_manager_state_table);
int trasition_rule_size = sizeof(FsmTransitionRule);
int table_size = transition_table_size / trasition_rule_size;
fsm_init(fsm, fsm_state_disconnected, IEEE11073_20601_manager_state_table,
table_size);
}

/** @} */