#include <iostream>
#include <fstream>
#include <regex>
#include <chrono>
#include <vector>
#include <algorithm>
#include <string>
#include "HashMap.h"
int main () {
	using namespace std;
	using namespace std::chrono;

	ifstream input = ifstream("./WarAndPeace.txt");

	auto start = steady_clock::now();
	cout << "Parsing War and Peace" << endl;

	size_t nombre_lu = 0;


	// prochain mot lu
	string word;
	vector<pair<string, int>> word_counts;
	HashMap<std::string, int> wordCountMap(1000);
	// une regex qui reconnait les caractères anormaux (négation des lettres)
	regex re( R"([^a-zA-Z])");
	while (input >> word) {
		// élimine la ponctuation et les caractères spéciaux
		word = regex_replace ( word, re, "");
		// passe en lowercase
		transform(word.begin(),word.end(),word.begin(),::tolower);

		// if (!word.empty()) {

		bool found = false;
		int* count = wordCountMap.get(word);
		            if (count) {
		                // If the word exists, increment the count
		                (*count)++;
		            } else {
		                // Otherwise, insert the word with an initial count of 1
		                wordCountMap.put(word, 1);
		            }

		            /*for (auto& pair : word_counts) {
		                if (pair.first == word) {
		                    pair.second++;
		                    found = true;
		                    break;
		                }
		            }
		            if (!found) {
		                            word_counts.push_back(make_pair(word, 1));
		                        }
		    //    }
				*/
		// word est maintenant "tout propre"
		if (nombre_lu % 100 == 0)
			// on affiche un mot "propre" sur 100
			cout << nombre_lu << ": "<< word << endl;
		nombre_lu++;
	}
	input.close();

	cout << "Finished Parsing War and Peace" << endl;

	auto end = steady_clock::now();
    cout << "Parsing took "
              << duration_cast<milliseconds>(end - start).count()
              << "ms.\n";

    cout << "Found a total of " << nombre_lu << " words." << endl;
    vector<string> targets = {"war", "peace", "toto"};
        for (const auto& target : targets) {
        	int* count = wordCountMap.get(target);
        	        if (count) {
        	            cout << "The word '" << target << "' appears " << *count << " times." << endl;
        	        } else {
        	            cout << "The word '" << target << "' does not appear in the text." << endl;
        	        }
        }

    return 0;
}


