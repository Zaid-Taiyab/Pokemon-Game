#include "battle.h"

using namespace std;
using json = json11::Json;

// Calculates the total damage of an attack using the Gen 1 Pokémon damage formula.
 
int Battle::calculateDamage(float attackStat, float defenseStat, float power, float STAB, float damageMultiplier, float randomMultiplier) {
        
    // Assuming a default level of 9 for the attacking Pokémon.
    float level = 9.0f; 
    float damage = ((((level*2.0f)/5.0f + 2.0f)*power*(attackStat/defenseStat))/50+2)*STAB*randomMultiplier*damageMultiplier;
    return static_cast<int>(round(damage));    
}

// Attempts to apply move damage from the attacker to the defender.
bool Battle::applyMoveDamage(Pokemon* attacker, Pokemon* defender, Move* attackersMove, RNG& rng) {
    uniform_int_distribution<int> hitMiss(1, 100);   // For determining hit/miss.
    uniform_int_distribution<int> uni(217, 255);     // For random damage multiplier.

    float attackStat = 0.0f, defenseStat = 0.0f;

    // Get the attacker's attack value and the defender's defense value based on the attacker's move
    // if the attacker's move category is physical then set the attackStat and defenseStat to getAttack and getDefense 
    // else attackStat and defenseStat should be set to getSpAttack and getSpDefense.
    //---
    if(attackersMove->getCategory()=="Physical"){
        attackStat=attacker->getAttack();
        defenseStat=defender->getDefense();
    }
    else {
        attackStat=attacker->getSpAttack();
        defenseStat=defender->getSpDefense();
    }
    // Calculate the Same Type Attack Bonus (STAB).
    // If the attacker's type is of the same type as the attackers's move then set STAB to 1.5f else default to 1.0f.
    //---
    float STAB;
    if(attacker->getType()==attackersMove->getType()){
        STAB=1.5f;
    }
    else {
        STAB=1.0f;
    }
    // Get the defenders damage multiplier to determine the effectiveness of the attacker's move.
    //---
    float damageMultiplier=defender->getDamageMultiplier(attackersMove->getType());
    // Random factor for damage calculation.
    float randomMultiplier = static_cast<float>(uni(rng)) / 255.0f;
    float power=attackersMove->getPower();
    // Calculate the final damage by calling the calculateDamage function.
    //---
    int damage = calculateDamage(attackStat,defenseStat,power,STAB,damageMultiplier,randomMultiplier); // REPLACE ME!!!!!!!!!!!!!!!! (variables included to avoid error message when compiling, this calculation is not correct)

    cout << attacker->getName() << " uses " << attackersMove->getName() << "!" << endl;

    // Apply damage if the move hits.
    if ((hitMiss(rng) % 100 + 1) <= attackersMove->getAccuracy() && attackersMove->usePP()) {
        // Apply damage to the defender here.
        // Ensure HP does not fall below 0 by setting negative values to 0.
        //---
        int newHp=defender->getHP()-damage;
        if(newHp<0){
            newHp=0;
        }
        defender->setHP(newHp);
        cout << "\033[38;5;1m" << attacker->getName() << " hits " << defender->getName()  << " for " << damage << "\033[0m" << endl;
    } else {
        // Uses ANSI escape codes to change text color for emphasis. "\033[38;5;1m" sets the color, and "\033[0m" resets it back to default.
        // Experiment with them to add color to terminal output, enhancing the game's visual appeal and immersion.
        cout << "\033[38;5;2mThe move misses!\033[0m" << endl;
    }

    // Check if the defender's HP is 0 or less, indicating fainting, and display a message.
    // Return true to indicate the defender has fainted.
    if (defender->getHP() <= 0) {
        cout << "\033[38;5;208m" << defender->getName() << " fainted!" << "\033[0m" << endl;
        return true; // Defender fainted.
    }
    cout << endl;
    return false; // Defender did not faint.
}

