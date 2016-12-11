#ifndef GAMEDATA_H_INCLUDED
#define GAMEDATA_H_INCLUDED

#include <string>
#include <memory>

//GrylloEngine Thread state structures.
namespace Gryl
{
class WindowRunner;

struct ThreadState
{
    std::string belongsTo;
    bool canModify = true;
    // some more.
};

struct GraphicThreadState : ThreadState
{
    std::shared_ptr<WindowRunner> currentWindow;
};
}

//Extended: Jump Game's thread structure.
namespace Jump
{
struct GameProcessorThreadState : Gryl::ThreadState
{

};

}

#endif // GAMEDATA_H_INCLUDED
