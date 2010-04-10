#include "PlayerHuman.h"
#include "GameState.h"
#include "PlayerState.h"

#include <iostream>
#include <sstream>

#include "poker_defs.h"
#include "inlines/eval.h"

using namespace std;

int PlayerHuman::operator()(GameState& game_state, PlayerState& player_state, int minimum_money) {
	CardMask cards;
	
	CardMask_OR(cards, game_state.flop, player_state.cards());	
	
	cout << endl;
	cout << "Player " << player_state.get_number() << " (min/max : " << minimum_money << "/" << player_state.get_money() << ")";
	
	if (StdDeck_numCards(cards) >= 5) {
		cout << "  [ "; 
		int eval = StdDeck_StdRules_EVAL_N(cards, StdDeck_numCards(cards));
		HandVal_print(eval); 
		cout << " ]";
	}
	cout << endl;                
	
	cout << "(-1 to fold, 0 to check, number to raise)" << endl;
	cout << " > ";
	
	
	
	int x;
	cin >> x; 
	cout << endl;
	
	return x;
}
