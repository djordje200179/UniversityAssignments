#include <iostream>
#include "Biblioteka.hpp"
#include "Knjiga.hpp"

using namespace std;

int main() {
	Knjiga naDriniCuprija("Na Drini cuprija", "Ivo Andric");
	Knjiga stoGodinaSamoce("Sto godina samoce", "Garsija Markez");

	Biblioteka biblioteka("Gradska biblioteka", 10);
	biblioteka += naDriniCuprija;
	biblioteka += stoGodinaSamoce;

	cout << biblioteka;

	return 0;
}