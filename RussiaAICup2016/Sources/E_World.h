//
//File: E_World.h
//Author: Ivlev Alexander. Stef
//Created: 07/11/2016
//


#pragma once

#include "model\World.h"
#include "C_Vector2D.h"
#include "C_Singleton.h"
#include "E_Types.h"
#include "E_WorldObjects.h"

#ifdef ENABLE_VISUALIZATOR
#include "Visualizator.h"
#endif

namespace AICup
{

  class World: public Singleton<World> {
  public:
    World();

    void update(const model::World& world);

    static inline const model::World& model() {
      return *instance().modelWorld;
    }

    const std::vector<model::Tree>& trees() const;
    const std::vector<model::Building>& buildings() const;
    const std::vector<model::Minion>& minions() const;
    const std::vector<model::Wizard>& wizards() const;

    const std::vector<Looking>& getVisionZone() const;


    Obstacles allObstacles(const model::CircularUnit& unit, const bool onlyStatic = false) const;
    Obstacles obstacles(const model::CircularUnit& unit, const double range) const;
    ObstaclesGroups createGroup(const Obstacles& obstacles, const double radius) const;

    const model::LivingUnit* unit(long long id) const;

    std::vector<const model::LivingUnit*> around(const model::Wizard& unit, const model::Faction faction, double radius = -1) const;
    std::vector<const model::LivingUnit*> aroundEnemies(const model::Wizard& unit, const double radius = -1) const;

    const int wizardCount(model::LaneType line) const;
    const int wizardCount(model::LaneType line, const model::Wizard& excludeWizard) const;
    const model::LaneType positionToLine(const double x, const double y) const;

#ifdef ENABLE_VISUALIZATOR
    void visualization(const Visualizator& visualizator) const;
#endif // ENABLE_VISUALIZATOR

  private:
    void initBuildings();
    void initTrees();

    void updateVisionZone();
    void updateSupposedData();

  private:
    const model::World* modelWorld;

    std::vector<Looking> visionZone;
    std::vector<model::Tree> supposedTrees;
    std::vector<model::Building> supposedBuilding;
  };
}