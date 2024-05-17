#include "data.h"
#include "pokemon.h"
#include "team.h"
#include "battle.h"

#include <iostream>
#include <fstream>
#include <string>
#include <cstdarg> // For va_list, va_start, and va_end
#include <cstdio>  // For printf and vprintf
#include <sstream>
#include <stdio.h>
#include <stdarg.h>
#include <random>


using namespace std;
using RNG = std::mt19937;


void printPassed(const char *format, ...) {
    va_list args;

    // Start red text
    printf("\x1b[32mTest PASSED ");

    // Initialize args to store all arguments after format
    va_start(args, format);

    // Print the formatted string with the variable arguments
    vprintf(format, args);

    // End red text
    printf("\x1b[0m");

    // Cleanup args
    va_end(args);
}

void printFailed(const char *format, ...) {
    va_list args;

    // Start red text
    printf("\x1b[31mTest Failed ");

    // Initialize args to store all arguments after format
    va_start(args, format);

    // Print the formatted string with the variable arguments
    vprintf(format, args);

    // End red text
    printf("\x1b[0m");

    // Cleanup args
    va_end(args);
}

template<typename T>
std::string toString(const T& value) {
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

template<typename T>
bool eval(const std::string& functionName, const T& value, const T& expected) {
   if (value != expected) {
        std::string valueStr = toString(value);
        std::string expectedStr = toString(expected);
        printFailed("for %s because %s did not match the expected value of %s \n", functionName.c_str(), valueStr.c_str(), expectedStr.c_str());
        return false;
    }
    return true; // Add a return statement for when the values do match
}

bool comparePokemon(const char * functionName, Pokemon *pm1, Pokemon *pm2){

    if (pm1->getName() != pm2->getName()){
        printFailed("for %s because pm1->getName() '%s' != pm2->getName() '%s' \n", functionName, pm1->getName().c_str(), pm2->getName().c_str());
        return false;
    }
    if (pm1->getType() != pm2->getType()){
        printFailed("for %s because pm1->getType() '%s' != pm2->getType() '%s' \n", functionName, pm1->getType().c_str(), pm2->getType().c_str());
        return false;
    }
    if (pm1->getHP() != pm2->getHP()){
        printFailed("for %s because pm1->getHP() '%d' != pm2->getHP() '%d' \n", functionName, pm1->getHP(), pm2->getHP());
        return false;
    }
    if (pm1->getAttack() != pm2->getAttack()){
        printFailed("for %s because pm1->getAttack() '%d' != pm2->getAttack() '%d' \n", functionName, pm1->getAttack(), pm2->getAttack());
        return false;
    }
    if (pm1->getDefense() != pm2->getDefense()){
        printFailed("for %s because pm1->getDefense() '%d' != pm2->getDefense() '%d' \n", functionName, pm1->getDefense(), pm2->getDefense());
        return false;
    }
    if (pm1->getSpAttack() != pm2->getSpAttack()){
        printFailed("for %s because pm1->getSpAttack() '%d' != pm2->getSpAttack() '%d' \n", functionName, pm1->getSpAttack(), pm2->getSpAttack());
        return false;
    }
    if (pm1->getSpDefense() != pm2->getSpDefense()){
        printFailed("for %s because pm1->getSpDefense() '%d' != pm2->getSpDefense() '%d' \n", functionName, pm1->getSpDefense(), pm2->getSpDefense());
        return false;
    }
    if (pm1->getSpeed() != pm2->getSpeed()){
        printFailed("for %s because pm1->getSpeed() '%d' != pm2->getSpeed() '%d' \n", functionName, pm1->getSpeed(), pm2->getSpeed());
        return false;
    }
    return true; 
}

bool comparePokemonToJsonStr(const char * functionName, Pokemon *pm1, string jsonStrPm2){
    string err;
    const auto json = json::parse(jsonStrPm2, err);
    unique_ptr<Pokemon> jpm2 = pokemonFactory(json);
    if (comparePokemon(functionName, pm1, jpm2.get() )){
        return true;
    }
    return false; 
}

bool testNormalPokemon(string functionName, Pokemon *npm ){
    
    string jsonstr = R"json({
        "id": 10,
        "name": "Pidgey",
        "type": "Normal",
        "base": {
            "HP": 40,
            "Attack": 45,
            "Defense": 40,
            "Sp. Attack": 35,
            "Sp. Defense": 35,
            "Speed": 56
        }
    }
    )json";
    functionName = functionName + " " + (__func__+4);
    cout << "\tTesting " << functionName << endl;
    if (!comparePokemonToJsonStr(functionName.c_str(), npm, jsonstr)){
       return false; 
    }
    if (!eval<float>(functionName, npm->getDamageMultiplier(NORMAL), 1.0)){
        printFailed("for %s because Normal's getDamageMultipler for NORMAL returned %f but it should have been 1.0\n", functionName, npm->getDamageMultiplier(NORMAL));
        return false;
    }
    
    if (!eval<float>(functionName, npm->getDamageMultiplier(GRASS), 1.0)){
        printFailed("for %s because  Normal's getDamageMultipler for GRASS returned %f but it should have been 1.0\n", functionName, npm->getDamageMultiplier(GRASS));
        return false;
    }
     cout << "\tPassed for " << functionName << endl;
    return true;
}

