#pragma once

#include <SFML/Graphics.hpp>
#include <cmath>


//FIXME: for some reason if the setRadius function is not called the shape is not being drawn
//maybe because points are not being initialised until that method
//is called.

class RoundedRectangleShape : public sf::Shape {
public:
    RoundedRectangleShape(sf::Vector2f size = {0, 0}, float radius = 0, unsigned int cornerPointCount = 15) 
        : mySize(size), myRadius(radius), myCornerPointCount(cornerPointCount) {
        update();
    }

    void setSize(const sf::Vector2f& size) { mySize = size; update(); }
    void setRadius(float radius) { myRadius = radius; update(); }

    std::size_t getPointCount() const override { return myCornerPointCount * 4; }

    sf::Vector2f getPoint(std::size_t index) const override {
        if (myRadius == 0.f) return {0, 0};

        unsigned int cornerIndex = index / myCornerPointCount;
        unsigned int pointInCorner = index % myCornerPointCount;
        
        // Map angles sequentially through all four quadrants clockwise
        float deltaAngle = 90.f / (myCornerPointCount - 1);
        float angle = (cornerIndex * 90.f + pointInCorner * deltaAngle) * 3.14159265f / 180.f;

        sf::Vector2f center;
        switch (cornerIndex) {
            case 0: center = {mySize.x - myRadius, mySize.y - myRadius}; break; // Bottom-Right
            case 1: center = {myRadius, mySize.y - myRadius}; break;            // Bottom-Left
            case 2: center = {myRadius, myRadius}; break;                       // Top-Left
            case 3: center = {mySize.x - myRadius, myRadius}; break;            // Top-Right
        }

        return center + sf::Vector2f(std::cos(angle) * myRadius, std::sin(angle) * myRadius);
    }

private:
    sf::Vector2f mySize;
    float myRadius;
    unsigned int myCornerPointCount;
};
