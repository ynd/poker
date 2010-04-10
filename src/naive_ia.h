#ifndef naive_ia_h
#define naive_ia_h 

#include "TheGame.h"
#include "PlayerHuman.h"
#include "GameState.h"
#include "PlayerState.h"

int random_player(GameState& game_state, PlayerState& player_state, int minimum_money);
bool mask_in_mask(CardMask a, CardMask b);
int conservateur(GameState& game_state, PlayerState& player_state, int minimum_money);
int elaborate_player(GameState& game_state, PlayerState& player_state, int minimum_money);

#endif