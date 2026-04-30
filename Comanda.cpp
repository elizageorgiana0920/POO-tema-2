#include "Comanda.h"
#include "Gestiune.h"
#include "Produs.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <sstream>

/// Inițializarea membrilor statici
int Comanda::numarComenzi = 0;
int Comanda::timpOcupatBarista = 0;

Comanda::Comanda() : idComanda(++numarComenzi), totalPlata(0.0f) {
    /// GENERARE TIMESTAMP
    std::time_t t = std::time(nullptr);
    std::tm* acum = std::localtime(&t);
    std::stringstream ss;

    /// Formatare dată: DD/MM/YYYY HH:MM
    ss << std::setfill('0') << std::setw(2) << acum->tm_mday << "/"
       << std::setfill('0') << std::setw(2) << (acum->tm_mon + 1) << "/"
       << (acum->tm_year + 1900) << " "
       << std::setfill('0') << std::setw(2) << acum->tm_hour << ":"
       << std::setfill('0') << std::setw(2) << acum->tm_min;

    this->dataOraComanda = ss.str();

    /// cod unic pentru client
    this->codRidicare = "QR-" + std::to_string(idComanda) + "-" + std::to_string(t % 1000);
}
///aici se creeaza o comanda pas cu pas, cu pointeri la Produs si total
void Comanda::adaugaProdus(std::shared_ptr<Produs> p) {
    if (p != nullptr) {
        produseComandate.push_back(p);

        totalPlata += p->getPretFinal();
    }
}
void Comanda::afisareSumarConsola() const {

    std::cout << "\n--- Sumar Comanda Curenta ---\n";

    for (const auto& p : produseComandate) {

        std::cout << " + " << p->getNume() << " (" << p->getPretFinal() << " RON)\n";

    }

    std::cout << "TOTAL DE PLATA: " << totalPlata << " RON\n";

}

void Comanda::finalizeazaComanda(Gestiune& g) {
    std::time_t t = std::time(nullptr);
    int oraCurenta = std::localtime(&t)->tm_hour;

    /// Transmitem produsele clasei Gestiune pentru a scădea stocul și a aduna profitul
    for (auto& p : produseComandate) {
        g.proceseazaComanda(p, oraCurenta);
    }

    /// Timpul de așteptare pentru acest client = timpul celor din fața lui + timpul produselor sale
    int timpPregatire = 60;
    int timpAsteptareFinal = timpOcupatBarista + timpPregatire;
    timpOcupatBarista += timpPregatire; /// Actualizăm coada pentru următorul client

    /// folosesc std::ios::app (append) pentru a adăuga bonul la final, fără a șterge istoricul
    std::ofstream fisierIstoric("registru.txt", std::ios::app);

    if (fisierIstoric.is_open()) {
        fisierIstoric << "------------------------------------------\n";
        fisierIstoric << "BON FISCAL #" << idComanda << " | DATA: " << dataOraComanda << "\n";
        fisierIstoric << "COD RIDICARE: " << codRidicare << "\n";
        fisierIstoric << "PRODUSE:\n";

        for (const auto& p : produseComandate) {
            fisierIstoric << " - " << p->getNume() << " : " << p->getPretFinal() << " RON\n";
        }

        fisierIstoric << "TOTAL: " << totalPlata << " RON\n";
        fisierIstoric << "TIMP ASTEPTARE ESTIMAT: " << timpAsteptareFinal << " secunde\n";
        fisierIstoric << "------------------------------------------\n\n";
        fisierIstoric.close();
    }

    std::cout << "\n Comanda finalizata cu succes! Cod ridicare: " << codRidicare << "\n";
}
