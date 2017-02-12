#ifndef USER_DEFINED_H
#define	USER_DEFINED_H

// DALI addresses
#define DA_HOVEDSOV 1
#define DA_KONTOR 4
#define DA_TV_STUE_OPPE 5
#define DA_BAD_OPPE 65
#define DA_BAD_NEDE 12
#define DA_VASKEROM 13
#define DA_GANG_NEDE 66

// Script indexes
#define SCRIPT_HOME 0
#define SCRIPT_HOME_HOBBY 1
#define SCRIPT_AWAY 2

script_entry_t scripts[10][SCRIPT_SIZE] = 
{
	// Script hjemme
	{
		{SCRIPT_DALI_LEVEL, DA_KONTOR, 0, 200},
		{SCRIPT_DALI_LEVEL, DA_TV_STUE_OPPE, 0, 200},
		{SCRIPT_DALI_LEVEL, DA_BAD_OPPE, 0, 200},
		{SCRIPT_DALI_LEVEL, DA_GANG_NEDE, 0, 200},
		{SCRIPT_RELAY_ON, 2, 3, R6},	// Skråvegg
		{SCRIPT_RELAY_ON, 3, 3, R3},	// Stikk TV oppe/nede
		{SCRIPT_RELAY_ON, 8, 3, R1|R6},	// Stuevegg, kjøkkenstrøm
		{SCRIPT_RELAY_ON, 9, 3, R5|R6},	// kjøkkenøy og -benk
		// hapcan_control_dimmer(10,3,70); // Spisebord 70%
		{SCRIPT_FINISHED, 0, 0, 0}
	},
	// Script hjemme, via sportsbod
	{
		{SCRIPT_DALI_LEVEL, DA_KONTOR, 0, 200},
		{SCRIPT_DALI_LEVEL, DA_TV_STUE_OPPE, 0, 200},
		{SCRIPT_DALI_LEVEL, DA_BAD_OPPE, 0, 200},
		{SCRIPT_DALI_LEVEL, DA_GANG_NEDE, 0, 200},
		{SCRIPT_DALI_LEVEL, DA_VASKEROM, 0, 200},
		{SCRIPT_RELAY_ON, 2, 3, R2|R6},	// Sportsbod, skråvegg
		{SCRIPT_RELAY_ON, 3, 3, R3},	// Stikk TV oppe/nede
		{SCRIPT_RELAY_ON, 8, 3, R1|R6},	// Stuevegg, kjøkkenstrøm
		{SCRIPT_RELAY_ON, 9, 3, R5|R6},	// kjøkkenøy og -benk
		// hapcan_control_dimmer(10,3,70); // Spisebord 70%
		{SCRIPT_FINISHED, 0, 0, 0}
	},
	// Script borte
	{
		{SCRIPT_DALI_LEVEL, DA_HOVEDSOV, 0, 0},
		{SCRIPT_DALI_LEVEL, DA_KONTOR, 0, 0},
		{SCRIPT_DALI_LEVEL, DA_TV_STUE_OPPE, 0, 0},
		{SCRIPT_DALI_LEVEL, DA_BAD_OPPE, 0, 0},
		{SCRIPT_DALI_LEVEL, DA_BAD_NEDE, 0, 0},
		{SCRIPT_DALI_LEVEL, DA_VASKEROM, 0, 0},
		{SCRIPT_DALI_LEVEL, DA_GANG_NEDE, 0, 0},
		{SCRIPT_RELAY_OFF, 2, 3, R1|R2|R3|R4|R5|R6},// Sov sør, sportsbod, sov nord, teknisk, speil bad, skråvegg
		{SCRIPT_RELAY_OFF, 3, 3, R3},				// Stikk TV oppe/nede
		{SCRIPT_RELAY_OFF, 8, 3, R1|R3|R6},			// Stuevegg, speil bad oppe, kjøkkenstrøm
		{SCRIPT_RELAY_OFF, 9, 3, R2|R5|R6},			// Hovedsov, kjøkkenøy og -benk
		{SCRIPT_DIM_LEVEL, 10, 3, 0},				// Spisebord
		{SCRIPT_FINISHED, 0, 0, 0}
	}
};

memcell_t memcells[MEMCELLS_SIZE] = {
	// Node, group, button, dali address, action type
	
	// 1. etg.
	{1, 1, 2, ACTION_TOUCHDIM, 1, 0},  // Hovedsoverom 1
	{2, 1, 2, ACTION_TOUCHDIM, 1, 0},  // Hovedsoverom 2
	{3, 1, 1, ACTION_TOUCHDIM, 65, 0}, // Bad
	{4, 1, 3, ACTION_TOUCHDIM, 5, 0},  // Stue
	{5, 1, 1, ACTION_TOUCHDIM, 4, 0},  // Kontor
	
	// U. etg.
	{8, 1, 3, ACTION_TOUCHDIM, 66, 0},  // Gang nede
	{12, 1, 1, ACTION_TOUCHDIM, 13, 0},  // Vaskerom
	{11, 1, 1, ACTION_TOUCHDIM, 12, 0}, // Bad nede
	
	// Hovedsoverom 2, knapp 1
	{2, 1, 1, ACTION_SCRIPT, SCRIPT_AWAY, CLOSED_AND_HELD_4S},
	{2, 1, 1, ACTION_SCRIPT, SCRIPT_HOME, OPEN_BETWEEN_400MS_AND_4S} ,

	// Hovedsoverom 1, knapp 1
	{1, 1, 1, ACTION_SCRIPT, SCRIPT_AWAY, CLOSED_AND_HELD_4S},
	{1, 1, 1, ACTION_SCRIPT, SCRIPT_HOME, OPEN_BETWEEN_400MS_AND_4S},
	
	// Hovedinngang, knapp 3
	{7, 1, 3, ACTION_SCRIPT, SCRIPT_AWAY, CLOSED_AND_HELD_4S},
	{7, 1, 3, ACTION_SCRIPT, SCRIPT_HOME, OPEN_BETWEEN_400MS_AND_4S},
	{7, 1, 3, ACTION_SCRIPT, SCRIPT_HOME, OPEN_WITHIN_400MS},
	
	// Inngang, sportsbod, knapp 1
	{14, 1, 1, ACTION_SCRIPT, SCRIPT_AWAY, CLOSED_AND_HELD_4S},
	{14, 1, 1, ACTION_SCRIPT, SCRIPT_HOME_HOBBY, OPEN_BETWEEN_400MS_AND_4S},
	{14, 1, 1, ACTION_SCRIPT, SCRIPT_HOME_HOBBY, OPEN_WITHIN_400MS}
};

#endif
