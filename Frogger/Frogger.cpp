#define IMM2D_WIDTH 200
#define IMM2D_HEIGHT 200
#define IMM2D_SCALE 3
#define IMM2D_IMPLEMENTATION
#include "immediate2d.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>
#include <cstdlib>
using namespace std;

class ObjektIgre {
protected:
    int PozicijaX;
    int PozicijaY;
    int Sirina;
    int Visina;
public:
    ObjektIgre(int sirina, int visina, int pozicijaX, int pozicijaY)
        : Sirina(sirina), Visina(visina), PozicijaX(pozicijaX), PozicijaY(pozicijaY) {}

    int getX() const {
        return PozicijaX;
    }

    int getY() const {
        return PozicijaY;
    }

    int getSirina() const {
        return Sirina;
    }

    int getVisina() const {
        return Visina;
    }

    void setX(int pozicijaX) {
        PozicijaX = pozicijaX;
    }

    void setY(int pozicijaY) {
        PozicijaY = pozicijaY;
    }

    void setSirina(int sirina) {
        Sirina = sirina;
    }

    void setVisina(int visina) {
        Visina = visina;
    }

    bool diraObjekt(ObjektIgre& objekt, int tolerancija) const {
        int veciX = PozicijaX;
        int manjiX = objekt.getX();
        int veciY = PozicijaY;
        int manjiY = objekt.getY();
        int vecaSirina = Sirina;
        int manjaSirina = objekt.getSirina();
        int vecaVisina = Visina;
        int manjaVisina = objekt.getVisina();
        if (manjaSirina > vecaSirina) {
            swap(manjiX, veciX);
            swap(manjaSirina, vecaSirina);
        }
        if (manjaVisina > vecaVisina) {
            swap(manjiY, veciY);
            swap(manjaVisina, vecaVisina);
        }
        if ((manjiX + tolerancija > veciX && manjiX + tolerancija < veciX + vecaSirina ||
            manjiX - tolerancija + manjaSirina > veciX && manjiX - tolerancija + manjaSirina < veciX + vecaSirina) &&
            (manjiY > veciY && manjiY < veciY + vecaVisina ||
                manjiY + manjaVisina > veciY && manjiY + manjaVisina < veciY + vecaVisina)) {
            return TRUE;
        }
        return FALSE;
    }

    bool jeNaEkranu() const {
        if (getX() < -getSirina() || getX() > 200) {
            return FALSE;
        }
        return TRUE;
    }
};

class ObjektSaNacrtom : public ObjektIgre {
protected:
    vector<string> Nacrt;
public:
    ObjektSaNacrtom(vector<string> nacrt, int pozicijaX, int pozicijaY) :
        ObjektIgre(nacrt[0].size(), nacrt.size(), pozicijaX, pozicijaY), Nacrt(nacrt) {}

    void setNacrt(vector<string> nacrt) {
        Nacrt = nacrt;
        Visina = nacrt.size();
        Sirina = nacrt[0].size();
    }

    vector<string> getNacrt() const {
        return Nacrt;
    }

    void nacrtaj(int novaPozicijaX, int novaPozicijaY) {
        for (int j = 0; j < Visina; j++) {
            for (int i = 0; i < Sirina; i++) {
                switch (Nacrt[j][i])
                {
                case 'g':
                    DrawPixel(novaPozicijaX + i, novaPozicijaY + j, Green);
                    break;
                case 'r':
                    DrawPixel(novaPozicijaX + i, novaPozicijaY + j, Red);
                    break;
                case 'y':
                    DrawPixel(novaPozicijaX + i, novaPozicijaY + j, Yellow);
                    break;
                case 'w':
                    DrawPixel(novaPozicijaX + i, novaPozicijaY + j, White);
                    break;
                case 'b':
                    DrawPixel(novaPozicijaX + i, novaPozicijaY + j, Black);
                    break;
                case 'p':
                    DrawPixel(novaPozicijaX + i, novaPozicijaY + j, Magenta);
                    break;
                case 'd':
                    DrawPixel(novaPozicijaX + i, novaPozicijaY + j, Brown);
                    break;
                default:
                    break;
                }
            }
        }
        PozicijaX = novaPozicijaX;
        PozicijaY = novaPozicijaY;
    }

