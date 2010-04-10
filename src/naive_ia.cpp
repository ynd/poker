#include <vector>
#include "TheGame.h"
#include "PlayerHuman.h"
#include "GameState.h"
#include "PlayerState.h"
#include "poker_defs.h"
#include "inlines/eval.h"
#include "random.h"

using namespace std;

int random_player(GameState& game_state, PlayerState& player_state, int minimum_money) {
	if (get_random(10) < 5) { return -1; }
	int pourcent = get_random(100); 
	
	int bet = player_state.get_money() * pourcent / 100;
	return max(bet, minimum_money);
}

bool mask_in_mask(CardMask a, CardMask b) {
	CardMask temp;
	CardMask_RESET(temp);

	CardMask_AND(temp, a, b);
	return StdDeck_numCards(temp) > 0;
}

int conservateur(GameState& game_state, PlayerState& player_state, int minimum_money) {
	vector<CardMask> ok;
	
	// we play only A and K
	ok.push_back(StdDeck_MASK(StdDeck_MAKE_CARD(12,0))); 
	ok.push_back(StdDeck_MASK(StdDeck_MAKE_CARD(12,1))); 
	ok.push_back(StdDeck_MASK(StdDeck_MAKE_CARD(12,2))); 
	ok.push_back(StdDeck_MASK(StdDeck_MAKE_CARD(12,3))); 

	ok.push_back(StdDeck_MASK(StdDeck_MAKE_CARD(11,0))); 
	ok.push_back(StdDeck_MASK(StdDeck_MAKE_CARD(11,1))); 
	ok.push_back(StdDeck_MASK(StdDeck_MAKE_CARD(11,2))); 
	ok.push_back(StdDeck_MASK(StdDeck_MAKE_CARD(11,3))); 

	int count = 0;

	for(int i = 0; i < ok.size(); i++) {
		if (mask_in_mask(ok[i], player_state.cards())) {
			count++;
		}
	}

	if (count >= 2) { 
		return player_state.get_money(); 
	}
	
	return -1;
}

int elaborate_player(GameState& game_state, PlayerState& player_state, int minimum_money) {
	
	// compare to a pair of 2 and return a bet proportionnal to the difference of value of his hand	
	CardMask pairHand;
 	CardMask_OR(pairHand, StdDeck_MASK(StdDeck_MAKE_CARD(1,0)), StdDeck_MASK(StdDeck_MAKE_CARD(1,1)));
	
	float eval = Hand_EVAL_N(pairHand,2) / Hand_EVAL_N(player_state.cards(),StdDeck_numCards(player_state.cards()));
	if(eval<=1)
	{
	  float diff = 1 - eval; 
	  return player_state.get_money() * diff; 
	}
	
	return -1;

}
