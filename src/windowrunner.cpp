#include "windowrunner.h"
#include "helpers/logger.h"
#include <chrono>

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
    while(sfrw->isOpen() && !win.needToClose)
    {
        sf::Event event;
        while (sfrw->pollEvent(event))
        {
            if (event.type == sf::Event::Closed )
            {
                win.needToClose = true;
                break;
            }
        }

        sfrw->clear();
        sfrw->draw(shape);
        sfrw->display();
    }
}

// ============    MultiThreaded Loops    =========== //
void WindowRunner::defaultRenderLoopProc( WindowRunner& win )     // Use these 2 when MultiThreading.
{
    // Startup operations
    if(win.window == nullptr)
        throw std::runtime_error("win.window is NOT created!");

    std::shared_ptr<sf::RenderWindow> sfrw = win.window;
    sfrw->setActive(true); // OpenGL context must be Active in a render thread.

    // Initialize all inner Drawable widgets .

    HLOGF("WindowRunner::RenderLoop(): Initialized. Starting Rendering Loop...\n");

    // Start RenderLoop
    while(sfrw->isOpen() && !win.needToClose)
    {
        sfrw->clear();

        // Draw All The Stuff.

        sfrw->display();

        // Sleep if needed - this should be improved by Clock (if time passed processing is less than 10 ms - sleep the remaining time).
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    HLOGF("WindowRunner::RenderLoop(): Loop Ended. Condition : (!sfrw->isOpen() || win.needsToClose)\n");

    // Do rendering cleanup .
}

void WindowRunner::defaultMainEventLoopProc( WindowRunner& win )
{
    // Startup operations
    if(win.window == nullptr)
        throw std::runtime_error("win.window is NOT created!");

    std::shared_ptr<sf::RenderWindow> sfrw = win.window;
    //sfrw->setActive(false); // In event loop no drawing, so window's GL context must be NOT current (InActive). This is also called in Starter.

    // Maybe intialize some event handling system parts

    HLOGF("WindowRunner::EventLoop(): Initialized. Starting Loop...\n");

    // Start EventLoop
    while(sfrw->isOpen() && !win.needToClose)
    {
        sf::Event event;
        while (sfrw->pollEvent(event))
        {
            if ( event.type == sf::Event::Closed ) // If Native window requests closing, post the close message.
                win.needToClose = true;

            // Translate gotten event to GrylloEngine format.
            // Forward event to all the inner Elements (Widgets) for processing.
            // Maybe do some FrontEnd event conversion.
        }

        // Sleep if needed - this should be improved by Clock (if time passed processing is less than 10 ms - sleep the remaining time).
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    // Do some clenup tasks.
}

//------------------ WindowRunner Implementation ------------------//

// ++++++ Private & Protected. ++++++

// InnerImpl window creator, used when creating window pointer from a thread.
// TODO: Move to WindowImpl.
void WindowRunner::createWindowImpl()
{
    //std::lock_guard<std::mutex> l0ck(this->mtx_thisObjectSynchronized);       // Private methods should be sync'd by default.

    window = std::make_shared<sf::RenderWindow>( wndVidMode.md, wndTitle.c_str(), wndStyle, wndSetts );
    //window.setActive(false);

    window->setVerticalSyncEnabled(wnd_vertSyncEnabled);
    window->setFramerateLimit(wnd_fps);
}

void WindowRunner::closeWindowImpl()
{
    //if(!window->isOpen()) return;
    //std::lock_guard<std::mutex> l0ck(this->mtx_thisObjectSynchronized);

    window->close();
}

// This function is equivalent to Close()
void WindowRunner::stopAllThreads()
{
    this->close();
}

void WindowRunner::closeChilds()
{
    for(auto&& i : childWindows){
        i->close();
    }
}

// Reset the whole object.
void WindowRunner::reset()
{
    *this = WindowRunner();
}

// ++++++ Public ++++++

// Default constructor. We initialize the thread functional objects here.

WindowRunner::WindowRunner() : mainEventLoopProc( useRenderThread ? defaultMainEventLoopProc : defaultUnifiedLoopProc ),
                               renderLoopProc( defaultRenderLoopProc )
{
    mtx_staticWindowCounter.lock();
    wnd_counter++;
    mtx_staticWindowCounter.unlock();
}

WindowRunner::WindowRunner(bool startListener,
                const std::string& title,
                const VideoMode& mode,
                uint32_t style,
                const sf::ContextSettings& setts,
                const WindowProperties& props ) : WindowRunner() // We call the default constr too.
{
    create(startListener, title, mode, style, setts, props);
}

WindowRunner::~WindowRunner()
{
    this->close();

    mtx_staticWindowCounter.lock();
    wnd_counter--;
    mtx_staticWindowCounter.unlock();

    if(window != nullptr)
        HLOGF("RenderWindow is not nullptr.\n");
}

void WindowRunner::create( bool startRenderingAndClose,
                const std::string& title,
                const VideoMode& mode,
                uint32_t style,
                const sf::ContextSettings& setts,
                const WindowProperties& props)
{
    std::lock_guard<std::mutex> l0ck(this->mtx_thisObjectSynchronized);

    // These are for limited bugfixing support.
    wndVidMode = mode;
    wndTitle = title;
    wndStyle = style;
    wndSetts = setts;

    // if mainLoopProc is set default. MUST USE BOOST FOR THIS TO WORK!!! ( boost::optional<> )
    //if(_mainProc == DEFAULT)
    //   Set the custom procedures.

    if(startRenderingAndClose)
    {
        // Everything is managed here - WindowImpl creation, rendering and eventThread spawning, and joining and closing WindowImpl.
        // Process can be paused or stopped from another thread using pause, resume or close functions.
        startWindowProcess();
    }
}

// Most important one : The thread this one is called from becomes a rendering thread.
// All stuff of Opening-Closing Threads are Managed Here.
void WindowRunner::startWindowProcess()
{
    std::lock_guard<std::mutex> l0ck(this->mtx_thisObjectSynchronized);  // This function on this window must be running only on 1 thread.

    // This thread must create a WindowImpl.
    HLOGF("WindowRunner::startWindowProcess: Creating WindowImpl...\n");
    createWindowImpl();

    // This is EventThread, so we must set this window's GL context to InActive.
    window->setActive(false);

    // We backup the current RenderThreadUsage property, coz it might change while threads are running.
    bool t_useRenderThread = useRenderThread;

    if(t_useRenderThread){
        HLOGF(" Spawning Render Thread...\n");
        renderThread = std::thread(renderLoopProc, std::ref(*this));  // Start this Render Thread!!!
    }

    isWindowOpen = true;

    HLOGF(" Starting mainEventLoopProc in this thread.\n");
    mainEventLoopProc(std::ref(*this));  // Start event handling thread (at last!)

    // Now the Event Loop (mainEventLoopProc) Returns ----> Window Needs To Close!!!

    // If the RenderThread is still running, we must deal with it!
    if(t_useRenderThread && renderThread.joinable())
    {
        // Post a Close Message if not posted already
        needToClose = true;

        // Event Thread now joins this one after stopping.
        renderThread.join();
    }

    // Lastly, we Close our native WindowImpl.
    closeWindowImpl();

    isWindowOpen = false;

    // We must notify all the waiting threads to resume, if those waiting threads called Close() method on this WindowRunner.
    close_condVar.notify_all();
}

void WindowRunner::pauseAllRunningThreads()
{
    std::lock_guard<std::mutex> l0ck(this->mtx_thisObjectSynchronized);

    (graphThst->m_Mutex).try_lock();
    (eventThst->m_Mutex).try_lock();
    graphicsPausedByExternal = true;
    eventsPausedByExternal = true;
}

void WindowRunner::resumeAllRunningThreads()
{
    std::lock_guard<std::mutex> l0ck(this->mtx_thisObjectSynchronized);

    if(graphicsPausedByExternal)
    {
        (graphThst->m_Mutex).unlock();
        graphicsPausedByExternal = false;
    }
    if(eventsPausedByExternal)
    {
        (eventThst->m_Mutex).unlock();
        eventsPausedByExternal = false;
    }
}

void WindowRunner::pauseRendering()
{
    std::lock_guard<std::mutex> l0ck(this->mtx_thisObjectSynchronized);

    (graphThst->m_Mutex).try_lock();
    graphicsPausedByExternal = true;
}

void WindowRunner::resumeRendering()
{
    std::lock_guard<std::mutex> l0ck(this->mtx_thisObjectSynchronized);

    if(graphicsPausedByExternal)
    {
        (graphThst->m_Mutex).unlock();
        graphicsPausedByExternal = false;
    }
}

// This method closes a window from a different thread than the rendering one. It waits until window is fully closed.
void WindowRunner::close()
{
    if(!isWindowOpen)
        return;

    std::unique_lock<std::mutex> lck( close_mutex );

    // Post a quit message to threads.
    needToClose = true;

    // Now wait until notified by the rendering thread (Using Instance-Local Mutex).
    close_condVar.wait(lck);

    if(closeChildsOnReturn)
        closeChilds();
}

// This method just posts a quit message, but doesn't wait until window is closed.
void WindowRunner::postCloseMessage()
{
    needToClose = true;
}

//==============================================================//

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

void WindowRunner::attachChildWindow(const std::shared_ptr<WindowRunner>& po)
{
    if( !allowDuplicateReferences && std::find(childWindows.begin(), childWindows.end(), po) != childWindows.end() ) // Duplicate found.
    {
        throw std::logic_error("1: Pointer is already in vect, duplicates are not allowed.");
    }

    std::lock_guard<std::mutex> l0ck(this->mtx_thisObjectSynchronized);
    childWindows.push_back(po);
}

void WindowRunner::attachChildWidget(const std::shared_ptr<Widget>& po)
{
    if( !allowDuplicateReferences && std::find(innerWidgets.begin(), innerWidgets.end(), po) != innerWidgets.end() ) // Duplicate found.
    {
        throw std::logic_error("1: Pointer is already in vect, duplicates are not allowed.");
    }

    std::lock_guard<std::mutex> l0ck(this->mtx_thisObjectSynchronized);
    innerWidgets.push_back(po);
}

unsigned int WindowRunner::getWindowInstanceCount() const{
    return wnd_counter;
}

}
