#ifndef BATTLE_H
#define BATTLE_H
#include <iostream>
#include "team.h"
#include <random>
#include "pokemon.h"
#include "move.h"
#include "json11.hpp"
#include "data.h"

using namespace std;
using json = json11::Json;
using RNG = mt19937;

class Battle {

public:
    Battle(){}

    void start(Team* team, vector<unique_ptr<Pokemon>>& pokemons, const vector<Move>& moves);

    bool applyMoveDamage(Pokemon* attacker, Pokemon* defender, Move* move, RNG& rng);
    int calculateDamage(float attackStat, float defenseStat, float power, float STAB, float damageMultiplier, float randomMultiplier);

    Pokemon* getRandomPokemon(vector<std::unique_ptr<Pokemon>>& pokemons, const vector<Move>& moves, RNG& rng);
    int selectMove(const Pokemon* pokemon);

};

#endif 
