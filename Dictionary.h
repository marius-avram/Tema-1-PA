#ifndef __DICTIONARY_H__
#define __DICTIONARY_H__

#pragma once

#include <iostream> 
#include <string>
#include <map>

#define MAX_WORD_NUMBER 8000
#define INF 64
#define ERROR_MARGIN 2

typedef std::pair<std::string, int> dictionaryEntry;

/* Contine informatii despre cuvintele care sunt canditate pentru 
 * inlocuirea cuvintelor gresite. */
struct wordInfo{
	dictionaryEntry entry;
	int distance;

	wordInfo() { };

	wordInfo(std::string word, int frequency, int distance) :
			entry(word, frequency), distance(distance){ };
	
	wordInfo(dictionaryEntry entry, int distance) :
			entry(entry), distance(distance) { };
};


struct Dictionary {
 
	std::map<std::string, int> dictionary;
	int wordNo;

	/* Constructorul default */
	Dictionary() : wordNo(MAX_WORD_NUMBER) {}

	/* Determina daca un cuvant se regaseste in dictionar */
	bool isDictionaryEntry(std::string word){
		return dictionary.count(word) > 0;
	}

	/* Returneaza frecventa unui cuvant din dictionar */
	int getFrequency(std::string word){
		return dictionary[word];
	}


	/* Metoda care face citirea din fisier a intregului dictionar */
	friend std::istream& operator>> (std::istream& in, Dictionary& dict);

};

#endif
