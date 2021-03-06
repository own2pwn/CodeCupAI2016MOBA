//
//File: R_RoundOne.h
//Author: Ivlev Alexander. Stef
//Created: 11/12/2016
//

#pragma once

#include "R_Role.h"
#include "R_SkillBuild.h"
#include "C_Extensions.h"

namespace AICup
{
  class RoundOneSkillBuild: public SkillBuild {
  public:
    RoundOneSkillBuild() : SkillBuild({
    }) {
    }
  };

  class RoundOneRole: public Role {
  public:
    RoundOneRole() {
      audacity = 0;
      linePressureWizards = 0.25;
      lineAudacityWizards = 2.0;

      importanceOfXP = 0.5;
      importanceOfBonus = 0.75;


      buildPriority = 3.0;
      minionPriority = 1.0;
      treePriority = 7.0;
      wizardPriority = 1.5;

      audacityBuild = 1.0;
      audacityMinion = 2.0;
      audacityWizard = 2.0;
      attackSkillPriority = 1.5;

      desireChangeLine = 0.9;
      changeLineForeFrontPriority = 0.25;
      changeLinePathLengthPriority = 1.5;
      changeLineWizardCountPriority = 0.5;
      changeLineWizardCountOnlyFriend = false;
      changeLineTowerBalancePriority = 1.0; // ����� ���� ��� ������ ��������� �����, ��� ������ ������
      changeLineLaneStrengthPriority = 0.7;
      changeLineExpiriencePriority = 0.8;


      attackMeleeWinThreshold = 0.2;
      attackWizardMeleePriority = 1.2;
      attackBuildMeleePriority = 1.0;
      attackMinionMeleePriority = 1.0;

      friendWizardConfidence = 0.8;
    }

    void update(const model::Wizard& self) override {
      Role::update(self);

      const auto realLife = self.getLife();

      audacity = -3.5 * (1 - (float(realLife) / float(self.getMaxLife())));

      linePressureWizards = 0.75 * float(realLife) / float(self.getMaxLife());
      lineAudacityWizards = 0.5 + 3.5 * (1 - (float(realLife) / float(self.getMaxLife())));
    }
  };
}

