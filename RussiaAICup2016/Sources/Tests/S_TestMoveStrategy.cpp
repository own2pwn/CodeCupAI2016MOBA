#include "S_TestMoveStrategy.h"
#include "E_World.h"

using namespace AICup;

TestMoveStrategy::TestMoveStrategy(const CommandFabric& fabric, const Algorithm::PathFinder& pathFinder):
  CommandStrategy(fabric, pathFinder) {
  currentMoveCommandIndex = 0;

  allMoveCommands.push_back(fabric.moveToPoint(1200, 1200));
  allMoveCommands.push_back(fabric.moveToLine(model::LANE_BOTTOM));
  allMoveCommands.push_back(fabric.moveToPoint(200, 3800));
  allMoveCommands.push_back(fabric.moveToLine(model::LANE_TOP));
  allMoveCommands.push_back(fabric.moveToLine(model::LANE_MIDDLE));
  allMoveCommands.push_back(fabric.moveToPoint(800, 3200));
  allMoveCommands.push_back(fabric.moveToLine(model::LANE_BOTTOM));
}

void TestMoveStrategy::update(const Wizard& self, model::Move& move) {
  CommandStrategy::clear();

  auto& command = allMoveCommands[currentMoveCommandIndex];

  while (!command->check(self)) {
    currentMoveCommandIndex = (currentMoveCommandIndex+1)% allMoveCommands.size();
    allMoveCommands.push_back(allMoveCommands[currentMoveCommandIndex]);
  }

  CommandStrategy::update(self, move);
}