#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow window{sf::VideoMode{{1280u, 720u}}, "Business game"};
    window.setFramerateLimit(60);

    while(window.isOpen())
    {
        while(const std::optional event = window.pollEvent())
        {
            if(event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }

        window.clear();
        window.display();
    }

    return 0;
}
