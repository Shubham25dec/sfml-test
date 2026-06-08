#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <math.h>
#include <fstream>
#include <iostream>

#include "animation.hpp"


namespace qq{

typedef std::vector<std::vector<int>> Grid;

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

enum GridKind{
	FOUR_BY_FOUR, //4x4
	FIVE_BY_FIVE, //5x5
	SIX_BY_SIX,  //6x6
	EIGHT_BY_EIGHT //8x8
};

int grid_size_from_kind(const GridKind& kind){
	switch (kind){
		case FOUR_BY_FOUR:
		    return 4;
		case FIVE_BY_FIVE:
			return 5;
		case SIX_BY_SIX:
			return 6;
		case EIGHT_BY_EIGHT:
			return 8;
	}
}

GridKind grid_kind_from_size(int grid_size){
	switch (grid_size){
		case 4: return FOUR_BY_FOUR;
		case 5: return FIVE_BY_FIVE;
		case 6: return SIX_BY_SIX;
		case 8: return EIGHT_BY_EIGHT;
	}
	return FOUR_BY_FOUR; //in case grid_size is something else
}

std::string filepath_for_grid_kind(GridKind kind){
	switch (kind){
		case FOUR_BY_FOUR:
		    return SAVE_DIR + "4.grid";
		case FIVE_BY_FIVE:
			return SAVE_DIR + "5.grid";
		case SIX_BY_SIX:
			return SAVE_DIR + "6.grid";
		case EIGHT_BY_EIGHT:
			return SAVE_DIR + "7.grid";
	}
}

bool file_good(const std::string& name) {
    std::ifstream f(name.c_str());
    return f.good();
    //file exists and accessible
}

bool is_number(const std::string& str) {
    return !str.empty() && std::all_of(str.begin(), str.end(), [](unsigned char c) {
        return std::isdigit(c);
    });
}

int get_row(int index, int grid_size){
	return (int) (index / grid_size);
}
int get_col(int index, int grid_size){
	return (int) (index % grid_size);
}

bool write_grid_to_file(const Grid& grid){
	const std::string filepath = filepath_for_grid_kind(grid_kind_from_size(grid.size()));
	std::ofstream outfile(filepath, std::ios::binary);
	if (!outfile){
		std::cerr << "E: Could not open file: " << filepath << std::endl;
		return false;
	}
	outfile << grid.size() << " ";
	for (auto row: grid){
		for (int value: row){
			outfile << value << " ";
		}
	}
	outfile.close();
	return true;
}

bool read_grid_from_file(Grid& grid){
	const std::string filepath = filepath_for_grid_kind(grid_kind_from_size(grid.size()));
	
    std::ifstream infile(filepath);
    if (!infile){
    	std::cerr << "E: Could not open file: " << filepath << std::endl;
    	return false;
    }
    int grid_size = -1;
    std::string word;
    int index = 0;
    while (infile >> word){
        //std::cout << word << std::endl;
    	if (is_number(word)){
    		if (grid_size == -1){
    			grid_size = std::stoi(word);
    			if (grid_size != (int) grid.size()){
    				std::cerr << "E: grid size does not match the one from file" << std::endl;
    				return false;
    			}
    		}else{
    		    int row = get_row(index, grid_size);
    		    int col = get_col(index, grid_size);
    		    int val = std::stoi(word);
    			grid[row][col] = val ;
    			index++ ;
    		}
    	}else{
    		std::cerr << "W: contains non-digit token: " << word << std::endl;
    		return false;
    	}
    }
    return true;
}


struct HS_Container{
	int four = 0;
	int five = 0;
	int six  = 0;
	int eight= 0;
}; //high score container

bool read_high_scores_from_file(HS_Container& hs_container, const std::string& filename = HIGH_SCORE_FILEPATH){
	if (!file_good(filename)){
		std::cerr << "E: file not found or permission denied: " << filename << std::endl;
		return false;
	}
	std::ifstream infile(filename);
	if (!infile){
		std::cerr << "E: Failed to open file for reading: " << filename << std::endl;
		return false;
	}
	size_t index = 0;
	std::string line;
	while (std::getline(infile, line, ',')){
		if (line.size() > 0 && is_number(line)){
			if (index == 0){
				hs_container.four = std::stoi(line);
			}else if (index == 1){
				hs_container.five = std::stoi(line);
			}else if (index == 2){
				hs_container.six = std::stoi(line);
			}else if (index == 3){
				hs_container.eight = std::stoi(line);
			}else {
				break;
			}
			index++ ;
		}
	}
	infile.close();
	return (index > 0)? true : false;
}

bool write_high_scores_to_file(const HS_Container& hs_container, const std::string& filename = HIGH_SCORE_FILEPATH){
	std::ofstream outfile(filename);
	if (!outfile){
		std::cerr << "E: Failed to open file for writing" << filename << std::endl;
		return false;
	}
	outfile << hs_container.four  << ",";
	outfile << hs_container.five  << ",";
	outfile << hs_container.six   << ",";
	outfile << hs_container.eight << ",";
	outfile.close();
	return true;
}


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



/*
void print_grid(const Grid& grid){
	std::cout << "grid:\n";
	for (auto row: grid){
		for (auto col: row){
			std::cout << col << ", ";
		}
		std::cout << "\n";
	}
}
*/


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
		 	return {0, 0}; //assert(false && "Inavlid direction");
	}
}

sf::Vector2i _index_map_helper(SwipeDirection direction, int size, int r, int c){
	switch (direction){
		case LEFT : return {r, c} ; break;
		case RIGHT : return {r, size-1-c }; break;
		case UP : return {c, r}; break ;
		case DOWN : return {size-1-c, r}; break;
		case INVALID_DIRECTION :
			return {r, c};
		    //assert(false && "Invalid Direction");
	}
}

bool swipe_grid(Grid& grid, Grid& new_grid, SwipeDirection direction, anim::AnimationManager& animan){
     //returns if there was a change in grid
    //assert(grid.size() == new_grid.size() && "grid sizes must be same!");
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
