//#include "windowrunner.h"
#include "eventhandling.h"
#include "helpers/logger.h"

namespace Gryl
{

WidgetEventListener_Importable::WidgetEventListener_Importable (
                std::shared_ptr<CustomEventDeterminer> det,
                std::function< void( Widget& par, const RawEvent& ) > onEvnt ) : WidgetEventListener(det), onEvent_Ptr(onEvnt)
{ }

void WidgetEventListener_Importable::onEvent( Widget& par, const RawEvent& e )
{
    onEvent_Ptr(par, e);
}

}
