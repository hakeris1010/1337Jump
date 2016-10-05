#include <iostream>
#include <SFML/Graphics.hpp>
#include "world.h"

void uzdtest()
{
    World wrld("nyan.jpg", 0, 2); // pasaulis, kuriame sokineja zmogeliukas
    Entity ents[3];                   // random butybes, skirtos testavimui
    Jumper *jmp = new Jumper(100, 20, 200); // sokinetojas.

    for(int i=0; i<3; i++) //sukuriam 3 butybes ir priskiriam props'am skaicius.
        ents[i] = Entity({std::string(1, '0'+i)});

    std::cout<<"--- Start ---\n"<<wrld<<"\n"<<*jmp<<"\n"; //parodom kokia situacija pradzioje
    for(int i=0; i<3; i++)
        std::cout<<"ent["<<i<<"]: "<<ents[i]<<"\n";
    std::cout<<"\nEnt.count: "<<Entity::getCount()<<"\n\n";

    std::vector<std::string> jprops = jmp->getProperties(); //parodom dziamperio propsus
    for(auto i=jprops.begin(); i<jprops.end(); ++i){
        std::cout<<"Jprop "<<i-jprops.begin()<<": "<< *i <<",";
    }

    std::cout<<"\nJumping up.\n"; //pasokam, ir pakeiciam pasaulio auksti.
    jmp->jumpUp(wrld, 50);

    std::cout<<"\n--- End ---\n"<<wrld<<"\n"<<*jmp<<"\n"; //parodom kokia situacija pabaigoje
    for(int i=0; i<3; i++)
        std::cout<<"ent["<<i<<"]: "<<ents[i]<<"\n";

    delete jmp;
    std::cout<<"\nEnt.count: "<<Entity::getCount()<<"\n\n";
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(shape);
        window.display();
    }

    return 0;
}
