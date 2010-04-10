#ifndef round_h
#define round_h

#include <iostream>
#include <deque>

#include "PlayerState.h"
#include "GameState.h"
#include "random.h"
#include "poker_defs.h"
#include "inlines/eval.h"
#include "random.h"

using namespace std;

template <class IA_A, class IA_B>
class Round {

	CardMask all_cards_;
	
	pair<int,int> blinds_;

	deque<PlayerState*> player_real_order_;
	
	IA_A agent_a_;
	IA_B agent_b_;
	
	bool first_round;

	void add_blinds(GameState& game_state, deque<PlayerState*> player_order);
	pair<PlayerState*, int> analyse_winner(GameState& game_state, deque<PlayerState*> player_order);

	int round_betting(GameState& game_state, deque<PlayerState*> player_order, bool preflop);

	int get_money(int minimum_bet, int maximum_bet, GameState& game_state, PlayerState* player_state);

	pair<PlayerState*,int> play(deque<PlayerState*> player_order);

 	void generate_card(CardMask& all_cards, CardMask& player);

	void generate_cards(GameState& game_state, deque<PlayerState*> player_order);

	string get_state(int);
	
public:
	Round(IA_A agent_a, IA_B agent_b, PlayerState& player_state_a, PlayerState& player_state_b, pair<int,int> blinds) :
		agent_a_(agent_a),
		agent_b_(agent_b),
		first_round(true), 
		blinds_(blinds)
	{
		player_real_order_.push_back(&player_state_a);
		player_real_order_.push_back(&player_state_b);
	}

	void start_round();
};


template <class IA_A, class IA_B>
void Round<IA_A, IA_B>::start_round() {
	if (!first_round) {
		player_real_order_.push_back(player_real_order_.front());
		player_real_order_.pop_front();
	}
	first_round = false;
	
	for (int i = 0; i < player_real_order_.size(); i++) {
		player_real_order_[i]->set_dealer(i == 0);
	}
	
	pair<PlayerState*,int> result = play(player_real_order_);
	
	// draw game
	if (result.first == (PlayerState*)0) {
		//cout << "Draw!" << endl << endl << endl;
		player_real_order_[0]->cash(result.second/2);
		player_real_order_[1]->cash(result.second/2);
		
	} else {
		//cout << result.first->get_number() << " takes pot of: " << result.second << endl << endl << endl;
		result.first->cash(result.second);
	}
}

template <class IA_A, class IA_B>
pair<PlayerState*, int> Round<IA_A, IA_B>::play(deque<PlayerState*> player_order) {
	
	GameState game_state; 
	CardMask_RESET(all_cards_);
	for (int i = 0; i < player_order.size(); i++) {
		player_order[i]->reset_cards();
	}
	
	add_blinds(game_state, player_order);
	
	for (int i = 0; i < 4; i++) {
		game_state.current_state_ = (Game_Possible_State)i;
		
		//cout << endl;
		//cout << "State: " << game_state.get_state_string() << endl;
		//cout << "Pot: " << game_state.pot << "$" << endl;
		
		generate_cards(game_state, player_order);
		//cout << endl;
		
		int result = round_betting(game_state, player_order, i==0);
		
		// someone has fold (or failed to raise correcly)
		if (result != -1) {
			return make_pair(player_order[result], game_state.pot);
		}
		
		if (i == 0) {
			// rotate order after preflop
			player_order.push_back(player_order.front()); 
			player_order.pop_front();
		}
	}

	return analyse_winner(game_state, player_order);
}

template <class IA_A, class IA_B>
pair<PlayerState*, int>  Round<IA_A, IA_B>::analyse_winner(GameState& game_state, deque<PlayerState*> player_order) {
	CardMask player1, player2;

	CardMask_OR(player1, game_state.flop, player_order[0]->cards());	
	CardMask_OR(player2, game_state.flop, player_order[1]->cards());	

	int result_1 = Hand_EVAL_N(player1, 7);
	int result_2 = Hand_EVAL_N(player2, 7);
	
	//cout << player_order[0]->get_number() << " ";
	//HandVal_print(result_1);                  
	//cout << endl;
    
	//cout << player_order[1]->get_number() << " ";
	//HandVal_print(result_2);                  
	//cout << endl;
    

	if (result_1 > result_2) {
		return make_pair(player_order[0], game_state.pot);
	} else if (result_1 < result_2) {
		return make_pair(player_order[1], game_state.pot);
	} else {
		return make_pair((PlayerState*)0, game_state.pot);
	}
}

