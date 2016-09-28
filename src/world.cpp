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

//entity
size_t Entity::counter = 0; //initialize

Entity::Entity(std::string spPat) : spritePath(spPat)
{ counter++; }

Entity::Entity(std::initializer_list<std::string> lst) : props(lst)
{ counter++; }

const std::vector<std::string>& Entity::getProperties()
{
    return props;
}

std::string Entity::getSpritePath() const
{
    return spritePath;
}

//jumper

Jumper::Jumper(int _hp, int _atk, int _jmpHgt) : hp(_hp), atk(_atk), jumpHeight(_jmpHgt)
{ }

const std::vector<std::string>& Jumper::getProperties()
{
    props.push_back(std::string("moving entity"));
    return props;
}

const std::vector<int>& Jumper::getJumperProps() const
{
    return std::vector<int>({hp, atk, jumpHeight});
}

void Jumper::jumpUp(World& wrld)
{
    wrld.propagate(jumpHeight);
}

void Jumper::jumpUp(World& wrld, int newJumpHeight)
{
    wrld.propagate(newJumpHeight);
}

//end;
