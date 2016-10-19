/*========================================================
                  Uzd.1 by Kestutis Dirma
                       UML Diagrams
----------------------------------------------
         World
----------------------------------------------
- bckgrImg   : std::string
- loadType   : char
- difficulty : char
- height     : int
- goUp(hgt : int) : void
----------------------------------------------
+ loadResources() : void
+ getType()       : char
+ getDiff()       : char
+ propagate(speed : int) : void
+ getWorldString() : std::string
----------------------------------------------

----------------------------------------------
         Entity
----------------------------------------------
- counter    : size_t
# spritePath : std::string
# props      : std::vector<std::string>
----------------------------------------------
+ getProperties() : std::vector<std::string>
+ getSpritePath() : std::string
+ getCount()      : size_t
----------------------------------------------
                   / \
                    |
                    |
----------------------------------------------
         Jumper
----------------------------------------------
- hp         : int
- atk        : int
- jumpHeight : int
----------------------------------------------
+ getProperties()  : std::vector<std::string>
+ getJumperProps() : std::vector<int>
+ jumpUp(wrld : World& )         : void
+ jumpUp(wrld : World&, newJumpHeight : int) : void
----------------------------------------------

========================================================*/

#ifndef WORLD_H_INCLUDED
#define WORLD_H_INCLUDED

#include <ostream>
#include <string>
#include <vector>
#include <initializer_list>

class World
{
private:
    std::string bckgrImg;
    char loadType = 0;
    char difficulty = 0;
    int height = 0;

    void goUp(int hgt);
public:
    World();
    World(std::string bimg, char ltyp, char diff);
    ~World();

    void loadResources();
    char getType() const;
    char getDiff() const;
    void propagate(int hgt = 10);
    std::string getWorldString();

    friend std::ostream& operator<<(std::ostream& os, const World& wrld);
};

//showers
inline std::ostream& operator<<(std::ostream& os, const World& wrld)
{
    return os<<"World: "<<wrld.bckgrImg<<", "<<(int)wrld.difficulty<<", "<<(int)wrld.loadType<<", "<<wrld.height;
}

#endif // WORLD_H_INCLUDED
