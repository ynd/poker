#ifndef gamestate_h
#define gamestate_h 

#include "poker_defs.h"
#include "inlines/eval.h"
#include <string>

using std::string;

enum Game_Possible_State { PREFLOP, FLOP, TURN, RIVER };

class GameState {

public:
	Game_Possible_State current_state_;

	GameState() : current_state_(PREFLOP), pot(0) {
		CardMask_RESET(flop);
	}
	
	std::string get_state_string();
	CardMask flop;
	int pot;
};

#endif