    vector<string> rotiraniNacrtDole() const {
        vector<string> okrenutiNacrt;
        for (int i = Visina - 1; i >= 0; i--) {
            string redak = "";
            for (int j = Sirina - 1; j >= 0; j--) {
                redak.push_back(Nacrt[i][j]);
            }
            okrenutiNacrt.push_back(redak);
        }
        return okrenutiNacrt;
    }

    vector<string> rotiraniNacrtLijevo() const {
        vector<string> okrenutiNacrt;
        for (int i = Sirina - 1; i >= 0; i--) {
            string redak = "";
            for (int j = 0; j < Visina; j++) {
                redak.push_back(Nacrt[j][i]);
            }
            okrenutiNacrt.push_back(redak);
        }
        return okrenutiNacrt;
    }

    vector<string> rotiraniNacrtDesno() const {
        vector<string> okrenutiNacrt;
        for (int i = 0; i < Sirina; i++) {
            string redak = "";
            for (int j = Visina - 1; j >= 0; j--) {
                redak.push_back(Nacrt[j][i]);
            }
            okrenutiNacrt.push_back(redak);
        }
        return okrenutiNacrt;
    }

    vector<string> horizontalnoZrcalniNacrt() const {
        vector<string> okrenutiNacrt = Nacrt;
        for (int i = 0; i < Visina; i++) {
            reverse(okrenutiNacrt[i].begin(), okrenutiNacrt[i].end());
        }
        return okrenutiNacrt;
    }
};

class Zaba : public ObjektSaNacrtom {
private:
    int Skok = 20;
    char Smjer;
    vector<string> NacrtU;
    vector<string> NacrtD;
    vector<string> NacrtL;
    vector<string> NacrtR;

public:
    Zaba(int pozicijaX, int pozicijaY) : ObjektSaNacrtom({
        " g  ygyy  g ",
        "gg rgyygr gg",
        " g ggyygg g ",
        " ggygyyyggg ",
        "   ygyggy   ",
        " ggyyyyyygg ",
        " g ggyyyg g ",
        "gg  gygg  gg",
        " g        g "
        }, pozicijaX, pozicijaY), NacrtU(getNacrt()), NacrtD(rotiraniNacrtDole()),
        NacrtL(rotiraniNacrtLijevo()), NacrtR(rotiraniNacrtDesno()), Smjer(Up) {}

    void setNacrt(vector<string> nacrt, char smjer) {
        if (smjer != Smjer) {
            Nacrt = nacrt;
            Visina = nacrt.size();
            Sirina = nacrt[0].size();

            if (!(abs(smjer - Smjer) == 2 || smjer - Smjer == 0)) {
                if (Smjer == Up || Smjer == Down) {
                    setY(getY() - Sirina / 4);
                }
                else
                {
                    setY(getY() + Visina / 4);
                }
            }

            Smjer = smjer;
        }
    }

    int getSkok() const {
        return Skok;
    }

    char getSmjer() const {
        return Smjer;
    }

    vector<string> getOdrereniNacrt(char smjer) const {
        switch (smjer)
        {
        case Down:
            return NacrtD;
            break;
        case Left:
            return NacrtL;
            break;
        case Right:
            return NacrtR;
            break;
        default:
            return NacrtU;
            break;
        }
    }

    bool mozeSkociti(char smjer) const {
        switch (smjer)
        {
        case Up:
            if (PozicijaY - Skok >= 0) return TRUE;
            break;
        case Down:
            if (PozicijaY + Skok + Visina <= 180) return TRUE;
            break;
        case Right:
            if (PozicijaX + Skok + Sirina <= 200) return TRUE;
            break;
        case Left:
            if (PozicijaX - Skok >= 0) return TRUE;
            break;
        default:
            break;
        }
        return FALSE;
    }

    void skoci(char smjer) {
        switch (smjer)
        {
        case Up:
            PozicijaY -= Skok;
            break;
        case Down:
            PozicijaY += Skok;
            break;
        case Left:
            PozicijaX -= Skok;
            break;
        case Right:
            PozicijaX += Skok;
            break;
        }
        setNacrt(getOdrereniNacrt(smjer), smjer);
    }
};

