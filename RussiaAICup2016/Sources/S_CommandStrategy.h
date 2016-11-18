//
//File: CM_CommandStategy.h
//Author: Ivlev Alexander. Stef
//Created: 13/11/2016
//

#pragma once

#include "model\Wizard.h"
#include "model\Move.h"

#include "CM_MoveCommand.h"
#include "CM_AttackCommand.h"
#include "CM_CommandFabric.h"

#ifdef ENABLE_VISUALIZATOR
#include "Visualizator.h"
#endif


namespace AICup
{
  class CommandStategy {
  public:
    CommandStategy(const CommandFabric& fabric);

    virtual void update(const model::Wizard& self, model::Move& move);

#ifdef ENABLE_VISUALIZATOR
    virtual void visualization(const Visualizator& visualizator) const;
#endif // ENABLE_VISUALIZATOR

  protected:
    void clear();

  private:
    const Vector move(const model::Wizard& self, TurnStyle& turnStyle);
    const model::LivingUnit& attack(const model::Wizard& self, model::ActionType& action);

  protected:
    const CommandFabric& fabric;

    std::vector<MoveCommandPtr> moveCommands;
    std::vector<AttackCommandPtr> attackCommands;
  };

  typedef std::shared_ptr<CommandStategy> CommandStategyPtr;
};