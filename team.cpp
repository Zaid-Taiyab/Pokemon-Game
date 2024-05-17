#include "team.h"

using namespace std;
using json = json11::Json;

/**
 * Tries to add a new Pokémon to the team. If the team has not reached its maximum size, it clones the given Pokémon and adds it to the team.
 * This method ensures that the team does not exceed its predefined size limit.
 *
 * @param pokemon A unique pointer to the Pokémon to be added to the team.
 * @return True if the Pokémon is successfully added, false if the team is already at its maximum capacity.
 */
bool Team::addPokemon(const unique_ptr<Pokemon>& pokemon) {
    if (team.size() < MAX_TEAM_SIZE) {
        // Clone the Pokémon and add the clone to the team.
        //---
        
        //std::unique_ptr<Pokemon> clonedPokemon = std::make_unique<Pokemon>(*pokemon);
        //team.push_back(std::move(pokemon));
        //cout<<pokemon.get()->getName()<<endl;
        team.push_back(pokemon->clone());
        return true;
    }
    // Return false if the team is already full.
    return false;
}


/**
 * Removes a Pokémon from the team based on its index in the team's list. This method ensures that the team can dynamically change its members.
 *
 * @param index The position of the Pokémon in the team's list to be removed.
 * @return True if the Pokémon is successfully removed, indicating the index was within the valid range. False if the index is out of range, indicating no Pokémon was removed.
 */
bool Team::removePokemon(size_t index) {
    if (index < team.size()) {
        // If the index is valid, erase the Pokémon at that index from the team and return true.
        //---
        team.erase(team.begin()+index);
        return true;
    }
    // Return false if the index is invalid, meaning no Pokémon is removed.
    return false;
}

/**
 * Determines if the team has reached its maximum capacity.
 *
 * @return True if the team's size equals the maximum team size, indicating the team is full. False otherwise, indicating there's room for more Pokémon.
 */
bool Team::isFull() const {
    // Check if the current team size equals the maximum team size.
    //---
    if(team.size()>=3){
        return true;
    }
    return false; // Placeholder return statement. Replace with the actual condition check.
}


/**
 * Fetches a pointer to a Pokémon in the team based on its position (index) in the team's list.
 * This method facilitates access to specific Pokémon for battle, display, or other purposes.
 *
 * @param index The index of the Pokémon in the team list.
 * @return A pointer to the Pokémon if the index is valid. Returns nullptr if the index is out of range, indicating no Pokémon exists at that position.
 */
Pokemon* Team::getPokemon(size_t index) {
    // If the index is within the bounds of the team array, return a pointer to the Pokémon at that index.
    //---
    if (index < team.size()) {
       return team.at(index).get();
    }
    return nullptr; // Return nullptr if the index is out of bounds.
}


/**
 * Gets the number of Pokémon currently in the team.
 * @return The size of the team.
 */
int Team::getSize() const {
    // Return the number of Pokémon in the team.
    return team.size();
}

// Displays the stats of each Pokémon in the team, numbered in the order they were added.
void Team::printTeam() const {
    if (team.size() == 0) printf("You have no Pokemon in your team");
    for (size_t i = 0; i < team.size(); i++) {
        cout << i+1 << ". ";
        cout<<team[i].get()->getName()<<endl;
        team[i]->displayStats();
    }
    printf("\n");
}

/**
 * Finds a Pokémon by its name in a list and returns a clone of it.
 * @param pokemons The list of Pokémon to search through.
 * @param name The name of the Pokémon to find.
 * @return A unique_ptr to a clone of the found Pokémon, or nullptr if not found.
 */
unique_ptr<Pokemon> Team::findPokemonByName(vector<unique_ptr<Pokemon>>& pokemons, string name) {
    // Loop through the given list to find a Pokémon by name and return a clone if found.
    //---
    for(auto& pokemonPtr:pokemons){
        if (pokemonPtr.get()->getName().compare(name)==0){
            return pokemonPtr->clone();
            //make_unique<Pokemon>(pokemonPtr);
        }
    }
    return nullptr; // Return nullptr if the Pokémon is not found.
}

