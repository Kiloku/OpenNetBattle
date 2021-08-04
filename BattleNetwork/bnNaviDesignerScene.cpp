#include "bnNaviDesignerScene.h"
#include "bnTextureResourceManager.h"
#include <Segues/BlackWashFade.h>

#include "bnGridBackground.h"
#include "SFML/Graphics/Color.hpp"
#include "SFML/System/Vector2.hpp"
//A custom designed navi should have a "body plan".
//The "body plan" contains pieces picked by the user from an initial set.
//Each piece can be colored based on palettes.
//Possibly add a "body type" as well. Lean (proto, mega), Bulky (Guts), Tall (Normal, Glyde), floaty (Roll, Bass)
NaviDesignerScene::NaviDesignerScene(swoosh::ActivityController& controller) :
  label(Font::Style::thin),
  textbox(360,40, Font::Style::thin),
  Scene(controller)
{
  label.setScale(2.f, 2.f);

  baseSprite.setTexture(*LOAD_TEXTURE(DESIGNER_BASE));
  baseSprite.setScale(3.f, 3.f);
  baseSprite.setPosition(sf::Vector2f(240.f, 50.f));
  
  
  head = generateSelector(sf::Vector2f(100.f, 30.f), &headPieces);
  body = generateSelector(sf::Vector2f(100.f, 90.f), &bodyPieces);
  
  selected = &head;
  bg = new GridBackground();

  headPieces = std::vector<NaviDesignerScene::Piece>{
    {"Helmet", "A basic defensive helmet.", *LOAD_TEXTURE(DESIGNER_HELMET)},
    {"Helmet w. Mask", "A mask will help!", *LOAD_TEXTURE(DESIGNER_HELMET_WITH_MASK)},
    {"Helmet w. Visor", "It just looks so cool.", *LOAD_TEXTURE(DESIGNER_HELMET_WITH_VISOR)},
    {"Helmet Full Cover", "It just looks so cool.", *LOAD_TEXTURE(DESIGNER_HELMET_FULL_COVER)},
  };

  headPieceSprite.setTexture(headPieces[0].texture);
  headPieceSprite.setScale(3.f, 3.f);
  headPieceSprite.setPosition(sf::Vector2f(240.f+51.f, 47.f));

  bodyPieces = std::vector<NaviDesignerScene::Piece>{
    {"Plate", "An extra layer of protection!", *LOAD_TEXTURE(DESIGNER_PLATE)},
    {"ShoulderGrd", "For the melee focused navis.", *LOAD_TEXTURE(DESIGNER_SHOULDERPADS)},
    {"PlateAndShldr", "Full protection!", *LOAD_TEXTURE(DESIGNER_PLATE_AND_SHOULDERPADS)},
  };

  bodyPieceSprite.setTexture(bodyPieces[0].texture);
  bodyPieceSprite.setScale(3.f, 3.f);
  bodyPieceSprite.setPosition(sf::Vector2f(240.f + 25.f, 50.f + 21.f));

  // legPieces = std::vector<NaviDesignerScene::Piece>{
  //   {"Boots", "Basic footwear."},
  //   {"Skates", "Roll around!"}
  // };
  

  // Text box navigator
  textbox.setPosition(100, 205);
  textbox.SetTextColor(sf::Color::Yellow);
  textbox.Mute();

    textbox.SetText("NaviDesigner");
    textbox.CompleteAll();

  setView(sf::Vector2u(480, 320));
}

NaviDesignerScene::~NaviDesignerScene()
{
}

NaviDesignerScene::PieceSelector NaviDesignerScene::generateSelector(sf::Vector2f position, std::vector<Piece>* pieceList)
{
  PieceSelector result;
  
  //make a function to build those structs more easily.
  result.pieceList = pieceList;
  result.position = position;
  
  result.left.setTexture(*LOAD_TEXTURE(FOLDER_CURSOR));
  result.left.setScale(-2.f,2.f);
  result.left.setPosition(position);

  result.right.setTexture(*LOAD_TEXTURE(FOLDER_CURSOR));
  result.right.setScale(2.f, 2.f);
  result.right.setPosition(position + sf::Vector2f(50.f, 0.f));
  
  return result;
}


void NaviDesignerScene::onLeave()
{
}

void NaviDesignerScene::onExit()
{
}

void NaviDesignerScene::onEnter()
{
}

void NaviDesignerScene::onResume()
{
}

void NaviDesignerScene::onStart()
{
}

void NaviDesignerScene::onUpdate(double elapsed)
{
  textbox.Update(elapsed);

  bool selection_changed = true;
  
  if (Input().Has(InputEvents::pressed_ui_up)) {
    if (selected == &body) selected = &head;
  }
  else if (Input().Has(InputEvents::pressed_ui_down)) {
    if (selected == &head) selected = &body;
  }
  else if (Input().Has(InputEvents::pressed_ui_right))
  {
    if (selected->current == selected->pieceList->size()-1) selected->current = 0;
    else selected->current += 1;
  }
  else if (Input().Has(InputEvents::pressed_ui_left))
  {
    if (selected->current == 0) selected->current = selected->pieceList->size()-1;
    else selected->current -= 1;
  }
  else if (Input().Has(InputEvents::pressed_cancel)) {
    using namespace swoosh::types;
    getController().pop<segue<BlackWashFade, milli<500>>>();
    Audio().Play(AudioType::CHIP_DESC_CLOSE);
  }
  else
  {
    selection_changed = false;
  }

  std::vector<Piece> selectedPieceList = *(selected->pieceList);
  
  // textbox.SetText("batata");
  if (selection_changed)
  {
    headPieceSprite.setTexture(headPieces[head.current].texture);
    bodyPieceSprite.setTexture(bodyPieces[body.current].texture);
    textbox.SetText(selectedPieceList[selected->current].name);
    textbox.CompleteAll();
  }
  auto bounce = std::sin((float)totalElapsed * 20.0f);
  totalElapsed += static_cast<float>(elapsed);
}

void NaviDesignerScene::onDraw(sf::RenderTexture& surface)
{
  surface.draw(*bg);

  head.left.setColor(sf::Color::White);
  head.right.setColor(sf::Color::White);
  body.left.setColor(sf::Color::White);
  body.right.setColor(sf::Color::White);
  
  selected->left.setColor(sf::Color::Green);
  selected->right.setColor(sf::Color::Green);
  
  surface.draw(baseSprite);
  surface.draw(headPieceSprite);
  surface.draw(bodyPieceSprite);
  // head.left.setColor(sf::Color::Green);
  surface.draw(head.left);
  surface.draw(head.right);
  surface.draw(body.left);
  surface.draw(body.right);

  surface.draw(textbox);


}

void NaviDesignerScene::onEnd()
{
}
