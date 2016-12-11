#ifndef WINDOWRUNNER_H_INCLUDED
#define WINDOWRUNNER_H_INCLUDED

#include <cstdint>
#include <memory>
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include "threadstate.h"
#include "contexts.hpp"
#include "widgets.h"
#include "windowhelpers.h"

namespace Gryl
{
const uint32_t GRYL_DEF_WIDTH  = 300;
const uint32_t GRYL_DEF_HEIGHT = 300;
const uint32_t GRYL_DEF_BPP    = 32;
const uint32_t GRYL_DEF_FPS    = 60;

// GrylloEngine implementation of a Window Instance Class.
class WindowRunner : public Widget
{
protected:
    // Base window of SFML
    sf::RenderWindow window;
    std::shared_ptr<GraphicThreadState> thr_st;

    bool wnd_vertSyncEnabled = true;
    uint32_t wnd_fps = GRYL_DEF_FPS;

    static unsigned int wnd_counter;

public:
    WindowRunner();
    WindowRunner(bool startListener,
                 const VideoMode& mode = VideoMode(GRYL_DEF_WIDTH, GRYL_DEF_HEIGHT, GRYL_DEF_BPP),
                 const std::string& title = std::string(),
                 uint32_t style = sf::Style::Close,
                 const sf::ContextSettings& setts = sf::ContextSettings());
    ~WindowRunner();

    void create(bool startListener = false,
                const VideoMode& mode = VideoMode(GRYL_DEF_WIDTH, GRYL_DEF_HEIGHT, GRYL_DEF_BPP),
                const std::string& title = std::string(),
                uint32_t style = sf::Style::Close,
                const sf::ContextSettings& setts = sf::ContextSettings());

    //original window method's
    sf::RenderWindow& getSfmlWindowRef(); // use only when modifying the inner contents.
    const sf::RenderWindow& getSfmlWindowConst() const;
    std::shared_ptr<GraphicThreadState> getGraphicThreadState() const;

    //void startListeningLoop(WindowListener listener);
    void startListeningLoop();

    unsigned int getWindowInstanceCount() const;

    //overrided from Widget
    void addInnerWidget( Widget& widg );

    friend class WindowEventListener;
    friend class EventHandler;
};

}

#endif // WINDOWRUNNER_H_INCLUDED
