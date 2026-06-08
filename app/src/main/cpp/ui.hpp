#pragma once

#include <SFML/Graphics.hpp>

#include "utils.hpp"
#include "rounded_rect.hpp"

namespace ui{


enum ButtonState{
	NORMAL,
	CLICKED,
	RELEASED
};

struct TextButton{ //A touch screen button which changes states on press - unpress.
	int touch_id = -1;
	ButtonState state = NORMAL;
	RoundedRectangleShape rect;
	sf::Text button_text;
	
	TextButton(sf::Vector2f pos, sf::Font& font, sf::Vector2f size={100, 70}, std::string text = ""){
		rect.setSize(size);
		rect.setRadius(10);
		auto bounds = rect.getLocalBounds();
		rect.setOrigin(bounds.width/2, bounds.height/2);
		rect.setPosition(pos);
		
		button_text.setFont(font);
		button_text.setCharacterSize(size.x);
		button_text.setString(text);
		bounds = button_text.getLocalBounds();
		button_text.setOrigin(bounds.left + bounds.width/2, bounds.top + bounds.height/2); //bounds.left added to fix internal glyph padding,
		//which causes text to appear slightly below center
		button_text.setPosition(pos);
		float text_padding = 0.85f; //text occupies 85%
		float scaleX = (size.x * text_padding) / bounds.width;
		float scaleY = (size.y * text_padding) / bounds.height;
		float scale = (scaleX < scaleY) ? scaleX : scaleY ;
		button_text.setScale(scale, scale);
		button_text.setFillColor(sf::Color::Black);
	}
	
	void draw(sf::RenderWindow& window){
		window.draw(rect);
		window.draw(button_text);
	}
	
	void checkClick(sf::Vector2f click_pos, unsigned int fingerid){
		//if (sprite.getGlobalBounds().contains(click_pos.x, click_pos.y)){
		if (rect.getGlobalBounds().contains(click_pos)){
			state = CLICKED;
			reverseColors();
			touch_id = fingerid;
		}
	}
	
	void checkUnclick(unsigned int fingerid){
		if (fingerid == touch_id){
			state = RELEASED;
			reverseColors();
			touch_id = -1;
		}
	}
	
	void handleTouchEvent(sf::Event event, sf::RenderWindow& window){
		if (event.type == sf::Event::TouchBegan){
			auto mapped_pos = getMappedPos(window, event);
			checkClick(mapped_pos, event.touch.finger);
		}
		else if (event.type == sf::Event::TouchEnded){
			checkUnclick(event.touch.finger);
		}
	}
	
	sf::Vector2f getMappedPos(sf::RenderWindow& window, sf::Event event){
		return window.mapPixelToCoords(sf::Vector2i({event.touch.x, event.touch.y}));
	}
	
	void reverseColors(){
		sf::Color rc = rect.getFillColor();
		sf::Color tc = button_text.getFillColor();
		rect.setFillColor(tc);
		button_text.setFillColor(rc);
	}
	
	void resetState(bool reverse_color=true){
		touch_id = -1;
		state = NORMAL;
		if (reverse_color){
			reverseColors();
		}
	}
	
	bool is_clicked(bool reset_on_click=true){
	 bool result =  (state == CLICKED);
	 if (result && reset_on_click)
	 	resetState(false);
	 return result;
	}
	
	bool is_released(bool reset_on_release=true){
		bool result = (state == RELEASED);
		if (result && reset_on_release)
			resetState(false);
		return result;
	}
	
	void setBgColor(sf::Color color){
		rect.setFillColor(color);
	}
	
	void setTextColor(sf::Color color){
		button_text.setFillColor(color);
	}

}; //TextButton


}; //ui
