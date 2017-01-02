#include "entity.h"
#include "GrylloEngine/helpers/logger.h"

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
    mout<<spritePath<<"\n";
    return spritePath;
}

