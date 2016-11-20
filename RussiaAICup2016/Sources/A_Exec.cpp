#include "A_Exec.h"
#include "C_Extensions.h"
#include "C_Math.h"

#include "E_Game.h"

using namespace AICup;

bool Algorithm::execMove(const model::Wizard& self, const TurnStyle style, const Vector& direction, const double speedLimit, model::Move& move) {
  ///������ � ��� � �� ����� ��� ��� ����� ��������, �� ��� ���� ������� �� �����
  Vector speed = Vector(direction.x, -direction.y).normal().rotated(self.getAngle());
  speed.y *= -1;

  double maxSpeed = (speed.x > 0) ? EX::maxSpeed(self) : EX::maxBackwardSpeed(self);
  maxSpeed = maxSpeed * abs(speed.x/(speed.x+speed.y)) + EX::maxStrafeSpeed(self) * abs(speed.y/(speed.x + speed.y));

  if (speedLimit < 0) {
    speed *= MIN(maxSpeed, direction.length());
  } else {
    speed *= MIN(maxSpeed, MIN(speedLimit, direction.length()));
  }

  move.setSpeed(speed.x);
  move.setStrafeSpeed(speed.y);

  switch (style) {
  case TurnStyle::TURN:
    move.setTurn(self.getAngleTo(self.getX() + direction.x, self.getY() + direction.y));
    break;
  case TurnStyle::NO_TURN:
    break;
  case TurnStyle::BACK_TURN:
    move.setTurn(self.getAngleTo(self.getX() - direction.x, self.getY() - direction.y));
    break;
  default:
    assert(false && "incorrect turn style");
  }

  return true;
}

bool Algorithm::execAttack(const model::Wizard& self, const model::ActionType action, const model::LivingUnit& unit, model::Move& move) {
  const double angleDeviation = self.getAngleTo(unit);

  /// � ����� ������ �������������� � �����, ��� ��������� ����� �����
  move.setTurn(angleDeviation);

  /// �� ��� ����, ����� ���������
  if (abs(angleDeviation) > Game::model().getStaffSector() * 0.5) {
    return false;
  }

  move.setCastAngle(angleDeviation);

  const double distance = self.getDistanceTo(unit);

  move.setMinCastDistance(distance - unit.getRadius());
  move.setMaxCastDistance(distance + unit.getRadius() * 2); // � �������
  move.setAction(action);
}