#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Main.hpp>
#include <SFML/Audio.hpp>

#include "helper.hpp"//namespace qq
#include "utils.hpp"
#include "constants.hpp"
#include "game.hpp"
#include "animation.hpp"

//++++++++++++++++++++++++++++++++++++
#if defined(__ANDROID__)
#include <android/native_activity.h>
#include <SFML/System/NativeActivity.hpp>

std::string getAndroidSaveDir() {
    //access the underlying ANativeActivity structure from SFML
    ANativeActivity* activity = sf::getNativeActivity();
    if (activity != nullptr && activity->internalDataPath != nullptr) {
        return std::string(activity->internalDataPath) + "/";
    }else{
    	std::cerr << "Failed to get internal directory path" << std::endl;
    }
    return "./";
}
#endif
//++++++++++++++++++++++++++++++++++++


int main(){
    sf::Font font = utils::load_font("font.ttf");
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "2048 game");

//++++++++++++++++++++++++++++++++++++
    #if defined(__ANDROID__)
    	SAVE_DIR = getAndroidSaveDir();
    	HIGH_SCORE_FILEPATH = SAVE_DIR + "high_scores.txt";
    #endif
//++++++++++++++++++++++++++++++++++++

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
    
    qq::GridKind kind = qq::FOUR_BY_FOUR;
	Game game(window, font, clock, move_sound, animan,theme_button, reset_button, kind);
	
	while (1){
		if (scene::menuScreen(window, font) == 1){
			game.mainloop(); //TODO: gameloop will exit app on back key press, so currently there is no way to come back from the function!
		}else{
			scene::aboutScreen(window, font);
		}
	}	
return 0;
}//main
