#include "windowrunner.h"
#include "helpers/logger.h"
//#include <exception>

namespace Gryl
{
// Statics
unsigned int WindowRunner::wnd_counter = 0;

// ============ SingleThread Unified Loop =========== //
void WindowRunner::defaultUnifiedLoopProc( WindowRunner& win ) // Use this when SingleThreading
{
    // Startup operations
    if(win.window == nullptr)
        throw std::runtime_error("win.window is NOT created!");

    std::shared_ptr<sf::RenderWindow> sfrw = win.window;
    //sfrw.setActive(true);

    //Test operations
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);
    // Main Loop
    while(sfrw->isOpen())
    {
        sf::Event event;
        while (sfrw->pollEvent(event))
        {
            if (event.type == sf::Event::Closed || win.needToClose)
                sfrw->close();
        }

        sfrw->clear();
        sfrw->draw(shape);
        sfrw->display();
    }
}

// ============    MultiThreaded Loops    =========== //
void WindowRunner::defaultMainRenderLoopProc( WindowRunner& win )     // Use these 2 when MultiThreading.
{

}

void WindowRunner::defaultEventLoopProc( WindowRunner& win )
{

}

//------------------ WindowRunner Implementation ------------------//

// ++++++ Private & Protected. ++++++

// InnerImpl window creator, used when creating window pointer from a thread.
// TODO: Move to WindowImpl.
void WindowRunner::createSfmlWindow()
{
    window = std::make_shared<sf::RenderWindow>( wndVidMode.md, wndTitle.c_str(), wndStyle, wndSetts );
    //window.setActive(false);

    window->setVerticalSyncEnabled(wnd_vertSyncEnabled);
    window->setFramerateLimit(wnd_fps);
}

// ++++++ Public ++++++

WindowRunner::WindowRunner() : mainRenderLoopProc( useEventThread ? defaultMainRenderLoopProc : defaultUnifiedLoopProc ),
                               eventLoopProc( defaultEventLoopProc )
{
    wnd_counter++;
}

WindowRunner::WindowRunner(bool startListener,
                const std::string& title,
                const VideoMode& mode,
                uint32_t style,
                const sf::ContextSettings& setts,
                const WindowProperties& props )
{
    create(startListener, title, mode, style, setts, props);
}

WindowRunner::~WindowRunner()
{
    stopAllThreads();

    wnd_counter--;

    if(window != nullptr)
        HLOGF("RenderWindow is not nullptr.\n");
}

void WindowRunner::create( bool startListener,
                const std::string& title,
                const VideoMode& mode,
                uint32_t style,
                const sf::ContextSettings& setts,
                const WindowProperties& props)
{
    // These are for limited bugfixing support.
    wndVidMode = mode;
    wndTitle = title;
    wndStyle = style;
    wndSetts = setts;

    // if mainLoopProc is set default. MUST USE BOOST FOR THIS TO WORK!!! ( boost::optional<> )
    //if(_mainProc == DEFAULT)
    {
        if(useEventThread)
            mainRenderLoopProc = defaultMainRenderLoopProc;
        else
            mainRenderLoopProc = defaultUnifiedLoopProc;
    }
    eventLoopProc = defaultEventLoopProc;

    if(startListener)
        startListening();
}

// PointerRefs
std::shared_ptr<sf::RenderWindow> WindowRunner::getSfmlWindowPtr() const{ // use only when modifying the inner contents.
    return window;
}

// ThreadState returner.
std::shared_ptr<GraphicThreadState> WindowRunner::getGraphicThreadState() const{
    return graphThst;
}

std::shared_ptr<EventThreadState> WindowRunner::getEventThreadState() const{
    return eventThst;
}

// Most important one : This one is started in a rendering thread.
void WindowRunner::startListening()
{
    HLOGF("WindowRunner::startListening: Creating SFML Window...\n");
    createSfmlWindow();     // Rendering thread (Current) must create a window.

    if(useEventThread){
        HLOGF(" Spawning Event Thread...\n");
        eventThread = std::thread(eventLoopProc, std::ref(*this));
    }

    HLOGF(" Starting mainRenderLoopProc in this thread.\n");
    mainRenderLoopProc(std::ref(*this));  // Start rendering thread (at last!)

    HLOGF(" Listener returned.\n");
}

void WindowRunner::stopAllThreads()
{
    needToClose = true;

    // Maybe notify a specific not_var or change a MuTeX

    if(eventThread.joinable())
        eventThread.join();
}

void WindowRunner::attachChildWindow(const std::shared_ptr<WindowRunner>& po)
{
    if( !allowDuplicateReferences && std::find(childWindows.begin(), childWindows.end(), po) != childWindows.end() ) // Duplicate found.
    {
        throw std::logic_error("1: Pointer is already in vect, duplicates are not allowed.");
    }
    childWindows.push_back(po);
}

void WindowRunner::attachChildWidget(const std::shared_ptr<Widget>& po)
{
    if( !allowDuplicateReferences && std::find(innerWidgets.begin(), innerWidgets.end(), po) != innerWidgets.end() ) // Duplicate found.
    {
        throw std::logic_error("1: Pointer is already in vect, duplicates are not allowed.");
    }
    innerWidgets.push_back(po);
}

unsigned int WindowRunner::getWindowInstanceCount() const{
    return wnd_counter;
}

}
