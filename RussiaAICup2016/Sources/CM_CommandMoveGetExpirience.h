//
//File: CM_CommandMoveGetExpirience.h
//Author: Ivlev Alexander. Stef
//Created: 15/11/2016
//


#pragma once

#include "CM_MoveCommand.h"
#include "C_Vector2D.h"

namespace AICup
{
  class CommandMoveGetExpirience: public MoveCommand {
  public:
    CommandMoveGetExpirience(Algorithm::PathFinder& finder);

    bool check(const Wizard& self) override;

    void execute(const Wizard& self, Result& result) override;


#ifdef ENABLE_VISUALIZATOR
    void visualization(const model::Wizard& self, const Visualizator& visualizator) const override;
#endif // ENABLE_VISUALIZATOR

  private:
    const model::LivingUnit* unit;
    MoveCommandPtr followCommand;
  };
}