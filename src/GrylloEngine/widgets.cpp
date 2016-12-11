#include "widgets.h"

namespace Gryl
{
unsigned int Widget::Widg_counter = 0;

unsigned int Widget::getWidgetInstanceCount() const{
    return Widg_counter;
}

//Construction site.
Widget::Widget(){
    Widg_counter++;
}

Widget::~Widget(){
    Widg_counter--;
}

const std::vector<std::shared_ptr<Gryl::Widget>>& Widget::getInnerWidgetVector() const{
    return widgets;
}

void Widget::setWidgetProperties(const WidgetProperties& _props)
{

}

void Widget::addInnerWidget( Gryl::Widget& widg )
{

}

void Widget::addListener(const WidgetEventListener& _list)
{

}

}
