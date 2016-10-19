#ifndef ENTITY_H_INCLUDED
#define ENTITY_H_INCLUDED

#include <string>
#include <initializer_list>
#include <ostream>
#include <vector>

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

inline std::ostream& operator<<(std::ostream& os, const Entity& ent)
{
    os<<"Entity: ";
    for(auto i = ent.props.begin(); i < ent.props.end(); ++i)
    {
        os<<*i<<", ";
    }
    return os<<ent.spritePath;
}

#endif // ENTITY_H_INCLUDED
