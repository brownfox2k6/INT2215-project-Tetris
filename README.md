This is Vietnamese version. [README in English](/README_en.md)

# Thông tin về project
[Video demo game](https://youtu.be/FZnFTIYFcQo)
## Tác giả
- **Họ tên:** Nguyễn Trọng Đại
- **Lớp khoá học:** QH-2024-I/CQ-I-CS4
- **Mã sinh viên:** 24021400

## Ngôn ngữ lập trình và phiên bản
C++17 trở lên

## Thư viện đồ hoạ
  - [SDL 3.2.8](https://github.com/libsdl-org/SDL/releases/tag/release-3.2.8)
  - [SDL_image 3.2.4](https://github.com/libsdl-org/SDL_image/releases/tag/release-3.2.4)
  - [SDL_ttf 3.2.0](https://github.com/libsdl-org/SDL_ttf/releases/tag/release-3.2.0)

## Lịch sử cập nhật
  - [Commits history](https://github.com/brownfox2k6/INT2215-project-Tetris/commits/main/)
  - [Releases](https://github.com/brownfox2k6/INT2215-project-Tetris/releases)
 
# Tetris
_Nguồn: [Tetris - Wikipedia tiếng Việt](https://vi.wikipedia.org/wiki/Tetris)_
## Tổng quan
Trò chơi có bảy loại khối hình: I (thẳng đứng), J, L, O (vuông), S, T, Z. Ta thấy mỗi khối gạch được cấu tạo từ 4 hình vuông nhỏ xếp lại với nhau. Ta có thể coi các khối gạch đó như là những hình chữ nhật có kích thước khác nhau.

Các hình khác được tạo ra khi xoay các khối cơ bản này các góc tương ứng 90 độ, 180 độ, 270 độ.

Một chuỗi ngẫu nhiên của Tetrominoes rơi xuống sân chơi (một trục đứng hình chữ nhật, được gọi là "tốt" hay "ma trận").

## Cách chơi
Mục tiêu của trò chơi là di chuyển các khối gạch đang rơi từ từ xuống trong kích thước hình chữ nhật 20 hàng x 10 cột (trên màn hình). Chỗ nào có gạch rồi thì không di chuyển được tới vị trí đó. Người chơi xếp những khối hình sao cho khối hình lấp đầy 1 hàng ngang để ghi điểm và hàng ngang ấy sẽ biến mất.

Ta có hai bảng:
- Một bảng chính gồm 24 dòng và 10 cột. Ta sẽ chỉ thể hiện ra màn hình 20 dòng còn 4 dòng kia thì không. Tại sao thế? Vì khi ta tạo 1 khối gạch mới để rơi xuống ta sẽ tạo ở khoảng 4 dòng trên cùng. Và người chơi sẽ không thấy cho đến khi nó rơi xuống dần lộ ra.
- Một bảng Next thể hiện những khối gạch tiếp theo sẽ được cho vào màn chơi khi khối gạch hiện tại đã đặt xong.

Một nhóm 4 khối sẽ rơi từ phía trên cùng của màn hình, di chuyển các khối và xoay chúng cho đến khi chúng rơi xuống phía dưới cùng của màn hình, sau đó nhóm 4 khối tiếp theo sẽ rơi xuống.

Nếu để cho những khối hình cao quá màn hình, trò chơi sẽ kết thúc.

Trò chơi kết thúc khi khối gạch không rơi xuống được nữa.

Tất cả các Tetrominoes có khả năng hoàn thành một và hai dòng. J, L có thể có ba. Chỉ có Tetromino chữ I có khả năng để xóa bốn dòng cùng một lúc, và điều này được gọi là một "Tetris". Xóa nhiều nhất chỉ được 4 hàng/1 lần.

# Các phím bấm
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

# Tốc độ rơi tự động
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

# Cơ chế tính điểm và lên cấp
- Cứ phá được 10 hàng thì sẽ lên 1 level (tối đa là level 15)
- Tính điểm:
  - Phá được 1 hàng: 100 × level
  - Phá được 2 hàng: 300 × level
  - Phá được 3 hàng: 500 × level
  - Phá được 4 hàng: 800 × level

# Cấu hình biên dịch
```
g++ -std=c++17 main.cpp ./src/*.cpp -o Tetris.exe -mwindows -Isrc/include -Lsrc/lib -lSDL3 -lSDL3_image -lSDL3_ttf
```

# Một số hình ảnh của game

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


# Nguồn tham khảo
- Code mẫu tại [trang chủ của SDL](https://examples.libsdl.org/SDL3/)
- Cơ chế xoay Tetromino: [Super Rotation System](https://tetris.wiki/Super_Rotation_System)
- Tất cả file ảnh ở [đây](src/images) và audio ở [đây](src/audios) được lấy từ web chơi Tetris online [tetris.com](https://tetris.com/play-tetris/)
- Cách viết class `Timer`: [Lazy Foo' Productions - Advanced Timers](https://lazyfoo.net/tutorials/SDL/23_advanced_timers/index.php)
- ℹ️ Ngoài ra, **không** sử dụng code ở những nguồn không được liệt kê ở trên
