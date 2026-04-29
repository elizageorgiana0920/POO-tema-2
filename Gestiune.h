#ifndef GESTIUNE_H
#define GESTIUNE_H

#include <vector>
#include <string>
#include <map>
#include <memory>
#include "Ingredient.h"

class Produs;

class Gestiune
{
private:
    std::vector<Ingredient> listaIngrediente;
    std::vector<std::shared_ptr<Produs>> meniu;
    std::vector<std::string> istoricComenzi;

    static float profitTotal;

    std::map<std::string, int> frecventaProduse;
    std::map<std::string, int> frecventaIngrediente;
    std::map<int, int> comenziPerOra;

public:
    Gestiune();
    ~Gestiune();

    ///incarca date
    void incarcaIngrediente(std::string& fisier);
    void incarcaMeniuBauturi(std::string& fisier);
    void incarcaMeniuPatiserie(std::string& fisier);

    ///operatii meniu
    void afisareMeniu(bool doarDisponibile = false) const;
    void afisareMeniuFiltrat(const std::string& numeIng, bool contine) const;
    void afisareMeniuVegan() const;
    void afisareMeniuFaraZahar() const;
    void afisareMeniuFaraLactoza() const;

    Produs* cautaProdus(const std::string& nume) const;
    Ingredient* gasesteIngredient(const std::string& nume);

    void verificaStocuriCritice() const;
    void reaprovizionare(const std::string& numeIng, int cantitate);
    void verificaPatiserieExpirata();

    static float getProfitTotal() { return profitTotal; }

    ///procesare comanda
    void proceseazaComanda(Produs* p);
    void adaugaInRegistru(const std::string& intrare);
    void inregistreazaOraComanda(int ora);

    ///raport
    void afisareRapoarte() const;
    void afisareProfitTotal() const;
};
#endif
