#include "windowrunner.h"
#include "logger.h"
//#include <exception>

namespace Gryl
{
// Statics
unsigned int WindowRunner::wnd_counter = 0;

// ============ SingleThread Unified Loop =========== //
void WindowRunner::defaultMainUnifiedLoop( std::weak_ptr<WindowRunner> winp ) // Use this when SingleThreading
{
    auto win = std::make_shared<WindowRunner>(winp);
    // Startup operations
    if(win->window == nullptr)
        win->createSfmlWindow();
    else
        throw std::runtime_error("win->window is already created!");

    std::shared_ptr<sf::RenderWindow> sfrw = win->getSfmlWindowPtr();
    //sfrw.setActive(true);

    //Cosmetic operations
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);
    // Main Loop
    while(sfrw->isOpen())
    {
        sf::Event event;
        while (sfrw->pollEvent(event))
        {
            if (event.type == sf::Event::Closed || win->needToClose)
                sfrw->close();
        }

        sfrw->clear();
        sfrw->draw(shape);
        sfrw->display();
    }
}

// ============    MultiThreaded Loops    =========== //
void WindowRunner::defaultProcessLoop( std::weak_ptr<WindowRunner> win )     // Use these 2 when MultiThreading.
{

}

void WindowRunner::defaultRenderingLoop( std::weak_ptr<WindowRunner> win )
{

}

//==================//
//------------------ WindowRunner Implementation ------------------//

WindowRunner::WindowRunner() : mainLoopProc( defaultMainUnifiedLoop ), renderingLoopProc(defaultRenderingLoop)
{
    wnd_counter++;
}

WindowRunner::WindowRunner(bool startListener,
                           const std::string& title,
                           const VideoMode& mode,
                           uint32_t style,
                           const sf::ContextSettings& setts,
                           bool startListenerInSeparateThread,
                           bool useRenderThread,
                           //const std::function< void(std::shared_ptr<WindowRunner>) >& _mainProc,
                           const std::function< void(std::weak_ptr<WindowRunner>) >& _renderLoop )
{
    create(startListener, title, mode, style, setts, startListenerInSeparateThread, useRenderThread/*, _mainProc*/, _renderLoop);
}

WindowRunner::~WindowRunner()
{
    wnd_counter--;

    if(mainThread.joinable())
        mainThread.join();
    if(rendThread.joinable())
        rendThread.join();

    if(window != nullptr)
        HLOGF("RenderWindow is not nullptr.\n");
}

// InnerImpl window creator, used when creating window pointer from a thread.
void WindowRunner::createSfmlWindow()
{
    window = std::shared_ptr<sf::RenderWindow>(new sf::RenderWindow());

    window->setVerticalSyncEnabled(wnd_vertSyncEnabled);
    window->setFramerateLimit(wnd_fps);
    window->create(wndVidMode.md, wndTitle.c_str(), wndStyle, wndSetts);
}

void WindowRunner::create( bool startListener,
                           const std::string& title,
                           const VideoMode& mode,
                           uint32_t style,
                           const sf::ContextSettings& setts,
                           bool startListenerInSeparateThread,
                           bool useRenderThread,
                           //const std::function< void(std::shared_ptr<WindowRunner>) >& _mainProc,
                           const std::function< void(std::weak_ptr<WindowRunner>) >& _renderProc)
{
    //window.setVerticalSyncEnabled(wnd_vertSyncEnabled);
    //window.setFramerateLimit(wnd_fps);
    //window.create(mode.md, title.c_str(), style, setts);

    // These are for limited bugfixing support.
    wndVidMode = mode;
    wndTitle = title;
    wndStyle = style;
    wndSetts = setts;

    //window.setActive(false);

    // --- We haven't implemented it yet. ---
    //listenInSeparateThread = startListenerInSeparateThread;
    //useRenderingThread = useRenderThread;

    renderingLoopProc = _renderProc;

    // if mainLoopProc is set default. MUST USE BOOST FOR THIS TO WORK!!! ( boost::optional<> )
    //if(_mainProc == DEFAULT)
    {
        if(useRenderingThread)
            mainLoopProc = defaultProcessLoop;
        else
            mainLoopProc = defaultMainUnifiedLoop;
    }

    if(startListener)
        startListening();
}

//other funczorz
// BoostPointas
/*boost::optional<sf::RenderWindow&> WindowRunner::getSfmlWindowRef(){
    return (window!=nullptr ? *window : (boost::optional<sf::RenderWindow&>()));
} // use only when modifying the inner contents.

boost::optional<const sf::RenderWindow&> WindowRunner::getSfmlWindowConst() const{
    return (window!=nullptr ? *window : boost::optional<const sf::RenderWindow&>());
}*/

// PointerRefs
std::shared_ptr<sf::RenderWindow> WindowRunner::getSfmlWindowPtr(){ // use only when modifying the inner contents.
    return window;
}

const std::shared_ptr<const sf::RenderWindow>& WindowRunner::getSfmlWindowConstPtr() const{
    return window;
}

// ThreadState returner.
std::shared_ptr<GraphicThreadState> WindowRunner::getGraphicThreadState() const{
    return thr_st;
}

unsigned int WindowRunner::getWindowInstanceCount() const{
    return wnd_counter;
}

// Serious shit there.
// Overrided from Widget.
// void WindowRunner::addInnerWidget( Widget& widg ){ }

void WindowRunner::startListening()
{
    if( useRenderingThread ) // Spawn the thread for rendering. This renderer can be run ONLY IN A THREAD.
    {
        HLOGF("Launching RenderThread...\n");
        rendThread = std::thread( renderingLoopProc, std::shared_ptr<WindowRunner>( this ) );
    }

    if(listenInSeparateThread) // If processer is set to run on a different thread.
    {
        HLOGF("Launching MainLoopThread...\n");
        mainThread = std::thread( mainLoopProc, std::weak_ptr<WindowRunner>( this ) );
    }
    else // If processer is set to run on a caller's thread (Current one).
    {
        HLOGF("Calling MainLoop proc.\n");
        mainLoopProc( std::weak_ptr<WindowRunner>( this ) );
    }

    HLOGF("Listener returned.\n");
}

void WindowRunner::stopAllThreads()
{
    needToClose = true;

    if(mainThread.joinable())
        mainThread.join();
    if(rendThread.joinable())
        rendThread.join();
}

}
