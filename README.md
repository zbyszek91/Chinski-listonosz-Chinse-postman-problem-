# Chinski-listonosz-Chinse-postman-problem-
Rozpatrywany graf jest grafem spójnym, nieskierowanym i ważonym.

W grafie eulerowskim rozwiązanie problemu chińskiego listonosza stanowi cykl Eulera, jako że jest to z definicji ścieżka zamknięta przechodząca przez wszystkie krawędzie w grafie.

W grafie półeulerowskim rozwiązaniem problemu jest ścieżka Eulera (z definicji łącząca jedyne dwa wierzchołki nieparzystego stopnia w grafie) wraz z minimalną ścieżką (czyli ścieżką o najmniejszej sumie wag krawędzi) łączącą wierzchołki nieparzystego stopnia. Do wyznaczania najkrótszej ścieżki pomiędzy dwoma wierzchołkami służy algorytm Dijkstry.

Jeżeli rozpatrywany graf G nie jest półeulerowski, to znaczy, że posiada przynajmniej 4 wierzchołki nieparzystego stopnia. Z wszystkich wierzchołków nieparzystego stopnia grafu G należy utworzyć graf H, w taki sposób aby graf H był grafem pełnym z wagami na krawędziach odpowiadającymi najmniejszej ścieżce pomiędzy wierzchołkami w grafie G. W grafie H należy znaleźć minimalne skojarzenie doskonałe i uzupełnić o nie graf G. Graf G jest teraz multigrafem eulerowskim w którym rozwiązaniem problemu chińskiego listonosza jest cykl Eulera

Problem Chińskiego listonosza 
Pierwsze linia podaje liczbę wierzchołków n  
Druga linia podaje liczbę krawędzi m 
Każda z kolejnych m linii zawiera krotkę (u, v, c) reprezentującą krawędź, gdzie u i v to punkty końcowe (indeksowanie oparte na 0) krawędzi, a c to jej koszt.
uzycie: ./example -f <filename> <--minwaga | --maxwaga
 --minwaga dla idealnego dopasowania minimalnej wagi
 --maxwaga dla maksymalnego dopasowania kardynalnosci
czyli robimy folder w notatniku z danymi wpisujemy w kompilatorze  <filename> i wybieramy opcję --minwaga lub --maxwaga
Przykładowe wejście

10
16
0 1 15
0 2 4
1 2 3
1 5 2
1 6 2
2 3 1
2 4 2
3 4 5
4 6 4
4 7 1
4 8 3
5 6 1
6 7 2
7 8 3
7 9 2
8 9 1
przykładowe wyjście 
