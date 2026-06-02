#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <math.h>
#include <iostream>

#include "animation.hpp"


namespace qq{

typedef  std::vector<std::vector<int>> Grid;

enum SwipeDirection{
	LEFT,
	RIGHT,
	UP,
	DOWN,
	INVALID_DIRECTION
};

enum SwipeState{
	SWIPING, //user swiping touch
	NOT_SWIPING, //user has swiped before but isn't swiping rn
	YET_TO_SWIPE //user never swiped touch so touch_pos variables not initialised yet
};

SwipeDirection  get_swipe_direction(sf::Vector2i touch_begin, sf::Vector2i touch_end){
	float dx, dy;
	dx = touch_begin.x - touch_end.x ;
	dy = touch_begin.y - touch_end.y ;
	
	if (std::abs(dx) > std::abs(dy)){ //horizontal swipe
		return  dx > 0 ? LEFT : RIGHT;
	}
	if (std::abs(dy) > std::abs(dx)){ //vertical swipe
	   return dy > 0 ? UP : DOWN;
	}
	return INVALID_DIRECTION;
}

void init_grid(Grid& grid, int size, int value=0){
	std::vector<int> row ;
	row.assign(size, value); // fill <size> <value>s in row
	grid.assign(size, row);
}



//TODO: TEMPORARY REMOVE LATER
void print_grid(const Grid& grid){
	std::cout << "grid:\n";
	for (auto row: grid){
		for (auto col: row){
			std::cout << col << ", ";
		}
		std::cout << "\n";
	}
}


#include <cassert>
sf:: Vector2i direction_to_vector(SwipeDirection direction){
	switch (direction){
		case UP:
			return {0, -1};
		case DOWN:
			return {0, 1};
		case LEFT:
		 	return {-1, 0};
		 case RIGHT:
		 	return {1, 0};
		 case INVALID_DIRECTION:
		 	assert(false && "Inavlid direction");
	}
}

sf::Vector2i _index_map_helper(SwipeDirection direction, int size, int r, int c){
	switch (direction){
		case LEFT : return {r, c} ; break;
		case RIGHT : return {r, size-1-c }; break;
		case UP : return {c, r}; break ;
		case DOWN : return {size-1-c, r}; break;
		case INVALID_DIRECTION : 
		    assert(false && "Invalid Direction");
	}
	assert(false && "Unreachable");
}

bool swipe_grid(Grid& grid, Grid& new_grid, SwipeDirection direction, anim::AnimationManager& animan){
     //returns if there was a change in grid
    assert(grid.size() == new_grid.size() && "grid sizes must be same!");
    bool changed = false;
	int size = grid.size();
	for (int outer = 0; outer < size; outer++){
		int target = 0; //next free spot along 'left'
		int last_merge_col = -1 ; //where a merge happened in this line
		for (int inner=0; inner < size; inner++){
			sf::Vector2i rc = _index_map_helper(direction, size, outer, inner) ;
			int value = grid[rc.x][rc.y];
			if (value == 0) continue;
			//coordinates if we placed/merged at new_grid[outer][target]
			sf::Vector2i nrc = _index_map_helper(direction, size, outer, target) ;
			//check for merge possibility with previous placed tile
			sf::Vector2i prc = (target > 0 ) ? _index_map_helper(direction, size, outer, target - 1) : sf::Vector2i(-69 ,-69) ;
			bool can_merge = (
				(target > 0) && 
				(new_grid[prc.x][prc.y] == value) &&
				(last_merge_col != (target - 1))
			);
			if (can_merge){
					//can_merge = true;
					new_grid[prc.x][prc.y] *= 2;
					changed = true;
					animan.add_merge_animation(size,  prc, value);
					last_merge_col = target - 1;
			} else { // merge not possible
				    if (rc.x != nrc.x || rc.y != nrc.y){ //move
				    //(int grid_size, sf::Font& font, sf::Vector2i from_indx, sf::Vector2i to_indx, int cell_value, qq::SwipeDirection direction
				        animan.add_move_animation(size, rc, nrc, value, direction_to_vector(direction));
				    	changed = true;
				    }
					new_grid[nrc.x][nrc.y] = value;
					target ++;
			}
		}
	}
	return changed;
}



} ;// namespace qq
