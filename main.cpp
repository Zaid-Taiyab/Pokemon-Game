#include <iostream>
#include <set>
#include <string>
#include <vector>

#include "move.h"
#include "pokemon.h"
#include "json11.hpp"
#include "team.h"
#include "battle.h"
#include "data.h"
#include "gym.h"

using namespace std;
using json = json11::Json;

// Function to get and validate user input.
int getInput() {
    int input;
    while (true) {
        cout << "Enter your choice: ";
        if (!(cin >> input)) {  // Check for end of input stream
            if (cin.eof()) {
                cout << "End of input reached.\n";
                exit(0);
            } else {
                cin.clear();    // Clear error state
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore remaining input
                cout << "Invalid input. Please enter an integer.\n";
            }
        } else {
            cout<<endl;
            break;
        }
    }
    return input;
}

/**
 * Prompts the user to select a Pokémon type from a predefined list.
 * Updates the passed selectedType reference with the user's choice.
 *
 * @param selectedType Reference to a string where the selected Pokémon type will be stored.
 * @return True if a valid type is selected, false otherwise (indicating an invalid selection or out of range choice).
 */
bool selectPokemonType(string& selectedType) {
    // Array of available Pokémon types for selection
    string types[] = {"", "Normal", "Fire", "Water", "Electric", "Grass", "Rock"};

    // Display the types to the user
    cout << "Choose Type:\n";
    for (int i = 1; i < 7; i++) {
        cout << i << ". " << types[i] << "\n";
    }

    // Call the getInput() function to get the user's choice
    //---
    int choice=getInput();
    // Check if the choice is invalid return false (see for loop for valid range of values.)
    // Otherwise, update selectedType 
    if(choice>6||choice<1){
        return false;
    }
    //---
    selectedType=types[choice];
    return true;
}


/**
 * Selects a Pokémon of the specified type from a list of available Pokémon.
 *
 * Displays the available Pokémon of the specified type and prompts the user to select one.
 * If a valid choice is made, returns a clone of the selected Pokémon.
 *
 * @param selectedType The type of Pokémon to select.
 * @param pokemons A vector containing pointers to available Pokémon.
 * @return A unique pointer to a clone of the selected Pokémon, or nullptr if no valid choice was made.
 */
unique_ptr<Pokemon> selectPokemonFromType(const string& selectedType, vector<unique_ptr<Pokemon>>& pokemons) {
    cout << "Select your " << selectedType << " type Pokemon:\n";

    // Map to associate an ID with each Pokémon of the selected type
    map<int, Pokemon*> idToPokemonMap;

    // Display and add each Pokémon of the selected type to the idToPokemonMap.
    // Remember to use get() to retrieve the raw pointer to the Pokémon before adding it to the map.
    // Start the index at 1 and increment the id for each Pokémon added to the map.
    int id = 1;
    for (auto& pokemon : pokemons) {
        //--
        if (pokemon->getType()==selectedType){
            cout<<pokemon->getName()<<endl;
            idToPokemonMap[id]=pokemon.get();
            id++;
        }
    }
    
    // Call the getInput() function to get the user's choice
    //---
    int choice=getInput();
    // Use the find() method on the idToPokemonMap to search for the Pokémon corresponding to the user's choice.
    // Example: auto foundItem = myMap.find(key);
    auto foundItem = idToPokemonMap.find(choice);
    // If the Pokémon is found, return a clone of it using the clone() method.
    //---
    if(foundItem!=idToPokemonMap.end()){
    return foundItem->second->clone();
    }
    // If no valid choice was made, return nullptr
    return nullptr;
}

/**
 * Allows the user to select up to 4 moves for a specified Pokémon from a list of available moves based on the Pokémon's type.
 * This function displays moves of the Pokémon's type and the "Normal" type, then lets the user choose up to 4 moves,
 * ensuring no duplicates are selected. The function assumes that each move's type is either the same as the Pokémon's type or "Normal".
 *
 * @param selectedPokemon A reference to a unique pointer pointing to the selected Pokémon.
 * @param moves A vector containing all available moves.
 * @param selectedType The type of the selected Pokémon.
 */
