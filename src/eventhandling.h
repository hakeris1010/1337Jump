#ifndef WINDOWHELPERS_H_INCLUDED
#define WINDOWHELPERS_H_INCLUDED

//#include <function>
#include <memory>
#include <SFML/Graphics.hpp>
#include "event.h"

namespace Gryl
{
class Widget;

// Event determination class, which is intended to be totally overridable, for creating custom events.
struct CustomEventDeterminer
{
    CustomEventDeterminer(){ }
    virtual ~CustomEventDeterminer(){ }

    virtual long checkEvent( Widget& par, const RawEvent& e )=0; //we return long value NonZero if event occured.

    //Protected (or private) stuff is intended to be inheriatable.
};

// NuStuff, Inspired by JavaScript.
// Widget Event Listeners.

struct WidgetEventListener
{
    WidgetEventListener(){ }
    WidgetEventListener(std::shared_ptr<CustomEventDeterminer> det) : hnd( det ){ }

    // It's intended to be inh3rit4ble, so virtual dest.
    virtual ~WidgetEventListener(){ }

    // The function performing specific Action based on Event happened.
    virtual void onEvent( Widget& par, const RawEvent& e ) = 0;

    protected:
    // We call this one on onEvent(), passing the got event, and get TRUE if specific custom EV happen'd (e.g. button pressed 3 timez).
    // Poiter coz one instance might be attached to several Widgets.
    std::shared_ptr<CustomEventDeterminer> hnd = nullptr;
};

// We got this too, with fuction onEvent as pointer, coz we might want to import this func on RunTime.
struct WidgetEventListener_Importable : public WidgetEventListener
{
    WidgetEventListener_Importable(){ }
    WidgetEventListener_Importable(std::shared_ptr<CustomEventDeterminer>,
                                   std::function< void( Widget& par, const RawEvent& ) >);

    void onEvent( Widget&, const RawEvent& );

    private:
    std::function< void( Widget&, const RawEvent& ) > onEvent_Ptr;
};

}
//('Coz kawaii desu.) :3

#endif // WINDOWHELPERS_H_INCLUDED
