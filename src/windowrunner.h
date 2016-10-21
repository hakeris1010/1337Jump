#ifndef WINDOWRUNNER_H_INCLUDED
#define WINDOWRUNNER_H_INCLUDED

#include <cstdint>
#include <memory>
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include "threadstate.h"
#include "GrylloEngine/contexts.hpp"
#include "widgets.h"

namespace Gryl
{
const uint32_t GRYL_DEF_WIDTH  = 300;
const uint32_t GRYL_DEF_HEIGHT = 300;
const uint32_t GRYL_DEF_BPP    = 32;
const uint32_t GRYL_DEF_FPS    = 60;

// GrylloEngine implementation of a Window Instance Class.
class WindowRunner
{
protected:
    sf::RenderWindow window;
    GraphicThreadState* thr_st = nullptr;

    //Maybe not needed (saved in sf::RenderWindow)
    VideoMode mode;
    std::string wnd_title;
    sf::ContextSettings wnd_consets;
    uint32_t style = sf::Style::Close;
    //end of maybe not needed.

    bool wnd_vertSyncEnabled = true;
    uint32_t wnd_fps = GRYL_DEF_FPS;

    static unsigned int wnd_counter;

    std::vector< std::shared_ptr<Gryl::Widget> > widgets;

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

    sf::RenderWindow& getSfmlWindowRef() const;
    GraphicThreadState* getGraphicThreadState() const;
    const std::vector< std::shared_ptr<Gryl::Widget> >& getWidgetVector() const;
};

}

#endif // WINDOWRUNNER_H_INCLUDED
