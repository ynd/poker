#include "GameState.h"
#include <string>

using namespace std;

string GameState::get_state_string() {
	if (current_state_ == 0) { return "Preflop"; }
	if (current_state_ == 1) { return "Flop"; }
	if (current_state_ == 2) { return "Turn"; }
	if (current_state_ == 3) { return "River"; }
	else { return "Unknown state"; }
}
