# 🎭 Hệ Thống Quản Lý Rạp Chiếu Phim (Teatro)

Dự án C++ cấu trúc dữ liệu và giải thuật hoàn chỉnh, mô phỏng hệ thống vận hành rạp phim thực tế.
- **Độ khó:** ⭐⭐⭐ (Intermediate)
- **Công nghệ:** C++11 / C++14 (Console CLI)
- **Môi trường phát triển:** Microsoft Visual Studio 2022

---

## 🛠 1. Kiến Trúc Cấu Trúc Dữ Liệu & Mối Liên Kết
Hệ thống không đứng độc lập mà giao thoa chặt chẽ giữa 4 loại cấu trúc dữ liệu để tối ưu hóa bộ nhớ và tốc độ xử lý:

1. **BST (Binary Search Tree)**: Quản lý danh sách các bộ phim. Khóa tìm kiếm chính là `Movie ID`. Mỗi Node trên cây là một thực thể phim độc lập.
2. **2D Array (Ma trận 2 chiều)**: Nằm **bên trong** từng Node của cây BST. Mỗi bộ phim sở hữu riêng một ma trận ghế kích thước $5 \times 5$ để quản lý trạng thái chỗ ngồi trống hoặc đã đặt.
3. **Queue (Hàng đợi FIFO)**: Quản lý luồng yêu cầu hoàn vé của khách hàng. Yêu cầu nào gửi trước sẽ được quản lý rạp duyệt hoàn tiền trước theo đúng tính chất tuyến tính.
4. **Stack (Ngăn xếp LIFO)**: Bộ nhớ đệm lưu lại tối đa `5 thao tác` đặt/hủy vé gần nhất. Hỗ trợ tính năng Hoàn tác (Undo) tức thời bằng cơ chế đảo ngược hành động.

---

## 📊 2. Phân Tích Độ Phức Tạp Thuật Toán (Big O)

| Chức năng | Cấu trúc dữ liệu | Độ phức tạp thời gian (Thời gian xử lý) | Giải thích |
| :--- | :--- | :--- | :--- |
| **Tìm kiếm phim** | Cây BST | $O(\log n)$ | Loại bỏ một nửa số nhánh cây sau mỗi bước so sánh ID. |
| **Đặt / Hủy vé** | Ma trận 2D | $O(1)$ | Truy cập trực tiếp vào tọa độ ghế `[Row][Col]` trong mảng. |
| **Đổi vị trí ghế** | Ma trận 2D | $O(1)$ | Chuỗi thao tác giải phóng ghế cũ và lấp đầy ghế mới. |
| **Xếp hàng hoàn vé**| Hàng đợi Queue | $O(1)$ | Đẩy yêu cầu vào cuối hàng (`push`) mất thời gian cố định. |
| **Duyệt hoàn vé** | Hàng đợi Queue | $O(1)$ | Lấy yêu cầu đầu tiên ra xử lý (`pop`) theo cơ chế FIFO. |
| **Hoàn tác (Undo)** | Ngăn xếp Stack | $O(1)$ | Lấy trạng thái trên cùng của Stack (`top`) để đảo ngược. |

---

## 🚀 3. Hướng Dẫn Chạy Dự Án
1. Tải toàn bộ thư mục `Cinema-Project/DỰ ÁN` về máy tính của bạn.
2. Tìm và nhấp đúp chuột để mở file giải pháp dự án: **`DỰ ÁN.sln`** bằng phần mềm **Visual Studio**.
3. Tại giao diện Visual Studio, nhấn tổ hợp phím **`Ctrl + F5`** (hoặc nút Start không debug) để biên dịch hệ thống.
4. Trải nghiệm giao diện điều khiển tương tác trực tiếp bằng cách gõ phím số `1, 2, 3, 4, 5, 6, 7` trên màn hình Console đen.

---

## 📅 4. Kịch Bản Kiểm Thử Hệ Thống (Demo Test)
Để kiểm tra tính liên kết của dự án, bạn có thể thực hiện theo kịch bản mẫu sau:
* **Bước 1**: Nhấn phím `1` xem sơ đồ ghế phim `101`. Toàn bộ ghế sẽ hiển thị trống dạng `[ ]`.
* **Bước 2**: Nhấn phím `2` đặt ghế tại vị trí hàng `2` cột `3`. Ghế sẽ chuyển trạng thái thành `[X]`.
* **Bước 3**: Nhấn phím `6` để thử tính năng **Undo**. Hệ thống sẽ tự động hủy ghế vừa đặt, đưa sơ đồ về trạng thái trống ban đầu một cách thông minh.
* **Bước 4**: Nhấn phím `7` để kiểm tra chức năng xuất file dữ liệu. Hệ thống sẽ kết xuất ra file `BaoCaoDoanhThu.txt` nằm ngay trong thư mục dự án.
