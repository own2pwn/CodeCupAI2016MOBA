//
//File: S_FisrtStrategy.h
//Author: Ivlev Alexander. Stef
//Created: 14/11/2016
//

#pragma once

#include "S_CommandStrategy.h"

namespace AICup
{
  class FirstStrategy: public CommandStrategy {
  public:
    FirstStrategy(const CommandFabric& fabric, const Algorithm::PathFinder& pathFinder);

    void update(const model::Wizard& self, model::Move& move) override;

  private:
    void init(const model::Wizard& self);

    void changeLane(const model::Wizard& self);

  private:
    bool isInitialized;
    model::LaneType myLine;
    int lastChangeLineTick;

    MoveCommandPtr moveToBonus;
  };
};