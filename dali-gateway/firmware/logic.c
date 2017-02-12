#include "logic.h"

#include "dali.h"
#include "can.h"
#include "uart.h"

// HAPCAN frame ID struct, 32 bits
typedef struct {
	unsigned int group : 8;
	unsigned int node : 8;
	unsigned int response_flag : 1;
	unsigned int frame_type : 12;
	unsigned int : 3;
} hapcan_id_t;

// Memory cell contains HAPCAN node, group and button channel for button modules specifically
typedef struct{
	uint8_t hc_node_nr;
	uint8_t hc_group_nr;
	uint8_t hc_button;
	uint8_t action_type;
	uint8_t dali_addr_or_script;
	uint8_t script_button_action;
}memcell_t;

// Script entry
typedef struct{
	uint8_t script_type;
	uint8_t dali_addr_or_hapcan_node;
	uint8_t hapcan_group;
	uint8_t channel_or_level;
}script_entry_t;

#define SCRIPT_FINISHED 0
#define SCRIPT_DALI_LEVEL 1
#define SCRIPT_RELAY_ON   2
#define SCRIPT_RELAY_OFF  3
#define SCRIPT_DIM_LEVEL  4

// HAPCAN channels for relay direct control
#define R1 0x01
#define R2 0x02
#define R3 0x04
#define R4 0x08
#define R5 0x10
#define R6 0x20

// HAPCAN frame types (just a few are used for this device)
#define FRAME_TYPE_BUTTON 0x301
#define FRAME_TYPE_DALI_DIMMER 0x322 // TYPE NOT COORDINATED - just for testing
#define FRAME_TYPE_DIRECT_CONTROL 0x10a

// HAPCAN button frame values
#define OPEN 0x00
#define CLOSED 0xff
#define OPEN_WITHIN_400MS 0xfc
#define CLOSED_AND_HELD_400MS 0xfe
#define CLOSED_AND_HELD_4S 0xfd
#define OPEN_BETWEEN_400MS_AND_4S 0xfb

// HAPCAN address
#define THIS_NODE 1
#define THIS_GROUP 2

// HAPCAN direct control instructions
#define INSTRUCTION_SET_TO 0x00
#define INSTRUCTION_TOGGLE 0x01

// Remember states and dim levels as DALI doesn't have a toggle command
// Address 0-63 = short, 64-79 = group, 80 = broadcast
#define DALI_STATES_SIZE 81

#define DALI_STATE_ON_bm			0x01
#define DALI_STATE_DIM_UP_bm		0x02
#define DALI_STATE_DIM_DOWN_bm		0x04
#define DALI_STATE_DIM_LAST_UP_bm	0x08

uint8_t dali_states[DALI_STATES_SIZE]; // 64 short addresses, 16 groups
uint8_t dali_dimlevel[DALI_STATES_SIZE]; // 64 short addresses, 16 groups

#define ACTION_TOUCHDIM 0
#define ACTION_SCRIPT 1

#define MEMCELLS_SIZE 64
#define SCRIPT_SIZE 32
// memcell_t memcells[MEMCELLS_SIZE];

#include "user_defined.h"

// Convert DALI address to array index for dali_states and dali_dimlevel
uint8_t dali_channel_to_index(uint8_t channel){
	if (channel == 127) return 80;
	else return channel;
}

uint8_t index_to_dali_channel(uint8_t index){
	if (index == 80) return 127;
	else return index;
}

// HAPCAN level 0 is off, and dimming range is 1-255
// DALI dim level is limited, eg. 149-254, and doesn't reserve a value for off
uint8_t dali_level_to_hapcan(uint8_t dali_level){
	return ((uint32_t)dali_level - DIM_LOWER) * 0xfe / (DIM_UPPER-DIM_LOWER) + 1;
}

uint8_t hapcan_level_to_dali(uint8_t hc_level){
	return ((uint32_t)hc_level-1) * (DIM_UPPER-DIM_LOWER) / 0xfe + DIM_LOWER;
}

