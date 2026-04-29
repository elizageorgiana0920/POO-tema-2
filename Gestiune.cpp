#include "Gestiune.h"
#include "Bautura.h"
#include "Patiserie.h"
#include "Sandwich.h"
#include "Exceptii.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>

float Gestiune::profitTotal = 0.0f;

Gestiune::Gestiune() {
    std::ifstream f("profit_total.txt");
    if (!(f >> profitTotal)) profitTotal = 0.0f;
}

Gestiune::~Gestiune() {
    std::ofstream fprofit("profit_total.txt");
    fprofit << profitTotal;
    // Aici s-ar putea adauga salvarea stocului inapoi in fisier
}

// --- INCARCARE DATE ---

void Gestiune::incarcaIngrediente(const std::string& fisier) {
    std::ifstream fin(fisier);
    if (!fin) throw DateInvalideException("Fisier ingrediente lipsa!");
    Ingredient temp;
    while (fin >> temp) listaIngrediente.push_back(temp);
}

void Gestiune::incarcaToppinguri(const std::string& fisier) {
    std::ifstream fin(fisier);
    if (!fin) return;
    Ingredient temp;
    while (fin >> temp) listaToppinguri.push_back(temp);
}

void Gestiune::incarcaMeniuBauturi(const std::string& fisier) {
    std::ifstream fin(fisier);
    std::string nume, numeIng;
    float pretBaza;
    int timp, calda;
    while (fin >> nume >> pretBaza >> timp >> calda) {
        auto b = std::make_shared<Bautura>(nume, pretBaza, timp, calda != 0);
        while (fin.peek() != '\n' && fin >> numeIng) {
            Ingredient* ref = gasesteIngredient(numeIng);
            if (ref) b->adaugaIngredient(ref);
        }
        meniu.push_back(b);
    }
}

void Gestiune::incarcaMeniuPatiserie(const std::string& fisier) {
    std::ifstream fin(fisier);
    std::string nume; float pret; int timp, stoc, v, z, l;
    std::time_t expira; float kcal;
    while (fin >> nume >> pret >> timp >> expira >> stoc >> v >> z >> l >> kcal) {
        meniu.push_back(std::make_shared<Patiserie>(nume, pret, timp, expira, stoc, v!=0, z!=0, l!=0, kcal));
    }
}

void Gestiune::incarcaMeniuSandwich(const std::string& fisier) {
    std::ifstream fin(fisier);
    std::string nume; float pret; int timp, stoc, v, z, l, incalz;
    std::time_t expira; float kcal;
    while (fin >> nume >> pret >> timp >> expira >> stoc >> v >> z >> l >> kcal >> incalz) {
        meniu.push_back(std::make_shared<Sandwich>(nume, pret, timp, expira, stoc, v!=0, z!=0, l!=0, kcal, incalz!=0));
    }
}

// --- FILTRARE SI CAUTARE ---

void Gestiune::afisareMeniu(bool doarDisponibile) const {
    std::cout << "\n========== MENIU COMPLET ==========\n";
    for (const auto& p : meniu) {
        if (doarDisponibile && !p->esteDisponibil()) continue;
        p->afiseaza(std::cout);
    }
}

void Gestiune::afisareMeniuPersonalizat(bool vegan, bool fZahar, bool fLactoza, const std::string& numeIng, bool saContina) const {
    std::cout << "\n========== FILTRARE PERSONALIZATA ==========\n";
    bool gasit = false;
    for (const auto& p : meniu) {
        if (vegan && !p->esteVegan()) continue;
        if (fZahar && !p->esteFaraZahar()) continue;
        if (fLactoza && !p->esteFaraLactoza()) continue;

        if (!numeIng.empty()) {
            bool areIng = false;
            for (auto* ing : p->getIngrediente()) {
                if (ing && ing->getNume() == numeIng) { areIng = true; break; }
            }
            if (saContina && !areIng) continue;
            if (!saContina && areIng) continue;
        }

        if (p->esteDisponibil()) {
            p->afiseaza(std::cout);
            gasit = true;
        }
    }
    if (!gasit) std::cout << "Niciun produs nu corespunde filtrelor.\n";
}

void Gestiune::cautaSiAfiseazaProdus(const std::string& nume) const {
    for (const auto& p : meniu) {
        if (p->getNume() == nume) { p->afiseaza(std::cout); return; }
    }
    std::cout << "Produsul nu a fost gasit.\n";
}

// --- LOGICA ADMIN ---

void Gestiune::afisareStocuriIngrediente() const {
    std::cout << "\n--- STOCURI INGREDIENTE ---\n";
    for (const auto& ing : listaIngrediente) {
        std::cout << std::left << std::setw(20) << ing.getNume() << ": " << ing.getStoc()
                  << (ing.atentieStocMic() ? " [REAPROVIZIONARE!]" : "") << "\n";
    }
}

void Gestiune::reaprovizionare(const std::string& numeIng, int cantitate) {
    Ingredient* ing = gasesteIngredient(numeIng);
    if (ing) ing->reaprovizionare(cantitate);
}

void Gestiune::verificaPatiserieExpirata() {
    for (auto& p : meniu) {
        auto pat = std::dynamic_pointer_cast<Patiserie>(p);
        if (pat && pat->esteExpirat()) pat->marcheazaExpirat();
    }
}

// --- PROCESARE SI RAPOARTE ---

void Gestiune::proceseazaComanda(std::shared_ptr<Produs> p, int ora) {
    if (!p->esteDisponibil()) throw StocInsuficientException(p->getNume());

    // Consumam ingredientele (pentru Bauturi)
    for (auto* ing : p->getIngrediente()) {
        ing->consumaStoc();
        frecventaIngrediente[ing->getNume()]++;
    }

    profitTotal += (p->calculeazaPretFinal() - p->getPretBaza());
    frecventaProduse[p->getNume()]++;
    comenziPerOra[ora]++;
}

void Gestiune::afisareRapoarte() const {
    std::cout << "\n======== RAPORT BUSINESS ========\n";
    if (!frecventaProduse.empty()) {
        auto top = std::max_element(frecventaProduse.begin(), frecventaProduse.end(),
                   [](const auto& a, const auto& b){ return a.second < b.second; });
        std::cout << "Top Produs: " << top->first << " (" << top->second << ")\n";
    }
    std::cout << "Profit Total: " << profitTotal << " RON\n";
}

// --- HELPERI ---

Ingredient* Gestiune::gasesteIngredient(const std::string& nume) {
    for (auto& ing : listaIngrediente) if (ing.getNume() == nume) return &ing;
    return nullptr;
}

Ingredient* Gestiune::gasesteTopping(const std::string& nume) {
    for (auto& top : listaToppinguri) if (top.getNume() == nume) return &top;
    return nullptr;
}

Produs* Gestiune::gasesteProdusDupaNume(const std::string& nume) const {
    for (const auto& p : meniu) if (p->getNume() == nume) return p.get();
    return nullptr;
}