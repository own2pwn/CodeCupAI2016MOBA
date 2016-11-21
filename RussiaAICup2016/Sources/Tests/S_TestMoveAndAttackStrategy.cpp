#include "S_TestMoveAndAttackStrategy.h"
#include "E_World.h"

using namespace AICup;

TestMoveAndAttackStrategy::TestMoveAndAttackStrategy(const CommandFabric& fabric) : CommandStategy(fabric) {
  currentMoveCommandIndex = 2;

  allMoveCommands.push_back(fabric.moveToPoint(800, 3200));
  allMoveCommands.push_back(fabric.moveToPoint(200, 2400));
  allMoveCommands.push_back(fabric.moveToPoint(1200, 1200));
  allMoveCommands.push_back(fabric.moveToPoint(1600, 2400));
  allMoveCommands.push_back(fabric.moveToPoint(1200, 3800));
  allMoveCommands.push_back(fabric.moveToPoint(2800, 3800));
  allMoveCommands.push_back(fabric.moveToPoint(2800, 2800));
  allMoveCommands.push_back(fabric.moveToPoint(1600, 2400));
}

void TestMoveAndAttackStrategy::update(const model::Wizard& self, model::Move& move) {
  CommandStategy::clear();

  if (2.5 * 2.5 > self.getSpeedX()*self.getSpeedX() + self.getSpeedY()*self.getSpeedY()) {
    printf("Small Speed: %f %f\n", self.getSpeedX(), self.getSpeedY());
  }

  for (const auto& enemy : World::instance().aroundEnemies(self)) {
    const auto attackCommand = fabric.attack(enemy->getId());
    if (attackCommand->check(self)) {
      attackCommands.push_back(attackCommand);
      break;
    }
  }

  auto& command = allMoveCommands[currentMoveCommandIndex];

  while (!command->check(self)) {
    currentMoveCommandIndex = (currentMoveCommandIndex + 1) % allMoveCommands.size();
    command = allMoveCommands[currentMoveCommandIndex];
  }

  moveCommands.push_back(command);

  CommandStategy::update(self, move);
}