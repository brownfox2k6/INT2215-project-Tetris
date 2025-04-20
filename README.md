# INT2215-project-Tetris

## Thông tin về project
- **Mô tả game:** [Wikipedia](https://vi.wikipedia.org/wiki/Tetris)
- **Tác giả:** Nguyễn Trọng Đại - 24021400
- **Ngôn ngữ lập trình:** C++17 trở lên
- **Thư viện đồ hoạ:**
  - [SDL 3.2.8](https://github.com/libsdl-org/SDL/releases/tag/release-3.2.8)
  - [SDL_image 3.2.4](https://github.com/libsdl-org/SDL_image/releases/tag/release-3.2.4)
  - [SDL_ttf 3.2.0](https://github.com/libsdl-org/SDL_ttf/releases/tag/release-3.2.0)

## Các phím bấm
|   Key   | Action       |
|:-------:| ------------ |
|   `←`   | Move left    |
|   `→`   | Move right   |
|   `↓`   | Soft drop    |
| `Space` | Hard drop    |
|   `↑`   | Rotate right |
|   `X`   | Rotate left  |
|   `C`   | Hold         |

## Tốc độ rơi tự động
Sau mỗi level, tốc độ rơi của các Tetromino sẽ nhanh hơn (dữ liệu lấy từ ChatGPT)
| Level | Tốc độ rơi (ms/hàng) |
|:----- |:-------------------- |
| 1     | 800                  |
| 2     | 717                  |
| 3     | 633                  |
| 4     | 550                  |
| 5     | 467                  |
| 6     | 383                  |
| 7     | 300                  |
| 8     | 217                  |
| 9     | 133                  |
| 10    | 100                  |
| 11    | 83                   |
| 12    | 67                   |
| 13    | 50                   |
| 14    | 33                   |
| 15    | 17                   |

## Cơ chế tính điểm và lên cấp
- Cứ phá được 10 hàng thì sẽ lên 1 level (tối đa là level 15)
- Tính điểm:
  - Phá được 1 hàng: 100 × level
  - Phá được 2 hàng: 300 × level
  - Phá được 3 hàng: 500 × level
  - Phá được 4 hàng: 800 × level

## Cấu hình biên dịch
```
g++ -std=c++17 main.cpp ./src/*.cpp -o Tetris.exe -mwindows -Isrc/include -Lsrc/lib -lSDL3 -lSDL3_image -lSDL3_ttf
```

## Một số hình ảnh của game

<details><summary>Khi vừa mở game</summary>
  
  ![image](https://github.com/user-attachments/assets/8e010ca0-8a9f-4985-aa24-8e55be7b4c7d)
</details>

<details><summary>Trong thời gian chờ bắt đầu game</summary>
  
  ![image](https://github.com/user-attachments/assets/0cad485a-1a9e-439e-9096-b07725e9f47d)
</details>

<details><summary>Khi chơi - Tetromino đang rơi</summary>
  
  ![image](https://github.com/user-attachments/assets/38d74b86-d78f-40e2-b195-5b003a797444)
</details>

<details><summary>Khi chơi - Tetromino đang chạm đáy</summary>
  
  ![image](https://github.com/user-attachments/assets/36343149-505c-41e8-ac2a-413db2816514)
</details>

<details><summary>Tạm dừng</summary>
  
  ![image](https://github.com/user-attachments/assets/ae4ce8af-6a87-45d6-80b8-1e570336f592)
</details>

<details><summary>Trong thời gian chờ tiếp tục game</summary>
  
  ![image](https://github.com/user-attachments/assets/d19a5a33-1e40-45ef-97cd-a7f92927e5ae)
</details>

<details><summary>Game over</summary>
  
  ![image](https://github.com/user-attachments/assets/0f0bc50b-68cf-45dd-a785-f7a40c67b9ff)
</details>


## Nguồn tham khảo
- Code mẫu tại [trang chủ của SDL](https://examples.libsdl.org/SDL3/)
- Cơ chế xoay Tetromino: [Super Rotation System](https://tetris.wiki/Super_Rotation_System)
- Tất cả file ảnh ở [đây](src/images) và audio ở [đây](src/audios) được lấy từ [web chơi Tetris online](https://tetris.com/play-tetris/)
- ⚠️ Ngoài ra, cam kết **không** sử dụng code ở những nguồn không được liệt kê ở trên
