#ifndef JUMPER_H_INCLUDED
#define JUMPER_H_INCLUDED

#include <ostream>
#include <string>
#include <vector>
#include "entity.h"
#include "world.h"

class Jumper : Entity
{
private:
    int hp;
    int atk;
    int jumpHeight;
public:
    Jumper(){ }
    Jumper(int _hp, int _atk, int _jmpHgt);
    virtual ~Jumper(){ }

    const std::vector<std::string>& getProperties() override;
    std::vector<int> getJumperProps() const;

    void jumpUp(World& wrld);
    void jumpUp(World& wrld, int newJumpHeight);

    friend std::ostream& operator<<(std::ostream& os, const Jumper& ent); //friend shower, to access priv fields
};

inline std::ostream& operator<<(std::ostream& os, const Jumper& jmp)
{
    return os<<"Jumper: "<<jmp.spritePath<<", "<<jmp.hp<<", "<<jmp.atk<<", "<<jmp.jumpHeight;
}

#endif // JUMPER_H_INCLUDED
