#include "PlayerState.h"

#include "poker_defs.h"
#include "inlines/eval.h"

ostream& operator<<(ostream& output, const PlayerState& player_state) {	
	output << "Player " << player_state.number_ << " has " << player_state.money_ << "$";
	
	return output;
}

void PlayerState::reset_cards() {
	CardMask_RESET(cards_);
}
