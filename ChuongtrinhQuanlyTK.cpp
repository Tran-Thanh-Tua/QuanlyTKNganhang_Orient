#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
#include <limits>

using namespace std;

//DINH DANG MAU SAC
#define W "\033[0m"
#define R "\033[31m"
#define G "\033[38;5;46m"
#define Y "\033[38;5;226m"

//HAM KIEM TRA SO TIEN NHAP VAO HOP LE
double NhapSoTien(const string& thongBao) {
    double tien;
    cout << thongBao;
    cin >> tien;
    if (cin.fail() || tien < 0) { //so tien phai la so thuc khong am
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << R << "So tien nhap vao khong hop le!\n" << W;
        return -1;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return tien;
}

//HAM KIEM TRA SO NGAY GUI HOP LE
int NhapSoNgay() {
    int ngay;
    cout << "\nNhap so ngay da gui den hien tai: ";
    cin >> ngay;
    if (cin.fail() || ngay < 0) { //so ngay phai la so nguyen khong am
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << R << "So ngay nhap vao khong hop le!\n" << W;
        return -1;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return ngay;
}

//HAM QUAY VE MENU CHINH SAU KHI HOAN TAT MOT CHUC NANG
void Pause() {
    cout << "\nNhan Enter de quay lai menu chinh...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}
void PauseIn() {
    cout << "\nNhan Enter de quay lai menu chinh...";
    cin.get();
}

//LOP TAI KHOAN
class TaiKhoan {
protected:
    string soTK;
    string chuTK;
    double soDu;
public:
    TaiKhoan(string stk = "", string ctk = "", double sodu = 0)
        : soTK(stk), chuTK(ctk), soDu(sodu) {}
    virtual void NapTien(double tien) {
        if (tien > 0) soDu += tien;
    }
    virtual bool RutTien(double tien) {
        if (tien <= soDu) {
            soDu -= tien;
            return true;
        }
        return false;
    }
    virtual void XemThongTin() const {
        cout << "So TK: " << soTK << "\n"
            << "Chu TK: " << chuTK << "\n"
            << "So du: " << fixed << setprecision(0) << soDu << " VND\n";
    }
    string getSoTK() const { return soTK; }
    string getChuTK() const { return chuTK; }
    double getSoDu() const { return soDu; }
    virtual ~TaiKhoan() {}
};

//LOP TAI KHOAN TIET KIEM - KE THUA LOP TAI KHOAN
class TaiKhoanTietKiem : public TaiKhoan {
private:
    const double laiSuatThang = 0.05;
public:
    TaiKhoanTietKiem(string stk = "", string ctk = "", double tienGui = 0)
        : TaiKhoan(stk, ctk, 0) {
        if (tienGui >= 1000000) soDu = tienGui;
        else cout << R << "So tien gui phai tu 1.000.000 VND tro len!\n" << W;
    }

    bool HopLe() const { return soDu >= 1000000; }

    double TinhTienNhan(int soNgayGui) const {
        return (soNgayGui >= 30) ? soDu * (1 + laiSuatThang) : soDu;
    }

    bool RutTien(double = 0) override {
        int soNgayGui = NhapSoNgay();
        if (soNgayGui == -1) { PauseIn(); return false; }

        double tienNhan = TinhTienNhan(soNgayGui);
        cout << "\n=> So tien rut: " << fixed << setprecision(0) << tienNhan << " VND ";
        if (soNgayGui >= 30) cout << G << "(gom ca tien lai 5%)\n" << W;
        else cout << R << "(Khong co lai: Rut truoc han)\n" << W;

        cout << "Xac nhan rut toan bo (y/n): ";
        char c; cin >> c; cin.ignore();
        if (c == 'y' || c == 'Y') {
            soDu = 0;
            cout << G << "\nRut tien thanh cong!\n" << W;
        }
        else {
            cout << "\nDa huy rut tien.\n";
        }
        PauseIn();
        return true;
    }

    void XemThongTin() const override {
        TaiKhoan::XemThongTin();
        cout << "Loai TK: Tiet kiem\n";
    }
};

//LOP TAI KHOAN THANH TOAN - KE THUA LOP TAI KHOAN
class TaiKhoanThanhToan : public TaiKhoan {
public:
    TaiKhoanThanhToan(string stk = "", string ctk = "", double sodu = 0)
        : TaiKhoan(stk, ctk, sodu) {}

    bool RutTien(double tien) override {
        if (tien < 50000 || tien > 100000000) {
            cout << R << "So tien rut khong hop le!\n" << W;
            return false;
        }
        if (tien <= soDu) {
            soDu -= tien;
            cout << G << "Rut thanh cong " << fixed << setprecision(0) << tien << " VND\n" << W;
            cout << "So du con lai: " << fixed << setprecision(0) << soDu << " VND\n";
            return true;
        }
        cout << R << "Khong du so du! Rut that bai.\n" << W;
        return false;
    }

    void XemThongTin() const override {
        TaiKhoan::XemThongTin();
        cout << "Loai TK: Thanh toan\n";
    }
};

//CHUC NANG 4: LUU VA THOAT CHUONG TRINH
vector<TaiKhoan*> dsTaiKhoan;
const string FILE_NAME = "TaiKhoan.txt";
void LuuVaoFile() {
    ofstream f(FILE_NAME);
    for (const auto& tk : dsTaiKhoan) {
        auto tkTietKiem = dynamic_cast<TaiKhoanTietKiem*>(tk);
        if (tkTietKiem) {
            f << "TIETKIEM\n";
            f << tk->getSoTK() << "\n" << tk->getChuTK() << "\n"
                << tk->getSoDu() << "\n";
        }
        else {
            f << "THANHTOAN\n";
            f << tk->getSoTK() << "\n" << tk->getChuTK() << "\n" << tk->getSoDu() << "\n";
        }
    }
    f.close();
    cout << G << "Da luu thanh cong vao " << FILE_NAME << "\n" << W;
}

//DOC FILE TaiKhoan.txt moi lan chay chuong trinh: Loai tai khoan - So tai khoan - Ten chu tai khoan - So du
void DocTuFile() {
    ifstream f(FILE_NAME);
    if (!f.is_open()) return;
    string line;
    while (getline(f, line)) {
        if (line == "TIETKIEM") {
            string soTK, chuTK;
            double soDu;
            getline(f, soTK);
            getline(f, chuTK);
            f >> soDu; f.ignore();
            dsTaiKhoan.push_back(new TaiKhoanTietKiem(soTK, chuTK, soDu));
        }
        else if (line == "THANHTOAN") {
            string soTK, chuTK;
            double soDu;
            getline(f, soTK);
            getline(f, chuTK);
            f >> soDu; f.ignore();
            dsTaiKhoan.push_back(new TaiKhoanThanhToan(soTK, chuTK, soDu));
        }
    }
    f.close();
}

//CHUC NANG 1: THEM TAI KHOAN MOI
void ThemTaiKhoan() {
    system("cls");
    cout << "==========" << Y << " THEM TAI KHOAN MOI " << W << "==========\n";
    string soTK, chuTK;
    cout << "Nhap so tai khoan: ";
    getline(cin >> ws, soTK);
    cout << "Nhap ten chu tai khoan: ";
    getline(cin, chuTK);

    cout << "\nChon loai tai khoan:\n1. Tiet kiem\n2. Thanh toan\nLua chon (1-2): ";
    string loai; cin >> loai;

    if (loai == "1") {
        double tien = NhapSoTien("\nNhap so tien gui (toi thieu 1.000.000 VND): ");
        if (tien == -1) { PauseIn(); return; }
        TaiKhoanTietKiem* tk = new TaiKhoanTietKiem(soTK, chuTK, tien);
        if (tk->getSoDu() >= 1000000) {
            dsTaiKhoan.push_back(tk);
            cout << G << "Them tai khoan tiet kiem thanh cong!\n" << W;
        }
        else {
            cout << "Them tai khoan that bai.\n";
            delete tk;
        }
    }
    else if (loai == "2") {
        double tien = NhapSoTien("\nNhap so du ban dau: ");
        if (tien == -1) { PauseIn(); return; }
        dsTaiKhoan.push_back(new TaiKhoanThanhToan(soTK, chuTK, tien));
        cout << G << "Them tai khoan thanh toan thanh cong!\n" << W;
    }
    else {
        cout << R << "Lua chon khong hop le!\n" << W;
    }
    Pause();
}

//CHUC NANG 2: HIEN THI DANH SACH TAI KHOAN
void HienThiTatCa() {
    system("cls");
    cout << "==========" << Y << " DANH SACH TAI KHOAN " << W << "==========\n";
    if (dsTaiKhoan.empty()) {
        cout << "Chua co tai khoan nao.\n\n";
    }
    else {
        for (size_t i = 0; i < dsTaiKhoan.size(); ++i) {
            cout << "--------------" << Y << " Tai khoan " << i + 1 << W << " --------------\n";
            dsTaiKhoan[i]->XemThongTin();
            cout << "-----------------------------------------\n\n";
        }
    }
    cout << "Nhan Enter de quay lai...";
    cin.get();
}

//CHUC NANG 3: TIM VA THAO TAC TREN TAI KHOAN
TaiKhoan* TimTaiKhoan(const string& soTK) {
    for (auto tk : dsTaiKhoan) {
        if (tk->getSoTK() == soTK) return tk;
    }
    return nullptr;
}
void ThaoTacTrenTaiKhoan() {
    system("cls");
    cout << "==========" << Y << " THAO TAC TREN TAI KHOAN " << W << "==========\n";
    string soTK;
    cout << "Nhap so tai khoan: ";
    getline(cin >> ws, soTK);

    TaiKhoan* tk = TimTaiKhoan(soTK);
    if (!tk) {
        cout << R << "Khong tim thay tai khoan!\n" << W;
        PauseIn();
        return;
    }
    tk->XemThongTin();

    cout << "\nChon thao tac:\n1. Nap tien\n2. Rut tien\nLua chon (1-2): ";
    string chon; cin >> chon;

    if (chon == "1") {
        double tien = NhapSoTien("\nNhap so tien nap: ");
        if (tien == -1) { PauseIn(); return; }
        tk->NapTien(tien);
        cout << G << "Nap thanh cong! So du moi: "
            << fixed << setprecision(0) << tk->getSoDu() << " VND\n" << W;
    }
    else if (chon == "2") {
        auto tkTietKiem = dynamic_cast<TaiKhoanTietKiem*>(tk);
        if (tkTietKiem) {
            tkTietKiem->RutTien();
            return;
        }
        else {
            double tien = NhapSoTien("\nNhap so tien can rut (50.000 - 100.000.000 VND): ");
            if (tien == -1) { Pause(); return; }
            tk->RutTien(tien);
        }
    }
    else {
        cout << R << "Lua chon khong hop le!\n" << W;
    }
    Pause();
}

//HAM MAIN
int main() {
    cout << fixed << setprecision(0);
    DocTuFile();
    string luaChon;
    do {
        system("cls");
        cout << "=====================================\n";
        cout << Y << "     QUAN LY TAI KHOAN NGAN HANG \n" << W;
        cout << "=====================================\n";
        cout << "1. Them tai khoan moi\n";
        cout << "2. Hien thi tat ca tai khoan\n";
        cout << "3. Tim va thao tac tren tai khoan\n";
        cout << "4. Luu va thoat chuong trinh\n";
        cout << "5. Thoat khong luu\n";
        cout << "=====================================\n";
        cout << "Nhap lua chon (1-5): ";
        cin >> luaChon;
        cin.ignore();

        if (luaChon == "1") ThemTaiKhoan();
        else if (luaChon == "2") { HienThiTatCa(); }
        else if (luaChon == "3") ThaoTacTrenTaiKhoan();
        else if (luaChon == "4") { LuuVaoFile(); break; }
        else if (luaChon == "5") { cout << "Thoat khong luu.\n"; break; } //THOAT CHUONG TRINH
        else { cout << "Lua chon khong hop le!\n\nNhan Enter de quay lai..."; cin.get(); }
    } while (true);

    for (auto tk : dsTaiKhoan) delete tk;
    dsTaiKhoan.clear();
    return 0;
}