void logic_init(void){
	uint8_t j;
	for (j=0; j<DALI_STATES_SIZE; j++){
		dali_dimlevel[j] = DIM_UPPER;
		// Will start dimming down on first dim action
		dali_states[j] |= DALI_STATE_DIM_LAST_UP_bm; 
	}
}

void hapcan_control_relay(uint8_t module, uint8_t group, uint8_t channel, uint8_t state){
	uint32_t id_buffer;
	hapcan_id_t* id = (hapcan_id_t*)&id_buffer;
	id->frame_type = FRAME_TYPE_DIRECT_CONTROL;
	id->response_flag = 0; // Should be 1 if this was a request
	id->node = THIS_NODE;
	id->group = THIS_GROUP;
	
	CANTxMessageBuffer* msg = can_tx_prepare_buffer();

	msg->CMSGEID.IDE = 1; // Enable EID
	msg->CMSGEID.DLC = 8; // Length 8 byte

	msg->CMSGEID.EID = id_buffer & 0x0003ffff;
	msg->CMSGSID.SID = (id_buffer & 0x1ffc0000) >> 18;
	
	msg->CMSGDATA0.Byte0 = state;
	msg->CMSGDATA0.Byte1 = channel;
	msg->CMSGDATA0.Byte2 = module;
	msg->CMSGDATA0.Byte3 = group;
	msg->CMSGDATA1.Byte4 = 0; // Delay
	msg->CMSGDATA1.Byte5 = 0xff;
	msg->CMSGDATA1.Byte6 = 0xff;
	msg->CMSGDATA1.Byte7 = 0xff;
	
	can_tx_finish_buffer();
}

void hapcan_control_dimmer(uint8_t module, uint8_t group, uint8_t level){
	uint32_t id_buffer;
	hapcan_id_t* id = (hapcan_id_t*)&id_buffer;
	id->frame_type = FRAME_TYPE_DIRECT_CONTROL;
	id->response_flag = 0; // Should be 1 if this was a request
	id->node = THIS_NODE;
	id->group = THIS_GROUP;
	
	CANTxMessageBuffer* msg = can_tx_prepare_buffer();

	msg->CMSGEID.IDE = 1; // Enable EID
	msg->CMSGEID.DLC = 8; // Length 8 byte

	msg->CMSGEID.EID = id_buffer & 0x0003ffff;
	msg->CMSGSID.SID = (id_buffer & 0x1ffc0000) >> 18;
	
	msg->CMSGDATA0.Byte0 = 0; // Set to
	msg->CMSGDATA0.Byte1 = level;
	msg->CMSGDATA0.Byte2 = module;
	msg->CMSGDATA0.Byte3 = group;
	msg->CMSGDATA1.Byte4 = 0; // Delay
	msg->CMSGDATA1.Byte5 = 0xff;
	msg->CMSGDATA1.Byte6 = 0xff;
	msg->CMSGDATA1.Byte7 = 0xff;
	
	can_tx_finish_buffer();
}

void send_hapcan_dimmer_frame(uint8_t channel, uint8_t hapcan_level){
	uint32_t id_buffer;
	hapcan_id_t* id = (hapcan_id_t*)&id_buffer;
	id->frame_type = FRAME_TYPE_DALI_DIMMER;
	id->response_flag = 0; // Should be 1 if this was a request
	id->node = THIS_NODE;
	id->group = THIS_GROUP;
	
	CANTxMessageBuffer* msg = can_tx_prepare_buffer();

	msg->CMSGEID.IDE = 1; // Enable EID
	msg->CMSGEID.DLC = 8; // Length 8 byte

	msg->CMSGEID.EID = id_buffer & 0x0003ffff;
	msg->CMSGSID.SID = (id_buffer & 0x1ffc0000) >> 18;
	
	msg->CMSGDATA0.Byte0 = 0xff;
	msg->CMSGDATA0.Byte1 = 0xff;
	msg->CMSGDATA0.Byte2 = channel;
	msg->CMSGDATA0.Byte3 = hapcan_level;
	msg->CMSGDATA1.Byte4 = 0xff;
	msg->CMSGDATA1.Byte5 = 0xff;
	msg->CMSGDATA1.Byte6 = 0xff;
	msg->CMSGDATA1.Byte7 = 0x00;
	
	can_tx_finish_buffer();
}