/**
 * Finds a Move by its name in a list of Moves.
 * @param moves The list of Moves to search through.
 * @param name The name of the Move to find.
 * @return A pointer to the found Move, or nullptr if not found.
 */
Move* Team::getMove(const vector<Move>& moves, string name) {
    // Iterate through the list of moves and return a new Move pointer if the name matches.
    //---
    for(auto& move:moves){
        if (move.getName().compare(name)==0){
            return new Move{move};
        }
    }
    return nullptr; // Return nullptr if the Move is not found.
}


/**
 * Loads team configuration from a JSON file. This includes creating Pokémon based on their names and assigning their moves from the file.
 * It's a good introduction to parsing JSON data with json11.
 *
 * @param filename The name of the JSON file to load.
 * @param pokemons A reference to a vector of Pokémon, used to find and clone the specified Pokémon.
 * @param moves A reference to a vector of available Moves, used to assign moves to the Pokémon.
 * @return True if the team is successfully loaded from the file, false if there are errors or if specific Pokémon/moves cannot be found.
 */
/**/
bool Team::loadTeam(const string& filename, vector<unique_ptr<Pokemon>>& pokemons, const vector<Move>& moves) {
    string err;
    //string fileContents; 
    // You need to load the file content into this string.
    // Example of reading a file into a string (pseudo-code):
    std::ifstream ifs(filename);
    std::string fileContents((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
    //---
    
    // Parse the JSON content
    auto jsonCBs = json::parse(fileContents, err);
    if (!err.empty()) {
        cout << "Failed to parse JSON: " << err << endl;
        return false;
    }

    // Process each Pokémon configuration in the JSON array
    for (const auto& pm : jsonCBs.array_items()) {
        // Example of accessing a Pokémon's name and moves from the JSON object:
        string name = pm["name"].string_value();
        auto movesJson = pm["moves"].array_items();
        // Find the Pokémon by name using 'findPokemonByName'
        // For each move in 'movesJson', find the move using 'getMove' and add it to the Pokémon
        // If any Pokémon or moves are not found, print an error message and return false
        //---
        unique_ptr<Pokemon> pokemonPtrVariable = findPokemonByName(pokemons,name);
        if(pokemonPtrVariable==NULL){
            printf("error finding pokemon");
            return false;
        }
        string nameOfMove;
        for (const auto& pm : movesJson){
        nameOfMove=pm.string_value();
        if(getMove(moves,nameOfMove)==NULL){
            printf("error finding move");
            return false;
        }
        pokemonPtrVariable.get()->addMove(*getMove(moves,nameOfMove));
        }
        // After processing each Pokémon and its moves, add it to the team
        //---;
        addPokemon(pokemonPtrVariable);
    }

    return true; // Return true if all Pokémon and their moves are successfully processed
}


/**
 * Saves the team's composition to a JSON file. Each Pokémon's name and their moves are included.
 * @param filename The name of the file to write the team's composition to.
 *
 * @note Students are not required to modify this function.
 */
void Team::writeTeam(const string& filename) const {
    vector<json> teamJson;

    // Iterates through each team member to create a JSON representation
    for (const auto& member : team) {
        vector<string> moves;
        // Converts each move of the member to a string and stores in a vector
        for (const auto& move : member->getMoves()) {
            moves.push_back(move.getName()); // Gets the name of the move
        }

        // Creates a JSON object for the team member and adds it to the teamJson vector
        teamJson.push_back(json::object {
                {"name", member->getName()}, // Adds the member's name
                {"moves", json(moves)} // Adds the member's moves
        });
    }

    ofstream outFile(filename); // Creates an ofstream to write to the file
    if (outFile.is_open()) {
        json teamArray = json::array(teamJson); // Converts the team vector into a JSON array
        outFile << teamArray.dump(); // Writes the JSON array to the file
        outFile.close();
    } else {
        cerr << "Failed to open " << filename << " for writing.\n";
    }
}



