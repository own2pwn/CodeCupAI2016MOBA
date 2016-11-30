//
//File: R_RolePusher.h
//Author: Ivlev Alexander. Stef
//Created: 30/11/2016
//

#pragma once

#include "R_Role.h"

namespace AICup
{
  class RolePusher: public Role {
  public:
    RolePusher() :
      Role({
        SkillBranches::meleeDamageFireBolt,
        SkillBranches::moveHast,
        SkillBranches::rangeMagicMissile,
        SkillBranches::magicalDamageFrostBolt,
        SkillBranches::armorShield,
      }) {
      audacity = -1.5;

      importanceOfXP = 1.5;
      importanceOfBonus = 0.5;


      buildPriority = 3.0;
      minionPriority = 1.0;
      treePriority = 1.5;
      wizardPriority = 1.5;

      audacityBuild = 1.5;
      audacityMinion = 1.0;
      audacityWizard = 1.25;
      attackSkillPriority = 1.5;

      desireChangeLine = 0.5;
      changeLinePathLengthPriority = 0.5;
      changeLineWizardCountPriority = 2;
      changeLineTowerBalancePriority = 2;
      changeLineLaneStrengthPriority = 2.5;
    }

    void update(const model::Wizard& self, model::Move& move) override {
      Role::update(self, move);

      audacity = -1.5 - 5 * (1 - (float(self.getLife()) / float(self.getMaxLife())));
    }
  };
}

