#include <fstream>
#include <random>
#include <stdexcept>
#include <iostream>
#include <vector>
#include <algorithm>
#include "pendu.h"

std::string Pendu::toLowerCase(std::string str) {
	transform(str.begin(), str.end(), str.begin(), ::tolower);
	return str;
}

Pendu::Pendu(std::string pathToFile) : etatJeu(Etat::inProgress) {
	std::ifstream liste { pathToFile };
	if (!liste) {
		std::cerr << "Une erreur est survenue durant l'acces a `res/words.txt` : veuillez verifier votre installation" << std::endl;
		exit(1);
	}

	std::string ligne { "" };
	std::vector<std::string> mots;

	while (std::getline(liste, ligne)) {
		mots.push_back(ligne);
	}
	liste.close();

	std::random_device rd;
	std::mt19937 generator { rd() };
	std::uniform_int_distribution<int> dist { 0, static_cast<int>(mots.size() - 1) };

	int choixAleatoire { dist(generator) };
	this->mot = mots[choixAleatoire];
	this->mot = this->toLowerCase(mot);
	for (int i = 0 ; i < static_cast<int>(this->mot.length()) ; i++) {
		this->caracteresReveles.append("_");
	}
}

bool Pendu::estPresent(char lettre) {
	bool resultat { false };
	for (int i = 0; i < static_cast<int>(this->mot.length()); i++) {
		if (tolower(lettre) == tolower(this->mot.at(i))) {
			resultat = true;
		}
	}
	return resultat;
}

void Pendu::ajouteAuxTrouves(char lettre) {
	bool estComplet { true };
	for (int i = 0; i < static_cast<int>(this->mot.length()); i++) {
		if (tolower(lettre) == this->mot.at(i)) {
			std::string tmp { "" };
			tmp += lettre;
			this->caracteresReveles.replace(i, 1, tmp);
		}
		if (tolower(this->caracteresReveles.at(i)) == '_') {
			estComplet = false;
		}
	}
	if (estComplet) {
		this->etatJeu = Etat::passed;
	}
}

void Pendu::decrementerChances() {
	this->chances--;
	if (!(this->chances > 0)) {
		this->etatJeu = Etat::failed;
	}
}

std::string Pendu::getMot() {
	return this->mot;
}

int Pendu::getChances() {
	return this->chances;
}

std::string Pendu::getCaracteresReveles() {
	return this->caracteresReveles;
}

Pendu::Etat Pendu::getEtatJeu() {
	return this->etatJeu;
}

void Pendu::setEtatJeu(Pendu::Etat etat) {
	this->etatJeu = etat;
}