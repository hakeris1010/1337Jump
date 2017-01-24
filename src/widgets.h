#ifndef WIDGETS_H_INCLUDED
#define WIDGETS_H_INCLUDED

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <memory>
#include "eventhandling.h"

namespace Gryl
{
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
    std::vector< std::shared_ptr<Gryl::Widget> > innerWidgets; //inner widg3ts
    std::vector< std::shared_ptr<WidgetEventListener> > listeners;

    static unsigned int Widg_counter;

public:
    Widget(std::shared_ptr< Widget > par3nt = nullptr);
    Widget(std::shared_ptr< Widget > par3nt, const WidgetProperties&,
           const std::vector< std::shared_ptr<WidgetEventListener> >& = std::vector< std::shared_ptr<WidgetEventListener> >());
    virtual ~Widget();

    virtual void setWidgetProperties(const WidgetProperties& _props);
    virtual void addInnerWidget( std::shared_ptr<Widget> const& wdg );
    virtual void addListener( std::shared_ptr<WidgetEventListener> const& lst );

    virtual void processEvent(const RawEvent& ev){ }
    virtual void updateView(){ }

    const std::vector< std::shared_ptr<Gryl::Widget> >& getInnerWidgetVector() const;
    unsigned int getWidgetInstanceCount() const;
};

}

#endif // WIDGETS_H_INCLUDED
