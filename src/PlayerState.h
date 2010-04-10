#ifndef playerstate_h
#define playerstate_h 

#include <iostream>
#include <string>

#include "GameState.h"
#include "poker_defs.h"
#include "inlines/eval.h"

using namespace std;

class PlayerState {
	int money_;
	int number_;
	bool dealer_; 
	
	CardMask cards_;

public:

	PlayerState(int number, int starting_money) : number_(number), dealer_(false), money_(starting_money) { 
	}
	
	int get_number() const { return number_; }
	
	int get_money() const { return money_; }
	void set_money(int money) { money_ = money; }
	void pay(int money) { money_ -= money; }
	void cash(int money) { money_ += money; }
	
	bool is_dealer() const { return dealer_; }
	void set_dealer(bool dealer) { dealer_ = dealer; }
	
	void reset_cards();
	CardMask& cards() { return cards_; }
	
	friend ostream& operator<<(ostream& output, const PlayerState&);
};


#endif