bool testGrassPokemon(string functionName, Pokemon *npm ){
    
    string jsonstr = R"json({
        "id": 1,
        "name": "Bulbasaur",
        "type": "Grass",
        "base": {
            "HP": 45,
            "Attack": 49,
            "Defense": 49,
            "Sp. Attack": 65,
            "Sp. Defense": 65,
            "Speed": 45
        }
    }
    )json";
    functionName = functionName + " " + (__func__+4);
    cout << "\tTesting " << functionName << endl;
    cout << npm->getType() <<endl;
    if (!comparePokemonToJsonStr(functionName.c_str(), npm, jsonstr)){
        return false;
    }
    if (!eval<float>(functionName, npm->getDamageMultiplier(FIRE), 2.0)){
        printFailed("for %s because Water's getDamageMultipler for FIRE returned %f but it should have been 2.0\n", functionName, npm->getDamageMultiplier(FIRE));
        return false;
    }
    
    if (!eval<float>(functionName, npm->getDamageMultiplier(ELECTRIC), 0.5)){
        printFailed("for %s because Water's getDamageMultipler for ELECTRIC returned %f but it should have been .5\n", functionName, npm->getDamageMultiplier(ELECTRIC));
        return false;
    }

    cout << "\tPassed for " << functionName << endl;
    return true;    
}


bool testFirePokemon(string functionName, Pokemon *npm ){
    
    string jsonstr = R"json({
        "id": 4,
        "name": "Charmander",
        "type": "Fire",
        "base": {
            "HP": 39,
            "Attack": 52,
            "Defense": 43,
            "Sp. Attack": 60,
            "Sp. Defense": 50,
            "Speed": 65
        }
    }
    )json";
    functionName = functionName + " " + (__func__+4);
    cout << "\tTesting " << functionName << endl;
    if (!comparePokemonToJsonStr(functionName.c_str(), npm, jsonstr)){
        return false;
    }
    if (!eval<float>(functionName, npm->getDamageMultiplier(WATER), 2.0)){
        printFailed("for %s because Water's getDamageMultipler for WATER returned %f but it should have been 2.0\n", functionName, npm->getDamageMultiplier(WATER));
        return false;
    }
    
    if (!eval<float>(functionName, npm->getDamageMultiplier(FIRE), 0.5)){
        printFailed("for %s because Water's getDamageMultipler for FIRE returned %f but it should have been .5\n", functionName, npm->getDamageMultiplier(FIRE));
        return false;
    }

    cout << "\tPassed for " << functionName << endl;
    return true;
}

