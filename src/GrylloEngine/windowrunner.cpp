#include "windowrunner.h"
#include "logger.h"

namespace Gryl
{
unsigned int WindowRunner::wnd_counter = 0;

WindowRunner::WindowRunner() //: defListen(*this)
{
    wnd_counter++;
}

WindowRunner::WindowRunner(bool startListener,
                           const VideoMode& mode,
                           const std::string& title,
                           uint32_t style,
                           const sf::ContextSettings& setts) : WindowRunner()
{
    create(startListener, mode, title, style, setts);
}

WindowRunner::~WindowRunner()
{
    wnd_counter--;
}

void WindowRunner::create(bool startListener,
                           const VideoMode& mode,
                           const std::string& title,
                           uint32_t style,
                           const sf::ContextSettings& setts)
{
    window.setVerticalSyncEnabled(wnd_vertSyncEnabled);
    window.setFramerateLimit(wnd_fps);
    window.create(mode.md, title.c_str(), style, setts);

    if(startListener)
        startListeningLoop();
}

//other funczorz

sf::RenderWindow& WindowRunner::getSfmlWindowRef(){
    return window;
} // use only when modifying the inner contents.

const sf::RenderWindow& WindowRunner::getSfmlWindowConst() const{
    return window;
}

std::shared_ptr<GraphicThreadState> WindowRunner::getGraphicThreadState() const{
    return thr_st;
}

unsigned int WindowRunner::getWindowInstanceCount() const{
    return wnd_counter;
}

// Serious shit there.
// Overrided from Widget.
void WindowRunner::addInnerWidget( Widget& widg )
{

}

//void WindowRunner::startListeningLoop(WindowListener listener){ }

void WindowRunner::startListeningLoop()
{

}

}
