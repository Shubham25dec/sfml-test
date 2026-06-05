#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Main.hpp>
#include <SFML/Audio.hpp>


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
    anim::AnimationManager animan(font, screen_size);
    
    sf::SoundBuffer move_sound_buffer;
    if (!move_sound_buffer.loadFromFile("move.ogg")){
    	std::cerr << "Error: failed to load sound file: move.ogg\n";
    	window.close();
    	std::exit(1);
    }
    sf::Sound move_sound;
    move_sound.setBuffer(move_sound_buffer);
    move_sound.play();
    
    float PAD   = 20;
    float btn_w = screen_size.x / 7;
    float btn_h = btn_w / 2; //screen_size.y / 32;
    float btn_x = screen_size.x - btn_w/2 - PAD;
    float btn_y = 80;
    ui::TextButton reset_button({btn_x, btn_y}, font, {btn_w, btn_h}, "Reset");
  
    btn_x -= btn_w + PAD;
    ui::TextButton theme_button({btn_x, btn_y}, font, {btn_w, btn_h}, "Theme");
    
    int grid_size = 4;
	Game game(window, font, clock, move_sound, animan,theme_button, reset_button, grid_size);
	game.mainloop();
	
return 0;
}//main
