#include "Dictionary.h"

/* Citeste din fisier tot dictionarul */
std::istream& operator>> (std::istream& in, Dictionary& dict){
	std::string word; 
	int freq;

	for (int i=0; i<MAX_WORD_NUMBER; i++){
		in >> word >> freq;
		dict.dictionary.insert( std::pair<std::string, int>(word, freq)); 
	}

	return in;
}

