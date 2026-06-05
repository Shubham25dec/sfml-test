#pragma once

#include <SFML/Graphics.hpp>

#include "constants.hpp"
#include "ui.hpp"

namespace scene{

enum Answer{
	NO,
	YES,
	NOTHING
};


Answer askYesNo(sf::RenderWindow& window, sf::Font& font,  std::string msg="Yes or no"){
	sf::Text text;
	text.setFont(font);
	text.setString(msg);
	text.setCharacterSize(40);
	
	sf::Vector2f pos = {100, 100};
	text.setPosition(pos);
	pos.y += 120;
	sf::Vector2f size = {120, 60};
	
	ui::TextButton y_button(pos, font, size, " Yes ");
	pos.x += 250;
	ui::TextButton n_button(pos, font, size, " No ");
	
	while (true){
		sf::Event event;
		while (window.pollEvent(event)){
			if (event.type == sf::Event::Closed){
				return NOTHING;
			}else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape){
				return NOTHING;
			}else if (event.type == sf::Event::TouchBegan || event.type == sf::Event::TouchEnded){
				y_button.handleTouchEvent(event, window);
				n_button.handleTouchEvent(event, window);
			}
		}
		
		if (y_button.is_released()) return YES;
		if (n_button.is_released()) return NO;
		
		window.clear(BG_COLOR);
		
		window.draw(text);
		y_button.draw(window);
		n_button.draw(window);
		
		window.display();
	}
}


void menuScreen(sf::RenderWindow& window, sf::Font& font){
	;
}
	
};//namespace 