class Prepreka : public ObjektSaNacrtom {
private:
    int Brzina;
public:
    Prepreka(int pozicijaX, int pozicijaY, int brzina, char bojaAuta) : ObjektSaNacrtom({
        "            pppp    ",
        "    pppp    pppp    ",
        "    pppp    pppp  yy",
        "     yy      yy   yy",
        "    " + string(13, bojaAuta) + " yy",
        "  " + string(3, bojaAuta) + "bbb" + string(7, bojaAuta) + "yyyyy",
        string(3, bojaAuta) + "bb" + string(12, bojaAuta) + " yy",
        string(3, bojaAuta) + "bb" + string(12, bojaAuta) + " yy",
        "  " + string(3, bojaAuta) + "bbb" + string(7, bojaAuta) + "yyyyy",
        "    " + string(13, bojaAuta) + " yy",
        "     yy      yy   yy",
        "    pppp    pppp  yy",
        "    pppp    pppp    ",
        "            pppp    "
        }, pozicijaX, pozicijaY), Brzina(brzina) {
        if (brzina > 0) {
            setNacrt(horizontalnoZrcalniNacrt());
        }
    }

        Prepreka(int pozicijaX, int pozicijaY, int brzina, int duljinaDrva) : ObjektSaNacrtom({
        "   d" + string(duljinaDrva, 'd') + "dddww   ",
        "  dd" + string(duljinaDrva, 'd') + "ddwddw  ",
        " ddd" + string(duljinaDrva, 'd') + "dwddddw ",
        " ddd" + string(duljinaDrva, 'd') + "dwddddw ",
        "dddd" + string(duljinaDrva, 'd') + "wddddddw",
        "dddd" + string(duljinaDrva, 'd') + "wddddddw",
        "dddd" + string(duljinaDrva, 'd') + "wddddddw",
        "dddd" + string(duljinaDrva, 'd') + "wddddddw",
        "dddd" + string(duljinaDrva, 'd') + "wddddddw",
        "dddd" + string(duljinaDrva, 'd') + "wddddddw",
        " ddd" + string(duljinaDrva, 'd') + "dwddddw ",
        " ddd" + string(duljinaDrva, 'd') + "dwddddw ",
        "  dd" + string(duljinaDrva, 'd') + "ddwddw  ",
        "   d" + string(duljinaDrva, 'd') + "dddww   "
        }, pozicijaX, pozicijaY), Brzina(brzina) {}

    int getBrzina() const {
        return Brzina;
    }
};

class Lopoc : public ObjektIgre {
private:
public:
    Lopoc(int pozicijaX, int pozicijaY) : ObjektIgre(20, 20, pozicijaX, pozicijaY) {
        nacrtaj(pozicijaX, pozicijaY);
    }

    void nacrtaj(int novaPozicijaX, int novaPozicijaY) {
        DrawCircle(novaPozicijaX + 10, novaPozicijaY + 10, 10, LightGreen);
        PozicijaX = novaPozicijaX;
        PozicijaY = novaPozicijaY;
    }
};

class Gumb : ObjektIgre {
private:
    string Tekst;
public:
    Gumb(int sirina, int visina, int pozicijaX, int pozicijaY, string tekst) : 
        ObjektIgre(sirina, visina, pozicijaX, pozicijaY), Tekst(tekst){
        nacrtaj(pozicijaX, pozicijaY);
    }

    void nacrtaj(int novaPozicijaX, int novaPozicijaY) {
        DrawString(PozicijaX, PozicijaY, Tekst.c_str(), "Arial", 12, White);
        DrawString(PozicijaX, PozicijaY, string(1, Tekst[0]).c_str(), "Arial", 12, Green);
        PozicijaX = novaPozicijaX;
        PozicijaY = novaPozicijaY;
    }

    bool jeKliknut() const {
        if ((MouseX() >= PozicijaX && MouseX() <= PozicijaX + Sirina && MouseY() >= PozicijaY && MouseY() <= PozicijaY + Visina) &&
            LeftMousePressed()) return TRUE;
        else return FALSE;
    }
};

class Igra {
private:
    int Tezina = 0;
    int Zivoti = 3;
    int Rezultat = 0;
    int Rekord = 0;
    int ZadnjiRezultat = 0;

    void nacrtajPozadinu() const {
        DrawRectangle(0, 0, 200, 20, Blue);
        DrawRectangle(0, 20, 200, 20, Blue);
        DrawRectangle(0, 40, 200, 20, Blue);
        DrawRectangle(0, 60, 200, 20, Blue);
        DrawRectangle(0, 80, 200, 20, LightGray);
        DrawRectangle(0, 160, 200, 20, LightGray);
        DrawString(0, 182, (string("Bodovi: ") + to_string(Rezultat)).c_str(), "Arial", 10, Green);
    }

