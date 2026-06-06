#pragma once
#include <string>

#define WIDTH_PAD_RATIO  1/100
//ratio of screen width to the padding b/w cells
//so if width is W cell padding would be W/100


#define LR_PAD_RATIO 1/20
//left right padding
//ratio of screen width to the grid's left and right padding.
//for more padding multiply above constant by a +ve int.

#define CELL_TEXT_RATIO  1/3 
//ratio of text size (numbers text) to the cell size
//if cell_size is S the size of text would be S/3

const std::string HIGH_SCORE_FILENAME = "high_scores.txt";
