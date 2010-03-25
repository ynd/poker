#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

#include "NeuralNetwork.h"
#include "Phenotype.h"
#include "PopulationEvolver.h"
#include <cmath>

enum piece {EMPTY = 0, O, X};
char piece_to_char(int p);
int const winning_rows[][3] = {{0, 3, 6},
                               {1, 4, 7},
                               {2, 5, 8},
                               {0, 1, 2},
                               {3, 4, 5},
                               {6, 7, 8},
                               {0, 4, 8},
                               {2, 4, 6}};
class Position {
    public:
        piece board[9];
        bool possible_moves[9];

    public:
        Position(); // this creates an empty board
        Position(const Position &p); // copy constructor
        piece getSquare(const int square) const; // const as it doesn't modify this
        void setSquare(const int square, piece p);
        void refreshPossibleMoves(); // updates possible_moves
        int getNextMove(); // get square of next possible move
    void print() const; // prints board to stdout
    bool isOver() const; // is game over?
    // isWin will return X or O depending on who won
    int isWin() const; // is the game won?
    int isTie() const; // is the game a tie?

    unsigned int countEmptySquares() const; // count how many empty squares there are
    piece toPlay() const; // who plays at this position?
    void play(int square); // play in square "square"
    void undo(int square); // clear square "square"
};

Position::Position() {
    unsigned int i;
    for (i=0; i<9; i++) {
        board[i] = EMPTY;
    }
}

Position::Position(const Position& pos) {
    unsigned int i;
    for (i=0; i<9; i++){
        board[i] = pos.board[i];
    }
}

piece Position::getSquare(const int square) const {
    return board[square];
}

void Position::setSquare(const int square, piece p) {
    board[square] = p;
}

void Position::refreshPossibleMoves() {
    unsigned int i;
    for (i=0; i<9; i++){
        if (board[i] == EMPTY){
            possible_moves[i] = true;
        }

        else {
            possible_moves[i] = false;
        }
    }
}

int Position::getNextMove() {
    unsigned int i;
    for (i=0; i<9; i++){
        if (possible_moves[i]) {
            possible_moves[i] = false;
            return (i);
        }
    }
    return 0;
}

char piece_to_char(int p) {
    if (p == EMPTY) return ' ';
    else if (p == X) return 'X';
    else return 'O';
}

void Position::print() const {
    char print_board[9];
    unsigned int i;
    for (i=0; i<9; i++){
        print_board[i] = piece_to_char(board[i]);
    }
    std::cout << print_board[0] << "  |  " << print_board[1] << "  |  " << print_board[2] << std::endl;
    std::cout << print_board[3] << "  |  " << print_board[4] << "  |  " << print_board[5] << std::endl;
    std::cout << print_board[6] << "  |  " << print_board[7] << "  |  " << print_board[8] << std::endl;
}

int Position::isWin() const {
    unsigned int i, index1, index2, index3;
    for (i=0; i<8; i++){
        index1 = winning_rows[i][0];
        index2 = winning_rows[i][1];
        index3 = winning_rows[i][2];
        if (board[index1] == board[index2] && board[index2] == board[index3] && board[index1] != EMPTY) {
                return board[index1];
            }
    }
    return 0;
}

int Position::isTie() const {
    unsigned int i;
    for (i=0; i<9; i++){
        if (board[i] == EMPTY) return 0;
    }
    return 2;
}

bool Position::isOver() const {
    return (isWin() || isTie());
}

unsigned int Position::countEmptySquares() const {
    unsigned int i, depth = 0;
    for (i=0; i<9; i++){
        if (board[i] == EMPTY) depth++;
    }
    return depth;
}

piece Position::toPlay() const {
    unsigned count = countEmptySquares();
    
    if (count & 1) return X;
    else return O;
}

void Position::play(int square) {
    board[square] = toPlay();
}

void Position::undo(int square) {
    board[square] = EMPTY;
}

vector<double> get_input_layer(Position game) {
    vector<double> input_layer(9);
    
    for (int i = 0; i < 9; i++) {
        if (game.getSquare(i) == game.toPlay()) {
            input_layer[i] = 1.0;
        }
        else if (game.getSquare(i) != game.toPlay() && game.getSquare(i) != EMPTY) {
            input_layer[i] = -1.0;
        }        
        else {
            input_layer[i] = 0.0;
        }
    }
    
    return input_layer;
}

int get_best_move(NeuralNetwork* network, Position game) {
    double best_evaluation = -100.0;
    int best_move = 0;
    
    vector<double> output = network->get_output(get_input_layer(game));
    
    game.refreshPossibleMoves();
    for (int i = 0; i < 9; i++) {
        if (game.possible_moves[i]) {
            if (output[i] > best_evaluation) {
                best_evaluation = output[i];
                best_move = i;
            }
        }
    }
    
    return best_move;
}

int get_best_move_verbose(NeuralNetwork* network, Position game) {
    double best_evaluation = -100.0;
    int best_move = 0;
    
    vector<double> output = network->get_output(get_input_layer(game));
    
    cout << output[0] << "   " << output[1] << "   " << output[2] << endl;
    cout << output[3] << "   " << output[4] << "   " << output[5] << endl;
    cout << output[6] << "   " << output[7] << "   " << output[8] << endl;
    
    game.refreshPossibleMoves();
    for (int i = 0; i < 9; i++) {
        if (game.possible_moves[i]) {
            if (output[i] > best_evaluation) {
                best_evaluation = output[i];
                best_move = i;
            }
        }
    }
    
    return best_move;
}

