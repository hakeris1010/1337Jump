#ifndef WIDGETS_H_INCLUDED
#define WIDGETS_H_INCLUDED

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <memory>
#include "eventhandling.h"

namespace Gryl
{
class WindowRunner;

// The object which can create and dispatch events. Example - native windows.
class EventDispatcher
{
public:
    //Constructors are bassic.
    virtual ~EventDispatcher(){}
};

struct WidgetProperties
{
    std::string name, type;
    // Some more serious stuff to come.
};

class Widget : public EventDispatcher
{
protected:
    std::shared_ptr< Widget > parent = nullptr;

    WidgetProperties props;
    std::vector< std::shared_ptr<Widget> > innerWidgets;
    std::vector< std::shared_ptr<WidgetEventListener> > listeners;

    static unsigned int Widg_counter;

public:
    Widget();
    Widget(std::shared_ptr< Widget > par3nt, const WidgetProperties&,
           const std::vector< std::shared_ptr<WidgetEventListener> >& = std::vector< std::shared_ptr<WidgetEventListener> >());
    virtual ~Widget();

    virtual void setWidgetProperties(const WidgetProperties& _props);
    virtual void addInnerWidget( std::shared_ptr<Widget> const& wdg );
    virtual void addListener( std::shared_ptr<WidgetEventListener> const& lst );

    const std::vector< std::shared_ptr<Gryl::Widget> >& getInnerWidgetVector() const;
    unsigned int getWidgetInstanceCount() const;

    // These methods are totally overridable, and they are called directly from the parent.
    // This is called from event processor thread, forwarding event from a parent.
    virtual void processEvent(const RawEvent& ev)=0;

    // A Draw() method must be called only from a Render Thread (The one with Active OpenGL Context). If called from any other thread, can cause malfunctions.
    virtual void draw(WindowRunner& wr);
};

}

#endif // WIDGETS_H_INCLUDED
