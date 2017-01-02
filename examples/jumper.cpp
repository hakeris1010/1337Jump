#include "jumper.h"

//jumper

Jumper::Jumper(int _hp, int _atk, int _jmpHgt) : hp(_hp), atk(_atk), jumpHeight(_jmpHgt)
{ }

const std::vector<std::string>& Jumper::getProperties()
{
    props.push_back(std::string("moving entity"));
    return props;
}

std::vector<int> Jumper::getJumperProps() const
{
    return std::vector<int>({hp, atk, jumpHeight});
}

void Jumper::jumpUp(World& wrld)
{
    wrld.propagate(jumpHeight);
}

void Jumper::jumpUp(World& wrld, int newJumpHeight)
{
    wrld.propagate(newJumpHeight);
}

