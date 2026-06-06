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


void aboutScreen(sf::RenderWindow& window, sf::Font& font){
	sf::Text text;
	text.setFont(font);
	text.setCharacterSize(25);
	text.setString("A clone of the classic 2048 game.\n(C++ beginner project)\n\nDeveloped by: Shubham\nshubhamsingh25sn@gmail.com");
	
	sf::Vector2f pos = {100, 100};
	text.setPosition(pos);
	
	while (true){
		sf::Event event;
		while (window.pollEvent(event)){
			if (event.type == sf::Event::Closed){
				return;
			}else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape){
				return ;
			}
		}
			
		window.clear(BG_COLOR);
		
		window.draw(text);
		
		window.display();
	}
}


int menuScreen(sf::RenderWindow& window, sf::Font& font){
	//return 0 for about screen, 1 for playing
	sf::Text text;
	text.setFont(font);
	text.setCharacterSize(25);
	text.setString("MENU SCREEN (TODO)");
	
	sf::Vector2f pos = {100, 100};
	text.setPosition(pos);
	
	pos.y += 100;
	sf::Vector2f size = {100, 50};
	ui::TextButton play_button(pos, font, size, "Play");
	pos.y += 100;
	ui::TextButton about_button(pos, font, size, "about");
	
	while (true){
		sf::Event event;
		while (window.pollEvent(event)){
			if (event.type == sf::Event::Closed){
				return NOTHING;
			}else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape){
				std::exit(0);
			}else if (event.type == sf::Event::TouchBegan || event.type == sf::Event::TouchEnded){
				play_button.handleTouchEvent(event, window);
				about_button.handleTouchEvent(event, window);
			}
		}
			
		if (about_button.is_released()) return 0;
		if (play_button.is_released()) return 1;
		
		window.clear(BG_COLOR);
		
		window.draw(text);
		play_button.draw(window);
		about_button.draw(window);
		
		window.display();
	}
}
	
};//namespace 
