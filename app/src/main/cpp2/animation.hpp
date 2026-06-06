#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>

#include "constants.hpp"
#include "rounded_rect.hpp"
#include "color_themes.hpp"

namespace anim{

template <typename T>
void center_object_origin(T& shape){
	auto bounds = shape.getLocalBounds();
	shape.setOrigin(
		{ bounds.left + bounds.width / 2.0f,
		   bounds.top + bounds.height / 2.0f }
	);
}
	
struct CellMergeAnimation{
	sf::Vector2i to_index; //grid 2d index where animation happening (merging cells)
	sf::Text value_before_text;
	sf::Text value_after_text;
	int value_before;
	int value_after;
	sf::Vector2f pos;
	
	RoundedRectangleShape rounded_rect;
	
	float current_scale = 0.1f;
	float animation_speed = 4.0f;
	
	bool completed = false;
	
	CellMergeAnimation(sf::Vector2u screen_size, int grid_size, sf::Font& font, sf::Vector2i index, int initial_value, bool is_spawn=false){
		float WIDTH    = screen_size.x;
		float CELL_PAD = WIDTH * WIDTH_PAD_RATIO;
		float X_OFFSET = (WIDTH / LR_PAD_RATIO) + CELL_PAD/2;
		WIDTH    = WIDTH - ((X_OFFSET - CELL_PAD/2) * 2);
		//total available width is now smaller due to LR padding!
		float Y_OFFSET = (screen_size.y - WIDTH) / 2;
		//Y_OFFSET is where grid drawing begins on screen
		//it is calculated so grid is right in the middle of screen
		//X_OFFSET isnt necessarily useful since we create the grid,
		//based on the width of screen and we dont want any, offset
		//in the x direction (left of grid)
		//instead an LR padding will be implemented
		//for left-right padding to the grid. 
		//TODO: ↑
		to_index = index;
		if (is_spawn){
			value_before = 0;
			value_after = initial_value;
		}else {
			value_before = initial_value;
			value_after = value_before * 2; //since a merge means value doubles
		}
		float cell_size = (WIDTH - (CELL_PAD * grid_size)) / grid_size ;
		float target_size = cell_size * CELL_TEXT_RATIO; 
		
		value_before_text.setFont(font);
		value_before_text.setCharacterSize(target_size);
		value_before_text.setString(std::to_string(value_before));
		center_object_origin(value_before_text);
		
		value_after_text.setFont(font);
		value_after_text.setCharacterSize(target_size);
		value_after_text.setString(std::to_string(value_after));
		center_object_origin(value_after_text);
		value_after_text.setScale(current_scale, current_scale);
		
		int x = to_index.x;
		int y = to_index.y;
		pos = {
			(float)(y * (cell_size+CELL_PAD) + X_OFFSET) + cell_size/2 + CELL_PAD/2,
			(float)(x * (cell_size+CELL_PAD) + Y_OFFSET) + cell_size/2 - CELL_PAD/2
		};
		value_after_text.setPosition(pos);
		value_before_text.setPosition(pos);
		
		rounded_rect.setSize({(float)cell_size, (float) cell_size});
		rounded_rect.setRadius(10);
		center_object_origin(rounded_rect);
		rounded_rect.setPosition(pos);
	}

	void draw(sf::RenderWindow& window, Theme& theme){
		rounded_rect.setFillColor(get_color(theme, value_before, true));
		value_before_text.setFillColor(get_color(theme,  value_before, false));
		rounded_rect.setScale(1.0f, 1.0f);
		window.draw(rounded_rect);
		window.draw(value_before_text);
		
		rounded_rect.setFillColor(get_color(theme, value_after, true));
		value_after_text.setFillColor(get_color(theme, value_after, false));
		rounded_rect.setScale(current_scale, current_scale);
		window.draw(rounded_rect);
		window.draw(value_after_text);
	}
	
	void update(float dt){
		if (completed) return;
		float scale_delta = animation_speed * dt;
		current_scale += scale_delta;
		if (current_scale >= 1.0f){
			current_scale = 1.0f;
			completed = true;
		}
		value_after_text.setScale(current_scale, current_scale);
	}
	

}; //CellMergeAnimation



struct CellMoveAnimation{
	sf::Text value_text;
	sf::Vector2i direction, to_index, from_index;
	sf::Vector2f  current_pos, target_pos;
	int value;
	RoundedRectangleShape rounded_rect;
	
	float animation_speed;
	bool completed  = false;
	