bool testWaterPokemon(string functionName, Pokemon *npm ){
    
    string jsonstr = R"json({
        "id": 7,
        "name": "Squirtle",
        "type": "Water",
        "base": {
            "HP": 44,
            "Attack": 48,
            "Defense": 65,
            "Sp. Attack": 50,
            "Sp. Defense": 64,
            "Speed": 43
        }
    }
    )json";
    functionName = functionName + " " + (__func__+4);
    cout << "\tTesting " << functionName << endl;
    if (!comparePokemonToJsonStr(functionName.c_str(), npm, jsonstr)){
        return false; 
    }
    if (!eval<float>(functionName, npm->getDamageMultiplier(NORMAL), 1.0)){
        printFailed("for %s because Water's getDamageMultipler for NORMAL returned %f but it should have been 1.0\n", functionName, npm->getDamageMultiplier(NORMAL));
        return false;
    }
    
    if (!eval<float>(functionName, npm->getDamageMultiplier(FIRE), 0.5)){
        printFailed("for %s because Water's getDamageMultipler for FIRE returned %f but it should have been .5\n", functionName, npm->getDamageMultiplier(FIRE));
        return false;
    }
    
    
    cout << "\tPassed for " << functionName << endl;
    return true;
    
}


bool testElectricPokemon(string functionName, Pokemon *npm ){
    
    string jsonstr = R"json({
        "id": 17,
        "name": "Pikachu",
        "type": "Electric",
        "base": {
            "HP": 35,
            "Attack": 55,
            "Defense": 40,
            "Sp. Attack": 50,
            "Sp. Defense": 50,
            "Speed": 90
        }
    }
    )json";
    functionName = functionName + " " + (__func__+4);
    cout << "\tTesting " << functionName << endl;
    if (!comparePokemonToJsonStr(functionName.c_str(), npm, jsonstr)){
        return false; 
    }
    
    if (!eval<float>(functionName, npm->getDamageMultiplier(ROCK), 1.0)){
        printFailed("for %s because Electric's getDamageMultipler for ROCK returned %f but it should have been 1.0\n", functionName, npm->getDamageMultiplier(ROCK));
        return false;
    }
    
    if (!eval<float>(functionName, npm->getDamageMultiplier(ELECTRIC), 0.5)){
        printFailed("for %s because Eletric's getDamageMultipler for ELECTRIC returned %f but it should have been .5\n", functionName, npm->getDamageMultiplier(ELECTRIC));
        return false;
    }
    
    cout << "\tPassed for " << functionName << endl;
    return true;
}

bool testPokemon(){
    const char * functionName = (__func__+4);
    cout << "Starting test for " << functionName << endl;
    Pokemon *npm =  new NormalPokemon("Pidgey", 40, 45, 40, 35, 35, 56);
    if (!testNormalPokemon(functionName, npm)){
        return false; 
    }
    delete npm;
    npm =  new GrassPokemon("Bulbasaur", 45, 49, 49, 65, 65, 45);
    if (!testGrassPokemon(functionName, npm)){
        return false; 
    }

    delete npm;
    npm =  new FirePokemon("Charmander", 39, 52, 43, 60, 50, 65);
    if (!testFirePokemon(functionName, npm)){
        return false; 
    }

    delete npm;
    npm =  new WaterPokemon("Squirtle", 44, 48, 65, 50, 64, 43);
    if (!testWaterPokemon(functionName, npm)){
        return false; 
    }

    delete npm;
    npm =  new ElectricPokemon("Pikachu", 35, 55, 40, 50, 50, 90);
    if (!testElectricPokemon(functionName, npm)){
        return false; 
    }

    printPassed("for %s \n", functionName);
    return true; 
}

bool testLoadPokemon(){
    const char * functionName = (__func__+4);
    vector<unique_ptr<Pokemon>> pokemons = loadPokemon("pokemon.json");

    if (pokemons[1]->getName() == "Pidgeotto"){
        cout << "The program has not implemented the sub-classes yet, will test Pidgeotto" << endl;
        printFailed("for %s because pokemons[1].getName() == 'Pidgeotto', which is not the second pokemon but the 11th, the pokemonFactory must implement all the types\n", functionName);
        return false;
    }
    
    cout << "Starting test for " << functionName << endl;
    
    Pokemon *npm =  pokemons[0].get();
    if (!testGrassPokemon(functionName, npm)){
        return false; 
    }
    
    npm =  pokemons[3].get();
    if (!testFirePokemon(functionName, npm)){
        return false; 
    }
 
    npm =  pokemons[6].get();
    if (!testWaterPokemon(functionName, npm)){
        return false; 
    }
    npm =  pokemons[9].get();
    if (!testNormalPokemon(functionName, npm)){
        return false; 
    }
    
    npm =  pokemons[16].get();
    if (!testElectricPokemon(functionName, npm)){
        return false; 
    }

    printPassed("for %s \n", functionName);
    return true; 
}


