#ifndef EVENT_HPP_INCLUDED
#define EVENT_HPP_INCLUDED

#include <SFML/Window.hpp>

// Gryl::Event class, wrapper over SFML Event
struct Event
{
    Event(sf::Event ev) : e(ev) {}
    virtual ~Event();
    sf::Event e;
};

#endif // EVENT_HPP_INCLUDED
