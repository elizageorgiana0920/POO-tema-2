#ifndef GESTIUNE_H
#define GESTIUNE_H

#include <vector>
#include <string>
#include <map>
#include <memory>
#include "Ingredient.h"
#include "Produs.h"

class Gestiune {
private:
    std::vector<Ingredient> listaIngrediente;
    std::vector<Ingredient> listaToppinguri;
    std::vector<std::shared_ptr<Produs>> meniu;
    std::vector<std::string> istoricComenzi;

    static float profitTotal;

    std::map<std::string, int> frecventaProduse;
    std::map<std::string, int> frecventaIngrediente;
    std::map<int, int> comenziPerOra;

public:
    Gestiune();
    ~Gestiune();

    // Incarcare date din fisiere
    void incarcaIngrediente(const std::string& fisier);
    void incarcaToppinguri(const std::string& fisier);
    void incarcaMeniuBauturi(const std::string& fisier);
    void incarcaMeniuPatiserie(const std::string& fisier);
    void incarcaMeniuSandwich(const std::string& fisier);

    // Operatii Meniu (Client)
    void afisareMeniu(bool doarDisponibile = false) const;
    void afisareMeniuPersonalizat(bool vegan, bool fZahar, bool fLactoza, const std::string& numeIng = "", bool saContina = true) const;
    void cautaSiAfiseazaProdus(const std::string& nume) const;

    // Operatii Manager (Admin)
    void afisareStocuriIngrediente() const;
    void reaprovizionare(const std::string& numeIng, int cantitate);
    void verificaPatiserieExpirata();
    void afisareRapoarte() const;

    // Helperi cautare
    Produs* gasesteProdusDupaNume(const std::string& nume) const;
    Ingredient* gasesteIngredient(const std::string& nume);
    Ingredient* gasesteTopping(const std::string& nume);

    // Procesare
    void proceseazaComanda(std::shared_ptr<Produs> p, int ora);
    static float getProfitTotal() { return profitTotal; }
};

#endif