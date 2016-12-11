#ifndef WIDGETS_H_INCLUDED
#define WIDGETS_H_INCLUDED

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <memory>
#include "windowhelpers.h"

namespace Gryl
{
struct WidgetProperties
{
    std::string name, type;
    // Some more serious stuff to come.
};

class Widget
{
protected:
    WidgetProperties props;
    std::vector< std::shared_ptr<Gryl::Widget> > widgets; //inner widg3ts
    std::vector< WidgetEventListener > listeners;

    static unsigned int Widg_counter;

public:
    Widget();
    Widget(const WidgetProperties&, const std::vector< WidgetEventListener >&);
    virtual ~Widget();

    virtual void setWidgetProperties(const WidgetProperties& _props);
    virtual void addListener(const WidgetEventListener& _list);
    virtual void addInnerWidget( Gryl::Widget& widg );

    const std::vector<std::shared_ptr<Gryl::Widget>>& getInnerWidgetVector() const;
    unsigned int getWidgetInstanceCount() const;
};

}

#endif // WIDGETS_H_INCLUDED
