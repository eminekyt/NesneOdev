#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>

// Taban sinif: Varlik
class Varlik {
protected:
    int x;
    int y;
    char sembol;

public:
    Varlik(int x, int y, char sembol) : x(x), y(y), sembol(sembol) {}
    virtual ~Varlik() {}

    int getX() const { return x; }
    int getY() const { return y; }
    char getSembol() const { return sembol; }

    void setPozisyon(int yeniX, int yeniY) {
        x = yeniX;
        y = yeniY;
    }
};

// Oyuncu sinifi: Varlik'tan turetiliyor
class Oyuncu : public Varlik {
private:
    int aclik;
    int susuzluk;

public:
    Oyuncu(int x, int y, int aclik, int susuzluk)
        : Varlik(x, y, '@'), aclik(aclik), susuzluk(susuzluk) {}

    void hareket(char tus, const std::vector<std::string>& harita) {
        int yeniX = x;
        int yeniY = y;

        if (tus == 'W' || tus == 'w') {
            yeniY--;
        } else if (tus == 'A' || tus == 'a') {
            yeniX--;
        } else if (tus == 'S' || tus == 's') {
            yeniY++;
        } else if (tus == 'D' || tus == 'd') {
            yeniX++;
        }

        // Harita sınırlarını kontrol et
        if (yeniX < 0 || yeniX >= harita[0].size() || yeniY < 0 || yeniY >= harita.size()) {
            std::cout << "Harita disina cikamazsiniz!" << std::endl;
            return;
        }

        // Engel kontrolü
        if (harita[yeniY][yeniX] == '#') {
            std::cout << "Engel var!" << std::endl;
            return;
        }

        // Pozisyonu güncelle
        setPozisyon(yeniX, yeniY);
        aclik--;
        susuzluk--;
    }

    bool kontrol() {
        if (aclik <= 0 || susuzluk <= 0) {
            std::cout << "Oyunu Kaybettiniz!" << std::endl;
            return false;
        }
        return true;
    }

    int getAclik() const { return aclik; }
    int getSusuzluk() const { return susuzluk; }
    void yemekBul() { aclik += 3; }
    void suBul() { susuzluk += 3; }
};

// Yemek sinifi: Varlik'tan turetiliyor
class Yemek : public Varlik {
public:
    Yemek(int x, int y) : Varlik(x, y, 'F') {}
};




int main() {
    OyunMotoru oyun;
    oyun.baslat();
    return 0;
}
