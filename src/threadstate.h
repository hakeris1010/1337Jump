#ifndef GAMEDATA_H_INCLUDED
#define GAMEDATA_H_INCLUDED

#include <string>

//GrylloEngine Thread state structures.
namespace Gryl
{
struct ThreadState
{
public:
    std::string belongsTo;
    bool canModify = true;


};

struct GraphicThreadState : ThreadState
{

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
