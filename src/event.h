#ifndef EVENT_HPP_INCLUDED
#define EVENT_HPP_INCLUDED

#include <SFML/Window.hpp>

// Gryl::RawEvent class, wrapper over SFML Event
struct RawEvent
{
    RawEvent(sf::Event ev) : e(ev) {}
    virtual ~RawEvent();
    sf::Event e;
};

#endif // EVENT_HPP_INCLUDED
