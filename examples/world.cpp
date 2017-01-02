#include "world.h"
#include <sstream>

//world

World::World(){ }
World::~World(){ }

World::World(std::string bimg, char ltyp, char diff) : bckgrImg(bimg), loadType(ltyp), difficulty(diff)
{ }

void World::goUp(int hgt)
{
    height+=hgt;
}

void World::loadResources()
{
    //load stuff.
}

char World::getType() const
{
    return this->loadType;
}

char World::getDiff() const
{
    return difficulty;
}

void World::propagate(int hgt)
{
    goUp(hgt);
}

std::string World::getWorldString()
{
    std::stringstream ss;
    ss<<*this;
    return ss.str();
}

//end;
