#include <iostream>
#include <string>
#include <queue>
#include <stack>
#include <fstream>
#include <iomanip>

using namespace std;

const int ROWS = 5;
const int COLS = 5;
const int MAX_UNDO = 5;

// Structure cho Thao tác Đặt vé (Dùng cho Stack Undo)
struct Action {
    string type; // "BOOK" hoặc "CANCEL"
    int movieId;
    int row;
    int col;
};

// Structure cho Yêu cầu hoàn vé (Dùng cho Queue)
struct RefundRequest {
    int movieId;
    string customerName;
    int row;
    int col;
};

// Node của Cây nhị phân tìm kiếm (BST) quản lý Phim
struct MovieNode {
    int id;
    string title;
    long long revenue;
    int seats[ROWS][COLS]; // 2D Array: 0 = Trống, 1 = Đã đặt
    MovieNode* left;
    MovieNode* right;

    MovieNode(int id, string title) {
        this->id = id;
        this->title = title;
        this->revenue = 0;
        this->left = this->right = nullptr;
        // Khởi tạo toàn bộ ghế trống
        for (int i = 0; i < ROWS; i++)
            for (int j = 0; j < COLS; j++)
                seats[i][j] = 0;
    }
};

// LỚP HỆ THỐNG QUẢN LÝ RẠP PHIM
class CinemaSystem {
private:
    MovieNode* root;
    queue<RefundRequest> refundQueue;
    stack<Action> undoStack;

    // Helper: Thêm phim vào BST
    MovieNode* insertMovie(MovieNode* node, int id, string title) {
        if (!node) return new MovieNode(id, title);
        if (id < node->id) node->left = insertMovie(node->left, id, title);
        else if (id > node->id) node->right = insertMovie(node->right, id, title);
        return node;
    }

    // Helper: Tìm phim trong BST
    MovieNode* searchMovie(MovieNode* node, int id) {
        if (!node || node->id == id) return node;
        if (id < node->id) return searchMovie(node->left, id);
        return searchMovie(node->right, id);
    }

    // Helper: Tìm Node thế mạng cho thao tác xóa BST
    MovieNode* findMin(MovieNode* node) {
        while (node && node->left) node = node->left;
        return node;
    }

    // Helper: Xóa phim khỏi BST
    MovieNode* deleteMovie(MovieNode* node, int id) {
        if (!node) return node;
        if (id < node->id) node->left = deleteMovie(node->left, id);
        else if (id > node->id) node->right = deleteMovie(node->right, id);
        else {
            if (!node->left) {
                MovieNode* temp = node->right;
                delete node;
                return temp;
            }
            else if (!node->right) {
                MovieNode* temp = node->left;
                delete node;
                return temp;
            }
            MovieNode* temp = findMin(node->right);
            node->id = temp->id;
            node->title = temp->title;
            node->revenue = temp->revenue;
            node->right = deleteMovie(node->right, temp->id);
        }
        return node;
    }

    // Helper: Xuất thống kê (Inorder Traversal)
    void printReport(MovieNode* node, ofstream& file) {
        if (!node) return;
        printReport(node->left, file);
        file << "ID: " << node->id << " | Phim: " << node->title
            << " | Doanh thu: " << node->revenue << " VND\n";
        printReport(node->right, file);
    }

    // Helper: Đẩy thao tác vào Stack giới hạn 5 lần
    void pushUndo(Action act) {
        if (undoStack.size() >= MAX_UNDO) {
            // Biện pháp giữ size = 5: Copy sang stack tạm, bỏ phần tử đáy
            stack<Action> temp;
            while (undoStack.size() > 1) {
                temp.push(undoStack.top());
                undoStack.pop();
            }
            undoStack.pop(); // Xóa phần tử cũ nhất ở đáy
            while (!temp.empty()) {
                undoStack.push(temp.top());
                temp.pop();
            }
        }
        undoStack.push(act);
    }

public:
    CinemaSystem() { root = nullptr; }

    // 1. Quản lý phim
    void addMovie(int id, string title) { root = insertMovie(root, id, title); }
    void removeMovie(int id) { root = deleteMovie(root, id); }

    // 2. Quản lý Ghế 2D Array
    void displaySeats(int movieId) {
        MovieNode* movie = searchMovie(root, movieId);
        if (!movie) { cout << "Không tìm thấy phim!\n"; return; }

        cout << "\n--- SƠ ĐỒ GHẾ PHIM: " << movie->title << " ---\n   ";
        for (int j = 0; j < COLS; j++) cout << " " << j << " ";
        cout << "\n";
        for (int i = 0; i < ROWS; i++) {
            cout << i << " |";
            for (int j = 0; j < COLS; j++) {
                cout << (movie->seats[i][j] == 0 ? " [ ]" : " [X]");
            }
            cout << "\n";
        }
    }

