#include "TheGame.h"
#include "naive_ia.h"
#include "PopulationEvolver.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

const int start_money = 5000;
const int number_of_game = 100;

template <class A, class B>
TheGame<A,B> create_game(A a, B b, int start_money, int max_round, pair<int,int> blinds) {
	return TheGame<A,B>(a, b, start_money, max_round, blinds);
}

class Player {
public:
    NeuralNetwork* network_;
    
    Player(NeuralNetwork* network) : network_(network) {}
    
    int operator() (GameState& game_state, PlayerState& player_state, int minimum_money) {
        vector<double> input;
        input.push_back(Hand_EVAL_N(player_state.cards(),StdDeck_numCards(player_state.cards())));
        
        return min((int)(player_state.get_money()*network_->get_output(input)[0]), minimum_money);
    }
};

double evaluate_fitness (NeuralNetwork* network, int generation) {
    int count = 0;
    
    Player player(network);
    
	for (int i = 0; i < number_of_game; i++) {
        network->clear_memory();
		pair<int,int> result = create_game(player, elaborate_player, start_money, 50, make_pair(50,100)).get_score();
		if (result.first > result.second) {
			count++;
		}
	}
	
    return count;
}

int main() {
	PopulationEvolver ev(1, 1, evaluate_fitness, 1000, 50, 12, 0.01, true);
	
	for (int i = 0; i < ev.population_.size(); i++) {
        ifstream save;
        
        stringstream filename;
        filename << "networks/network" << i << ".gv";
        save.open(filename.str().c_str());
        
        if (save.is_open()) {
            string current = "";
            while (current != "input_units:") {
                save >> current;
            }
            save >> ev.population_[i]->input_units_;
            
            while (current != "output_units:") {
                save >> current;
            }
            save >> ev.population_[i]->output_units_;
            
            save >> current;
            for (int j = 0; current != "*/"; j++) {
                ev.population_[i]->genes_[j] = atoi(current.c_str());
                save >> current;
            }
        }
    }
    
    for (int g = 0; g < 10000; g++) {
        ev.evolve(10);
        
        ev.get_population_fitness(-1);
    
        cerr << g << " - Best fitness = " << ev.population_[0]->fitness_;
        
        double average_neurons = 0.0;
        double average_fitness = 0.0;
        for (int i = 0; i < ev.population_.size(); i++) {
            average_neurons += ev.population_[i]->network_->hidden_neurons_.size();
            average_fitness += ev.population_[i]->fitness_;
        
            stringstream filename;
            filename << "networks/network" << i << ".gv";
        
            ofstream file;
            file.open(filename.str().c_str());
            file << ev.population_[i]->save();
            file.close();
        }
        
        average_neurons = average_neurons / ev.population_.size();
        average_fitness = average_fitness / ev.population_.size();
    
        cout << ", Average neurons = " << average_neurons << ", Average fitness = " << average_fitness << endl;
    }
    
    return 0;
}