    void pomakniPrepreke(vector<Prepreka>& prepreke) const {
        for (auto& prepreka : prepreke) {
            if (prepreka.getBrzina() > 0)
                prepreka.nacrtaj(prepreka.getX() + prepreka.getBrzina() + Tezina, prepreka.getY());
            else
                prepreka.nacrtaj(prepreka.getX() + prepreka.getBrzina() - Tezina, prepreka.getY());
            if (prepreka.getX() < -prepreka.getSirina() && prepreka.getBrzina() < 0) {
                prepreka.nacrtaj(200, prepreka.getY());
            }
            else if (prepreka.getX() > 200 + prepreka.getSirina() && prepreka.getBrzina() > 0) {
                prepreka.nacrtaj(-prepreka.getSirina(), prepreka.getY());
            }
        }
    }

    void makniZivot(Zaba& igrac) {
        Zivoti--;
        Wait(1000);
        igrac.setNacrt(igrac.getOdrereniNacrt(Up), Up);
        igrac.nacrtaj(94, 166);
        ClearInputBuffer();
    }

    void glavniIzbornikTekst() const {
        DrawString(55, 30, "Frogger", "Arial", 20, Green);
        DrawString(40, 60, "Napravio: Bojan Radulovic", "Arial", 8, White);
        DrawString(0, 162, (string("Zadnji rezultat: ") + to_string(ZadnjiRezultat)).c_str(), "Arial", 8, White);
        DrawString(0, 182, (string("Rekord: ") + to_string(Rekord)).c_str(), "Arial", 8, White);
    }

    int provjeraSudara(Zaba& igrac, vector<Prepreka>& prepreke, int tolerancija) const {
        for (unsigned int i = 0; i < prepreke.size(); i++) {
            if (prepreke[i].diraObjekt(igrac, tolerancija)) {
                return i;
            }
        }
        return -1;
    }

public:
    Igra() {
        ucitajRezultate();
    }

