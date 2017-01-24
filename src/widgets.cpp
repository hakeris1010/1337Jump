#include "widgets.h"

namespace Gryl
{
unsigned int Widget::Widg_counter = 0;

unsigned int Widget::getWidgetInstanceCount() const{
    return Widg_counter;
}

// Construction site.

Widget::Widget(std::shared_ptr< Widget > par3nt) : parent(par3nt) {
    Widg_counter++;
}

Widget::Widget( std::shared_ptr< Widget > par3nt,
                const WidgetProperties& pr0ps,
                const std::vector< std::shared_ptr<WidgetEventListener> >& evList ) : parent(par3nt), props(pr0ps), listeners(evList)
{
    Widg_counter++;
}

Widget::~Widget(){
    Widg_counter--;
}

const std::vector<std::shared_ptr<Gryl::Widget>>& Widget::getInnerWidgetVector() const{
    return innerWidgets;
}

void Widget::setWidgetProperties(const WidgetProperties& _props)
{
    props = _props;
}

void Widget::addInnerWidget( std::shared_ptr<Widget> const& wdg )
{
    innerWidgets.push_back(wdg);
}

void Widget::addListener( std::shared_ptr<WidgetEventListener> const& lst )
{
    listeners.push_back(lst);
}


}
