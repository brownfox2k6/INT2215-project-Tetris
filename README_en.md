This is English version. [Readme in Vietnamese](/README.md)

# Details about project
[Game demo video](https://youtu.be/FZnFTIYFcQo)
## Author
- **Name:** Nguyễn Trọng Đại
- **Class:** QH-2024-I/CQ-I-CS4
- **Student ID:** 24021400

## Programming language, graphics libraries and version
  - C++17 or above
  - [SDL 3.2.8](https://github.com/libsdl-org/SDL/releases/tag/release-3.2.8)
  - [SDL_image 3.2.4](https://github.com/libsdl-org/SDL_image/releases/tag/release-3.2.4)
  - [SDL_ttf 3.2.0](https://github.com/libsdl-org/SDL_ttf/releases/tag/release-3.2.0)

## Update history
  - [Commits history](https://github.com/brownfox2k6/INT2215-project-Tetris/commits/main/)
  - [Releases](https://github.com/brownfox2k6/INT2215-project-Tetris/releases)
 
# Tetris
_Source: Translated from [Tetris - Wikipedia tiếng Việt](https://vi.wikipedia.org/wiki/Tetris)_
## Overview
The game features seven types of blocks, known as Tetrominoes: I (straight line), J, L, O (square), S, T, and Z. Each block is made up of four smaller squares connected together, forming various rectangular shapes.

By rotating these blocks at angles of 90°, 180°, or 270°, different orientations of the shapes are created.

A random sequence of Tetrominoes falls down the playing field — a vertical rectangular grid referred to as the "matrix."

## Gameplay
The objective is to control and move the falling Tetrominoes within a rectangular grid of 20 rows and 10 columns. Players must place the blocks without overlapping existing ones. Successfully filling an entire horizontal line will clear that line from the grid and award points.

The game uses two main grids:
- Main Grid: A grid of 24 rows and 10 columns. Only the bottom 20 rows are visible to the player; the top 4 rows are hidden. This hidden area is where new Tetrominoes are spawned before they become visible as they fall into view.
- Next Grid: A small preview area that displays the upcoming Tetrominoes, allowing players to plan ahead.

Each Tetromino falls from the top of the screen. The player can move it left, right, or down faster, and rotate it before it locks into place at the bottom or on top of existing blocks. Once a block is placed, the next Tetromino immediately appears.

If the stacked blocks reach the top of the grid and a new Tetromino can no longer be placed, the game ends.

## Scoring
- All Tetrominoes are capable of clearing one or two lines at a time.
- The J and L shapes can sometimes clear three lines.
- Only the I-shaped Tetromino can clear four lines simultaneously, an action known as a "Tetris."
- The maximum number of lines that can be cleared at once is four.

# Key bindings
|   Key   | Action                  |
|:-------:| ----------------------- |
|   `←`   | Move left               |
|   `→`   | Move right              |
|   `↓`   | Soft drop               |
| `Space` | Hard drop               |
|   `↑`   | Rotate right            |
|   `X`   | Rotate left             |
|   `C`   | Hold                    |
|   `P`   | Pause/Resume/Start game |

# Auto-drop rate
When the level increases, the drop rate of Tetrominoes will be faster (the table below is taken from ChatGPT)
| Level | Drop rate (ms/row) |
|:----- |:------------------ |
| 1     | 800                |
| 2     | 717                |
| 3     | 633                |
| 4     | 550                |
| 5     | 467                |
| 6     | 383                |
| 7     | 300                |
| 8     | 217                |
| 9     | 133                |
| 10    | 100                |
| 11    | 83                 |
| 12    | 67                 |
| 13    | 50                 |
| 14    | 33                 |
| 15    | 17                 |

# Scoring and levelling
- You'll level up when done clearing 10 lines (level 15 is maximum you can reach)
- Scoring:
  - Clear 1 line: 100 × level
  - Clear 2 lines: 300 × level
  - Clear 3 lines: 500 × level
  - Clear 4 lines: 800 × level

# Compile command
```
g++ main.cpp ./src/*.cpp -oTetris.exe -Isrc/include -Lsrc/lib -lSDL3 -lSDL3_image -lSDL3_ttf -mwindows
```

# Some screenshots of this game
<details><summary>When open the game</summary>
  
  ![image](https://github.com/user-attachments/assets/8e010ca0-8a9f-4985-aa24-8e55be7b4c7d)
</details>

<details><summary>On count down to start the game</summary>
  
  ![image](https://github.com/user-attachments/assets/0cad485a-1a9e-439e-9096-b07725e9f47d)
</details>

<details><summary>On playing - Tetromino dropping</summary>
  
  ![image](https://github.com/user-attachments/assets/38d74b86-d78f-40e2-b195-5b003a797444)
</details>

<details><summary>On playing - Tetromino is reaching bottom</summary>
  
  ![image](https://github.com/user-attachments/assets/36343149-505c-41e8-ac2a-413db2816514)
</details>

<details><summary>Paused</summary>
  
  ![image](https://github.com/user-attachments/assets/ae4ce8af-6a87-45d6-80b8-1e570336f592)
</details>

<details><summary>On count down to resume the game</summary>
  
  ![image](https://github.com/user-attachments/assets/d19a5a33-1e40-45ef-97cd-a7f92927e5ae)
</details>

<details><summary>Game over</summary>
  
  ![image](https://github.com/user-attachments/assets/0f0bc50b-68cf-45dd-a785-f7a40c67b9ff)
</details>

# References
- Sample code at [SDL's homepage](https://examples.libsdl.org/SDL3/)
- Tetromino rotation mechanics: [Super Rotation System](https://tetris.wiki/Super_Rotation_System)
- All image files [here](src/images) và audios [here](src/audios) are taken from this web [tetris.com](https://tetris.com/play-tetris/)
- Implementation of `Timer` class: [Lazy Foo' Productions - Advanced Timers](https://lazyfoo.net/tutorials/SDL/23_advanced_timers/index.php)
- ℹ️ Also, **I do not** use code from sources not listed above.