    bool bookTicket(int movieId, int r, int col, bool isUndoAction = false) {
        MovieNode* movie = searchMovie(root, movieId);
        if (!movie) return false;

        if (r < 0 || r >= ROWS || col < 0 || col >= COLS) return false;
        if (movie->seats[r][col] == 1) return false;

        movie->seats[r][col] = 1;
        movie->revenue += 100000; // Giả định giá vé 100k

        if (!isUndoAction) {
            pushUndo({ "BOOK", movieId, r, col });
        }
        return true;
    }

    bool cancelTicket(int movieId, int r, int col, bool isUndoAction = false) {
        MovieNode* movie = searchMovie(root, movieId);
        if (!movie) return false;

        if (movie->seats[r][col] == 0) return false;

        movie->seats[r][col] = 0;
        movie->revenue -= 100000;

        if (!isUndoAction) {
            pushUndo({ "CANCEL", movieId, r, col });
        }
        return true;
    }

    // 3. Undo bằng Stack
    void undo() {
        if (undoStack.empty()) {
            cout << "Không có thao tác nào để hoàn tác!\n";
            return;
        }
        Action lastAction = undoStack.top();
        undoStack.pop();

        if (lastAction.type == "BOOK") {
            cancelTicket(lastAction.movieId, lastAction.row, lastAction.col, true);
            cout << "Undo thành công: Đã HỦY ghế vừa đặt tại [" << lastAction.row << "][" << lastAction.col << "]\n";
        }
        else if (lastAction.type == "CANCEL") {
            bookTicket(lastAction.movieId, lastAction.row, lastAction.col, true);
            cout << "Undo thành công: Đã ĐẶT LẠI ghế vừa hủy tại [" << lastAction.row << "][" << lastAction.col << "]\n";
        }
    }

    // 4. Hàng chờ hoàn vé bằng Queue
    void requestRefund(int movieId, string name, int r, int col) {
        refundQueue.push({ movieId, name, r, col });
        cout << "Đã gửi yêu cầu hoàn vé của " << name << " vào hàng chờ.\n";
    }

    void processRefund() {
        if (refundQueue.empty()) {
            cout << "Hàng chờ hoàn vé trống!\n";
            return;
        }
        RefundRequest req = refundQueue.front();
        refundQueue.pop();

        if (cancelTicket(req.movieId, req.row, req.col, true)) {
            cout << "Đã xử lý hoàn tiền cho khách hàng: " << req.customerName
                << " (Ghế [" << req.row << "][" << req.col << "])\n";
        }
        else {
            cout << "Xử lý thất bại! Ghế chưa được đặt hoặc thông tin sai.\n";
        }
    }

    // 5. Xuất báo cáo ra File
    void exportReport(string filename) {
        ofstream file(filename);
        if (!file.is_open()) {
            cout << "Lỗi mở file!\n";
            return;
        }
        file << "================ BÁO CÁO DOANH THU RẠP CHIẾU PHIM ================\n";
        printReport(root, file);
        file << "=================================================================\n";
        file.close();
        cout << "Đã xuất báo cáo thành công ra file " << filename << "\n";
    }
};

// HÀM MAIN KIỂM THỬ CHƯƠNG TRÌNH
int main() {
    CinemaSystem cinema;

    // Thêm phim mẫu
    cinema.addMovie(101, "Avengers: Endgame");
    cinema.addMovie(105, "Inception");
    cinema.addMovie(102, "Interstellar");

    cout << "--- THỬ NGHIỆM ĐẶT VẾ & HIỂN THỊ SƠ ĐỒ ---";
    cinema.displaySeats(101);
    cinema.bookTicket(101, 2, 3);
    cinema.bookTicket(101, 2, 4);
    cinema.displaySeats(101);

    cout << "\n--- THỬ NGHIỆM UNDO (STACK) ---";
    cinema.undo();
    cinema.displaySeats(101);

    cout << "\n--- THỬ NGHIỆM HÀNG CHỜ HOÀN VÉ (QUEUE) ---";
    cinema.requestRefund(101, "Nguyen Van A", 2, 3);
    cinema.processRefund();
    cinema.displaySeats(101);

    cout << "\n--- XUẤT BÁO CÁO ---";
    cinema.exportReport("DoanhThuRapPhim.txt");

    return 0;
}
