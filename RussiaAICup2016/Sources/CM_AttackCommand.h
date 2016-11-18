//
//File: CM_AttackCommand.h
//Author: Ivlev Alexander. Stef
//Created: 17/11/2016
//

#pragma once

#include "CM_Command.h"
#include "model\ActionType.h"

namespace AICup
{
  class AttackCommand;
  typedef std::shared_ptr<AttackCommand> AttackCommandPtr;

  class AttackCommand: public Command {
  public:
    struct Result {
      const model::LivingUnit* unit;
      model::ActionType action;
      int priority;
    };

  public:
    /// ��������� �������
    virtual void execute(const model::Wizard& self, Result& result) = 0;
  };
}
