#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Main.hpp>
#include <cstdlib>
#include <sstream>

int main()
{
    sf::VideoMode screenMode = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(screenMode, "SFML Bouncing Ball");

    // Font
    sf::Font font;
    font.loadFromFile("font.ttf");

    // FPS text
    sf::Text fpsText;
    fpsText.setFont(font);
    fpsText.setCharacterSize(40);
    fpsText.setFillColor(sf::Color::Yellow);
    fpsText.setPosition(10.f, 10.f);

    // Hello World text
    sf::Text helloText;
    helloText.setFont(font);
    helloText.setString("Hello World!");
    helloText.setCharacterSize(60);
    helloText.setFillColor(sf::Color::Green);
    sf::FloatRect textBounds = helloText.getLocalBounds();
    helloText.setPosition(
        screenMode.width / 2.f - textBounds.width / 2.f,
        screenMode.height / 2.f - textBounds.height / 2.f
    );

    // Ball
    float radius = 40.f;
    sf::CircleShape ball(radius);
    ball.setFillColor(sf::Color::White);
    ball.setPosition(screenMode.width / 2.f - radius, screenMode.height / 2.f - radius);

    sf::Vector2f velocity(10.f, 10.f);

    // FPS tracking
    sf::Clock clock;
    int frameCount = 0;
    float fpsTimer = 0.f;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::LostFocus)
                window.setActive(false);

            if (event.type == sf::Event::GainedFocus)
                window.setActive(true);

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
                window.close();
        }

        if (window.hasFocus())
        {
            float dt = clock.restart().asSeconds();
            fpsTimer += dt;
            frameCount++;

            if (fpsTimer >= 0.5f)
            {
                int fps = (int)(frameCount / fpsTimer);
                std::ostringstream ss;
                ss << "FPS: " << fps;
                fpsText.setString(ss.str());
                frameCount = 0;
                fpsTimer = 0.f;
            }

            ball.move(velocity);

            sf::Vector2f pos = ball.getPosition();

            if (pos.x <= 0 || pos.x + (radius * 2) >= screenMode.width)
                velocity.x = -velocity.x;
            if (pos.y <= 0 || pos.y + (radius * 2) >= screenMode.height)
                velocity.y = -velocity.y;

            window.clear(sf::Color(128, 128, 128));
            window.draw(helloText);
            window.draw(ball);
            window.draw(fpsText);
            window.display();
        }
        else
        {
            clock.restart();
            sf::sleep(sf::milliseconds(100));
        }
    }

    exit(0);
}