bool testMoveName(){
    char functionName[] = "moveName";

    Move m = Move("Ember", "Fire", "Special", 40, 100, 25);

    if (!eval<string>(functionName, m.getName(), "Ember")){        
        return false;
    }

    printPassed("for %s\n", functionName);
    return true;

}

bool testMovePP(){
    char functionName[] = "movePP";

    Move m = Move("Ember", "Fire", "Special", 40, 100, 25);

    m.usePP();

    if (!eval<int>(functionName, m.getPP(), 24)){
        return false;
    }

    printPassed("for %s\n", functionName);
    return true;

}

bool testLoadMoves(){
    const char * functionName = (__func__+4);
    vector<Move> moves = loadMoves("moves.json");    
    
    if (moves[1].getName() != "Body Slam"){
        printFailed("for %s because moves[1].getName() == '%s' instead of '%s'\n", functionName, moves[1].getName().c_str() , "Body Slam");
        return false; 
    }

    if (moves[2].getType() != "Water"){
        printFailed("for %s because moves[2].getType() == '%s' instead of '%s'\n", functionName, moves[2].getType().c_str() , "Water");
        return false; 
    }

    if (moves[3].getCategory() != "Special"){
        printFailed("for %s because moves[3].getCategory() == '%s' instead of '%s'\n", functionName, moves[3].getCategory().c_str() , "Special");
        return false; 
    }

    if (moves[4].getPower() != 35){
        printFailed("for %s because moves[4].getPower() == '%d' instead of '%d'\n", functionName, moves[4].getPower() , 35);
        return false; 
    }
    
    if (moves[5].getAccuracy() != 90){
        printFailed("for %s because moves[5].getAccuracy() == '%d' instead of '%d'\n", functionName, moves[5].getAccuracy() , 90);
        return false; 
    } 

    if (moves[6].getPP() != 15){
        printFailed("for %s because moves[6].getPP() == '%d' instead of '%d'\n", functionName, moves[6].getPP() , 15);
        return false; 
    }
    

    printPassed("for %s \n", functionName);
    return true; 
}

bool testTeam(){
    char functionName[]="Team";
    
    string jsonString = "[{\"name\": \"Pikachu\",\"moves\": [\"Thunder Shock\", \"Quick Attack\", \"Thunder Punch\", \"Thunder Shock\"]},{\"name\": \"Squirtle\",\"moves\": [\"Water Gun\", \"Clamp\", \"Bubble\", \"Waterfall\"]}]";
    std::ofstream outFile("/tmp/battlers.json");
    if (outFile.is_open()) {
        outFile << jsonString;
        outFile.close();
        std::cout << "JSON data written to /tmp/battlers.json" << std::endl;
    } else {
        std::cout << "Unable to open file for writing." << std::endl;
        return false;
    }

    vector<Move> moves = loadMoves("moves.json");

    vector<unique_ptr<Pokemon>> pokemons = loadPokemon("pokemon.json");

    Team *team = new Team();
    team->loadTeam("/tmp/battlers.json", pokemons, moves);

    Pokemon * pokemon1 = team->getPokemon(0);
    Pokemon * pokemon2 = team->getPokemon(1);

    if (!eval<string>(functionName, pokemon1->getName(), "Pikachu")){
        return false;
    }

    if (!eval<string>(functionName, pokemon2->getName(), "Squirtle")){
        return false;
    }

    delete(team);
    printPassed("for %s\n", functionName);
    return true;
}

bool testCalculateDamage() {
    char functionName[] = "CalculateDamage";
    Battle battle;

    // Normal
    int result = battle.calculateDamage(50, 50, 60, 1.0f, 1.0f, 1.0f);
    if (!eval<int>(functionName, result, 9)) {
        return false;
    }

    // STAB
    result = battle.calculateDamage(50, 50, 60, 1.5f, 1.0f, 1.0f);
    if (!eval<int>(functionName, result, 13)) {
        return false;
    }

    // Type advantage
    result = battle.calculateDamage(50, 50, 60, 1.0f, 2.0f, 1.0f);
    if (!eval<int>(functionName, result, 17)) {
        return false;
    }

    // Type disadvantage
    result = battle.calculateDamage(50, 50, 60, 1.0f, 0.5f, 1.0f);
    if (!eval<int>(functionName, result, 4)) {
        return false;
    }

    // Random
    result = battle.calculateDamage(50, 50, 60, 1.0f, 1.0f, 0.85f);
    if (!eval<int>(functionName, result, 7)) {
        return false;
    }

    printPassed("for %s\n", functionName);
    return true;
}

