#pragma once

#include <SFML/Graphics.hpp>

#include "constants.hpp"
#include "ui.hpp"
#include "helper.hpp"

namespace scene{

enum Answer{
	NO,
	YES,
	NOTHING
};


void handleFocusEvents(sf::RenderWindow& window, sf::Event event){
	if (event.type == sf::Event::LostFocus){
        window.setActive(false);
    }else if (event.type == sf::Event::GainedFocus){
        window.setActive(true);
    }
}


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
			handleFocusEvents(window, event);
			if (event.type == sf::Event::Closed){
				return NOTHING;
			}else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape){
				return NOTHING;
			}else if (event.type == sf::Event::TouchBegan || event.type == sf::Event::TouchEnded){
				y_button.handleTouchEvent(event, window);
				n_button.handleTouchEvent(event, window);
			}
		}
		
		if (!window.hasFocus()) {
			sf::sleep(sf::milliseconds(500));
			continue;
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
			handleFocusEvents(window, event);
			if (event.type == sf::Event::Closed){
				return;
			}else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape){
				return ;
			}
		}
		
		if (!window.hasFocus()) {
			sf::sleep(sf::milliseconds(500));
			continue;
		}
		
		window.clear(BG_COLOR);
		
		window.draw(text);
		
		window.display();
	}
}


qq::GridKind menuScreen(sf::RenderWindow& window, sf::Font& font){
	//returns gridkind selected by user to play
	
	sf::Vector2u screen_size = window.getSize();
	float WIDTH = screen_size.x;

	sf::Vector2f size = {WIDTH/4, WIDTH/6};
	sf::Vector2f pos = {WIDTH/2, size.y};

	ui::TextButton menu_label(pos, font, size, "2048");
	menu_label.setBgColor({0,0,0,0});
	menu_label.setTextColor({168, 120, 37});

	pos.y += size.y + 20;
	size = sf::Vector2f({WIDTH/5, WIDTH/10});
	
	ui::TextButton play4x4_button(pos, font, size, "4 X 4");
	pos.y += size.y + 20;
	ui::TextButton play5x5_button(pos, font, size, "5 X 5");
	pos.y += size.y + 20;
	ui::TextButton play6x6_button(pos, font, size, "6 X 6");
	pos.y += size.y + 20;
	ui::TextButton play8x8_button(pos, font, size, "8 X 8");
	pos.y += size.y + 20;
	pos.y += size.y + 20;
	ui::TextButton about_button(pos, font, size, "about");
	
	while (true){
		sf::Event event;
		while (window.pollEvent(event)){
			handleFocusEvents(window, event);
			if (event.type == sf::Event::Closed){
				window.close();
				std::exit(0);
			}else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape){
				window.close();
				std::exit(0);
			}else if (event.type == sf::Event::TouchBegan || event.type == sf::Event::TouchEnded){
				play4x4_button.handleTouchEvent(event, window);
				play5x5_button.handleTouchEvent(event, window);
				play6x6_button.handleTouchEvent(event, window);
				play8x8_button.handleTouchEvent(event, window);
				about_button.handleTouchEvent(event, window);
			}
		}
		
		if (!window.hasFocus()) {
			sf::sleep(sf::milliseconds(500));
			continue;
		}
			
		if (about_button.is_released()) aboutScreen(window, font);
		if (play4x4_button.is_released()) return qq::FOUR_BY_FOUR;
		if (play5x5_button.is_released()) return qq::FIVE_BY_FIVE;
		if (play6x6_button.is_released()) return qq::SIX_BY_SIX;
		if (play8x8_button.is_released()) return qq::EIGHT_BY_EIGHT;
		
		window.clear(BG_COLOR);
		
		menu_label.draw(window);
		play4x4_button.draw(window);
		play5x5_button.draw(window);
		play6x6_button.draw(window);
		play8x8_button.draw(window);
		about_button.draw(window);
		
		window.display();
	}
}
	
};//namespace 
