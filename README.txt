Tema 1 PA - AVRAM Marius, 324CB 

Pentru calcularea distantei dintre un cuvant din dictionar si un cuvant citit de la tastaura am folosit algoritmul care calculeaza distanta 
Levenshtein (sursa Wikipedia).

Initial pornisem de la ideea sa calculez pentru fiecare subsecventa in parte distanta Levenshtein insa am descoperit ca se poate lua din matricea pentru un anumit string distanta pentru toate substringurile de forma [0..i] cu i de la 0 la n care provin din acel string. Astfel am redus considerabil numarul de calcule/iteratii. 

Din fraza initiala introdusa de la tastura, dupa ce am eliminat spatii goale am luat substringuri de forma [i..n] cu i de la 0 la n si pentru fiecare string in parte am aplicat algoritmul Levenshtein modificat. Astfel am obtinut o matrice superior triunghiulara cu cele mai bune potriviri de cuvinte pentru fiecare subsecventa din fraza noastra.

In cele din urma am aplicat pe aceasta matrice superior triunghiulara un algoritm bottom-up(programare dinamica) care determina cea mai buna combinatie pentru cuvintele determinate anterior. Daca combinatia currentChoice[i][k] + curentChoice [k][j] este mai buna decat combinatia anterioara previousChoice [i][j] atunci previous coice se actualizeaza. Se actualizeaza in sensul ca se aduga cuvantul respectiv in stringul lui previousChoice [i][j], se insumeaza distanta si frecventa si se actualizeaza numarul de cuvinte din wordNumber[i][j]. Rezultatul se va afla in coltul dreapta sus al matricii.

Mai multe informatii despre algoritm au evidentiate in comentariile din cod.
