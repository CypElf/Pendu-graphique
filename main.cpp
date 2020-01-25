#include <SFML/Graphics.hpp>
#include <iostream>
#include "pendu.h"
#include "logo.h"

const unsigned int WIDTH = 1000;
const unsigned int HEIGHT = 600;

 void makeSprite(sf::Texture &texture, sf::Sprite &sprite, std::string pathToImage, sf::Vector2f position) {
    if (!texture.loadFromFile(pathToImage)) {
        std::cerr << "Une erreur est survenue durant l'acces a `" << pathToImage << "`, veuillez verifier votre installation" << std::endl;
        exit(1);
    }

    texture.setSmooth(true);
    sprite.setTexture(texture);
    sprite.setPosition(position);
}


int main()
{
    std::string pathToWords { "res/txt/words.txt" };
    Pendu jeu { pathToWords };

    // pour les build debug (avec la console)
    std::cout << jeu.getMot() << std::endl;

    // ----------------------------------------
    // initialisation de la fenêtre et des variables

    sf::RenderWindow window { sf::VideoMode(WIDTH, HEIGHT), "Pendu", sf::Style::Titlebar | sf::Style::Close };
    window.setIcon(logo.width, logo.height, logo.pixel_data);
    
    sf::Font arial;
    std::string pathToPolice { "res/txt/arial.ttf" };
    if (!arial.loadFromFile(pathToPolice)) {
        std::cerr << "Une erreur est survenue durant l'acces a `" << pathToPolice << "` : veuillez verifier votre installation" << std::endl;
        return EXIT_FAILURE;
    }

    sf::Texture step0Texture, step1Texture, step2Texture, step3Texture, step4Texture, step5Texture, step6Texture, step7Texture, step8Texture;
    sf::Sprite step0, step1, step2, step3, step4, step5, step6, step7, step8;
    const sf::Vector2f positionSteps {  sf::Vector2f(WIDTH / 10.f, HEIGHT / 5.f) };
    makeSprite(step0Texture, step0, "res/img/step0.png", positionSteps);
    makeSprite(step1Texture, step1, "res/img/step1.png", positionSteps);
    makeSprite(step2Texture, step2, "res/img/step2.png", positionSteps);
    makeSprite(step3Texture, step3, "res/img/step3.png", positionSteps);
    makeSprite(step4Texture, step4, "res/img/step4.png", positionSteps);
    makeSprite(step5Texture, step5, "res/img/step5.png", positionSteps);
    makeSprite(step6Texture, step6, "res/img/step6.png", positionSteps);
    makeSprite(step7Texture, step7, "res/img/step7.png", positionSteps);
    makeSprite(step8Texture, step8, "res/img/step8.png", positionSteps);

    sf::Text saisissezCaractere;
    saisissezCaractere.setFont(arial);
    saisissezCaractere.setString(L"Saisissez un caractère : ");
    saisissezCaractere.setPosition(sf::Vector2f(270.f, 520.f));
    saisissezCaractere.setFillColor(sf::Color::Black);

    sf::Text caractereSaisi;
    caractereSaisi.setFont(arial);
    caractereSaisi.setPosition(605.f, 520.f);
    caractereSaisi.setFillColor(sf::Color::Black);
    
    sf::Text motAffiche;
    motAffiche.setFont(arial);
    motAffiche.setPosition(sf::Vector2f(static_cast<float>(400 - jeu.getMot().length() * 3), 60.f));
    motAffiche.setFillColor(sf::Color::Black);

    sf::Text result;
    result.setFont(arial);
    result.setFillColor(sf::Color::Black);

    sf::Text entreePourRecommencer;
    entreePourRecommencer.setFont(arial);
    entreePourRecommencer.setString("Faites entrée pour recommencer.");
    entreePourRecommencer.setPosition(sf::Vector2f(370.f, 35.f));
    entreePourRecommencer.setFillColor(sf::Color::Black);
    entreePourRecommencer.setCharacterSize(15);

    sf::Text lettresEssayees;
    lettresEssayees.setFont(arial);
    lettresEssayees.setFillColor(sf::Color::Black);
    lettresEssayees.setPosition(570.f, 160.f);
    std::string lettresEssayeesDeafultString { "Lettres ayant échouées :\n\n" };
    lettresEssayees.setString(lettresEssayeesDeafultString);

    char lettreEnAttente { ' ' };
    int compteurDeLettres { 0 };

    // ----------------------------------------

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::TextEntered)
            {
                if (event.text.unicode < 128) {
                    char lettre { static_cast<char>(event.text.unicode) };

                    if ((lettre == '\n' || lettre == '\r') && (jeu.getEtatJeu() == Pendu::Etat::failed || jeu.getEtatJeu() == Pendu::Etat::passed)) {
                        jeu = Pendu(pathToWords);

                        // pour les build debug (avec la console)
                        std::cout << jeu.getMot() << std::endl;

                        lettresEssayees.setString(lettresEssayeesDeafultString);
                    }

                    else if (jeu.getEtatJeu() == Pendu::Etat::inProgress && (lettre == '\n' || lettre == '\r') && ((lettreEnAttente >= 65 /* 65 = A */ && lettreEnAttente <= 90 /* 90 = Z */) || (lettreEnAttente >= 97 /* 97 = a */ && lettreEnAttente <= 122 /* 122 = z */))) {
                        if (jeu.estPresent(std::tolower(lettreEnAttente))) {
                            jeu.ajouteAuxTrouves(std::tolower(lettreEnAttente));
                        }
                        else {
                            jeu.decrementerChances();
                            lettresEssayees.setString(lettresEssayees.getString() + "          " + lettreEnAttente + "    ");
                            compteurDeLettres++;
                            if (compteurDeLettres == 2) {
                                lettresEssayees.setString(lettresEssayees.getString() + "\n\n");
                                compteurDeLettres = 0;
                            }
                            
                        }
                        lettreEnAttente = ' ';
                    }
                    else if (lettre == 8 && jeu.getEtatJeu() == Pendu::Etat::inProgress) { // 8 = backspace (l'afficher directement ne met pas un caractère invisible mais une sorte de carré, d'où la necessité de gérer ce cas)
                        lettreEnAttente = ' ';
                    }
                    else if (jeu.getEtatJeu() == Pendu::Etat::inProgress) {
                        lettreEnAttente = lettre;
                    }
                }
            }
        }

        caractereSaisi.setString(lettreEnAttente);

        std::string caracteresReveles = jeu.getCaracteresReveles();
        std::string caracteresRevelesEspaces = " ";
        for (char c : caracteresReveles) {
            caracteresRevelesEspaces.append(1, c);
            caracteresRevelesEspaces.append(" ");
        }
        caracteresRevelesEspaces.pop_back();
        motAffiche.setString(caracteresRevelesEspaces);

        window.clear(sf::Color::White);

        if (jeu.getEtatJeu() == Pendu::Etat::failed) {
            result.setPosition(sf::Vector2f(220.f, 0.f));
            result.setString("Vous avez perdu ! Le mot était : " + jeu.getMot());
        }

        if (jeu.getEtatJeu() == Pendu::Etat::passed) {
            result.setPosition(sf::Vector2f(360.f, 0.f));
            result.setString("Vous avez trouvé !");
        }

        if (jeu.getEtatJeu() == Pendu::Etat::failed || jeu.getEtatJeu() == Pendu::Etat::passed) {
            window.draw(result);
            window.draw(entreePourRecommencer);
        }

        switch (jeu.getChances()) {
        case 8:
            window.draw(step0);
            break;
        case 7:
            window.draw(step1);
            break;
        case 6:
            window.draw(step2);
            break;
        case 5:
            window.draw(step3);
            break;
        case 4:
            window.draw(step4);
            break;
        case 3:
            window.draw(step5);
            break;
        case 2:
            window.draw(step6);
            break;
        case 1:
            window.draw(step7);
            break;
        case 0:
            window.draw(step8);
            break;
        }

        window.draw(saisissezCaractere);
        window.draw(caractereSaisi);
        window.draw(motAffiche);
        window.draw(lettresEssayees);
        window.display();
    }

    return 0;
}