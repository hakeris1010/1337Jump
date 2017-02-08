#ifndef WINDOWRUNNER_H_INCLUDED
#define WINDOWRUNNER_H_INCLUDED

#include <cstdint>
#include <memory>
#include <vector>
#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <SFML/Graphics.hpp>
#include "threadstate.h"
#include "contexts.h"
#include "widgets.h"
#include "eventhandling.h"
#include <boost/optional.hpp>

/* All the backend of GrylloEngine is powered by SFML by now.
    - TODOS: Move to more efficient ones, like GLEW/GLFW (And use raw OpenGL (duh)).
*/

#define GRYL_WINDOWRUNNER_VERSION "v0.3 pre"

namespace Gryl
{
struct WindowProperties : public WidgetProperties
{

};

// TODO: This is TEMP! To be moved into WindowImpl section.
static WindowProperties defProps;

// Several defines of this window.
const uint32_t GRYL_DEF_WIDTH  = 300;
const uint32_t GRYL_DEF_HEIGHT = 300;
const uint32_t GRYL_DEF_BPP    = 32;
const uint32_t GRYL_DEF_FPS    = 60;

/* GrylloEngine implementation of a Window Instance Class.
   -----------------------------------------------------
   - It's The Topmost EventDispatcher (All nested widgets are just graphical stuff inside this window)
   - Inherited from EventDispatcher for backwards comp and future implementations of WxWidgets-like API (Now NOT.)
   - The only EventDispatcher capable which is an EventPump, capable of getting events from the OS,
     through an inner implementation, this case a SFML window. (Might implement GLEW, SDL, or GTK lib's window in da future).

   - Dedicated for mostly Graphical tasks (though has an ability to run processing too).
*/

class WindowRunner : public EventDispatcher
{
private:
    // Delete the default copy constructors.
    WindowRunner(const WindowRunner&) = delete;
    WindowRunner(WindowRunner&&) = delete;
    WindowRunner& operator=(const WindowRunner&) = delete;
    WindowRunner& operator=(WindowRunner&&) = delete;

    static unsigned int wnd_counter;
    std::mutex mtx_staticWindowCounter;

//protected:
    // Base window of SFML
    std::shared_ptr<sf::RenderWindow> window = nullptr;
    bool isWindowOpen = false;

    // SFML Window properties
    bool wnd_vertSyncEnabled = true;
    uint32_t wnd_fps = GRYL_DEF_FPS;

    // These 4 should not be used. Now they're here because of the window creation difficulty.
    VideoMode wndVidMode;
    std::string wndTitle;
    uint32_t wndStyle = sf::Style::Close;
    sf::ContextSettings wndSetts;

    // Threading settings.
    bool useRenderThread = true;

    // TODO : This should be replaced with a Recursive Mutex.
    std::mutex mtx_thisObjectSynchronized;

    // These mutexes and CondVars are used for notifying threads waiting for window's close.
    std::mutex close_mutex;
    std::condition_variable close_condVar;

    std::shared_ptr<GraphicThreadState> graphThst = std::make_shared<GraphicThreadState>();
    std::shared_ptr<EventThreadState> eventThst = std::make_shared<EventThreadState>();

    // The Thread Objects of the window.
    std::thread renderThread;

    // Childs.
    std::vector<std::shared_ptr<WindowRunner>> childWindows;
    std::vector<std::shared_ptr<Widget>> innerWidgets;

    // Misc settings.
    bool allowDuplicateReferences = true; // Do we allow the duplicate references in child vectors.
    bool closeChildsOnReturn = true;

    /* Our main loop functions.
        If useRenderThread is specified, events are processed in a eventLoopProc, running in separate thread.
        If not, all the processing and rendering happens in mainRenderLoopProc, running in current thread. */

    std::function< void(WindowRunner&) > mainEventLoopProc;
    std::function< void(WindowRunner&) > renderLoopProc;

    // The Default Loop procedures.
    static void defaultUnifiedLoopProc( WindowRunner& );    // Events and Rendering in 1 thread

    static void defaultMainEventLoopProc( WindowRunner& ); // Events and Rendering in separate threads.
    static void defaultRenderLoopProc( WindowRunner& );

    // Runtime Fields

    // BIG TODO: Make these Runtime Variables Read-Only to other threads beside rendering and events! Use another class for those threads.
    volatile bool needToClose = false;
    volatile bool needToStopListening = false;
    volatile bool implCloseMessage = false;

    volatile bool graphicsPausedByExternal = false;
    volatile bool eventsPausedByExternal = false;

    // Private Methods.
    // These 2 MUST BE CALLED FROM A RENDERING THREAD!!!
    void createWindowImpl(); // TODO <--- MOVE TO WindowImpl class.
    void closeWindowImpl();

    //Possible to use this one only when multithreading.
    // This must be called FROM A RENDERING THREAD, THAT CALLED createWindowImpl()!!!
    void stopAllThreads();
    void closeChilds();

    void reset();

public:
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

    WindowRunner();
    WindowRunner(bool startListenerAndClose,
                 const std::string& title = std::string(),
                 const VideoMode& mode = VideoMode(GRYL_DEF_WIDTH, GRYL_DEF_HEIGHT, GRYL_DEF_BPP),
                 uint32_t style = sf::Style::Close,
                 const sf::ContextSettings& setts = sf::ContextSettings(),
                 const WindowProperties& props = defProps);
    virtual ~WindowRunner();

    void create(bool startListenerAndClose = false,
                const std::string& title = std::string(),
                const VideoMode& mode = VideoMode(GRYL_DEF_WIDTH, GRYL_DEF_HEIGHT, GRYL_DEF_BPP),
                uint32_t style = sf::Style::Close,
                const sf::ContextSettings& setts = sf::ContextSettings(),
                const WindowProperties& props = defProps);

    //This one starts the Render and EventHandle threads or just launches UnifiedLoopProc if no inner threading.
    void startWindowProcess();

    //Pause and resume the Rendering and Event Handling.
    void pauseRendering();
    void resumeRendering();
    void pauseAllRunningThreads();
    void resumeAllRunningThreads();

    //Stops the rendering and event handling, and closes the Impl window.
    void close();
    void postCloseMessage();

    std::shared_ptr<sf::RenderWindow> getSfmlWindowPtr() const; // use only when modifying the inner contents.
    std::shared_ptr<GraphicThreadState> getGraphicThreadState() const;
    std::shared_ptr<EventThreadState> getEventThreadState() const;

    void attachChildWindow(const std::shared_ptr<WindowRunner>&);
    void attachChildWidget(const std::shared_ptr<Widget>&);

    // Others.
    unsigned int getWindowInstanceCount() const;

    friend class WidgetEventListener;
    friend class WidgetEventListener_Importable;
    friend class CustomEventDeterminer;
};

}

#endif // WINDOWRUNNER_H_INCLUDED