	CellMoveAnimation(sf::Vector2u screen_size, int grid_size, sf::Font& font, sf::Vector2i from_indx, sf::Vector2i to_indx, int cell_value, sf::Vector2i  move_direction){
		float WIDTH    = screen_size.x;
		float CELL_PAD = WIDTH * WIDTH_PAD_RATIO;
		float X_OFFSET = (WIDTH / LR_PAD_RATIO) + CELL_PAD/2;
		WIDTH    = WIDTH - ((X_OFFSET - CELL_PAD/2) * 2);
		//total available width is now smaller due to LR padding!
		float Y_OFFSET = (screen_size.y - WIDTH) / 2;
		
		animation_speed = WIDTH*3.4; //pixels/sec
		from_index = from_indx;
		to_index = to_indx;
		value = cell_value;
		direction = move_direction ;
		
		float cell_size = (WIDTH - (CELL_PAD * grid_size)) / grid_size ;
		int x = from_index.x ;
		int y = from_index.y ;
		current_pos = {
			(float)(y * (cell_size+CELL_PAD) + X_OFFSET) + cell_size/2 + CELL_PAD/2,
			(float)(x * (cell_size+CELL_PAD) + Y_OFFSET) + cell_size/2 - CELL_PAD/2
		};
		value_text.setFont(font);
		value_text.setString(std::to_string(value));
		value_text.setCharacterSize(cell_size * CELL_TEXT_RATIO);
		center_object_origin(value_text);
		value_text.setPosition(current_pos);
		
		x = to_index.x ;
		y = to_index.y ;
		target_pos = {
			(float)(y * (cell_size+CELL_PAD) + X_OFFSET) + cell_size/2 + CELL_PAD/2,
			(float)(x * (cell_size+CELL_PAD) + Y_OFFSET) + cell_size/2
		};
		
		rounded_rect.setSize({(float)cell_size, (float) cell_size});
		rounded_rect.setRadius(10);
		center_object_origin(rounded_rect);
		rounded_rect.setPosition(current_pos);
		
	}
	
	void draw(sf::RenderWindow& window, Theme& theme){
		rounded_rect.setFillColor(get_color(theme, value, true));
		value_text.setFillColor(get_color(theme, value, false));
		window.draw(rounded_rect);
		window.draw(value_text);
	}
	
	//todo: remove repeated lines below↓
	void update(float dt){
		if (completed) return;
		current_pos.x += animation_speed * direction.x * dt;
		current_pos.y += animation_speed * direction.y * dt;
		rounded_rect.setPosition(current_pos);
		value_text.setPosition(current_pos);
		if (direction.x > 0){
			if (current_pos.x >= target_pos.x){
				value_text.setPosition(target_pos);
				rounded_rect.setPosition(target_pos);
				completed = true;
				return;
			}
		} else if (direction.x < 0){
			if (current_pos.x <= target_pos.x){
				value_text.setPosition(target_pos);
				rounded_rect.setPosition(target_pos);
				completed = true;
				return;
			}
		}
		
		if (direction.y > 0){
			if (current_pos.y >= target_pos.y){
				value_text.setPosition(target_pos);
				rounded_rect.setPosition(target_pos);
				completed = true;
				return;
			}
		} else if (direction.y < 0){
			if (current_pos.y <= target_pos.y){
				value_text.setPosition(target_pos);
				rounded_rect.setPosition(target_pos);
				completed = true;
				return;
			}
		}
	}
	
};//CellMoveAnimation;




struct AnimationManager{
	sf::Font& font;
	sf::Vector2u screen_size;
	
	std::vector<CellMergeAnimation> merge_anims;
	std::vector<CellMoveAnimation> move_anims;
	
	std::vector<sf::Vector2i> involved_indices;
	
	AnimationManager(sf::Font& ur_font, sf::Vector2u screen_dims): font(ur_font), screen_size(screen_dims){
		;
	}
	
	void add_merge_animation(int grid_size, sf::Vector2i index, int initial_value, bool is_spawn=false){ //Merge Animation
		merge_anims.emplace_back(screen_size, grid_size, font, index, initial_value, is_spawn);
		involved_indices.emplace_back(index);
	}
	
	//(int grid_size, sf::Font& font, sf::Vector2i from_indx, sf::Vector2i to_indx, int cell_value, qq::SwipeDirection direction
	void add_move_animation(int grid_size, sf::Vector2i from_indx, sf::Vector2i to_indx, int cell_value, sf::Vector2i direction){
		move_anims.emplace_back(screen_size, grid_size, font, from_indx, to_indx, cell_value, direction);
		involved_indices.emplace_back(to_indx);
		involved_indices.emplace_back(from_indx);
	}
	
	void draw_and_update(sf::RenderWindow& window, float dt, Theme& theme = classic_theme){
		if (!has_animation())
			return;
		
		for (auto& anim: merge_anims){
			anim.update(dt);
			anim.draw(window, theme);
			if (anim.completed){
				std::erase(involved_indices, anim.to_index);
			}
		}
		for (auto& anim: move_anims){
			anim.update(dt);
			anim.draw(window, theme);
			if (anim.completed){
				std::erase(involved_indices, anim.to_index);
				std::erase(involved_indices, anim.from_index);
			}
		}
		_remove_dead_anims();
	}
	
	
	bool has_animation(){
		return (merge_anims.size() + move_anims.size());
		//(0+0 = 0) if no anims otherwise (nonzero)
	}
	
	void _remove_dead_anims(){
		std::erase_if(
			merge_anims,
			[] (const CellMergeAnimation& cma){
				return cma.completed;
			}
		);
		
		std::erase_if(
			move_anims,
			[] (const CellMoveAnimation& cma){
				return cma.completed;
			}
		);
		
		/* //for older cpp standards
		std::erase(
			std::remove_if(
				merge_anims.begin(), merge_anims.end(),
				[] (const CellMergeAnimation& anim){
					return anim.completed;
				}
			),
			merge_anims.end()
		);
		*/
	}
	
	bool involves_index(sf::Vector2i index){
		return std::find(involved_indices.begin(), involved_indices.end(), index) != involved_indices.end(); 
	}
	
};//AnimationManager;


}; //namespace anim
