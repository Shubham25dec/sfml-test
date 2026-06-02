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
			std::cout << "Error: randChoice list cannot have size 0\n";
			std::abort();
		}
		std::uniform_int_distribution<> dist(0, list.size()-1);
		int index = dist(gen);
		return list.at(index);
	}
	char randChoice(std::string str){
		if (str.size() == 0){
			std::cout << "Error: randChoice string cannot have size 0\n";
			std::abort();
		}
		std::uniform_int_distribution<> dist(0, str.length()-1);
		int index = dist(gen);
		return str[index];
	}

};

RNG rng; //Globally accessible ready-to-use rng


struct Profiler {
    std::chrono::high_resolution_clock::time_point last_time;

    void begin_measure(std::string info="") {
        last_time = std::chrono::high_resolution_clock::now();
        std::cout << info << std::endl;
    }

    void end_measure(std::string info="") {
        auto now = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(now - last_time).count();
        std::cout << info << "{\n\tTime taken: " << elapsed << " microseconds\n}\n";
        last_time = now; // reset for next measurement
    }
};


inline std::string get_trimmed_string(const std::string& str, uint trim_from){
	std::string copy = str;
	copy.erase(trim_from);
	return copy;
}



std::string get_filename_no_ext(const std::string& fullPath) {
    // Find the last path separator
    size_t lastSlash = fullPath.find_last_of("/\\");
    std::string fileName = (lastSlash == std::string::npos) ? fullPath : fullPath.substr(lastSlash + 1);

    // Find the last dot for extension
    size_t lastDot = fileName.find_last_of('.');
    if (lastDot == std::string::npos) {
        return fileName; // no extension
    }
    return fileName.substr(0, lastDot);
}


inline std::string get_basename(const std::string& full_path){
	std::filesystem::path p(full_path);
	std::string basename = p.filename().string();
	return basename;
}


inline std::vector<std::string> list_dir(const std::string& path, bool files_only=false) {
    std::vector<std::string> filePaths;
    for (const auto& entry : std::filesystem::directory_iterator(path)) {
        if (!files_only){
        	filePaths.push_back(entry.path().string());
        	continue;
        }
         if (entry.is_regular_file()) {
            filePaths.push_back(entry.path().string());
        }
    }
    return filePaths;
}


inline sf::Font load_font(std::string path){
	sf::Font font;
		if (!(font.loadFromFile(path)))
	{
		std::cout << ("Couldn't load requested font: ")<<path<<"\n";
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


//inline void show_fps(sf::RenderWindow& window, sf::Clock& clock, sf::Text& text, float posx=10.0f, float posy=50.0f){
//	int fps = 1.0f / clock.restart().asSeconds();
//	text.setString("fps: " + std::to_string(fps));
//	text.setCharacterSize(30);
//	text.setPosition(posx, posy);
//	window.draw(text);
//}


inline void show_info(sf::RenderWindow& window, sf::Text& text, std::string info, sf::Vector2f pos={10, 10}, std::string prefix="Info"){
	text.setString(prefix + info);
	text.setPosition(pos);
	window.draw(text);
}

 
inline void draw_line(sf::RenderWindow &window, sf::Vector2f start, sf::Vector2f end, sf::Color color=sf::Color::White){
    sf::Vertex line[] =
    {
        sf::Vertex(start, color),
        sf::Vertex(end, color)
    };
    window.draw(line, 2, sf::Lines);
}


inline void draw_sprite_bounds(const sf::Sprite& sprite, sf::RenderWindow& window, sf::Color color=sf::Color::Red, bool draw_circle=true) {
	    sf::FloatRect bounds = sprite.getGlobalBounds();
	
	    sf::RectangleShape boundingRect;
	    boundingRect.setPosition(bounds.left, bounds.top);
	    boundingRect.setSize(sf::Vector2f(bounds.width, bounds.height));
	    boundingRect.setFillColor(sf::Color::Transparent);
	    boundingRect.setOutlineColor(color);
	    boundingRect.setOutlineThickness(1.f);
	    
	    if (draw_circle){
	    	sf::CircleShape circle;
	    	float larger = bounds.width;//(bounds.width > bounds.height)? bounds.width : bounds.height;
	    	float radius = larger/2;
	    	circle.setRadius(radius);
	    	circle.setOrigin(radius, radius);
	    	circle.setPosition(bounds.left+bounds.width/2, bounds.top+bounds.height/2);
	    	circle.setFillColor(sf::Color:: Transparent);
	    	circle.setOutlineThickness(3);
	    	circle.setOutlineColor(color);
	    	window.draw(circle);
	    }
	
	    window.draw(boundingRect);
	    
}


inline void draw_rect(const sf::FloatRect rect, sf::RenderWindow& window, sf::Color color=sf::Color::Red){
	    sf::RectangleShape boundingRect;
	    boundingRect.setPosition(rect.left, rect.top);
	    boundingRect.setSize(sf::Vector2f(rect.width, rect.height));
	    boundingRect.setFillColor(sf::Color::Transparent);
	    boundingRect.setOutlineColor(color);
	    boundingRect.setOutlineThickness(2.f);
	
	    window.draw(boundingRect);
}

inline void draw_circle(
				sf::RenderWindow& window,
				sf::Vector2f center,
				float radius,
				sf::Color color=sf::Color::White){
	sf::CircleShape circle(radius);
	circle.setOrigin(radius, radius);
	circle.setPosition(center);
	circle.setFillColor(color);
	window.draw(circle);
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
	sf::Font font;
	sf::Text text;
	sf::Clock fps_clock;
	sf::Clock delay_clock;
	float update_delay = 0.25; //sec
	int fps;
	int numerators=0; // all fps sum between delay
	int  count=0; //no of calls between delay
	// calculates avg fps between delay.
	
	FpsVisualiser(std::string font_path, int text_size=30){
		font = load_font(font_path);
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
