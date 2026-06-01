#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include <stdexcept>
#include <vector>

using namespace std;

// ==========================================
// 1. STRUKTUR KELAS TOKO ELEKTRONIK (OOP)
// ==========================================
class TokoElektronik {
private:
    // Enkapsulasi: Array berkapasitas 3 elemen untuk etalase
    array<string, 3> etalase;

public:
    // Constructor: Mengisi data awal etalase secara otomatis
    TokoElektronik() {
        etalase[0] = "Laptop ASUS ROG";
        etalase[1] = "Smartphone Samsung S24";
        etalase[2] = "Smart TV LG 43 Inch";
    }

    // Method Public dengan Mekanisme Exception Handling
    string ambilProduk(size_t nomorRak) {
        try {
            // Wajib menggunakan .at() untuk mengecek batas indeks
            return etalase.at(nomorRak);
        }
        catch (const out_of_range& e) {
            // Menangkap out_of_range bawaan, lalu melempar pesan kustom
            throw runtime_error("Gagal Mengambil Barang : Rak nomor " + to_string(nomorRak) + " kosong atau tidak tersedia!");
        }
    }
};

// ==========================================
// FUNGSI HELPER UNTUK MANAJEMEN GUDANG (CRUD)
// ==========================================
const string FILE_NAME = "gudang.txt";

// Fungsi mendisplay isi file (Read)
void tampilkanGudang() {
    ifstream file(FILE_NAME);
    string baris;
    cout << "\n=== DAFTAR BARANG DI GUDANG (gudang.txt) ===\n";
    if (file.is_open()) {
        int nomor = 1;
        bool adaData = false;
        while (getline(file, baris)) {
            if (!baris.empty()) {
                cout << nomor++ << ". " << baris << endl;
                adaData = true;
            }
        }
        if (!adaData) {
            cout << "[ Gudang masih kosong ]\n";
        }
        file.close();
    } else {
        cout << "[ Belum ada data gudang atau file tidak dapat dibuka ]\n";
    }
    cout << "============================================\n";
}

// Fungsi menambah barang (Create)
void tambahBarang() {
    ofstream file(FILE_NAME, ios::app); // ios::app untuk append (menambah di akhir)
    if (file.is_open()) {
        string namaBarang;
        cout << "Masukkan nama barang baru: ";
        cin.ignore();
        getline(cin, namaBarang);
        file << namaBarang << endl;
        file.close();
        cout << "✓ Barang berhasil ditambahkan ke gudang!\n";
    } else {
        cout << "✗ Gagal membuka file gudang.\n";
    }
}

// Fungsi mengubah barang (Update)
void updateBarang() {
    tampilkanGudang();
    ifstream fileIn(FILE_NAME);
    if (!fileIn.is_open()) return;

    vector<string> daftarBarang;
    string baris;
    while (getline(fileIn, baris)) {
        if (!baris.empty()) daftarBarang.push_back(baris);
    }
    fileIn.close();

    if (daftarBarang.empty()) return;

    int pilihan;
    cout << "Pilih nomor barang yang ingin diubah: ";
    cin >> pilihan;

    if (pilihan > 0 && pilihan <= static_cast<int>(daftarBarang.size())) {
        string namaBaru;
        cout << "Masukkan nama baru untuk '" << daftarBarang[pilihan - 1] << "': ";
        cin.ignore();
        getline(cin, namaBaru);
        
        daftarBarang[pilihan - 1] = namaBaru;

        // Tulis ulang file dengan data baru
        ofstream fileOut(FILE_NAME);
        for (const auto& barang : daftarBarang) {
            fileOut << barang << endl;
        }
        fileOut.close();
        cout << "✓ Data barang berhasil diperbarui!\n";
    } else {
        cout << "✗ Nomor barang tidak valid.\n";
    }
}

// Fungsi menghapus barang (Delete)
void deleteBarang() {
    tampilkanGudang();
    ifstream fileIn(FILE_NAME);
    if (!fileIn.is_open()) return;

    vector<string> daftarBarang;
    string baris;
    while (getline(fileIn, baris)) {
        if (!baris.empty()) daftarBarang.push_back(baris);
    }
    fileIn.close();

    if (daftarBarang.empty()) return;

    int pilihan;
    cout << "Pilih nomor barang yang ingin dihapus: ";
    cin >> pilihan;

    if (pilihan > 0 && pilihan <= static_cast<int>(daftarBarang.size())) {
        cout << "✓ '" << daftarBarang[pilihan - 1] << "' berhasil dihapus dari gudang.\n";
        daftarBarang.erase(daftarBarang.begin() + (pilihan - 1));

        // Tulis ulang file tanpa data yang dihapus
        ofstream fileOut(FILE_NAME);
        for (const auto& barang : daftarBarang) {
            fileOut << barang << endl;
        }
        fileOut.close();
    } else {
        cout << "✗ Nomor barang tidak valid.\n";
    }
}

// ==========================================
// 4. PENGUJIAN PADA FUNGSI MAIN
// ==========================================
int main() {
    TokoElektronik toko;
    int pilihanMenu = 0;

    do {
        // Otomatis menampilkan isi gudang di atas menu (Sesuai Poin 3 - Read otomatis)
        tampilkanGudang();

        cout << "\n=== MENU MANAJEMEN TOKO GIBRAN JAYA ===\n";
        cout << "1. Tambah Barang Gudang (Create)\n";
        cout << "2. Ubah Barang Gudang (Update)\n";
        cout << "3. Hapus Barang Gudang (Delete)\n";
        cout << "4. Jalankan Simulasi Etalase (Exception Handling)\n";
        cout << "5. Keluar\n";
        cout << "Pilihan Anda [1-5]: ";
        cin >> pilihanMenu;

        switch (pilihanMenu) {
            case 1:
                tambahBarang();
                break;
            case 2:
                updateBarang();
                break;
            case 3:
                deleteBarang();
                break;
            case 4:
                cout << "\n--- SIMULASI PENGAMBILAN BARANG ETALASE ---\n";
                
                // Skenario 1: Ambil indeks ke-1 (Harus Sukses)
                cout << "[Skenario 1] Mencoba mengambil barang di rak indeks ke-1...\n";
                try {
                    string barang = toko.ambilProduk(1);
                    cout << "Hasil: Sukses! Anda mengambil -> " << barang << "\n\n";
                } 
                catch (const runtime_error& e) {
                    cout << "Hasil: Terjadi Error -> " << e.what() << "\n\n";
                }

                // Skenario 2: Ambil indeks ke-5 (Harus Gagal & Tangkap Error Kustom)
                cout << "[Skenario 2] Mencoba mengambil barang di rak indeks ke-5...\n";
                try {
                    string barang = toko.ambilProduk(5);
                    cout << "Hasil: Sukses! Anda mengambil -> " << barang << "\n\n";
                } 
                catch (const runtime_error& e) {
                    // Menampilkan pesan error kustom yang di-throw dari class
                    cout << "Hasil: Terjadi Error -> " << e.what() << "\n\n";
                }
                
                cout << "-------------------------------------------\n";
                break;
            case 5:
                cout << "Terima kasih telah menggunakan sistem Toko Gibran Jaya!\n";
                break;
            default:
                cout << "Pilihan tidak valid! Silakan masukkan angka 1-5.\n";
                break;
        }

        // Penahan halaman agar user sempat membaca output sebelum menu di-refresh
        if (pilihanMenu != 5) {
            cout << "\nTekan Enter untuk kembali ke Menu Utama...";
            cin.ignore();
            cin.get();
        }

    } while (pilihanMenu != 5);

    return 0;
}