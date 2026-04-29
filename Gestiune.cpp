#include "Gestiune.h"
#include "Produs.h"
#include "Bautura.h"
#include "Patiserie.h"
#include "Exceptii.h"
#include <iostream>
#include <fstream>
#include <algorithm>

float Gestiune::profitTotal = 0.0f;

Gestiune::Gestiune()
{
    std::ifstream f("profit_total.txt");
    if (!(f >> profitTotal))
        profitTotal = 0.0f;
}

Gestiune::~Gestiune()
{
    std::ofstream fregistru("registru.txt");
    for (const auto& log : istoricComenzi)
        fregistru << log << "\n";

    std::ofstream fprofit("profit_total.txt");
    fprofit << profitTotal;

    verificaStocuriCritice();
}

void Gestiune::incarcaIngrediente(std::string& fisier)
{
    std::ifstream fin(fisier);
    if (!fin)
        throw DateInvalideException("Nu s-a putut deschide ingrediente.txt!");
    Ingredient temp;
    while (fin >> temp)
        listaIngrediente.push_back(temp);
}

void Gestiune::incarcaMeniuBauturi(const std::string& fisier)
{
    std::ifstream fin(fisier);
    std::string nume;
    float pret;
    int timp, nrIng;
    bool calda;

    while (fin >> nume >> pret >> timp >> calda >> nrIng)
    {
        auto b = std::make_shared<Bautura>(nume, pret, timp, calda);
        for (int i = 0; i < nrIng; i++)
        {
            std::string numeIng;
            fin >> numeIng;
            Ingredient* ref = gasesteIngredient(numeIng);
            if (ref)
                b->adaugaIngredient(ref);
        }
        meniu.push_back(b);
    }
}

void Gestiune::afisareMeniu(bool doarDisponibile) const
{
    std::cout << "\n==========MENIU ELIZA'S COFFEE SHOP ==========\n";
    int idx = 1;
    for (const auto& p : meniu)
    {
        if (doarDisponibile && !p->esteDisponibil())
            continue;
        std::cout << idx++ << ". ";
        p->afiseaza(std::cout);
        std::cout << "\n";
    }
}

void Gestiune::verificaPatiserieExpirata()
{
    for (auto& p : meniu)
    {
        Patiserie* pat = dynamic_cast<Patiserie*>(p.get());
        if (pat != nullptr)
            pat->marcheazaExpirat();
    }
}

void Gestiune::proceseazaComanda(Produs* p)
{
    if (!p->esteDisponibil())
        throw StocInsuficientException(p->getNume());

    for (auto* ing : p->getIngrediente())
    {
        if (ing)
        {
            ing->consumaStoc();
            frecventaIngrediente[ing->getNume()]++;
        }
    }

    float profit = p->pretFinal() * 0.4f;
    profitTotal += profit;
    frecventaProduse[p->getNume()]++;
}

void Gestiune::adaugaInRegistru(const std::string& intrare)
{
    istoricComenzi.push_back(intrare);
}

void Gestiune::afisareRapoarte() const
{
    std::cout << "\n========RAPOARTE BUSINESS ============\n";
    if (!frecventaProduse.empty())
    {
        std::string celMaiVandut;
        int maxV = 0;
        for (const auto& pereche : frecventaProduse)
            if (pereche.second > maxV)
            {
                maxV = pereche.second;
                celMaiVandut = pereche.first;
            }
        std::cout << "Cel mai vandut produs: " << celMaiVandut << " (" << maxV << " comenzi)\n";
    }

    if (!frecventaIngrediente.empty())
    {
        std::string celMaiFolosit;
        int maxI = 0;
        for (const auto& pereche : frecventaIngrediente)
            if (pereche.second > maxI)
            {
                maxI = pereche.second;
                celMaiFolosit = pereche.first;
            }
        std::cout << "Cel mai folosit ingredient: " << celMaiFolosit << " (" << maxI << " utilizari)\n";
    }

    if (!comenziPerOra.empty())
    {
        int oraVarf = 0, maxO = 0;
        for (const auto& pereche : comenziPerOra)
            if (pereche.second > maxO)
            {
                maxO = pereche.second;
                oraVarf = pereche.first;
            }
        std::cout << "Ora de varf: " << oraVarf << ":00 (" << maxO << " comenzi)\n";
    }
    afisareProfitTotal();
}


