#ifndef LOADER_H_INCLUDED
#define LOADER_H_INCLUDED

#include "resources.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

namespace JumpGame
{

class ResourceLoader
{
private:
    static std::size_type objectsLoaded = 0;

    ResourceLoader(){ } //make class non-copyable.
    ResourceLoader(const ResourceLoader&) = delete;
    ResourceLoader(ResourceLoader&&) = delete;
    ResourceLoader& operator=(const ResourceLoader&) = delete;
    ResourceLoader& operator=(ResourceLoader&&) = delete;
public:
    ~ResourceLoader(){ }

    template<class T>
    static T loadSfmlResource(std::string name, bool isEmbedded, bool isMemory);

    static sf::Sprite getSpriteFromTexture(const sf::Texture& txt, sf::IntRect prop);
};

template<class T>
T ResourceLoader::loadSfmlResource(std::string name, bool isEmbedded, bool isMemory)
{
    T retval;
    try
    {

    }
}

}

#endif // LOADER_H_INCLUDED