template <class IA_A, class IA_B>
void Round<IA_A, IA_B>::add_blinds(GameState& game_state, deque<PlayerState*> player_order) {
	int small_blind = blinds_.first;
	int big_blind = blinds_.second; 
	
	int minimum_bet = big_blind - small_blind;
	
	// dealer pay small blinds if he can, else he loses.
	if (player_order[0]->get_money() >= small_blind) {
		player_order[0]->pay(small_blind);
		game_state.pot += small_blind;
		
		//cout << player_order[0]->get_number() << " pay small blind(" << small_blind << ")" << endl;
	} else {
		int max_money = player_order[0]->get_money();
		player_order[0]->pay(max_money);
		game_state.pot += max_money;
		
		//cout << player_order[0]->get_number() << " can't pay small blind, so he's all-in (" << max_money << ")" << endl;
	}

	// other player pay big blinds if he can, else he loses.
	if (player_order[1]->get_money() >= big_blind) {
		player_order[1]->pay(big_blind);
		game_state.pot += big_blind;

		//cout << player_order[1]->get_number() << " pay big blind(" << big_blind << ")" << endl;
	} else {
		int max_money = player_order[1]->get_money();
		player_order[1]->pay(max_money);
		game_state.pot += max_money;
		
		//cout << player_order[1]->get_number() << " can't pay big blind, so he's all-in (" << max_money << ")" << endl;
	}
}

template <class IA_A, class IA_B>
int Round<IA_A, IA_B>::round_betting(GameState& game_state, deque<PlayerState*> player_order, bool preflop) {
	deque<int> to_play;
	to_play.push_back(0); // 0 is first player
	to_play.push_back(1); // 1 is second player
	
	int to_bet = 0;
	if (preflop) {
		to_bet = min(player_order[0]->get_money(), blinds_.second - blinds_.first);	
	}
	
	while (!to_play.empty()) {
		int who = to_play.front();
		int other = (who == 0 ? 1 : 0);
		to_play.pop_front();
		
		int bet = get_money(to_bet, player_order[other]->get_money(), game_state, player_order[who]);

		to_bet = bet - to_bet; 

		if (to_bet < 0) {
			//cout << "* " << player_order[who]->get_number() << " fold." << endl;
			return other;
		}
		
		//cout << "* " << player_order[who]->get_number() << " bet: " << bet << endl;
		game_state.pot += bet; 

		if (to_bet == 0) {
			if (to_play.empty()) {
				return -1;
			}
		} 
		
		else {
			if (to_play.empty()) {
				to_play.push_back(other);
			}
		}
	}
}

template <class IA_A, class IA_B>
int Round<IA_A, IA_B>::get_money(int minimum_bet, int other_money, GameState& game_state, PlayerState* player_state) {
	int bet = -1;

	// player is already all-in
	if (player_state->get_money() == 0) {
		return 0;
	}
	
	// other player is already all-in
	if (other_money+minimum_bet == 0) {
		return 0;
	}

	if (player_state->get_number() == 0) {
		bet = agent_a_(game_state, *player_state, minimum_bet); 
	}	else {
		bet = agent_b_(game_state, *player_state, minimum_bet); 
	}

	// TODO: What to do if the player gives more money than he has?
	// Presently, we'll just do as if he wanted to go all-in.
	bet = min(bet, player_state->get_money());

	// TODO: What to do if the player doesn't give enough money to call?
	// Presently, the player just lose this round, as in tournament.
	if (minimum_bet > bet) { return -1; }
	
	// We can't bet more than what the opponent has.  
	bet = min(bet, other_money+minimum_bet);
	
	player_state->pay(bet);
	
	return bet;
}

template <class IA_A, class IA_B>
void Round<IA_A, IA_B>::generate_cards(GameState& game_state, deque<PlayerState*> player_order) {
	if (game_state.current_state_ == PREFLOP) {
		for (int i = 0; i < player_order.size(); i++) {
			generate_card(all_cards_, player_order[i]->cards());
			generate_card(all_cards_, player_order[i]->cards());

			//char* hand = StdDeck_maskString(player_order[i]->cards());
			//cout << "Cards of " << player_order[i]->get_number() << " : " << hand << endl;
		}
	} else if (game_state.current_state_ == FLOP) {
		generate_card(all_cards_, game_state.flop);
		generate_card(all_cards_, game_state.flop);
		generate_card(all_cards_, game_state.flop);

		//char* hand = StdDeck_maskString(game_state.flop);
		//cout << "Flop: " << hand << endl;
	} else if (game_state.current_state_ == TURN || game_state.current_state_ == RIVER) {
		generate_card(all_cards_, game_state.flop);

		//char* hand = StdDeck_maskString(game_state.flop);
		//cout << "New card: " << hand << endl;
	}
}


template <class IA_A, class IA_B>
void Round<IA_A, IA_B>::generate_card(CardMask& all_cards, CardMask& player) {
	while(true) {
		// TODO: Get a real random
		int c = StdDeck_MAKE_CARD(get_random(13), get_random(4));
		if (!CardMask_CARD_IS_SET(all_cards, c)) {
			CardMask_SET(all_cards, c);
			CardMask_SET(player, c);
			return;
		}
	}
}


#endif