void selectMovesForPokemon(unique_ptr<Pokemon>& selectedPokemon, const vector<Move>& moves, const string& selectedType) {
    cout << "Choose up to 4 moves for " << selectedPokemon->getName() << ":\n";

    // Map to associate an ID with each Move of the selected type and Normal type.
    map<int, Move> moveChoices;

    // Loop through the moves to display those available for the selected Pokémon type and "Normal" type.
    // Add the move to the moveChoices map with the index as the key and a pointer to the move as the value
    // e.g, moveChoices[1] = &move;
    // The move index starts at 1
    // The display will look something like 
    // 1. Thunder
    // 2. Thunder Punch ...
    int moveIndex = 1;
    for (const auto& move : moves) {
        //---
        if(move.getType()==selectedType||move.getType()=="Normal"){
            cout<<move.getType()<<endl;
            moveChoices[moveIndex]=move;
            moveIndex++;
        }
    }
    

    // Initialize a vector to keep track of chosen move IDs to prevent duplicates.
    vector<int> chosenMoveIds;
    int selectedMoves = 0;  // Keep track of the number of moves selected.

    // Allow the user to select up to 4 moves.    
    while (selectedMoves < 4) {
        // Loop to select up to 4 moves:
        //     1. Use getInput() to read the user's choice.
        //     2. If choice is 0 then exit loop
        //     3. If choice is not found in moveChoices (map), then report invalid move and continue to get another input.
        //     4. Check if the chosen move is already selected by using the std::find function to search in chosenMoveIds,
        //              if the chosen move is in chosenMoveIds then print out error message and continue
        //     5. Add the choice to the chosen moves vector
        //     6. Add the move to the selected Pokemon
        //     7. Print: cout << "Your " << selectedPokemon->getName() << " learned " << moveChoices[choice].getName() << endl;
        //     8. Increment selectedMoves
        //---
        int choice=getInput();
        if(choice==0){
            break;
        }
        auto x=moveChoices.find(choice);
        if(x==moveChoices.end()){
            cout<<"Invalid move"<<endl;
        }
        auto duplicate=find(chosenMoveIds.begin(),chosenMoveIds.end(),choice);
        if(duplicate!=chosenMoveIds.end()){
            cout<<"Error, move already exists"<<endl;
        }
        else {
            chosenMoveIds.push_back(choice);
            selectedPokemon.get()->addMove((moveChoices[choice]));
            cout << "Your " << selectedPokemon->getName() << " learned " << moveChoices[choice].getName() << endl;
        }
        selectedMoves++;
    }

    // Provide summary feedback based on the number of moves selected.
    if (selectedMoves == 0) {
        cout << "No moves selected for " << selectedPokemon->getName() << ".\n";
    } else {
        cout << "Selection complete. " << selectedPokemon->getName() << " now knows " << selectedMoves << " move(s).\n";
    }
}

/**
 * @brief Adds a Pokémon with selected moves to the user's team.
 *
 * This function walks the user through selecting a Pokémon type, choosing a Pokémon of that type,
 * selecting up to four moves for it, and then attempting to add this Pokémon to their team.
 *
 * @param team Pointer to the user's team.
 * @param pokemons List of all available Pokémon.
 * @param moves List of all available moves.
 * @return True if the Pokémon is successfully added, false otherwise (e.g., team is full).
 *
 * @note Students are not required to modify this function.
 */
