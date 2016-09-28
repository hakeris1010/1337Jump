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

class Entity
{
private:
    static size_t counter;
protected:
    std::string spritePath;
    std::vector<std::string> props;

public:
    Entity(){ counter++; }
    Entity(std::string spPat);
    Entity(std::initializer_list<std::string> lst);
    ~Entity(){ counter--; }

    virtual const std::vector<std::string>& getProperties();
    std::string getSpritePath() const;
    static size_t getCount(){ return counter; }

    friend std::ostream& operator<<(std::ostream& os, const Entity& ent);
};

class Jumper : Entity
{
private:
    int hp;
    int atk;
    int jumpHeight;
public:
    Jumper(){ }
    Jumper(int _hp, int _atk, int _jmpHgt);
    ~Jumper(){ }

    const std::vector<std::string>& getProperties() override;
    const std::vector<int>& getJumperProps() const;

    void jumpUp(World& wrld);
    void jumpUp(World& wrld, int newJumpHeight);

    friend std::ostream& operator<<(std::ostream& os, const Jumper& ent); //friend shower, to access priv fields
};

//showers
inline std::ostream& operator<<(std::ostream& os, const World& wrld)
{
    return os<<"World: "<<wrld.bckgrImg<<", "<<(int)wrld.difficulty<<", "<<(int)wrld.loadType<<", "<<wrld.height;
}

inline std::ostream& operator<<(std::ostream& os, const Entity& ent)
{
    os<<"Entity: ";
    for(auto i = ent.props.begin(); i < ent.props.end(); ++i)
    {
        os<<*i<<", ";
    }
    return os<<ent.spritePath;
}

inline std::ostream& operator<<(std::ostream& os, const Jumper& jmp)
{
    return os<<"Jumper: "<<jmp.spritePath<<", "<<jmp.hp<<", "<<jmp.atk<<", "<<jmp.jumpHeight;
}

#endif // WORLD_H_INCLUDED
