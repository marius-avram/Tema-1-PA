#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <fstream>
#include "Dictionary.h"

typedef std::pair<int, wordInfo> wordAndIndex;

/* Determina distanta Levenshtein dintre primul cuvant dat ca parametru,
 * ce reprezinta un cuvant din dictionar si toate substringurile ce rezulta
 * din al doilea cuvant. 
 * Substringurile sunt de forma [0..i] unde i este o valoare de la 0 la nr de 
 * caractere din word2. Distantele substringurilor se afla pe ultima linie 
 * din matricea Levenshtein. De aceea este eficient sa calculam distantele 
 * pentru toate substringurile de forma enuntata mai sus pentru a nu repeta 
 * inutil aceleasi operatii.
 
 * Functia va returna un vector de perechi (int,wordInfo). int-ul reprezinta 
 * indexul substringului in matricea otinuta in urma aplicarii algoritmului 
 * Levenshtein. Iar wordInfo contine cuvantul din dictionar pentru care word2
 * are distanta minima.
 *
 */
std::vector<wordAndIndex> levenshteinDistance(dictionaryEntry word1, std::string word2){
	
	std::vector<wordAndIndex> result;
	int m = word1.first.size(), 
		n = word2.size();
	int d[INF][INF];

	if (word1.first == word2) { 
		/*Tratam cazul in care cuvantul dat este chiar unul din dictionar*/ 
		result.push_back(wordAndIndex(m-1,wordInfo(word1, 0)));
	} 
	else {
		/* Altfel face Levenshtein */
		for (int i=0; i<m+1; i++){

			for(int j=0; j<n+1; j++){
				if (j==0){
					d[i][j] = i;
				}
				if (i==0){
					d[i][j] = j;
				}
			}
		}

		for (int j=1; j<=n; j++){
			for (int i=1; i<=m; i++){
				if (word1.first[i-1] == word2[j-1]) {
					/* caracterele corespund */
					d[i][j] = d[i-1][j-1];
				}
				else {
					/* o stergere, insertie sau substitutie */
					d[i][j] = std::min(d[i-1][j]+1, std::min(d[i][j-1]+1, d[i-1][j-1]+1));
				}

			}
		}

		/* Se insereaza in vectorul de perechi rezultat cuvantul din dictionar dat 
		 * ca parametru impreuna cu distanta pe care trebuie sa o aiba substringul
		 * pentru a ajunge la el. In vector se introduc doar cuvinte care au
		 * distanta mai mica decat ERROR_MARGIN. S-a ales aceasta limita pentru 
		 * scaderea timpului de rulare total. */
		for (int j=1; j<=n; j++){
			if (d[m][j] <= ERROR_MARGIN){ 
			result.push_back(wordAndIndex(j-1, wordInfo(word1, d[m][j])));
			}
		}
	
	}


	return result;

}

/* Va genera cele mai bune alternative de cuvinte pentru subsecventele de 
 * forma (i..n) unde i este la 0 la n. Aceste alternative vor fi tinute 
 * intr-o matrice superior triunghiulara. Astfel pe prima linie vom avea 
 * alternativele pentru tot substringul, pe a doua linie vor fi toate 
 * alternativele pentru substringul de la 1 la n, etc. 
 * 
 * Se returneaza matricea de wordInfo. Pe langa cuvinte este nevoie sa stim 
 * distanta cu care s-a ajuns la acele cuvinte si frecventa.*/
 
std::vector< std::vector<wordInfo> > getBestMatches(Dictionary& d, std::string phrase){
	
	std::vector< std::vector<wordInfo> > tableMatches;
	std::string auxString;
	
	/*Se genereaza toate subStringurile */
	for (unsigned int i=0; i < phrase.size(); i++){
		auxString = phrase.substr(i, phrase.size() - i);
		std::map<std::string, int>::iterator it;
		tableMatches.push_back(std::vector<wordInfo>()); 
		
		for (unsigned int j = 0; j<phrase.size(); j++){ 
			tableMatches[i].push_back(wordInfo(dictionaryEntry(), INF));
		}
		/* Parcurgem tot dictionarul pentru subStringul de fata. 
		 * Levenshtein v-a returna toate rezultate care provin din substringul
		 * dat ca parametru */
		for (it = d.dictionary.begin(); it != d.dictionary.end(); ++it){
			
			std::vector<wordAndIndex> auxMatches = levenshteinDistance(
													dictionaryEntry((*it).first, (*it).second), 
													auxString);
			
			/* Se actualizeaza coloana din matricea tableMatches daca este cazul */
			for (unsigned int j=0; j< auxMatches.size(); j++){

				int index = auxMatches[j].first + i;
				wordInfo currentWord = auxMatches[j].second;

				if (currentWord.distance < tableMatches[i][index].distance){
					/* Distanta mai mica */
					tableMatches[i][index] = currentWord;
				}
				else if (currentWord.distance == tableMatches[i][index].distance) {
					if (currentWord.entry.second > tableMatches[i][index].entry.second){
						/* Distanta egala si frecventa mai mare */
						tableMatches[i][index] = currentWord;
					}
					else if (currentWord.entry.second == tableMatches[i][index].entry.second){
						if (currentWord.entry.first < tableMatches[i][index].entry.first){
							/* Distanta si frecventa egale, cuvantul gasit lexicografic
							 * mai mic decat cel din matrice */
							tableMatches[i][index] = currentWord;
						}
					}
				}
			}
		}
	
	}
	
	return tableMatches;
}

