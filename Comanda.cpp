#include "Comanda.h"
#include "Gestiune.h"
#include "Produs.h"
#include <iostream>
#include <fstream>
#include<ctime>
#include<iomanip>
#include<sstream>

int Comanda::numarComenzi = 0;
int Comanda::timpOcupatBarista = 0;

Comanda::Comanda() : idComanda(++numarComenzi), totalPlata(0.0f), timpPregatireComandaCurenta(0)
{
    std::time_t t = std::time(nullptr);
    std::tm* acum = std::localtime(&t);
    std::stringstream ss;

    ss << std::setfill('0') << std::setw(2) << acum->tm_mday << "/"
        << std::setfill('0') << std::setw(2) << (acum->tm_mon + 1) << "/"
        << (acum->tm_year + 1900) << " "
        << std::setfill('0') << std::setw(2) << acum->tm_hour << ":" <<
        std::setfill('0') << std::setw(2) << acum->tm_min;

    this->dataOraComanda = ss.str();
    this->codRidicare = "QR-" + std::to_string(idComanda) + "-" + std::to_string(t % 1000);
}


void Comanda::adaugaProdus(std::shared_ptr<Produs> p)
{
    if (p != nullptr)
    {
        produseComandate.push_back(p);
        totalPlata += p->pretFinal();
    }
}

int Comanda::calculeazaTimpAsteptareTotal() const
{
    int timpTotal = 0;
    for (const auto& p : produseComandate)
        timpTotal += p->timpPregatire();

    return timpTotal;
}

void Comanda::finalizeazaComanda(Gestiune& g)
{
    for (auto& p : produseComandate)
        g.proceseazaComanda(p.get());

    this->timpPregatireComandaCurenta = calculeazaTimpAsteptareTotal();
    int timpTotalAsteptare = timpOcupatBarista + timpPregatireComandaCurenta;
    timpOcupatBarista += timpPregatireComandaCurenta;

    float profitComanda = totalPlata * 0.4f;

    ///generare bon fiscal intr un txt
    std::string numeFisier = "Bon_Fiscal_" + std::to_string(idComanda) + ".txt";
    std::ofstream bon(numeFisier);
    if (bon.is_open())
    {
        bon << "=========BON FISCAL NR. " << idComanda << "=========\n";
        bon << "Produse comandate: " << produseComandate.size() << "\n";
        bon << "TOTAL DE PLATA: " << totalPlata << " RON\n";
        bon << "TIMP ASTEPTARE: " << timpTotalAsteptare << " secunde\n";
        bon << "=======================================================\n";
        bon.close();

        std::cout << "Comanda " << idComanda << " a fost finalizata. Bonul " << numeFisier << " a fost emis.\n";
    }

    std::string dateComanda = "Data: " + dataOraComanda + " | Suma: " + std::to_string(totalPlata) +
        " RON | Profit: " + std::to_string(profitComanda) + " RON";

    g.adaugaInRegistru(dateComanda);
}

void Comanda::afisareSumar() const
{
    std::cout << "Comanda #" << idComanda << " | Produse: " << produseComandate.size() << " | Total: " << totalPlata <<
        " RON\n";
}
