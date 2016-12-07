#include "S_CommandStrategy.h"
#include "A_Exec.h"
#include "C_Math.h"
#include "C_Extensions.h"
#include "C_Logger.h"

#include "E_World.h"
#include "A_Move.h"
#include <cassert>
#include <algorithm>


using namespace AICup;

CommandStrategy::CommandStrategy(const CommandFabric& fabric, const Algorithm::PathFinder& finder):
  fabric(fabric), pathFinder(finder) {
}

void CommandStrategy::update(const Wizard& self, model::Move& finalMove) {
  auto moveResults = moveCommandsToMoveResult(self);

  bool deactivateOtherTurn = false;
  {
    double speedLimit = -1;
    Vector direction;
    if (move(moveResults, self, speedLimit, direction)) {
      TurnStyle turnStyle = TurnStyle::TURN;
      Vector turnDirection = turn(moveResults, turnStyle, deactivateOtherTurn);

      /// ���� �� � ���������, ������ ��� �����... ��� �������� ��������
      if (direction.length() < 1.0e-5) {
        Algorithm::execAroundMove(self, finalMove);
      } else {
        assert(turnDirection.length() > 1.0e-5);
        /// ����� ��� ���������� � �������, � ��� �����, ����� ��� ���������� ������ �������� (���� ��� ������ ���)
        if (TurnStyle::SIDE_TURN != turnStyle) {
          turnDirection = direction;
        }

        Algorithm::execMove(self, turnStyle, turnDirection, direction, speedLimit, finalMove);
      }
    }
  }

  double turnSave = finalMove.getTurn();

  if (!attackCommands.empty()) {
    model::ActionType action;
    const model::LivingUnit* unit = attack(self, action);
    if (nullptr != unit) {
      Algorithm::execAttack(self, action, *unit, finalMove);
    }
  }

  if (!castCommands.empty()) {
    model::ActionType action;
    const auto unit = cast(self, action);
    if (nullptr != unit) {
      Algorithm::execCast(self, action, *unit, finalMove);
    }
  }

  if (deactivateOtherTurn) {
    finalMove.setTurn(turnSave);
  }
}

void CommandStrategy::clear() {
  moveCommands.clear();
  attackCommands.clear();
  castCommands.clear();
}

std::vector<MoveCommand::Result> CommandStrategy::moveCommandsToMoveResult(const Wizard& self) const {
  std::vector<MoveCommand::Result> moveResults;
  moveResults.resize(moveCommands.size());

  for (size_t index = 0; index < moveCommands.size(); index++) {
    moveResults[index].priority = moveCommands[index]->priority(self);

    if (moveResults[index].priority >= 1) {
      moveCommands[index]->execute(self, moveResults[index]);
    }
  }

  /// ������� �������, ������� ����� ��������, ��� �� ������������
  for (size_t index = 0; index < moveResults.size(); index++) {
    const size_t i = moveResults.size() - index - 1;
    if (moveResults[i].priority < 1 || moveResults[i].moveDirection.length() < 0.5) {
      moveResults.erase(moveResults.begin() + i);
    }
  }

  return moveResults;
}

const Vector CommandStrategy::turn(const std::vector<MoveCommand::Result>& moveResults, TurnStyle& turnStyle, bool& deactivateOtherTurn) {
  Vector result = Vector(0, 0);

  /// �������� ����� ���������������� ��� ��������
  int turnPriority = 0;
  for (const auto& move : moveResults) {
    if (move.turnPriority < turnPriority) {
      continue;
    }
    turnPriority = move.turnPriority;

    turnStyle = move.turnStyle;
    result = move.moveDirection;
    deactivateOtherTurn = move.deactivateOtherTurn;
  }
  return result;
}

const Vector CommandStrategy::calcMoveVector(const std::vector<MoveCommand::Result>& moveResults, const Wizard& self, double& speedLimit) {
  speedLimit = EX::maxSpeed(self);
  if (moveResults.empty()) { /// ���� ��� �������� ������ ��������� ������ (P.S. ��� �������� ���� ����, ����� ��� 0 �� ���������)
    return Vector(speedLimit, 0).rotated(self.getAngle());
  }

  double maxPriority = 0;
  Vector result = Vector(0, 0);
  for (const auto& moveIter : moveResults) {
    const auto direction = moveIter.moveDirection.normal();

    double sumPriority = 0;
    for (const auto& move : moveResults) {
      sumPriority += move.moveDirection.normal().dot(direction) * move.priority;
    }

    if (sumPriority > maxPriority) {
      sumPriority = maxPriority;
      result = moveIter.moveDirection;
      speedLimit = moveIter.speedLimit < 0 ? speedLimit : moveIter.speedLimit;
    }
  }

  return result;
}

