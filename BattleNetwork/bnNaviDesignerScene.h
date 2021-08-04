#pragma once
#include "bnScene.h"
#include "bnText.h"
#include "bnTextBox.h"

#include <vector>

#include "bnBackground.h"
#include "SFML/System/Vector2.hpp"

class NaviDesignerScene : public Scene 
{
public:
    struct Piece
    {
        std::string name;
        std::string description;
        Texture texture;
    };
    
private:
    
    struct PieceSelector
    {
        sf::Vector2f position;
        sf::Sprite left;
        sf::Sprite right;
        unsigned current = 0;
        std::vector<Piece>* pieceList;

    };
    
    PieceSelector* selected;
    Text label;

    
    Sprite baseSprite;
    Sprite headPieceSprite;
    Sprite bodyPieceSprite;
    
    PieceSelector head;
    PieceSelector body;
    // PieceSelector leg;
    
    std::vector<Piece> headPieces;
    std::vector<Piece> bodyPieces;
    // std::vector<Piece> legPieces;

    TextBox textbox;
    float totalElapsed{};


    Background* bg; /*!< background graphics */
    
public:
    NaviDesignerScene(swoosh::ActivityController& controller);
    ~NaviDesignerScene();

    PieceSelector generateSelector(sf::Vector2f position, std::vector<Piece>* pieceList);
    
    void onLeave() override;
    void onExit() override;
    void onEnter() override;
    void onResume() override;
    void onStart() override;
    void onUpdate(double elapsed) override;
    void onDraw(sf::RenderTexture& surface) override;
    void onEnd() override;
    
};
