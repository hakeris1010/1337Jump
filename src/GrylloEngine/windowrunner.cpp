#include "windowrunner.h"
#include "logger.h"

namespace Gryl
{
// Statics
unsigned int WindowRunner::wnd_counter = 0;

// ============ SingleThread Unified Loop =========== //
void WindowRunner::defaultMainUnifiedLoop( std::shared_ptr<WindowRunner> ) // Use this when SingleThreading
{

}

// ============    MultiThreaded Loops    =========== //
void WindowRunner::defaultProcessLoop( std::shared_ptr<WindowRunner> )     // Use these 2 when MultiThreading.
{

}

void WindowRunner::defaultRenderingLoop( std::shared_ptr<WindowRunner> )
{

}

//==================//
//------------------ WindowRunner Implementation ------------------//

WindowRunner::WindowRunner() : mainLoopProc( defaultMainUnifiedLoop ), renderingLoopProc(defaultRenderingLoop)
{
    wnd_counter++;
}

WindowRunner::WindowRunner(bool startListener,
                           bool startListenerInSeparateThread,
                           const VideoMode& mode,
                           const std::string& title,
                           uint32_t style,
                           const sf::ContextSettings& setts,
                           bool useRenderThread,
                           const std::function< void(std::shared_ptr<WindowRunner>) >& _mainProc,
                           const std::function< void(std::shared_ptr<WindowRunner>) >& _renderLoop )
{
    create(startListener, startListenerInSeparateThread, mode, title, style, setts, useRenderThread, _mainProc, _renderLoop);
}

WindowRunner::~WindowRunner()
{
    wnd_counter--;

    if(mainThread.joinable())
        mainThread.join();
    if(rendThread.joinable())
        rendThread.join();
}

void WindowRunner::create( bool startListener,
                           bool startListenerInSeparateThread,
                           const VideoMode& mode,
                           const std::string& title,
                           uint32_t style,
                           const sf::ContextSettings& setts,
                           bool useRenderThread,
                           const std::function< void(std::shared_ptr<WindowRunner>) >& _mainProc,
                           const std::function< void(std::shared_ptr<WindowRunner>) >& _renderProc)
{
    window.setVerticalSyncEnabled(wnd_vertSyncEnabled);
    window.setFramerateLimit(wnd_fps);
    window.create(mode.md, title.c_str(), style, setts);

    useRenderingThread = useRenderThread;
    listenInSeparateThread = startListenerInSeparateThread;

    renderingLoopProc = _renderProc;

    // if mainLoopProc is set default. MUST USE BOOST FOR THIS TO WORK!!! ( boost::optional<> )
    //if(_mainProc == DEFAULT)
    {
        if(useRenderThread)
            mainLoopProc = defaultProcessLoop;
        else
            mainLoopProc = defaultMainUnifiedLoop;
    }

    if(startListener)
        startListening();
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
// void WindowRunner::addInnerWidget( Widget& widg ){ }

void WindowRunner::startListening()
{
    if(listenInSeparateThread) // If processer is set to run on a different thread.
    {
        mainThread = std::thread( mainLoopProc, std::shared_ptr<WindowRunner>( this ) );
    }

    if( useRenderingThread ) // Spawn the thread for rendering. This renderer can be run ONLY IN A THREAD.
    {
        rendThread = std::thread( renderingLoopProc, std::shared_ptr<WindowRunner>( this ) );
    }

    if( ! listenInSeparateThread) // If processer is set to run on a caller's thread (Current one).
    {
        mainLoopProc( std::shared_ptr<WindowRunner>( this ) );
    }
}

void WindowRunner::stopAllThreads()
{
    if(mainThread.joinable())
        mainThread.join();
    if(rendThread.joinable())
        rendThread.join();
}

}
