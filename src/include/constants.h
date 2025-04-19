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

// path to audios
#define AUDIOS_PATH         "./src/audios/"
#define AUDIO_BGM_1         "Korobeiniki-BR-01.wav"
#define AUDIO_BGM_2         "Korobeiniki-CN-01.wav"
#define AUDIO_BGM_3         "Korobeiniki-FVR-01.wav"
#define AUDIO_BLOCKOUT      "blockout.wav"
#define AUDIO_BACK_TO_BACK  "backToBackTetris.wav"
#define AUDIO_COLLAPSE      "collapse.wav"
#define AUDIO_HARD_DROP     "hardDrop.wav"
#define AUDIO_HOLD          "hold.wav"
#define AUDIO_INPUT_FAILED  "inputFailed.wav"
#define AUDIO_LEVEL_UP      "levelUp.wav"
#define AUDIO_LINE_CLEAR    "lineClear.wav"
#define AUDIO_LOCK          "lock.wav"
#define AUDIO_MOVE          "move.wav"
#define AUDIO_ROTATE        "rotate.wav"
#define AUDIO_TETRIS        "tetris.wav"
#define AUDIO_WIN           "win.wav"

// define colors
#define COLOR_BLACK  0,   0,   0
#define COLOR_WHITE  255, 255, 255
#define COLOR_GREEN  0,   255, 0
#define COLOR_BLUE   0,   0,   255
#define COLOR_YELLOW 255, 255, 0
#define COLOR_RED    255, 0,   0

#endif  // constants_h_