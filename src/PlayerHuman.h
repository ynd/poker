#ifndef playerhuman_h
#define playerhuman_h

#include <string>

#include "GameState.h"
#include "PlayerState.h"

class PlayerHuman {
	int money_;
	std::string name_;
	
public:
	PlayerHuman(const std::string& name, int money) : name_(name), money_(money) {}

	int operator()(GameState& game_state, PlayerState& player_state, int minimum_money);
};


#endif 