bool testApplyMoveDamage() {
    char functionName[] = "testApplyMoveDamage";
    cout << "Starting test for " << functionName << endl;

    FirePokemon attacker("Charmander", 55, 50, 43, 60, 50, 65);
    GrassPokemon defender("Bulbasaur", 45, 49, 49, 65, 65, 45);
    Move ember("Ember", "Fire", "Special", 40, 100, 25);

    Battle battle;
    RNG rng(std::random_device{}());

    // Silencing the console output during the test to avoid clutter
    std::streambuf* origCoutStreamBuf = std::cout.rdbuf();
    std::ostringstream tempBuffer;
    std::cout.rdbuf(tempBuffer.rdbuf());


    // move hits
    bool faint = battle.applyMoveDamage(&attacker, &defender, &ember, rng);
    if (!eval<bool>(functionName, (defender.getHP() < 45 && !faint), true)) {
        std::cout.rdbuf(origCoutStreamBuf);
        printFailed("%s: Move should hit but did not.\n", functionName);
        return false;
    }

    defender.setHP(45);

    // move misses
    Move tackle("Tackle", "Normal", "Physical", 50, 0, 35);
    faint = battle.applyMoveDamage(&attacker, &defender, &tackle, rng);
    if (!eval<bool>(functionName, (defender.getHP() == 45 && !faint), true)) {
        std::cout.rdbuf(origCoutStreamBuf);
        printFailed("%s: Move should miss but did not.\n", functionName);
        return false;
    }

    // defender faints
    Move powerfulMove("moondrop", "Normal", "Special", 423814745, 100, 5);
    faint = battle.applyMoveDamage(&attacker, &defender, &powerfulMove, rng);
    if (!eval<bool>(functionName, (defender.getHP() == 0 && faint), true)) {
        std::cout.rdbuf(origCoutStreamBuf);
        printFailed("%s: Move did not cause defender to faint as expected.\n", functionName);
        return false;
    }

    std::cout.rdbuf(origCoutStreamBuf);

    printPassed("%s\n", functionName);
    return true;
}


int main(int argc, char ** argv){
    if (argc == 1){
        if (!testPokemon()){
            return 142;
        }
        if (!testLoadPokemon()){
            return 143;
        }
        if (!testMoveName()){
            return 144;
        }
        if (!testMovePP()){
            return 146;
        }
        if (!testLoadMoves()){
            return 146;
        }
        if (!testTeam()){
            return 146;
        }
        if (!testCalculateDamage()){
            return 147;
        }
        if (!testApplyMoveDamage()){
            return 147;
        }
        cout << "All tests completed successfully!" << endl;
        return 0;
    }

    string firstArg = argv[1];

    if (firstArg == "testPokemon" || firstArg == "1"){
        return testPokemon() ? 0 : 129;
    }

    if (firstArg == "testLoadPokemon" || firstArg == "2"){
        return testLoadPokemon() ? 0 : 129;
    }
    if (firstArg == "testMoveName" || firstArg == "3"){
        return testMoveName() ? 0 : 129;
    }
    if (firstArg == "testMovePP" || firstArg == "4"){
        return testMovePP() ? 0 : 129;
    }
    if (firstArg == "testLoadMoves" || firstArg == "5"){
        return testLoadMoves() ? 0 : 129;
    }
    if (firstArg == "testTeam" || firstArg == "6"){
        return testTeam() ? 0 : 129;
    }
    if (firstArg == "testCalculateDamage" || firstArg == "7") {
        return testCalculateDamage() ? 0 : 129;
    }
    if (firstArg == "testApplyMoveDamage" || firstArg == "8") {
        return testApplyMoveDamage() ? 0 : 129;
    }
    return 0;
}


