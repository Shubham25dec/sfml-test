#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Main.hpp>

int main()
{
    sf::VideoMode screenMode = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(screenMode, "SFML Bouncing Ball");

    float radius = 40.f;
    sf::CircleShape ball(radius);
    ball.setFillColor(sf::Color::White);
    ball.setPosition(screenMode.width / 2.f - radius, screenMode.height / 2.f - radius);

    sf::Vector2f velocity(10.f, 10.f);
    bool focused = true;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::LostFocus)
                focused = false;
            if (event.type == sf::Event::GainedFocus)
            {
                focused = true;
                // Recreate the window to restore the EGL context
                window.create(screenMode, "SFML Bouncing Ball");
            }
        }

        if (!focused)
        {
            sf::sleep(sf::milliseconds(100));
            continue;
        }

        ball.move(velocity);

        sf::Vector2f pos = ball.getPosition();

        if (pos.x <= 0 || pos.x + (radius * 2) >= screenMode.width)
            velocity.x = -velocity.x;
        if (pos.y <= 0 || pos.y + (radius * 2) >= screenMode.height)
            velocity.y = -velocity.y;

        window.clear(sf::Color(128, 128, 128));
        window.draw(ball);
        window.display();
    }

    return 0;
}