bool addPokemonToTeam(Team* team, vector<unique_ptr<Pokemon>>& pokemons, const vector<Move>& moves) {
    // Return false if the team is full.
    if (team->isFull()) {
        cout << "Team is full. Cannot add more Pokémon.\nTry removing a Pokemon.\n\n";
        return false;
    }

    string selectedType;
    // Exit the process if a valid Pokémon type isn't selected.
    if (!selectPokemonType(selectedType)) return false;

    // Select a Pokémon of the chosen type.
    unique_ptr<Pokemon> selectedPokemon = selectPokemonFromType(selectedType, pokemons);
    if (!selectedPokemon) return false; // Exit if no Pokémon was selected.

    // Allow the user to select moves for the chosen Pokémon.
    selectMovesForPokemon(selectedPokemon, moves, selectedType);

    // Attempt to add the Pokémon to the team, returning the result.
    if (!team->addPokemon(std::move(selectedPokemon))) {
        cout << "Team is full. Cannot add more Pokémon.\nTry removing a Pokemon.\n";
        return false;
    } else {
        cout << "Successfully added " << selectedPokemon->getName() << " to your team!\n";
        return true;
    }

    return false;
}

/**
 * Allows the user to remove a Pokémon from their team by selecting from a printed list of current team members.
 * It ensures the selection is valid and returns true if the removal is successful, false otherwise.
 *
 * @param team A pointer to the Team object representing the player's team.
 * @return True if the removal is successful, false otherwise.
 */
bool removePokemonFromTeam(Team* team) {
    cout << "Choose a Pokemon to Remove:\n";
    // Print your team. (HINT: team has a printTeam function)
    //---
    team->printTeam();
    // Prompt for input to select a Pokémon to remove by calling the getInput() function.
    // subtract 1 from choice to line up with 0-based list
    // Validate the chosen number is within the valid range
    //---
    cout<<"Select a Pokemon to remove"<<endl;
    int choice=getInput();
    choice--;
    if(choice>-1 && choice<team->getSize()){
    cout<<"Successfully removed";
    cout<<team->getPokemon(choice)->getName()<<endl;
    team->removePokemon(choice);
    return true;
    }
    // Attempt to remove the selected Pokémon from the team using the relevant method.
    // If removal is successful, print "Successfully removed {pokemon name}" and return true.
    // If removal fails, print "Failed to remove the Pokémon." and return false.
    //---
    cout<<"Failed to remove the Pokémon."<<endl;
    return false;
}

/**
 * @brief Main entry point for Pokémon Joust: CSE 240 Gym Edition.
 *
 * Features:
 * - Load moves and Pokémon from JSON files.
 * - Manage a Pokémon team, allowing the user to view, add, or remove Pokémon.
 * - Engage in battles against random opponents or specific gym leaders.
 * - Persist team changes to a file.
 *
 * @param argc Argument count.
 * @param argv Argument vector.
 * @return int Returns 0 on successful execution.
 *
 * @note Students are not required to modify this function.
 */

int main(int argc, char ** argv) {
    vector<Move> moves = loadMoves("moves.json");
    vector<unique_ptr<Pokemon>> pokemons = loadPokemon("pokemon.json");

    // Instantiate a team object and load tem
    string filename = "team.json";
    Team *team = new Team();
    team->loadTeam(filename, pokemons, moves);

    // Instantiate a battle object to fight a random Pokémon
    Battle *battle = new Battle();

    int choice;
    do {

        cout << "---------\n" << "MAIN MENU\n" << "---------\n";
        cout << "1. Show Pokemon\n";
        cout << "2. Choose Pokemon\n";
        cout << "3. Remove Pokemon\n";
        cout << "4. Battle Random\n";
        cout << "5. Enter Gym\n";
        cout << "6. Exit\n";

        choice = getInput();

        switch(choice) {
            case 1:
                // Print your team
                cout << "Your Pokemon:" << endl;
                team->printTeam();
                break;
            case 2:
                // Add Pokémon to team
                if (addPokemonToTeam(team, pokemons, moves)) {
                    team->writeTeam(filename);
                }
                break;
            case 3:
                // Remove Pokémon from team
                if (removePokemonFromTeam(team)) {
                    team->writeTeam(filename);
                }
                break;
            case 4:
                // Enter the battle loop with a random Pokémon.
                battle->start(team, pokemons, moves);
                break;
            case 5:
                // Enter the CSE 240 Gym to fight the staff.
                enterGym();
                break;
            case 6:
                // Quit
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while(choice!=6);

    delete(team);
    delete(battle);
    return 0;
}