// Will keep track of touch dimming and send DALI packets continously
void logic_tick(void){
	int index;
	for (index=0; index<DALI_STATES_SIZE; index++){
		
		// If dimming upwards, increase and continue dimming
		if (dali_states[index] & DALI_STATE_DIM_UP_bm){
			uint8_t dali_address = index_to_dali_channel(index);
			if ((dali_dimlevel[index] + DIM_STEP) < DIM_UPPER) dali_dimlevel[index] += DIM_STEP;
			else if (dali_dimlevel[index] != DIM_UPPER) dali_dimlevel[index] = DIM_UPPER;
			dali_tx_append((dali_address << DALI_ADDRESS_bp) | dali_dimlevel[index]);
			
			// If upper dim level is reached, stop sending, and report new level
			if (dali_dimlevel[index] == DIM_UPPER){
				dali_states[index] &= ~DALI_STATE_DIM_UP_bm;
				send_hapcan_dimmer_frame(dali_address, dali_level_to_hapcan(dali_dimlevel[index]));
			}
		}
		
		else if (dali_states[index] & DALI_STATE_DIM_DOWN_bm){
			uint8_t dali_address = index_to_dali_channel(index);
			if ((dali_dimlevel[index] - DIM_STEP) > DIM_LOWER) dali_dimlevel[index] -= DIM_STEP;
			else if (dali_dimlevel[index] != DIM_LOWER) dali_dimlevel[index] = DIM_LOWER;
			dali_tx_append((dali_address << DALI_ADDRESS_bp) | dali_dimlevel[index]);
			
			// If lower dim level is reached, stop sending, and report new level
			if (dali_dimlevel[index] == DIM_LOWER){
				dali_states[index] &= ~DALI_STATE_DIM_DOWN_bm;
				send_hapcan_dimmer_frame(dali_address, dali_level_to_hapcan(dali_dimlevel[index]));
			}
		}
	}
}

void dali_on(uint8_t dali_address){
	uint8_t index = dali_channel_to_index(dali_address);
	dali_tx_append((dali_address << DALI_ADDRESS_bp) | dali_dimlevel[index]);
	dali_states[index] |= DALI_STATE_ON_bm;
	send_hapcan_dimmer_frame(dali_address, dali_level_to_hapcan(dali_dimlevel[index]));
}

void dali_off(uint8_t dali_address){
	uint8_t index = dali_channel_to_index(dali_address);
	dali_tx_append((dali_address << DALI_ADDRESS_bp) | DALI_NOT_DIRECT_ARC | DALI_CMD_OFF);
	dali_states[index] &= ~DALI_STATE_ON_bm;
	send_hapcan_dimmer_frame(dali_address, 0);
}

void dali_level(uint8_t dali_address, uint8_t hapcan_level){
	uint8_t index = dali_channel_to_index(dali_address);
	if (hapcan_level == 0) dali_off(dali_address);
	else{
		uint8_t dali_level = hapcan_level_to_dali(hapcan_level);
		dali_tx_append((dali_address << DALI_ADDRESS_bp) | dali_level);
		dali_states[index] |= DALI_STATE_ON_bm;
		dali_dimlevel[index] = dali_level;
		send_hapcan_dimmer_frame(dali_address, hapcan_level);
	}
}

void dali_toggle(uint8_t dali_address){
	uint8_t index = dali_channel_to_index(dali_address);
	if (index >= DALI_STATES_SIZE) return;

	if (dali_states[index] & DALI_STATE_ON_bm) dali_off(dali_address);
	else dali_on(dali_address);
}