int get_random_move (Position game) {
    vector<int> possibles_moves;
    
    game.refreshPossibleMoves();
    for (int i = 0; i < 9; i++) {
        if (game.possible_moves[i]) {
            possibles_moves.push_back(i);
        }
    }
    
    return possibles_moves[rand() % possibles_moves.size()];
}

double evaluate_fitness_random(NeuralNetwork* network, vector<NeuralNetwork*> networks) {
    double wins = 0;
    
    for (int i = 0; i < 10; i++) {
        Position game;
        
        piece player;
        if (rand() % 2 == 0) {
            game.play(get_random_move(game));
            player = O;
        }
        else {
            player = X;
        }
        
        while (!game.isOver()) {
            game.play(get_best_move(network, game));
            
            if (!game.isOver()) {
                game.play(get_random_move(game));
            }
        }
        
        if (game.isWin() == player) {
            wins++;
        }
        else if (game.isTie()) {
            wins += 0.5;
        }
        
        network->clear_memory();
    }
    
    return wins;
}

double evaluate_fitness_coevolution(NeuralNetwork* network, vector<NeuralNetwork*> networks) {
    double wins = 0;
    
    for (int i = 0; i < 20; i++) {
        Position game;
        
        NeuralNetwork* enemy = networks[i];
        
        piece player;
        if (rand() % 2 == 0) {
            game.play(get_best_move(enemy, game));
            player = O;
        }
        else {
            player = X;
        }
        
        while (!game.isOver()) {
            game.play(get_best_move(network, game));
            
            if (!game.isOver()) {
                game.play(get_best_move(enemy, game));
            }
        }
        
        if (game.isWin() == player) {
            wins++;
        }
        else if (game.isTie()) {
            wins += 0.5;
        }
    }
    
    return wins / 2;
}

double evaluate_fitness_combination(NeuralNetwork* network, vector<NeuralNetwork*> networks) {
    return evaluate_fitness_random(network, networks)
        + evaluate_fitness_coevolution(network, networks);
}


double evaluate_fitness_xor (NeuralNetwork* network, vector<NeuralNetwork*> networks) {
    network->clear_memory();
    
    double error = 0.0;
    vector<double> input(2);
    input[0] = 0.0;
    input[1] = 0.0;
    error += abs(0.0 - network->get_output(input)[0]);
    network->clear_memory();
    
    input[0] = 0.0;
    input[1] = 1.0;
    error += abs(1.0 - network->get_output(input)[0]);
    network->clear_memory();
    
    input[0] = 1;
    input[1] = 0;
    error += abs(1.0 - network->get_output(input)[0]);
    network->clear_memory();
    
    input[0] = 1.0;
    input[1] = 1.0;
    error += abs(0.0 - network->get_output(input)[0]);
    network->clear_memory();
      
    return error;
}


void train_xor() {
    PopulationEvolver ev(2, 1, evaluate_fitness_xor, 150, 50, 12, 0.4, false);
    
    ev.evolve(5000);
    
    ev.get_population_fitness();
    
    cout << "Best fitness  = " << ev.population_[0]->fitness_ << endl;
    
    for (int i = 0; i < ev.population_.size(); i++) {
        stringstream filename;
        filename << "networks/network" << i << ".gv";
        
        ofstream file;
        file.open(filename.str().c_str());
        file << ev.population_[i]->save();
        file.close();
    }
    
    NeuralNetwork* net = Phenotype::get_network(ev.population_[0]);
    while (true) {
        vector<double> input(2);
    
        cout << "x0 = ";
        cin >> input[0];
        cout << "x1 = ";
        cin >> input[1];
        cout << "y = " << net->get_output(input)[0] << endl;
        cout << "------" << endl;
        net->clear_memory();
    }
}

int main (int argc, char * const argv[]) {
    train_xor();return 0;
    PopulationEvolver evolver(9, 9, evaluate_fitness_combination, 1000, 1000, 100, 0.4);
    
    for (int g = 0; g < 1000; g++) {
        evolver.evolve(10);
    
        evolver.get_population_fitness();
    
        cout << "Best fitness  = " << evolver.population_[0]->fitness_;
        
        double average_neurons = 0.0;
    
        for (int i = 0; i < evolver.population_.size(); i++) {
            NeuralNetwork* network = Phenotype::get_network(evolver.population_[i]);
            
            average_neurons += network->hidden_neurons_.size();
            
            ofstream file;
            stringstream filename;
            filename << "networks/network" << i << ".gv";
            stringstream label;
            label << "fitness = " << evolver.population_[i]->fitness_;
        
            file.open(filename.str().c_str());
            file << evolver.population_[i]->save();
            file.close();
        
            delete network;
        }
        
        cout << ", average hidden neurons = " << average_neurons / evolver.population_.size() << endl;
    }
        
    NeuralNetwork* network = Phenotype::get_network(evolver.population_[0]);
    
    Position game;
    while (true) {
        game.print();

        int human_move = 0;
        do {
            cout << "Player " << piece_to_char(game.toPlay()) << ", choose a move: ";
            cin >> human_move;
            cout << endl;
        } while (game.getSquare(human_move) != EMPTY);
        
        game.play(human_move);
        
        if (!game.isOver()) {
            game.play(get_best_move_verbose(network, game));
        }
        
        if (game.isOver()) {
            network->clear_memory();
            game.print();
            if (game.isWin()) {
                printf("------------ Winner =  %c--------------\n", piece_to_char(game.isWin()));
            }
            else {
                printf("------------ Tie --------------\n");
            }            
            
            for (int i = 0; i < 9; i++) {
                game.undo(i);
            }
            
            if (rand() % 2 == 0) {
                game.play(get_best_move(network, game));
            }
        }
    }
    
    delete network;
    
    return 0;
}
