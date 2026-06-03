#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Main.hpp>

#include "helper.hpp"//namespace qq
#include "utils.hpp"
#include "constants.hpp"
#include "game.hpp"
#include "animation.hpp"


int main(){
    sf::Font font = utils::load_font("font.ttf");
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "2048 game");
    sf::Vector2u screen_size = window.getSize();
    
    sf::Clock clock;
    utils::FpsVisualiser fps_viz(font);
    anim::AnimationManager animan(font, screen_size);

    int grid_size = 4;
	Game game(window, font, clock, fps_viz, animan, grid_size);
	
	game.mainloop();

return 0;
}//main
