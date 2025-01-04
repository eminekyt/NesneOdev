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

class Su : public Varlik {
public:
    Su(int x, int y) : Varlik(x, y, 'W') {}
};

// Harita sinifi
class Harita {
private:
    int genislik;
    int yukseklik;
    std::vector<std::string> harita;
    Oyuncu* oyuncu;
    std::vector<Yemek> yemekler;
    std::vector<Su> sular;

public:
    Harita(int genislik, int yukseklik, Oyuncu* oyuncu)
        : genislik(genislik), yukseklik(yukseklik), oyuncu(oyuncu) {
        harita = std::vector<std::string>(yukseklik, std::string(genislik, ' '));
        for (int i = 0; i < genislik; i++) {
            harita[0][i] = '#';
            harita[yukseklik - 1][i] = '#';
        }
        for (int i = 0; i < yukseklik; i++) {
            harita[i][0] = '#';
            harita[i][genislik - 1] = '#';
        }

        oyuncu->setPozisyon(1, 1);
        harita[oyuncu->getY()][oyuncu->getX()] = oyuncu->getSembol();

        srand(time(0));
        for (int i = 0; i < 3; i++) {
            int x = rand() % (genislik - 2) + 1;
            int y = rand() % (yukseklik - 2) + 1;
            yemekler.emplace_back(x, y);
            harita[y][x] = 'F';

            x = rand() % (genislik - 2) + 1;
            y = rand() % (yukseklik - 2) + 1;
            sular.emplace_back(x, y);
            harita[y][x] = 'W';
        }
    }

    void ciz() {
        for (const auto& satir : harita) {
            std::cout << satir << std::endl;
        }
    }

    void guncelle() {
        for (auto& satir : harita) {
            std::fill(satir.begin(), satir.end(), ' ');
        }
        for (int i = 0; i < genislik; i++) {
            harita[0][i] = '#';
            harita[yukseklik - 1][i] = '#';
        }
        for (int i = 0; i < yukseklik; i++) {
            harita[i][0] = '#';
            harita[i][genislik - 1] = '#';
        }
        harita[oyuncu->getY()][oyuncu->getX()] = oyuncu->getSembol();
        for (auto it = yemekler.begin(); it != yemekler.end(); ) {
            if (it->getX() == oyuncu->getX() && it->getY() == oyuncu->getY()) {
                oyuncu->yemekBul();
                it = yemekler.erase(it);
            } else {
                harita[it->getY()][it->getX()] = it->getSembol();
                ++it;
            }
        }
        for (auto it = sular.begin(); it != sular.end(); ) {
            if (it->getX() == oyuncu->getX() && it->getY() == oyuncu->getY()) {
                oyuncu->suBul();
                it = sular.erase(it);
            } else {
                harita[it->getY()][it->getX()] = it->getSembol();
                ++it;
            }
        }
    }

    char kontrolHedef(int x, int y) {
        return harita[y][x];
    }

    void temizleHedef(int x, int y) {
        harita[y][x] = ' ';
    }

    const std::vector<std::string>& getHarita() const {
        return harita;
    }
};

// Oyun Motoru sinifi
class OyunMotoru {
private:
    Oyuncu oyuncu;
    Harita harita;
    bool oyunDevam;

public:
    OyunMotoru() : oyuncu(1, 1, 8, 8), harita(10, 10, &oyuncu) {
        oyunDevam = true;
    }

    void baslat() {
        std::cout << "Hos Geldiniz! Ac ve Susuzluk Oyununa!" << std::endl;
        oyunDongusu();
    }

    void oyunDongusu() {
        while (oyunDevam) {
            harita.guncelle();
            harita.ciz();
            std::cout << "Aclik: " << oyuncu.getAclik() << "  Susuzluk: " << oyuncu.getSusuzluk() << std::endl;
            char tus;
            std::cout << "Hareket etmek icin bir tus girin (W/A/S/D): ";
            std::cin >> tus;
            oyuncu.hareket(tus, harita.getHarita());
            oyunDevam = oyuncu.kontrol();
        }
    }
};


int main() {
    OyunMotoru oyun;
    oyun.baslat();
    return 0;
}