bool CommandStrategy::move(std::vector<MoveCommand::Result>& moveResults, const Wizard& self, double& speedLimit, Vector& direction) {
  assert(0 != moveResults.size());

  const auto moveVector = calcMoveVector(moveResults, self, speedLimit);

  addAvoidProjectiles(moveResults, self, moveVector);

  if (moveResults.empty()) {
    return false;
  }

  const auto finalMoveVector = calcMoveVector(moveResults, self, speedLimit);

  direction = calculateCollisions(self, EX::pos(self) + finalMoveVector/*���� ���� ����*/, speedLimit);

  return true;
}

const Vector CommandStrategy::calculateCollisions(const Wizard& self, const Position& endPoint, const double speedLimit) {
  movePosition = endPoint;
  pathFinder.calculatePath(endPoint, path);
  assert(nullptr != path);

  const Position preEndPoint = path->calculateNearestCurvaturePoint(self.getVisionRange() / 2);

  /// need remove obstacles
  auto obstacles = World::instance().obstacles(self, self.getVisionRange(), preEndPoint - EX::pos(self));
  const auto treesForRemove = path->removeObstacles(obstacles);
  const auto obstaclesGroups = World::instance().createGroup(obstacles, self.getRadius());


  for (const auto& tree : treesForRemove) {
    addTreeForRemove(self, tree);
  }

  return Algorithm::move(self, preEndPoint, obstaclesGroups, self.getVisionRange()) * speedLimit;
}


void CommandStrategy::addTreeForRemove(const Wizard& self, const model::LivingUnit* tree) {
  LogAssert(nullptr != tree);

  const double distance = self.getDistanceTo(*tree);
  if (distance > self.getVisionRange()) {
    return;
  }

  const auto attack = fabric.attack(*tree);
  if (attack->check(self)) {
    attackCommands.push_back(attack);
  }
}


void CommandStrategy::addAvoidProjectiles(std::vector<MoveCommand::Result>& moveResults, const Wizard& self, const Vector& moveDirection) {

  for (const auto& projectile : World::instance().bullets()) {
    //if (projectile.faction == self.getFaction()) { // ���������� ���� �������, ��� ��� ��� �������� ����� � ���
    //  continue;
    //}
    const auto command = fabric.avoidProjectile(projectile, moveDirection);

    MoveCommand::Result moveCommandResult;
    if (command->check(self)) {
      moveCommandResult.priority = command->priority(self);
      command->execute(self, moveCommandResult);

      if (moveCommandResult.priority < 1 || moveCommandResult.moveDirection.length() < 0.5) {
        continue;
      }

      moveResults.push_back(moveCommandResult);
    }
  }
}

const model::LivingUnit* CommandStrategy::attack(const Wizard& self, model::ActionType& action) {
  AttackCommandPtr maxPriorityAttack = nullptr;
  double maxPriority = 0;

  for (const auto& attackCommand: attackCommands) {
    const double priority = attackCommand->priority(self);
    if (priority > maxPriority) {
      maxPriority = priority;
      maxPriorityAttack = attackCommand;
    }
  }

  if (nullptr == maxPriorityAttack) {
    return nullptr;
  }

  AttackCommand::Result result;
  maxPriorityAttack->execute(self, result);

  action = result.action;
  return result.unit;
}

const model::LivingUnit* CommandStrategy::cast(const Wizard& self, model::ActionType& action) {
  double maxPriority = 0;
  CastCommandPtr maxCastCommand = nullptr;

  for (const auto& castCommand: castCommands) {
    const double priority = castCommand->priority(self);
    if (priority > maxPriority) {
      maxPriority = priority;
      maxCastCommand = castCommand;
    }
  }

  if (nullptr == maxCastCommand) {
    return nullptr;
  }


  CastCommand::Result result;
  maxCastCommand->execute(self, result);

  action = result.action;
  return result.unit;
}


#ifdef ENABLE_VISUALIZATOR
void CommandStrategy::visualization(const model::Wizard& self, const Visualizator& visualizator) const {
  /*for (const auto& command : moveCommands) {
    command->visualization(self, visualizator);
  }*/

  if (Visualizator::PRE == visualizator.getStyle()) {
    for (const auto& move : moveCommandsToMoveResult(self)) {
      visualizator.line(self.getX(), self.getY(), self.getX() + move.moveDirection.x, self.getY() + move.moveDirection.y, 0xff0000);
    }
  }

  for (const auto& command : attackCommands) {
    command->visualization(self, visualizator);
  }

  if (Visualizator::POST == visualizator.getStyle()) {
    if (nullptr != path) {
      path->visualization(visualizator);

      visualizator.line(self.getX(), self.getY(), movePosition.x, movePosition.y, 0x000000);
    }
  }
}
#endif
