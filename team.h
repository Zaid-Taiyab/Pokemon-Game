#ifndef TEAM_H
#define TEAM_H

#include <fstream>
#include <string>
#include <vector>
#include <memory>

#include "pokemon.h"
#include "move.h"
#include "json11.hpp"
#include "data.h"

using namespace std;
using json = json11::Json;

class Team {
private:
    vector<unique_ptr<Pokemon>> team;
    static const int MAX_TEAM_SIZE = 3;

public:
    Team() = default;

    bool addPokemon(const unique_ptr<Pokemon>& pokemon);

    bool removePokemon(size_t index);

    bool isFull() const;

    Pokemon* getPokemon(size_t index);

    int getSize() const;

    void printTeam() const;

    void writeTeam(const std::string& filename) const;
    bool loadTeam(const std::string& filename, vector<unique_ptr<Pokemon>>& pokemons, const std::vector<Move>& availableMoves);
    
    Move* getMove(const vector<Move>& moves, string name);
    unique_ptr<Pokemon> findPokemonByName(vector<unique_ptr<Pokemon>>& pokemons, string name );
};

#endif // TEAM_H

