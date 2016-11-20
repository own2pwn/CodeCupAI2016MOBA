//
//File: CM_CommandMoveToPoint.cpp
//Author: Ivlev Alexander. Stef
//Created: 07/11/2016
//


#include "CM_CommandMoveToPoint.h"
#include "A_Move.h"
#include "E_World.h"

using namespace AICup;

CommandMoveToPoint::CommandMoveToPoint(const double x, const double y, const TurnStyle style, const double speedLimit):
  point(x, y), style(style), speedLimit(speedLimit) {
}

bool CommandMoveToPoint::check(const model::Wizard& self) {
  path = Algorithm::path(Position(self.getX(), self.getY()), point, pathLength);

  return path.size() >= 2 && pathLength > 1;
}

int CommandMoveToPoint::priority(const model::Wizard& self) {
  return 0;
}

void CommandMoveToPoint::execute(const model::Wizard& self, Result& result) {
  obstaclesGroups = World::instance().obstaclesGroup(self, self.getVisionRange() * 2);

  result.moveDirection = Algorithm::move(self, path, obstaclesGroups, self.getVisionRange() * 2);
  result.turnStyle = style;
  result.priority = priority(self);
  result.speedLimit = speedLimit;
}

#ifdef ENABLE_VISUALIZATOR
void CommandMoveToPoint::visualization(const Visualizator& visualizator) const {
  for (size_t index = 1; index < path.size(); index++) {
    visualizator.line(path[index - 1].x, path[index - 1].y, path[index].x, path[index].y, 0x00ff00);
  }

  for (const auto& group : obstaclesGroups) {
    for (size_t index = 1; index < group.size(); index++) {
      visualizator.line(group[index - 1].getX(), group[index - 1].getY(), group[index].getX(), group[index].getY(), 0xff00ff);
    }

    for (const auto& obstacle : group) {
      visualizator.circle(obstacle.getX(), obstacle.getY(), obstacle.getRadius(), 0xff00ff);
    }
  }

}
#endif // ENABLE_VISUALIZATOR