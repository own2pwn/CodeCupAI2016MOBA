//
//File: E_Game.cpp
//Author: Ivlev Alexander. Stef
//Created: 09/11/2016
//

#include "E_Game.h"

using namespace AICup;

void Game::update(const model::Game& Game) {
  modelGame = &Game;
}

const model::Game& Game::model() const {
  return *modelGame;
}

model::Faction Game::enemyFaction(const model::Faction faction) const {
  if (model::FACTION_ACADEMY == faction) {
    return model::FACTION_RENEGADES;
  } else if (model::FACTION_RENEGADES == faction) {
    return model::FACTION_ACADEMY;
  } else {
    return model::FACTION_NEUTRAL;
  }
}