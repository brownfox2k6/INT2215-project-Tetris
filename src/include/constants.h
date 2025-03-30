#ifndef constants_h_
#define constants_h_

// parameters for application initialization
#define WINDOW_TITLE   "Tetris game - [Bài tập lớn Lập trình nâng cao]"
#define WINDOW_WIDTH   800  // width of the main window
#define WINDOW_HEIGHT  600  // height of the main window
#define DISPLAY_SCALE  1.00f  

// parameters for displaying background
#define BACKGROUND_POS_X  0  // top-left X-coordinate of the main background
#define BACKGROUND_POS_Y  0  // top-left Y-coordinate of the main background

// parameters for displaying matrix
#define MATRIX_POS_X        264  // top-left X-coordinate of the matrix
#define MATRIX_POS_Y        33   // top-left Y-coordinate of the matrix
#define CELL_POS_X_INIT     270  // initial top-left X-coordinate of the top-left cell
#define CELL_POS_Y_INIT     40   // initial top-left Y-coordinate of the top-left cell
#define CELL_POS_INCREMENT  26   // distance between two top-left points of two consecutive cells

// for displaying NEXT and HOLD section
#define QUEUE_POS_X       600  // top-left X-coordinate of the NEXT section
#define QUEUE_POS_Y_INIT  105  // initial top-left Y-coordinate of the NEXT section
#define HOLD_POS_X        84   // top-left X-coordinate of the HOLD section
#define HOLD_POS_Y        105  // top-left Y-coordinate of the HOLD section
#define BLOCK_WIDTH       115  // width of a Tetromino image
#define BLOCK_HEIGHT      68   // height of a Tetromino image

// path to images
#define IMAGES_PATH  "./src/images/"
#define BACKGROUND_PNG  IMAGES_PATH "main-background.png"
#define LOGO_PNG        IMAGES_PATH "main-logo.png"
#define MATRIX_PNG      IMAGES_PATH "matrix.png"
#define DYING_PATH   IMAGES_PATH "dying/"
#define GHOST_PATH   IMAGES_PATH "ghost/"
#define LOCKED_PATH  IMAGES_PATH "locked/"
#define NORMAL_PATH  IMAGES_PATH "normal/"
#define WHOLE_PATH   IMAGES_PATH "whole/"
#define I_PNG  "I.png"
#define J_PNG  "J.png"
#define L_PNG  "L.png"
#define O_PNG  "O.png"
#define S_PNG  "S.png"
#define T_PNG  "T.png"
#define Z_PNG  "Z.png"

// path to fonts
#define FONTS_PATH  "./src/fonts/"

// define colors
#define COLOR_BLACK  0,   0,   0
#define COLOR_WHITE  255, 255, 255
#define COLOR_GREEN  0,   255, 0
#define COLOR_BLUE   0,   0,   255

#endif  // constants_h_