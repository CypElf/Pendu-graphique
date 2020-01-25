#ifndef PENDU_H
#define PENDU_H

#include <string>

class Pendu {
public:
    enum class Etat {
        inProgress,
        failed,
        passed
    };
    Pendu(std::string pathToFile);
    std::string getMot();
    std::string getCaracteresReveles();
    int getChances();
    Etat getEtatJeu();
    void setEtatJeu(Pendu::Etat etat);
    bool estPresent(char lettre);
    void decrementerChances();
    void ajouteAuxTrouves(char lettre);
private:
    std::string mot;
    std::string caracteresReveles;
    int chances { 8 };
    Etat etatJeu;
    std::string toLowerCase(std::string str);
};

#endif