void touchdim_dali(uint8_t button_action, uint8_t dali_address){
	uint8_t index = dali_channel_to_index(dali_address);
	if (index >= DALI_STATES_SIZE) return;
	
	switch(button_action){
		// If state is off, turn on immediately on button press, 
		// but don't update state yet
		case CLOSED:
			if (!(dali_states[index] & DALI_STATE_ON_bm))
				dali_tx_append((dali_address << DALI_ADDRESS_bp) | dali_dimlevel[index]);
			break;
		
		// Turn off if releasing within 400 ms and state is on,
		// and update state in both cases
		case OPEN_WITHIN_400MS:
			if (dali_states[index] & DALI_STATE_ON_bm) dali_off(dali_address);
			else{
				dali_states[index] |= DALI_STATE_ON_bm;
				send_hapcan_dimmer_frame(dali_address, dali_level_to_hapcan(dali_dimlevel[index]));
			}
			break;
			
		// React to simple button press with matching memory cell
		case CLOSED_AND_HELD_400MS:
			if (!(dali_states[index] & DALI_STATE_ON_bm))
				dali_states[index] |= DALI_STATE_ON_bm;
			if (dali_states[index] & DALI_STATE_DIM_LAST_UP_bm){
				dali_states[index] &= ~DALI_STATE_DIM_LAST_UP_bm;
				dali_states[index] &= ~DALI_STATE_DIM_UP_bm;
				dali_states[index] |= DALI_STATE_DIM_DOWN_bm;
			}
			else{
				dali_states[index] |= DALI_STATE_DIM_LAST_UP_bm;
				dali_states[index] |= DALI_STATE_DIM_UP_bm;
				dali_states[index] &= ~DALI_STATE_DIM_DOWN_bm;
			}
			break;

		case OPEN:
			// If coming from from dimming and did not reach end value
			if (dali_states[index]&(DALI_STATE_DIM_DOWN_bm|DALI_STATE_DIM_UP_bm)){
				send_hapcan_dimmer_frame(dali_address, dali_level_to_hapcan(dali_dimlevel[index]));
				dali_states[index] &= ~(DALI_STATE_DIM_DOWN_bm|DALI_STATE_DIM_UP_bm);
			}
			break;

	}
}

//void absolute_state(uint8_t button_action, uint8_t dali_address){
//	uint8_t index = dali_channel_to_index(dali_address);
//	if (index >= DALI_STATES_SIZE) return;
//	
//	switch(button_action){		
//		// React to simple button press with matching memory cell
//		case OPEN_BETWEEN_400MS_AND_4S:
//			dali_states[index] = DALI_STATE_ON_bm;
//		
//			break;
//	}
//}

//		{SCRIPT_DALI_LEVEL, DA_HOVEDSOV, 0, 0},
//		{SCRIPT_DALI_LEVEL, DA_KONTOR, 0, 0},
//		{SCRIPT_DALI_LEVEL, DA_TV_STUE_OPPE, 0, 0},
//		{SCRIPT_DALI_LEVEL, DA_BAD_OPPE, 0, 0},
//		{SCRIPT_DALI_LEVEL, DA_BAD_NEDE, 0, 0},
//		{SCRIPT_DALI_LEVEL, DA_VASKEROM, 0, 0},
//		{SCRIPT_DALI_LEVEL, DA_GANG_NEDE, 0, 0},
//		{SCRIPT_RELAY_OFF, 2, 3, R1|R2|R3|R4|R5|R6},// Sov sør, sportsbod, sov nord, teknisk, speil bad, skråvegg
//		{SCRIPT_RELAY_OFF, 3, 3, R3},				// Stikk TV oppe/nede
//		{SCRIPT_RELAY_OFF, 8, 3, R1|R3|R6},			// Stuevegg, speil bad oppe, kjøkkenstrøm
//		{SCRIPT_RELAY_OFF, 9, 3, R2|R5|R6},			// Hovedsov, kjøkkenøy og -benk
//		{SCRIPT_DIM_LEVEL, 10, 3, 0},				// Spisebord
//		{SCRIPT_FINISHED, 0, 0, 0}
		
void logic_run_script(uint8_t script_nr){
	int line;
	for (line=0; line<SCRIPT_SIZE; line++){
		script_entry_t s = scripts[script_nr][line];
		switch (s.script_type){
			case SCRIPT_DALI_LEVEL:
				dali_level(s.dali_addr_or_hapcan_node, s.channel_or_level);
				break;
			case SCRIPT_RELAY_OFF:
				hapcan_control_relay(s.dali_addr_or_hapcan_node, s.hapcan_group, s.channel_or_level, 0);
				break;
			case SCRIPT_RELAY_ON:
				hapcan_control_relay(s.dali_addr_or_hapcan_node, s.hapcan_group, s.channel_or_level, 1);
				break;
			case SCRIPT_DIM_LEVEL:
				hapcan_control_dimmer(s.dali_addr_or_hapcan_node, s.hapcan_group, s.channel_or_level);
				break;		
			case SCRIPT_FINISHED:
				line = SCRIPT_SIZE;
				break;		
		}
	}
}


