#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Main.hpp>

#include "utils.hpp"
#include "constants.hpp"
#include "game.hpp"


int main(){
    sf::Font font = utils::load_font("font.ttf");
	Game game(font, 4);
	game.mainloop();

return 0;
}//main
