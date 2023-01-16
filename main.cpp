/**
 * Title:   Stable Matching Problem
 * Purpose: ** Implementation of the Gale-Shapley algorithm to solve the stable matching problem **
 * Course:  CSC 3430 Winter 2023
 * Author:  ** Kaddijatou Baldeh **
 *
 */

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <queue>

using std::vector;
using std::queue;
using std::map;
using std::string;
using std::cerr;
using std::ifstream;
using std::cout;
using std::endl;

// Declaration of a helper function
bool checkPreference(const vector<string>& WomanPicks, const string& first, const string& second);

/**
 * Main function reads the men and women preference list from a .txt file.
 * Creates a 2D matrix of each preference
 * Matches each couples and output the info
 * O(n^2) time complexity by maintaining a list of free men in a queue and two list of preferences
 */

int main() {
    string preference, option;
    vector<string> menTemp, womenTemp;
    vector<vector<string>> menPick, womenPick;

    // Read men preference list from file
    ifstream read ("../listofmen.txt");
    if (read.fail()) {
        cerr << "Unable to open file" << endl;
        return -1;
    }
    // Create a 2d grid of the men preferences
    while (read >> preference){
        menTemp.push_back(preference);
        if(menTemp.size() == 4){
            menPick.push_back(menTemp);
            menTemp.clear();
        }
    }
    // Read women preference list from file
    ifstream in ("../listofwomen.txt");
    if (in.fail()) {
        cerr << "Unable to open file" << endl;
        return -1;
    }
    // Create a 2d grid of the women preferences
    while (in >> option){
        womenTemp.push_back(option);
        if(womenTemp.size() == 4){
            womenPick.push_back(womenTemp);
            womenTemp.clear();
        }
    }
    // Create a map of the preferences and couple
    map<string, vector<string>> menChoice, womenChoice;
    map<string, string> couples;
    // Queue to maintain a list of free men
    queue<string> freeMen;

    // Push the choices into the map and the list of men in a queue
    for (int i = 0; i < menPick.size(); ++i){
        for (int j = 1; j < menPick.size()+1; ++j){
            menChoice[menPick[i][0]].push_back(menPick[i][j]);
            womenChoice[womenPick[i][0]].push_back(womenPick[i][j]);
        }
        // Push the list of men in a queue
        freeMen.push(menPick[i][0]);
    }

    // While there is a man that is not in a relationship
    while(!freeMen.empty()){
        // Take a free man from the list of free men
        auto m = freeMen.front();
        // Take the man's choice list and select his first choice top w of women
        auto choiceList = menChoice[m];
        for (auto ptr = choiceList.begin(); ptr != choiceList.end(); ++ptr){
            auto w = *ptr;
            // If w is free then (m, w) become engaged
            if (couples.find(w) == couples.end()){
                couples[w] = m;
                break;
            }
            // Check if w prefers m’ to m
            auto otherMan = couples[w];
            if (checkPreference(womenChoice[w], m, otherMan)){
                // m becomes free if w prefers m' to m
                freeMen.push(otherMan);
                // (m, w) become engaged
                couples[w] = m;
                break;
            }
        }
        // Remove the paired man from the queue
        freeMen.pop();
    }
    // Print out the matched couples
    for(auto itr = couples.cbegin(); itr != couples.cend(); ++itr){
        cout << itr->first << " and " << itr->second << " are happy couples." << endl;
    }
    return 0;
}

/**
 * Implementation of Helper function to check the preference if m is before m'
 * @param WomanPicks woman's preference list of men
 * @param first represents m (m from the set of men)
 * @param second represents m' (m' from the set of men)
 * @return true if m comes before m', false otherwise
 */

bool checkPreference(const vector<string>& WomanPicks, const string& first, const string& second){
    for (auto itr = WomanPicks.begin(); itr != WomanPicks.end(); ++itr){
        if (*itr == first){
            return true;
        }
        if (*itr == second){
            return false;
        }
    }
    return false;
}




