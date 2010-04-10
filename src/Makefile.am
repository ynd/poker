CFLAGS=-O3 $(OPENMP_CFLAGS) -DVERSION=\"$(VERSION)\" -DPROG="\"$(PACKAGE)\"" -I/usr/local/include/poker-eval/
AM_CXXFLAGS=$(CFLAGS) -lpoker-eval
AM_LDFLAGS=$(OPENMP_CFLAGS)

bin_PROGRAMS = poker play
poker_SOURCES = \
	main.cpp \
	GameState.cpp \
	GameState.h \
	PlayerHuman.cpp \
	PlayerHuman.h \
	PlayerState.cpp \
	PlayerState.h \
	Round.cpp \
	Round.h \
	TheGame.cpp \
	TheGame.h \
	naive_ia.cpp \
	naive_ia.h \
	random.cpp \
	random.h \
	Individual.h \
	Individual.cpp \
	NeuralNetwork.h \
	NeuralNetwork.cpp \
	Neuron.h \
	Neuron.cpp \
	Phenotype.h \
	Phenotype.cpp \
	PopulationEvolver.h \
	PopulationEvolver.cpp \
	utilities.h \
	utilities.cpp
play_SOURCES = \
	play.cpp \
	GameState.cpp \
	GameState.h \
	PlayerHuman.cpp \
	PlayerHuman.h \
	PlayerState.cpp \
	PlayerState.h \
	Round.cpp \
	Round.h \
	TheGame.cpp \
	TheGame.h \
	naive_ia.cpp \
	naive_ia.h \
	random.cpp \
	random.h \
	Individual.h \
	Individual.cpp \
	NeuralNetwork.h \
	NeuralNetwork.cpp \
	Neuron.h \
	Neuron.cpp \
	Phenotype.h \
	Phenotype.cpp \
	PopulationEvolver.h \
	PopulationEvolver.cpp \
	utilities.h \
	utilities.cpp