    void glavniIzbornik() {

        Clear();
        ClearInputBuffer();
        glavniIzbornikTekst();
        Gumb kreni(40, 17, 81, 85, "Kreni");
        Gumb izlaz(40, 16, 83, 105, "Izlaz");
        Present();

        while (TRUE)
        {
            Wait(60);
            char k = LastBufferedKey();
            if (k == 'k' || k == 'K' || k == Enter || kreni.jeKliknut()) {
                resetirajVarijable();
                while (Zivoti)
                {
                    pokreniIgru();
                }
                zapisiRezultate();
                return;
            }
            else if (k == 'i' || k == 'I' || k == Esc || izlaz.jeKliknut()) {
                CloseWindow();
            }
        }
    }
    void pokreniIgru() {
        Clear();

        int nasumicnaX1 = rand() % 121;
        int nasumicnaX2 = rand() % 121;
        int nasumicnaX3 = rand() % 121;
        int razmakAutiju = 80;

        vector<Prepreka> auti = { Prepreka(nasumicnaX1, 143, -1, 'r'), Prepreka(nasumicnaX1 + razmakAutiju, 143, -1, 'r'),
            Prepreka(nasumicnaX2, 123, 2, 'w'), Prepreka(nasumicnaX2 - razmakAutiju, 123, 2, 'w'),
            Prepreka(nasumicnaX3, 103, -3, 'g'), Prepreka(nasumicnaX3 + razmakAutiju, 103, -3, 'g') };

        nasumicnaX1 = rand() % 101;
        nasumicnaX2 = rand() % 101;
        nasumicnaX3 = rand() % 101;
        int razmakDrva = 100;

        vector<Prepreka> drva = { Prepreka(nasumicnaX1 - 70, 63, 1, 58), Prepreka(nasumicnaX1 - 70 + razmakDrva, 63, 1, 58),
            Prepreka(nasumicnaX2 - 85, 43, -1, 73), Prepreka(nasumicnaX2 - 85 + razmakDrva, 43, -1, 73),
            Prepreka(nasumicnaX3 - 50, 23, 2, 38), Prepreka(nasumicnaX3 - 50 + razmakDrva, 23, 2, 38) };

        Lopoc lopoci[4] = { Lopoc(12, 1), Lopoc(63, 1), Lopoc(114, 1), Lopoc(165, 1) };

        Zaba igrac(94, 166);

        Zaba ikone[3] = { Zaba(140, 186), Zaba(160, 186), Zaba(180, 186) };

        int inputDelay = 1;
        int inputDelayTrenutni = inputDelay;

        chrono::steady_clock::time_point tStart;
        chrono::steady_clock::time_point tKraj;
        int prosloVrijeme;
        while (Zivoti)
        {
            tStart = chrono::high_resolution_clock::now();

            Clear();
            nacrtajPozadinu();

            // crtanje lopoca
            for (auto& lopoc : lopoci) {
                lopoc.nacrtaj(lopoc.getX(), lopoc.getY());
            }

            // crtanje ikona zivota
            for (int i = 0; i < Zivoti; i++) {
                ikone[i].nacrtaj(ikone[i].getX(), ikone[i].getY());
            }
            
            // pomak drva
            pomakniPrepreke(drva);

            // inputi
            if (inputDelayTrenutni <= 0) {
                inputDelayTrenutni = inputDelay;
                char k = LastBufferedKey();
                switch (k)
                {
                case Up:
                case Down:
                case Right:
                case Left:
                    if (igrac.mozeSkociti(k)) {
                        igrac.skoci(k);
                    }
                    break;
                case Esc:
                    Zivoti = 0;
                    return;
                    break;
                default:
                    inputDelayTrenutni = 0;
                    break;
                }
            }
            else
            {
                inputDelayTrenutni -= 1;
            }
            igrac.nacrtaj(igrac.getX(), igrac.getY());
            ClearInputBuffer();

            // pomak autiju
            pomakniPrepreke(auti);

            Present();

            // povjera je li zaba na ekranu
            if (!igrac.jeNaEkranu()) {
                makniZivot(igrac);
            }

            // detektiranje sudara auti
            if (igrac.getY() > 75) {
                if (provjeraSudara(igrac, auti, 1) != -1) {
                    makniZivot(igrac);
                }
            }
            
            // detektiranje sudara drva
            if (igrac.getY() < 70 && igrac.getY() > 20) {
                int i = provjeraSudara(igrac, drva, 3);
                if (i != -1) {
                    if (drva[i].getBrzina() >= 0)
                        igrac.nacrtaj(igrac.getX() + drva[i].getBrzina() + Tezina, igrac.getY());
                    else
                        igrac.nacrtaj(igrac.getX() + drva[i].getBrzina() - Tezina, igrac.getY());
                }
                else
                {
                    makniZivot(igrac);
                }
            }

            // detektiranje sudara lopoci
            if (igrac.getY() < 20) {
                bool dira_lopoc = FALSE;
                for (auto& lopoc : lopoci) {
                    if (lopoc.diraObjekt(igrac, 3)) {
                        Rezultat++;
                        if (!(Rezultat % 4)) {
                            Tezina++;
                            return;
                        }
                        igrac.nacrtaj(94, 166);
                        lopoc.nacrtaj(-50, -50);
                        dira_lopoc = TRUE;
                    }
                }
                if (!dira_lopoc)
                {
                    makniZivot(igrac);
                }
            }

            // postavljanje fiksne stope crtanja
            // Napomena: funkcija Sleep() je preneprecizna pa koristimo Spin-lock tehniku 
            do {
                tKraj = chrono::high_resolution_clock::now();
                prosloVrijeme = int(chrono::duration<double, milli>(tKraj - tStart).count());
            } while (prosloVrijeme < 75);
        }
    }

    void ucitajRezultate() {
        string zapis = "";
        ifstream Rezultati("FroggerBodovi.txt");
        if (Rezultati.good()) {
            getline(Rezultati, zapis);
            ZadnjiRezultat = stoi(zapis);

            getline(Rezultati, zapis);
            Rekord = stoi(zapis);
        }
        Rezultati.close();
    }

    void zapisiRezultate() {
        ZadnjiRezultat = Rezultat;
        if (ZadnjiRezultat > Rekord) {
            Rekord = ZadnjiRezultat;
        }
        ofstream Rezultati("FroggerBodovi.txt");
        if (Rezultati.good()) {
            Rezultati << ZadnjiRezultat << '\n';
            Rezultati << Rekord;
        }
        Rezultati.close();
    }

    void resetirajVarijable() {
        Tezina = 0;
        Zivoti = 3;
        Rezultat = 0;
    }
};

void run()
{
    srand((unsigned int)time(NULL));
    UseDoubleBuffering(TRUE);
    Igra igra;
    while (TRUE)
    {
        igra.glavniIzbornik();
    }
}