/* Din matricea superior triunghiulara cu cele mai bune alternative pentru 
 * fiecare subsecventa ia cea mai buna combinatie cu ajutorul programarii dinamice. 
 * 
 * Returneaza rezultatul sub forma de string. Acesta se afla in coltul din dreapta 
 * sus al matricii triunghiulare */
 
std::string correctPhrase(Dictionary& d, std::string phrase){
	
	/*Stergem spatiile din fraza data la tastatura. */
	for (unsigned int i=0; i<phrase.size(); i++){
		if (phrase[i] == ' ') phrase.erase(i,1);
	}
	
	
	/* Matricea ce contine cele mai bune cuvinte pentru fiecare subsecvent. */
	std::vector< std::vector<wordInfo> >  tableMatches = getBestMatches(d, phrase); 
	/* O matrice ce retine numarul de cuvinte din fiecare fraza. 
	 * Vom prelucra tabloul obtinut cu ajutorul functiei getBestMatches 
	 * astfel incat va ajunge sa contina fraze in loc de cuvinte. */
	std::vector< std::vector<int> > wordNumber;
	for (unsigned int i=0; i<tableMatches.size(); i++){
		wordNumber.push_back(std::vector<int>());
		for (unsigned int j=0; j<tableMatches[i].size(); j++){
			wordNumber[i].push_back(1);
		}
	}
	
	
	
	wordInfo previousChoice, currentChoice;
	/* Cu ajutorul programarii dinamice incercam sa gasim cea mai buna 
	 * combinatie de cuvinte */
	for (unsigned int len = 2; len <= tableMatches.size(); len++) {
		for (unsigned int i = 0 ; i <= tableMatches.size() - len; i++) {
			unsigned int j = i + len - 1;

			previousChoice = tableMatches[i][j];
			for (unsigned int k = i; k < j; k++) {

					/* Verificam daca o noua combinatie de cuvinte ne imbunatateste
					 * distanta, frecventa, nr. de cuvinte, etc */
					int newDist = tableMatches[i][k].distance + 
								  tableMatches[k+1][j].distance;
					int newFreq = tableMatches[i][k].entry.second + 
								  tableMatches[k+1][j].entry.second;
					int newWordNumber = wordNumber[i][k] + wordNumber[k+1][j];
					std::string newPhrase = tableMatches[i][k].entry.first;
					newPhrase += " ";
					newPhrase += tableMatches[k+1][j].entry.first;
					wordInfo currentChoice(newPhrase, newFreq, newDist);


					/*Se actualizeaza daca distanta este mai mica */
					if (newDist < previousChoice.distance){
						previousChoice = currentChoice;
						wordNumber[i][j] = newWordNumber;
					} 
					else if (newDist == previousChoice.distance){ 
						/*Se actualizeaza daca distanta egala si nr cuvinte mai mic*/
						if (newWordNumber < wordNumber[i][j]){
							previousChoice = currentChoice;
							wordNumber[i][j] = newWordNumber;
						}
						else if (newWordNumber == wordNumber[i][j]){
							/*Se actualizeaza daca frecventa este mai mare */
							if (newFreq > previousChoice.entry.second) { 
								previousChoice = currentChoice;
								wordNumber[i][j] = newWordNumber;
							}
							else if (newFreq == previousChoice.entry.second){
							/* Se actualizeaza daca este mai mic ortografic decat cel curent*/
								if (newPhrase < previousChoice.entry.first){
									previousChoice = currentChoice;
									wordNumber[i][j] = newWordNumber;								
								}
							}
						}
					}

				}

	
			tableMatches[i][j] = previousChoice;
			
		}
	}

	return tableMatches[0].back().entry.first;
}


int main(){
	std::ifstream fis;
	fis.open("dict.txt");
	if (!fis.is_open()){ 
		std::cerr << "Nu s-a gasit fisierul!";
		return -1;
	}
	Dictionary dictionary;
	fis >> dictionary;
	
	std::string phrase;
	getline(std::cin, phrase);
	
	if (phrase.size() <= 64){

		std::cout << correctPhrase(dictionary, phrase) << "\n";
		
	}
	else {
		std::cerr << "Dimensiunea stringul este > 64 !";
		return -1;
	}
	
	fis.close();

	return 0;
}