void logic_handle_can_msg(CANRxMessageBuffer* msg){
	uint32_t full_id = (msg->CMSGSID.SID << 18) | msg->CMSGEID.EID;
	hapcan_id_t* id = (hapcan_id_t*)&full_id;
	
	//uart_printf(1, "mod=%02x grp=%02x type=%03x r_flag=%u len=%u\r\n", id->node, id->group, id->frame_type, id->response_flag, msg->CMSGEID.DLC);
	
	// Search memory cells for indirect control from button devices
	// (not addressed to this group or node)
	if (id->frame_type == FRAME_TYPE_BUTTON){
		uint8_t j;
		for (j=0; j<MEMCELLS_SIZE; j++){
			// If matching memory cell
			if ((memcells[j].hc_group_nr == id->group) &&
				(memcells[j].hc_node_nr == id->node) &&
				(memcells[j].hc_button == msg->CMSGDATA0.Byte2)){

				switch (memcells[j].action_type){
					case ACTION_TOUCHDIM:
						touchdim_dali(msg->CMSGDATA0.Byte3, memcells[j].dali_addr_or_script);
						break;
						
					case ACTION_SCRIPT:
						// Run script if correct button action
						if (msg->CMSGDATA0.Byte3 == memcells[j].script_button_action)
							logic_run_script(memcells[j].dali_addr_or_script);
						break;
				}
			}
		}
	}
	
	// Check if packet is at least addressed to this group
	else if (msg->CMSGDATA0.Byte3 == THIS_GROUP){
		switch (id->frame_type){
			hapcan_id_t tx_id;
			tx_id.frame_type = 0x103;
			tx_id.response_flag = 1;
			tx_id.node = THIS_NODE;
			tx_id.group = THIS_GROUP;
			
		//	(msg->CMSGSID.SID << 18) | msg->CMSGEID.EID;
		
		//	case 0x103: // HW request to group
//				/* At this point the messages are loaded in FIFO0 */
//				CANTxMessageBuffer* transmitMessage;
//				transmitMessage = (CANTxMessageBuffer *)(PA_TO_KVA1(C1FIFOUA0));
//				transmitMessage->CMSGSID.SID = 0x100; /* CMSGSID */
//				transmitMessage->CMSGEID.IDE = 0;
//				transmitMessage->CMSGEID.DLC = 0x8;
//				transmitMessage->messageWord[2] = 0x12BC1245; /* CMSGDAT0 */
//				C1FIFOCON0SET = 0x2008; // Set UINC and TXREQ
//			}
		}

		// Check if packet is addressed to this node exactly
		if (msg->CMSGDATA0.Byte2 == THIS_NODE){
			switch (id->frame_type){
				// HAPCAN direct control frame
				case FRAME_TYPE_DIRECT_CONTROL:
					{
						uint8_t dali_address = msg->CMSGDATA1.Byte4;
						if (dali_channel_to_index(dali_address) >= DALI_STATES_SIZE) break;

						switch(msg->CMSGDATA0.Byte0){
							// Set to
							case INSTRUCTION_SET_TO:
								dali_level(dali_address, msg->CMSGDATA0.Byte1);
								break;

							// Toggle
							case INSTRUCTION_TOGGLE:
								dali_toggle(dali_address);
								break;

//									// RAW DALI message to be sent to DALI bus
//									uint16_t dali_msg = msg->CMSGDATA0.Byte1 | (msg->CMSGDATA0.Byte2 << 8);
//									// Send to DALI bus
//									uart_printf(1, "Direct control message %04x to DALI\r\n", dali_msg);
						}
					}
					break;
			}
		}
	}
}