void Gestiune::incarcaMeniuPatiserie(const std::string& fisier)
{
    std::ifstream fin(fisier);
    if (!fin)
        throw DateInvalideException("Nu s-a putut deschide patiserie.txt!");

    std::string nume;
    float pret;
    int valabilitate;
    while (fin >> nume >> pret >> valabilitate)
    {
        meniu.push_back(std::make_shared<Patiserie>(nume, pret, valabilitate));
    }
}

Ingredient* Gestiune::gasesteIngredient(const std::string& numeCauta)
{
    for (auto& ing : listaIngrediente)
        if (ing.getNume() == numeCauta)
            return &ing;
    return nullptr;
}


void Gestiune::afisareMeniuFiltrat(const std::string& numeIng, bool contine) const
{
    std::cout << "\n---Produse ";
    if (contine)
        std::cout << "cu ";
    else
        std::cout << "fara ";

    std::cout << numeIng << " ---\n";

    bool gasit = false;

    for (const auto& p : meniu)
    {
        if (p && p->contineIngredient(numeIng) == contine)
        {
            std::cout << " - " << p->getNume() << "\n";
            gasit = true;
        }
    }

    if (!gasit)
        std::cout << "Nu am gasit produse!\n";
}

void Gestiune::afisareProduseFaraIngredient(const std::string& numeIng) const
{
    std::cout << "Optiuni fara  " << numeIng << ":\n";
    for (const auto& p : meniu)
        if (!p->contineIngredient(numeIng))
            std::cout << " - " << p->getNume() << "\n";
}

void Gestiune::afisareMeniuVegan() const
{
    std::cout << "\n---Produse vegane---\n";
    for (const auto& p : meniu)
        if (p && p->esteVegan() && p->esteDisponibil())
            std::cout << " - " << p->getNume() << " | " << p->getPretFinal() << " RON \n";
}

void Gestiune::afisareMeniuFaraZahar() const
{
    std::cout << "\n---Produse fara zahar---\n";
    for (const auto& p : meniu)
        if (p->esteFaraZahar() && p->esteDisponibil())
            std::cout << " - " << p->getNume() << " | " << p->getPretFinal() << " RON \n";
}

void Gestiune::afisareMeniuFaraLactoza() const
{
    std::cout << "\n---Produse fara lactoza---\n";
    for (const auto& p : meniu)
        if (p->esteFaraLactoza() && p->esteDisponibil())
            std::cout << " - " << p->getNume() << " | " << p->getPretFinal() << " RON \n";
}

Produs* Gestiune::cautaProdus(const std::string& nume) const
{
    for (const auto& p : meniu)
        if (p->getNume() == nume)
            return p.get();
    throw ProdusInexistentException(nume);
}

void Gestiune::verificaStocuriCritice() const
{
    std::ofstream fout("reaprovizionare.txt");
    fout << "---RAPORT REAPROVIZIONARE URGENTA, STOC REDUS (<5) ---\n";
    for (const auto& ing : listaIngrediente)
        if (ing.atentieStocMic())
            fout << "ID: " << ing.getNume() << " | Stoc: " << ing.getStoc() << "\n";
}


void Gestiune::reaprovizionare(const std::string& numeIng, int cantitate)
{
    Ingredient* ing = gasesteIngredient(numeIng);
    if (!ing)
        throw DateInvalideException("Ingredientul " + numeIng + " nu exista!");

    ing->reaprovizionare(cantitate);
    std::cout << "Stoc actualizat: " << ing->getNume() << " -> " << ing->getStoc() << "\n";
}


void Gestiune::inregistreazaOraComanda(int ora)
{
    comenziPerOra[ora]++;
}


void Gestiune::afisareProfitTotal() const
{
    std::cout << "Profit total, din toate sesiunile: " << profitTotal << " RON\n";
}