// Selects a random Pokémon and assigns up to four moves to it.
Pokemon* Battle::getRandomPokemon(vector<std::unique_ptr<Pokemon>>& pokemons, const vector<Move>& moves, RNG& rng) {
    // Generate a random index to select a Pokémon from the vector
    std::uniform_int_distribution<int> pokeDist(0, pokemons.size() - 1);
    // Clone the selected Pokémon to ensure unique instances
    auto randomPokemon = pokemons[pokeDist(rng)]->clone();

    // Assign up to four moves to the random Pokémon
    for (int i = 0; i < 4; ++i) {
        std::vector<Move> typeMoves; // Container for moves that match the Pokémon's type
        // Filter moves by matching types
        for (const auto& move : moves) {
            if (move.getType() == randomPokemon->getType()) {
                typeMoves.push_back(move);
            }
        }

        // If there are matching type moves, randomly assign one; otherwise, assign any move
        if (!typeMoves.empty()) {
            std::uniform_int_distribution<> specificMoveDist(0, typeMoves.size() - 1);
            randomPokemon->addMove(typeMoves[specificMoveDist(rng)]);
        } else {
            // Fallback to assigning any move if no type match is found
            randomPokemon->addMove(moves[pokeDist(rng)]);
        }
    }

    // Release the unique_ptr's ownership and return the raw pointer to the Pokémon
    return randomPokemon.release();
}

// Prompts the user to select a move for the current Pokémon during battle.

int Battle::selectMove(const Pokemon* pokemon) {
    int moveChoice = 0;
    cout << "\n" << pokemon->getName() << "'s turn. Choose a move:" << endl;
    pokemon->displayMoves();
    printf("> ");
    while(true) {
        if(!(cin >> moveChoice)) {
            cin.clear(); // Clear error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard bad input
            cout << "Invalid input, please enter a number." << endl;
        } else {
            // Clamp the move choice within the valid range of available moves
            moveChoice = max(1, min(moveChoice, static_cast<int>(pokemon->getMoves().size())));
            break; // Exit loop if valid input
        }
    }
    return moveChoice - 1; // Adjust for 0-based index
}


 // Starts the battle between the player's selected Pokémon and a randomly chosen opponent Pokémon.

void Battle::start(Team* team, vector<unique_ptr<Pokemon>>& pokemons, const vector<Move>& moves) {
    RNG rng(random_device{}()); // Initialize random number generator for various selections

    int currentPokemonIndex = 0; // Index to keep track of the current Pokémon in the player's team
    Pokemon* pokemon1 = team->getPokemon(currentPokemonIndex); // Player's current Pokémon
    Pokemon* pokemon2 = getRandomPokemon(pokemons, moves, rng); // Randomly selected opponent Pokémon

    // Distribution for selecting an opponent Pokémon's move
    uniform_int_distribution<size_t> pokemon2MoveDist(0, pokemon2->getMoves().size() - 1); // For selecting pokemon2's move

    cout << "Battle starts between " << pokemon1->getName() << " and " << pokemon2->getName() << "!\n" << endl;

    // Main battle loop
    while (currentPokemonIndex < team->getSize()) {

        // Check if the current Pokémon has fainted
        while (pokemon1->getHP() == 0) {
            currentPokemonIndex++;
            if(currentPokemonIndex < team->getSize()) {
                pokemon1 = team->getPokemon(currentPokemonIndex);
                cout << "\nYou switched to " << pokemon1->getName() << "!\n";
                cout << "\nBattle starts between " << pokemon1->getName() << " and " << pokemon2->getName() << "!\n" << endl;
            } else {
                // All Pokémon in the player's team have fainted
                cout << "All your Pokémon have fainted.\n"
                     << "You scurry back to a Pokémon Center, protecting your exhausted Pokémon from any further harm...\n";
                // Reset the HP of all Pokémon in the team before exiting the battle
                for(int i = 0; i < team->getSize(); i++) {
                    team->getPokemon(i)->resetHP();
                }
                cout << "\033[38;5;153mYour Pokémon are healed!\033[0m\n";
                return;
            }
        }

        pokemon1->displayStats();
        pokemon2->displayStats();

        // Prompt the player to select a move for their Pokémon
        int moveChoice = selectMove(pokemon1);

        // Retrieve the selected moves for both Pokémon
        auto& pokemon1Move = pokemon1->getMove(moveChoice);
        auto& pokemon2Move = pokemon2->getMove(pokemon2MoveDist(rng));

        // Determine the order of attacks based on Pokémon speed
        if (pokemon1->getSpeed() < pokemon2->getSpeed()) {
            // Opponent Pokémon attacks first
            if (applyMoveDamage(pokemon2, pokemon1, &pokemon2Move, rng)) continue;
            if (applyMoveDamage(pokemon1, pokemon2, &pokemon1Move, rng)) break; // Opponent Pokémon faints.
        } else {
            // Player's Pokémon attacks first
            if (applyMoveDamage(pokemon1, pokemon2, &pokemon1Move, rng)) break; // Opponent Pokémon faints.
            if (applyMoveDamage(pokemon2, pokemon1, &pokemon2Move, rng)) continue;
        }
    }

    
    delete(pokemon2); // Clean up the dynamically allocated opponent Pokémon
}
