#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>

#include "utils.hpp"
#include "constants.hpp"
#include "helper.hpp" //namespace qq
#include "animation.hpp"
#include "rounded_rect.hpp"
#include "color_themes.hpp"
#include "ui.hpp"
#include "alt_scenes.hpp"

#define SWIPE_LEN  50
//how much long stroke is considered a swipe, 
//a swipe smaller in length than this value is not
//considered as a swipe during grid swiping!

struct Game{
    sf::RenderWindow window;
    
    sf::Font& _font;
    
	sf::Clock clock;
	utils::FpsVisualiser fps_viz;
	qq::Grid grid;
	
	anim::AnimationManager animan;
	Theme current_theme = forest_theme;
	
	RoundedRectangleShape  rounded_rect;
	sf::RectangleShape bg_rect;
	
	ui::TextButton reset_button;//
	
	sf::Vector2i last_spawn_index = {0, 0};
	int max_tile_number = 0;
	int score = 0;
	bool got2048 = false;
	bool game_over = false;
	
	int grid_size;
	int cell_size;
	
	//
	sf::Text text;
	//
	qq::SwipeState swipe_state = qq::YET_TO_SWIPE;
	sf::Vector2i  touch_down_pos, touch_up_pos = {0, 0};
	
	
	Game(sf::Font& font, int size=4): 
				window(sf::VideoMode::getDesktopMode(), "mygame"), 
				_font(font),
				fps_viz(font), 
				animan(font),
				reset_button({100, 100}, font, {100, 70}, "reset")
		{
		grid_size = size;
		//self.cell_size = (W - (CELL_PAD*self.grid_size))/self.grid_size
		cell_size = (WIDTH - (CELL_PAD * grid_size)) / grid_size ;
		window.setSize({WIDTH, HEIGHT});
		qq::init_grid(grid, grid_size, 0);
		qq::print_grid(grid);
		//set 2 initial cells
		_set_cell_and_update();
		_set_cell_and_update();
		qq::print_grid(grid);
		
		text.setFont(font);
		text.setCharacterSize(cell_size * CELL_TEXT_RATIO);
		anim::center_object_origin(text);
		
		rounded_rect.setSize({(float)cell_size, (float) cell_size});
		rounded_rect.setRadius(15);
		anim::center_object_origin(rounded_rect);
		
		bg_rect.setSize({WIDTH, WIDTH});
		bg_rect.setPosition(X_OFFSET - CELL_PAD / 2, Y_OFFSET-CELL_PAD / 2);
		bg_rect.setFillColor(sf::Color(187, 173, 160));
	}
	
	
	void mainloop(){
		
		while (window.isOpen()){
			_handle_events(); //VERY IMPORTANT!!
			float dt = clock.restart().asSeconds();
			if (!window.hasFocus()) {
				sf::sleep(sf::milliseconds(500));
				continue;
			}
			if (!game_over && is_game_over()){
				std::cout << "game over!!\n";
				qq::print_grid(grid);
				game_over = true;
			}
			if (!animan.has_animation() && 
				 !game_over && 
				 swipe_state == qq::NOT_SWIPING &&
				 utils::get_distance(touch_down_pos, touch_up_pos) >= SWIPE_LEN)
				 {
				qq::SwipeDirection swipe_dir = qq::get_swipe_direction(touch_down_pos, touch_up_pos);
				if (swipe_dir != qq::INVALID_DIRECTION){
					std::cout << "swiped : " << swipe_dir << "\n";
					qq::Grid new_grid;
					qq::init_grid(new_grid, grid_size);
					bool changed = qq::swipe_grid(grid, new_grid, swipe_dir, animan);
					if (changed) {
						grid = new_grid;
						qq::print_grid(grid);
						_set_cell_and_update();
					}
				}
				swipe_state = qq::YET_TO_SWIPE;
			}
			
			
			if (reset_button.is_released()){
				scene::askYesNo(window, _font, "Confirm reset??");
			}
			//drawing begin
			window.clear(BG_COLOR);
			_render_grid();
			
			animan.draw_and_update(window, dt, current_theme);
			
			reset_button.draw(window);
			
			if (game_over){
				utils::show_info(window, text, "Game Over!!", {100, 100}, "");
			}
			fps_viz.show_fps(window);
			//drawing end
			window.display();
		}
	}
	
	void _render_grid(){
		window.draw(bg_rect);
		for (int x = 0; x<grid_size; x++){
			for (int y=0; y< grid_size; y++){
				int value = grid[x][y];
				sf::Vector2f pos = {
				     (float)(y * (cell_size+CELL_PAD) + X_OFFSET) + cell_size/2,
				     (float)(x * (cell_size+CELL_PAD) + Y_OFFSET) + cell_size/2
				 };
				if (value && !animan.involves_index({x, y})){
					rounded_rect.setFillColor(get_color(current_theme, value, true));
					rounded_rect.setPosition(pos);
					window.draw(rounded_rect);
					text.setString(std::to_string(value));
					anim::center_object_origin(text);
					text.setPosition(pos);
					text.setFillColor(get_color(current_theme, value, false));
					window.draw(text);
				} else {
					rounded_rect.setFillColor(sf::Color::White);
					rounded_rect.setPosition(pos);
					window.draw(rounded_rect);
				}
			}
		}
	}
	
	
	void _handle_events(){
		sf::Event event;
		while (window.pollEvent(event)){
			if (event.type == sf::Event::Closed){
				window.close();
			} else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape){
				window.close();
			}else if (event.type == sf::Event::LostFocus){
            	std::cout << "App lost focus\n";
            	grid[0][0] = 2048;
            	window.setActive(false);
            } else if (event.type == sf::Event::GainedFocus){
            	std::cout << "App gained focus\n";
            	window.setActive(true);
            }
            if (event.type == sf::Event::TouchBegan || event.type == sf::Event::TouchEnded){
            	reset_button.handleTouchEvent(event, window);
	            if (event.type == sf::Event::TouchBegan){
	            	swipe_state = qq::SWIPING;
	            	touch_down_pos = {event.touch.x, event.touch.y};
	            } else if (event.type == sf::Event::TouchEnded){
	            	swipe_state = qq::NOT_SWIPING;
	            	touch_up_pos = {event.touch.x, event.touch.y};
	            }
            }
		} //event loop ends
	}

	void _set_cell_and_update(){
		std::vector<sf::Vector2i> empty_indices ;
		for (int x = 0; x < grid_size; x++){
			for (int y = 0; y < grid_size; y++){
				int value = grid[x][y];
				score += value;
				if (value == 0){
					empty_indices.emplace_back(x, y);
				}
				if (value > max_tile_number){
					max_tile_number = value;
				}
				if (value == 2048){
					got2048 = true;
				}
			}
		}
		if (empty_indices.size() > 0){
			sf::Vector2i empty_cell = utils::rng.randChoice(empty_indices);
			int n = (utils::rng.randFloat() < 0.9) ? 2 : 4;
			grid[empty_cell.x][empty_cell.y] = n;
			bool is_spawn = true;
			animan.add_merge_animation(grid_size, empty_cell, n, is_spawn);
			score += n;
		}
	}
	
	
	bool is_game_over(){
		for (int x=0; x<grid_size; x++){
			for (int y=0; y<grid_size; y++){
				int value = grid[x][y];
				if (value == 0) return false;
				//check right
				if (y+1 < grid_size && grid[x][y+1] == value) return false;
				//check down
				if (x+1 < grid_size && grid[x+1][y] == value) return false;
			}
		}
		return true;
	}
	
	
};//Game
