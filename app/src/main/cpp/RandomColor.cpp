#include "RandomColor.hpp"
#include <cstdlib>

sf::Color getRandomColor()
{
    return sf::Color(rand() % 256, rand() % 256, rand() % 256);
}
