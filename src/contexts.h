#ifndef CONTEXTS_HPP_INCLUDED
#define CONTEXTS_HPP_INCLUDED

#include <SFML/Graphics.hpp>

namespace Gryl
{
// GrylloEngine implementation of VideoMode
class VideoMode
{
private:
    sf::VideoMode md;
public:
    VideoMode(){ }
    VideoMode(unsigned int width, unsigned int height, unsigned int bitsPerPixel) : md(width, height, bitsPerPixel)
    { }

    friend class WindowRunner;
};

}

#endif // CONTEXTS_HPP_INCLUDED
