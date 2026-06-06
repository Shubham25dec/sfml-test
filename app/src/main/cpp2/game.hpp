#pragma once

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
    sf::RenderWindow& window;
    sf::Font& font;
	sf::Clock& clock;
	sf::Sound& move_sound;
	
	anim::AnimationManager& animan;

	qq::Grid grid;
	unsigned int theme_index;
	Theme current_theme;
	
	RoundedRectangleShape rounded_rect;//used to draw cell
	RoundedRectangleShape bg_rect;
	
	ui::TextButton& reset_button;//
	ui::TextButton& theme_button;
	
	
	//sf::Vector2i last_spawn_index = {0, 0};
	//int max_tile_number = 0;
	int score      = 0;
	int high_score = 0;
	bool got2048   = false;
	bool game_over = false;
	
	float WIDTH;
	float X_OFFSET; //Left padding for grid; 
	//the width of screen is adjusted to this padding automatically
	//appears for right side too.
	float Y_OFFSET;
	float CELL_PAD;
	
	int grid_size;
	float cell_size;
	
	//
	sf::Text number_text; //text used for nums
	sf::Text score_text; //text for score
	sf::Text high_score_text;
	sf::Text game_status_text; //text for win and over
	//
	qq::SwipeState swipe_state = qq::YET_TO_SWIPE;
	qq::HS_Container high_score_container;
	
	sf::Vector2i  touch_down_pos, touch_up_pos = {0, 0};
	
	
	Game(sf::RenderWindow& a_window,			
		 sf::Font& a_font,
		 sf::Clock& a_clock,
		 sf::Sound& a_move_sound,
		 anim::AnimationManager& a_animan,
		 ui::TextButton& a_theme_btn,
		 ui::TextButton& a_reset_btn,
		 qq::GridKind grid_kind = qq::FOUR_BY_FOUR
		 ): 
				window(a_window), 
				font(a_font),
				clock(a_clock),
				move_sound(a_move_sound),
				animan(a_animan),
				reset_button(a_reset_btn),
				theme_button(a_theme_btn)
		{
		grid_size = qq::grid_size_from_kind(grid_kind);
		sf::Vector2u screen_size = window.getSize();
		WIDTH    = screen_size.x;
		CELL_PAD = WIDTH * WIDTH_PAD_RATIO;
		X_OFFSET = (WIDTH / LR_PAD_RATIO) + CELL_PAD/2;
		WIDTH    = WIDTH - ((X_OFFSET - CELL_PAD/2) * 2); //total available width is now smaller due to LR padding (X_OFFSET)!
		Y_OFFSET = (screen_size.y - WIDTH) / 2;
		//y_offset is such that grid fits in middle of screen
	
		cell_size = (WIDTH - (CELL_PAD * grid_size)) / grid_size ;
		qq::init_grid(grid, grid_size, 0);
		//set 2 initial cells
		if (! _load_saved_game()){
			_set_cell_and_update();
			_set_cell_and_update();
		}else{ //game was loaded
			_set_cell_and_update(true); //true = update onl
		}
		
		theme_index = utils::rng.randInt(0, all_themes.size()-1);
		current_theme = all_themes[theme_index];
		
		number_text.setFont(font);
		number_text.setCharacterSize(cell_size * CELL_TEXT_RATIO);
		anim::center_object_origin(number_text);

		high_score_text.setFont(font);
		high_score_text.setCharacterSize(WIDTH/24);
		high_score_text.setString("High Score: " + std::to_string(high_score));
		high_score_text.setPosition({20, 60});
		
		score_text.setFont(font);
		score_text.setCharacterSize(WIDTH/24);
		score_text.setString("Score: 0");
		auto hst_bound = high_score_text.getGlobalBounds();
		score_text.setPosition({20, hst_bound.top + hst_bound.height + 20});
		
		game_status_text.setFont(font);
		game_status_text.setCharacterSize(WIDTH/18);
		game_status_text.setString("Acieved 2048");
		anim::center_object_origin(game_status_text);
		auto b = game_status_text.getGlobalBounds();
		sf::Vector2f pos = {(float)screen_size.x/2, Y_OFFSET - b.height * 2};
		game_status_text.setPosition(pos);
		
		
		rounded_rect.setSize({(float)cell_size, (float) cell_size});
		rounded_rect.setRadius(10);
		anim::center_object_origin(rounded_rect);
		
		bg_rect.setRadius(10);
		float W  = (cell_size + CELL_PAD) * grid_size + (CELL_PAD*2);
		bg_rect.setSize({W, W});
		float posX = (X_OFFSET - CELL_PAD);
		float posY = Y_OFFSET - (X_OFFSET - CELL_PAD)/2.3;
		bg_rect.setPosition({posX, posY});
		bg_rect.setFillColor(sf::Color(187, 173, 160));
		
		reset_button.setBgColor(sf::Color(113, 49, 209));
		theme_button.setBgColor(get_color(current_theme, 256));
		theme_button.setTextColor(get_color(current_theme, 256, false));
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
				//std::cout << "game over!!\n";
				//qq::print_grid(grid);
				game_over = true;
			}
			if (!animan.has_animation() && 
				 !game_over && 
				 swipe_state == qq::NOT_SWIPING &&
				 utils::get_distance(touch_down_pos, touch_up_pos) >= SWIPE_LEN)
				 {
				qq::SwipeDirection swipe_dir = qq::get_swipe_direction(touch_down_pos, touch_up_pos);
				if (swipe_dir != qq::INVALID_DIRECTION){
					//std::cout << "swiped : " << swipe_dir << "\n";
					qq::Grid new_grid;
					qq::init_grid(new_grid, grid_size);
					bool changed = qq::swipe_grid(grid, new_grid, swipe_dir, animan);
					if (changed) {
						move_sound.play();
						grid = new_grid;
						//qq::print_grid(grid);
						_set_cell_and_update();
					}
				}
				swipe_state = qq::YET_TO_SWIPE;
			}
			
			
			if (reset_button.is_released()){
				if (scene::askYesNo(window, font, "Confirm reset??") == scene::YES){
					qq::init_grid(grid, grid_size, 0);
					game_over = false;
					_set_cell_and_update();
					_set_cell_and_update();
				}
			}
			if (theme_button.is_released()){
				theme_index += 1;
				if (theme_index >= all_themes.size()){
					theme_index = 0;
				}
				current_theme = all_themes[theme_index];
				theme_button.setBgColor(get_color(current_theme, 256));
				theme_button.setTextColor(get_color(current_theme, 256, false));
			}
			
			//drawing begin
			window.clear(BG_COLOR);
			_render_grid();

			window.draw(high_score_text);
			window.draw(score_text);
			
			if (got2048) window.draw(game_status_text);
			
			animan.draw_and_update(window, dt, current_theme);
			
			reset_button.draw(window);
			theme_button.draw(window);
			
			if (game_over){
				game_status_text.setString(" Game Over! ");
				window.draw(game_status_text);
			}
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
				     (float)(y * (cell_size+CELL_PAD) + X_OFFSET) + cell_size/2 + CELL_PAD/2,
				     (float)(x * (cell_size+CELL_PAD) + Y_OFFSET) + cell_size/2 - CELL_PAD/2
				 };
				 
				if (value && !animan.involves_index({x, y})){
					rounded_rect.setFillColor(get_color(current_theme, value, true));
					rounded_rect.setPosition(pos);
					window.draw(rounded_rect);
					number_text.setString(std::to_string(value));
					anim::center_object_origin(number_text);
					number_text.setPosition(pos);
					number_text.setFillColor(get_color(current_theme, value, false));
					window.draw(number_text);
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
				_save_game();
				std::exit(0);
			} else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape){
				window.close();
				_save_game();
				std::exit(0);
			}else if (event.type == sf::Event::LostFocus){
            	std::cout << "App lost focus\n";
            	window.setActive(false);
            } else if (event.type == sf::Event::GainedFocus){
            	std::cout << "App gained focus\n";
            	window.setActive(true);
            }
            if (event.type == sf::Event::TouchBegan || event.type == sf::Event::TouchEnded){
            	reset_button.handleTouchEvent(event, window);
            	theme_button.handleTouchEvent(event, window);
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

	void _set_cell_and_update(bool update_only=false){
		score = 0;
		//max_tile_number = 0;
		got2048 = false;
		std::vector<sf::Vector2i> empty_indices ;
		for (int x = 0; x < grid_size; x++){
			for (int y = 0; y < grid_size; y++){
				int value = grid[x][y];
				score += value;
				if (value == 0){
					empty_indices.emplace_back(x, y);
				}
				//if (value > max_tile_number){
				//	max_tile_number = value;
				//}
				if (value == 2048){
					got2048 = true;
				}
			}
		}
		if (update_only) {
			score_text.setString("Score: " + std::to_string(score));
			return;
		}
		
		if (empty_indices.size() > 0){
			sf::Vector2i empty_cell = utils::rng.randChoice(empty_indices);
			int n = (utils::rng.randFloat() < 0.9) ? 2 : 4;
			grid[empty_cell.x][empty_cell.y] = n;
			bool is_spawn = true;
			animan.add_merge_animation(grid_size, empty_cell, n, is_spawn);
			score += n;
		}
		score_text.setString("Score: " + std::to_string(score));
		if (score > high_score){
			high_score = score;
			high_score_text.setString("High Score: " + std::to_string(high_score));
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

	bool _save_game(){
		switch (grid_size){
			case 4: high_score_container.four = high_score;  break;
			case 5: high_score_container.five = high_score;  break;
			case 6: high_score_container.six  = high_score;   break;
			case 8: high_score_container.eight= high_score; break;
		}
		qq::write_high_scores_to_file(high_score_container);
		std::string filename = qq::filename_for_grid_kind(qq::grid_kind_from_size(grid_size));
		return qq::write_grid_to_file(grid, filename);
	}

	bool _load_saved_game(){
		qq::read_high_scores_from_file(high_score_container);
		_set_high_score_from_container();
		high_score_text.setString("High Score: " + std::to_string(high_score));
		std::string filename = qq::filename_for_grid_kind(qq::grid_kind_from_size(grid_size));
		return qq::read_grid_from_file(grid, filename);
	}

	void _set_high_score_from_container(){
		switch (grid_size){
			case 4: high_score = high_score_container.four;  return;
			case 5: high_score = high_score_container.five;  return;
			case 6: high_score = high_score_container.six;   return;
			case 8: high_score = high_score_container.eight; return;
		}
		high_score = 0;
	}
};//Game
