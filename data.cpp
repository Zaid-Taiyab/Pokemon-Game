#include "data.h"

using namespace std;
using json = json11::Json;

/**
* This function reads the json file and returns json data
* @param file_path
* @return json data
*/
json read_json_file(const string &file_path) {

    ifstream t(file_path);
    string str((istreambuf_iterator<char>(t)), istreambuf_iterator<char>());

    string err;
    const auto json = json::parse(str, err);
    return json;
}

/**
 * Loads move data from a JSON file and creates a vector of Move objects.
 * Each move's data is extracted from the JSON and used to create a Move object.
 * @param filename The name of the JSON file containing moves data.
 * @return A vector of Move objects constructed from the JSON file data.
 */
vector<Move> loadMoves(string filename){
    json jsonMoves = read_json_file(filename);
    vector<Move> moves;
    // Parse the JSON array and create Move objects
    for (auto& entry : jsonMoves.array_items()){
        // Each 'entry' represents move data. Move::create constructs a Move objects and push_back adds it to the moves vector.
        Move *move = Move::create(entry);
        moves.push_back(*move);
    }
    return moves;
}

/**
 * Loads Pokémon data from a JSON file and creates a vector of unique_ptr to Pokémon objects.
 * Each Pokémon's data is extracted from the JSON and used to instantiate Pokémon objects through a factory method.
 * @param filename The name of the JSON file containing Pokémon data.
 * @return A vector of unique_ptr to Pokémon objects constructed from the JSON file data.
 */
vector<unique_ptr<Pokemon>> loadPokemon(string filename){
    json jsonPokemon = read_json_file(filename);
    vector<unique_ptr<Pokemon>> pokemons;
    // Iterate over each entry in the JSON array to create Pokémon objects.
    for (auto& entry : jsonPokemon.array_items()){
        unique_ptr<Pokemon> pokemon;
        // Utilize the pokemonFactory to create a unique_ptr to a Pokémon from the JSON entry.
        //--- 
        pokemon=pokemonFactory(entry);
        if (pokemon != nullptr){
            // Add the created unique_ptr<Pokemon> to the pokemons vector using std::move to transfer ownership
            // Note: std::move transfers the ownership of the Pokémon, ensuring proper memory management and preventing leaks.
            //---
            pokemons.push_back(std::move(pokemon));
        }
    }
    return pokemons;
}
