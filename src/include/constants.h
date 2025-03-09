#ifndef constants_h_
#define constants_h_

#define WINDOW_TITLE   "Tetris game - [Bài tập lớn Lập trình nâng cao]"
#define WINDOW_WIDTH   800  // width of the main window
#define WINDOW_HEIGHT  600  // height of the main window
#define DISPLAY_SCALE  1.00f  

#define BACKGROUND_POS_X  0  // top left X-coordinate of the main background
#define BACKGROUND_POS_Y  0  // top left Y-coordinate of the main background

#define MATRIX_WIDTH   272                                    // width of the matrix
#define MATRIX_HEIGHT  534                                    // height of the matrix
#define MATRIX_POS_X   ((WINDOW_WIDTH - MATRIX_WIDTH) / 2)    // top left X-coordinate of the matrix
#define MATRIX_POS_Y   ((WINDOW_HEIGHT - MATRIX_HEIGHT) / 2)  // top left Y-coordinate of the matrix

#define CELL_SIZE           25                  // size of a cell in the matrix (contains tetrominoes)
#define CELL_POS_X_INIT     (MATRIX_POS_X + 6)  // initial top left X-coordinate of the top left cell
#define CELL_POS_Y_INIT     (MATRIX_POS_Y + 7)  // initial top left Y-coordinate of the top left cell
#define CELL_POS_INCREMENT  (CELL_SIZE + 1)     // distance between top left points of two consecutive cells

// path to images
#define IMAGES_PATH  "./src/images/"
#define BACKGROUND_PNG  IMAGES_PATH "main-background.png"
#define LOGO_PNG        IMAGES_PATH "main_logo.png"
#define MATRIX_PNG      IMAGES_PATH "matrix.png"
#define DYING_PATH   IMAGES_PATH "dying/"
#define GHOST_PATH   IMAGES_PATH "ghost/"
#define LOCKED_PATH  IMAGES_PATH "locked/"
#define NORMAL_PATH  IMAGES_PATH "normal/"
#define I_PNG  "I.png"
#define J_PNG  "J.png"
#define L_PNG  "L.png"
#define O_PNG  "O.png"
#define S_PNG  "S.png"
#define T_PNG  "T.png"
#define Z_PNG  "Z.png"

#endif  // constants_h_