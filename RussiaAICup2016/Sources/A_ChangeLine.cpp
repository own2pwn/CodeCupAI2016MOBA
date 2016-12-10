#include "A_ChangeLine.h"
#include "E_InfluenceMap.h"
#include "E_World.h"
#include "E_Game.h"
#include "C_Math.h"
#include "E_Points.h"

using namespace AICup;


double Algorithm::calculateLinePriority(const Algorithm::PathFinder& finder, const Wizard& self, const model::LaneType lane) {
  const auto basePosition = Points::point(Points::ACADEMY_BASE);
  /// �������� ����� ��� ������ ���������� �����
  const auto position = InfluenceMap::instance().getForeFront(lane, 0);

  /// ������� ����� ������� ������ �� ����� �� ������� �����
  std::shared_ptr<Algorithm::Path> path;
  finder.calculatePath(position, path);
  double selfLength = path->getRealLength();

  /// ����������� ��� ������ ����� ���������� �� ����, � ����������� ��� ��������
  double baseLength = 0;
  double baseReverseLength = 0;
  double priority = 1;
  switch (lane) {
    case model::LANE_TOP:
    case model::LANE_BOTTOM:
      baseLength = abs((basePosition - position).x) + abs((basePosition - position).y);
      baseReverseLength = 7200 - baseLength;
      break;
    case model::LANE_MIDDLE:
      baseLength = sqrt(2) * (basePosition - position).length();
      baseReverseLength = 7200 - baseLength;
      priority = 0.95;
      break;
    default:
      break;
  }


  /// �� ������� ������� ����� �� �����
  double wizardCount = World::instance().wizardCount(lane, Game::friendFaction(), self);
  if (!self.getRole().getChangeLineWizardCountOnlyFriend()) {
    wizardCount -= World::instance().wizardCount(lane, Game::enemyFaction());
  }

  /// ������ ����� ������������� - ����� ������, ������������� ����� ������
  int towerBalance = World::instance().towerCount(lane, Game::friendFaction()) - World::instance().towerCount(lane, Game::enemyFaction());

  /// ������ �� �����
  /// ����� ������ ����� 1500, ����� ����� 4000
  const double laneStrength = InfluenceMap::instance().getLineStrength(lane);


  /// ��������� �� ����
  const auto lengthRole = self.getRole().getChangeLineForeFrontPriority();
  const auto distanceRole = self.getRole().getChangeLinePathLengthPriority();
  const auto wizardRole = self.getRole().getChangeLineWizardCountPriority();
  const auto towerRole = self.getRole().getChangeLineTowerBalancePriority();
  const auto laneStrengthRoley = self.getRole().getChangeLineLaneStrengthPriority();

  /// ��� �������� �� 0 �� 1000
  double lengthPriority = (baseReverseLength * baseReverseLength) / (14.4 * 3600);
  double distancePriority = 1000 - selfLength / 8;// ����� ������������ ��� ������������ ����� ���� 8000
  double wizardPriority = wizardRole > 0 ? (500 - 100 * wizardCount) : (500 + 100 * wizardCount);
  double towerPriority = towerRole > 0 ? (500 - 250 * towerBalance) : (500 + 250 * towerBalance);
  double laneStrengthPriority = 500 - (MAX(-1000, MIN(laneStrength, 1000)) / 2);

  return priority * (lengthPriority * lengthRole
    + distancePriority * distanceRole
    + wizardPriority * abs(wizardRole)
    + towerPriority * abs(towerRole)
    + laneStrengthPriority * laneStrengthRoley) / (lengthRole + distanceRole + abs(wizardRole) + abs(towerRole) + laneStrengthRoley);
}

bool equal(double a, double b) {
  return a - 1.0e-5 < b && b < a + 1.0e-5;
}


bool Algorithm::checkChangeLine(const Algorithm::PathFinder& finder, const Wizard& self, model::LaneType& lane) {
  /// �������� �� 0 �� 1000
  auto topPriority = calculateLinePriority(finder, self, model::LANE_TOP);
  auto middlePriority = calculateLinePriority(finder, self, model::LANE_MIDDLE);
  auto bottomPriority = calculateLinePriority(finder, self, model::LANE_BOTTOM);


  const auto minPriority = MIN(topPriority, MIN(middlePriority, bottomPriority));
  const auto maxPriority = MAX(topPriority, MAX(middlePriority, bottomPriority));

  // �� ������ ����� ���� ��� �������� ��������
  if (maxPriority - minPriority < 1000 * (1 - self.getRole().getDesireChangeLine())) {
    return false;
  }

  if (equal(topPriority, maxPriority)) {
    lane = model::LANE_TOP;
  } else if (equal(middlePriority, maxPriority)) {
    lane = model::LANE_MIDDLE;
  } else {
    lane = model::LANE_BOTTOM;
  }

  return true;
}