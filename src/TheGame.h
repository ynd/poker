#ifndef thegame_h
#define thegame_h

#include "PlayerState.h"
#include "Round.h"

template<class IA_A, class IA_B>
class TheGame {
private:
	PlayerState player_state_a_;
	PlayerState player_state_b_;
	Round<IA_A, IA_B> round_;
	
	int round_count_;
	int dealer_;
	int max_round_;
	
	void play() {
		while (!is_over()) {
			describe();

			round_.start_round();
			round_count_++;
		}

		describe();
	}

	bool is_over() const {
		return  player_state_a_.get_money() <= 0 || 
				player_state_b_.get_money() <= 0 || 
				round_count_ == max_round_;
	}
	
	void describe() {
		//cout << "---------" << endl;
		//cout << player_state_a_ << endl << player_state_b_ << endl << endl;
	}

public:
	TheGame(IA_A ia_a, IA_B ia_b, int start_money, int max_round, pair<int,int> blinds) : 
		round_count_(0),
		dealer_(0), 
		max_round_(max_round),
		player_state_a_(PlayerState(0, start_money)),
		player_state_b_(PlayerState(1, start_money)),
		round_(ia_a, ia_b, player_state_a_, player_state_b_, blinds)
	{
		play();
	}
	
	pair<int,int> get_score() {
		return make_pair(player_state_a_.get_money(), player_state_b_.get_money());
	}
};

#endif
