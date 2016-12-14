#ifndef WINDOWRUNNER_H_INCLUDED
#define WINDOWRUNNER_H_INCLUDED

#include <cstdint>
#include <memory>
#include <vector>
#include <string>
#include <thread>
#include <SFML/Graphics.hpp>
#include "threadstate.h"
#include "contexts.hpp"
#include "widgets.h"
#include "windowhelpers.h"

/* All the backend of GrylloEngine is powered by SFML by now.
    - TODOS: Move to more efficient ones, like SDL.
*/

namespace Gryl
{
const uint32_t GRYL_DEF_WIDTH  = 300;
const uint32_t GRYL_DEF_HEIGHT = 300;
const uint32_t GRYL_DEF_BPP    = 32;
const uint32_t GRYL_DEF_FPS    = 60;

/* GrylloEngine implementation of a Window Instance Class.
   -----------------------------------------------------
   - It's The Topmost Widget (All nested widgets are just graphical stuff inside this window)
   - Inherited from Widget for backwards comp and future implementations of WxWidgets-like API (Now NOT.)
   - The only Widget capable which is an EventPump, capable of getting events from the OS,
     through an inner implementation, this case a SFML window. (Might implement GLEW, SDL, or GTK lib's window in da future).

   - Dedicated for mostly Graphical tasks (though has an ability to run processing too).
*/

class WindowRunner : public Widget
{
private:
    static unsigned int wnd_counter;

protected:
    // Base window of SFML
    sf::RenderWindow window;
    std::shared_ptr<GraphicThreadState> thr_st;

    bool wnd_vertSyncEnabled = true;
    uint32_t wnd_fps = GRYL_DEF_FPS;

    bool useRenderingThread = false;
    bool listenInSeparateThread = false;

    /* Our main loop functions.
        If renderingInThread is specified, rendering happens in renderingLoop,
        If not, all the processing and rendering happens in mainLoop. */

    std::function< void(std::shared_ptr<WindowRunner>) > mainLoopProc;
    std::function< void(std::shared_ptr<WindowRunner>) > renderingLoopProc;

    // The Thread Objects of the window. Usually only 1st is used.
    std::thread mainThread;
    std::thread rendThread;

    // The EventLoop procedures.
    static void defaultMainUnifiedLoop( std::shared_ptr<WindowRunner> ); // Use this when SingleThreading

    static void defaultProcessLoop( std::shared_ptr<WindowRunner> );     // Use these 2 when MultiThreading.
    static void defaultRenderingLoop( std::shared_ptr<WindowRunner> );

    // Delete the default copy constructors.
    WindowRunner(const WindowRunner&) = delete;
    WindowRunner(WindowRunner&&) = delete;

public:
    WindowRunner();

    /* Constructor explanation:
        - startListenerInSeparateThread:
        1) if set, we'll run all WindowRunner's loops in separate threads:
            * min. 1 thread (defaultMainUnifiedLoop), if useRenderThread is false (Most likely case)
            * max. 2 threads (defaultProcessLoop and defaultRenderingLoop) if useRenderThread is set.
              In this case our data processing'll happen in mainLoopProc, and rendering in renderingLoopProc.
              However, this case is not likely to be used, because we intend to run our processing separated from WindowRunner object whatsoever.
        2) if false, we'll run our mainLoopProc in caller's thread.
            An additional thread might be spawned if useRenderThread is set (Not likely to happen).

        - useRenderThread - spawn thread dedicated for running only rendering processes (the renderingLoopProc function).
            * DON'T USE IF U DON'T KNOW WHATCHA'R DOIN' !!! It's mostly useless, coz the WindowRunner itself is dedicated for rendering.
              It's set mostly for universality, legacy support, and weirdos who like to do things their way.
            * Uses defaultRenderingLoop if different function is not passed.
    */

    WindowRunner(bool startListener,
                 bool startListenerInSeparateThread = false,
                 const VideoMode& mode = VideoMode(GRYL_DEF_WIDTH, GRYL_DEF_HEIGHT, GRYL_DEF_BPP),
                 const std::string& title = std::string(),
                 uint32_t style = sf::Style::Close,
                 const sf::ContextSettings& setts = sf::ContextSettings(),
                 bool useRenderThread = false,
                 const std::function< void(std::shared_ptr<WindowRunner>) >& _mainLoop = defaultMainUnifiedLoop,
                 const std::function< void(std::shared_ptr<WindowRunner>) >& _renderLoop = defaultRenderingLoop);
    ~WindowRunner();

    void create(bool startListener = false,
                bool startListenerInSeparateThread = false,
                const VideoMode& mode = VideoMode(GRYL_DEF_WIDTH, GRYL_DEF_HEIGHT, GRYL_DEF_BPP),
                const std::string& title = std::string(),
                uint32_t style = sf::Style::Close,
                const sf::ContextSettings& setts = sf::ContextSettings(),
                bool useRenderThread = false,
                const std::function< void(std::shared_ptr<WindowRunner>) >& _mainLoop = defaultMainUnifiedLoop,
                const std::function< void(std::shared_ptr<WindowRunner>) >& _renderLoop = defaultRenderingLoop);

    //original window method's
    sf::RenderWindow& getSfmlWindowRef(); // use only when modifying the inner contents.
    const sf::RenderWindow& getSfmlWindowConst() const;
    std::shared_ptr<GraphicThreadState> getGraphicThreadState() const;

    //This one starts the threads or just launches loopProc if no threading.
    void startListening();

    //Possible to use this one only when multithreading.
    void stopAllThreads();

    unsigned int getWindowInstanceCount() const;

    //overrided from Widget
    //void addInnerWidget( Widget& widg );

    friend class WidgetEventListener;
    friend class EventHandler;
};

}

#endif // WINDOWRUNNER_H_INCLUDED
