//
//File: CM_CommandMoveToLine.h
//Author: Ivlev Alexander. Stef
//Created: 07/11/2016
//


#pragma once

#include "CM_MoveCommand.h"
#include "model\LaneType.h"

namespace AICup
{
  class CommandMoveToLine: public MoveCommand {
  public:
    CommandMoveToLine(Algorithm::PathFinder& finder, model::LaneType line);

    bool check(const Wizard& self) override;

    void execute(const Wizard& self, Result& result) override;

#ifdef ENABLE_VISUALIZATOR
    void visualization(const model::Wizard& self, const Visualizator& visualizator) const override;
#endif // ENABLE_VISUALIZATOR

  private:
    const model::LaneType line;

    MoveCommandPtr commandMoveToPoint;
  };
}