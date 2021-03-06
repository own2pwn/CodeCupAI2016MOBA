#include "CM_CommandCastHast.h"
#include "E_Game.h"
#include "E_World.h"

using namespace AICup;

CommandCastHast::CommandCastHast() {
}

bool CommandCastHast::check(const Wizard& self) {
  /// ���� ����������
  if (!self.availableAction(model::ACTION_HASTE)) {
    return false;
  }

  /// ����� � ��������
  if (self.cooldown(model::ACTION_HASTE) > 0) {
    return false;
  }

  // ��� ����
  if (self.getMana() < Game::model().getHasteManacost()) {
    return false;
  }


  /// ��� ���� ���������� �� ������� ����, �� ����� ���� ������� �� �����
  if (self.getMana() >= 2 * Game::model().getHasteManacost()) {
    return true;
  }

  /// ��� � ��� ��� ������
  for (const auto& status : self.getStatuses()) {
    if (status.getType() == model::STATUS_HASTENED) {
      return false;
    }
  }

  return true;
}


void CommandCastHast::execute(const Wizard& self, Result& result) {
  result.unit = &self;
  result.action = model::ACTION_HASTE;
  result.priority = 1000;

  for (const auto& wizard : World::model().getWizards()) {
    if (wizard.getFaction() != self.getFaction() || wizard.getDistanceTo(self) > self.getCastRange()) {
      continue;
    }

    bool found = false;
    for (const auto& status : wizard.getStatuses()) {
      if (status.getType() == model::STATUS_HASTENED) {
        found = true;
        continue;
      }
    }

    if (!found) {
      result.unit = &wizard;
      return;
    }
  }

  /// ����������� ������ �� ���� �� ����� ������, ���� ��� ���� ���� ������
  for (const auto& status : self.getStatuses()) {
    if (status.getType() == model::STATUS_HASTENED) {
      result.priority = 0;
    }
  }
}