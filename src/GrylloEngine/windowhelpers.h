#ifndef WINDOWHELPERS_H_INCLUDED
#define WINDOWHELPERS_H_INCLUDED

//#include <function>
#include <memory>
#include <SFML/Graphics.hpp>

namespace Gryl
{
class Widget;

// Gryl::Event class, wrapper over SFML Event
struct Event
{
    Event(sf::Event ev) : e(ev) {}
    virtual ~Event();
    sf::Event e;
};

// EventHandler Functor, which is intended to be totally overridable, for creating custom events.
struct EventHandler
{
    EventHandler();
    EventHandler(std::function )
    virtual ~EventHandler();
    virtual long operator()(std::shared_ptr<Widget> par, const Event& ev); //we return long value NonZero if event occured.

    //Protected (or private) stuff is intended to be inheriatable.
};

// NuStuff, Inspired by JavaScript.
struct WidgetEventListener
{
    // We get widget as a param, but we can send a Widet too, coz it's inherited from a widget.
    WidgetEventListener(std::shared_ptr<Widget>,                               // Our parent Widget (in common case, a WindowRunner)
                        const std::function< void(std::shared_ptr<Widget>) >&, // The function performing specific Action based on Event happened.
                        const EventHandler& );                                 // The object performing calculations determining when an Event HappenEd.
    // It's intended to be inh3rit4ble, so virtual dest.
    virtual ~WidgetEventListener();

    protected:
    std::shared_ptr<Widget> parentWidg;           // Encapsulated objects.
    std::function< void(std::shared_ptr<Widget>) > funcToDo;
    EventHandler hnd;
};

}
//('Coz kawaii desu.) :3

#endif // WINDOWHELPERS_H_INCLUDED
