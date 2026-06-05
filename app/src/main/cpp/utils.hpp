#pragma once


#include <iostream>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include <filesystem>
#include <vector>
#include <string>
#include <random>
#include <cmath>
#include <chrono>



namespace utils{
struct RNG{
	std::mt19937 gen; //rn generator

	RNG(){
		gen.seed(static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count()));
	}
	
	int randInt(int min, int max){
		std::uniform_int_distribution<> dist(min, max); 
		int randomNumber = dist(gen);
		return randomNumber;
	}
	
	float randFloat(float min=0.0f, float max=1.0f){ //including both
		std::uniform_real_distribution<float> dist(min, max);
		float randomFloat = dist(gen);
		return randomFloat;
	}
	
	template <typename T> T randChoice(std::vector<T>& list){
		if (list.size() == 0){
			std::cerr << "Error: randChoice list cannot have size 0\n";
			std::abort();
		}
		std::uniform_int_distribution<> dist(0, list.size()-1);
		int index = dist(gen);
		return list.at(index);
	}
	char randChoice(std::string str){
		if (str.size() == 0){
			std::cerr << "Error: randChoice string cannot have size 0\n";
			std::abort();
		}
		std::uniform_int_distribution<> dist(0, str.length()-1);
		int index = dist(gen);
		return str[index];
	}

};

RNG rng; //Globally accessible ready-to-use rng


inline sf::Font load_font(std::string path){
	sf::Font font;
		if (!(font.loadFromFile(path)))
	{
		std::cerr << ("Couldn't load requested font: ")<<path<<"\n";
		exit(69);
	}
	return font;
}


inline sf::Text  get_render_text(sf::Font &font, int size=30, sf::Color color = sf::Color::White){
	sf::Text text;
	text.setFont(font);
	text.setCharacterSize(size);
	text.setFillColor(color);
	return text;
}


inline void show_info(sf::RenderWindow& window, sf::Text& text, std::string info, sf::Vector2f pos={10, 10}, std::string prefix="Info"){
	text.setString(prefix + info);
	text.setPosition(pos);
	window.draw(text);
}


inline float get_distance(const sf::Vector2f& A, const sf::Vector2f& B) {
	    float dx = B.x - A.x;
	    float dy = B.y - A.y;
	    return std::sqrt(dx * dx + dy * dy);
}

inline int get_distance(const sf::Vector2i& A, const sf::Vector2i& B){
	    float dx = B.x - A.x;
	    float dy = B.y - A.y;
	    return std::sqrt(dx * dx + dy * dy);
}

//TODO: create an independent struct for drawing text on screen

struct FpsVisualiser{
	sf::Text text;
	sf::Clock fps_clock;
	sf::Clock delay_clock;
	float update_delay = 0.25; //sec
	int fps;
	int numerators=0; // all fps sum between delay
	int  count=0; //no of calls between delay
	// calculates avg fps between delay.
	
	FpsVisualiser(sf::Font& font, int text_size=30){
		text = get_render_text(font);
		text.setCharacterSize(text_size);
	}
	
	void show_fps(sf::RenderWindow& window, sf::Vector2f pos={20, 40}, float rotation=0){
		int current_fps = 1.0f/fps_clock.restart().asSeconds();
		numerators+= current_fps;
		count +=1;
		if (delay_clock.getElapsedTime().asSeconds() > update_delay){
			fps = numerators/count; //current_fps average;
			numerators=0;
			count = 0;
			delay_clock.restart();
		}
		text.setPosition(pos);
		text.setRotation(rotation);
		text.setString("fps: " + std::to_string(fps));
		window.draw(text);
	}

};


}//namespace utils
