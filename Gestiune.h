
#ifndef GESTIUNE_H
#define GESTIUNE_H

#include <vector>
#include <string>
#include <map>
#include <memory>
#include "Ingredient.h"
#include "Produs.h"
#include "Comanda.h"

class Gestiune {
private:
    ///pointeri obiecte fizice
    std::vector<Ingredient> listaIngrediente;
    std::vector<Ingredient> listaToppinguri;
    ///clasa de baza, pemite amestecarea bauturi, sandwich
    std::vector<std::shared_ptr<Produs>> meniu;

    std::vector<std::string> istoricComenzi;
    std::vector<std::shared_ptr<Comanda>> coadaComenzi;

    static float profitTotal;

    ///asociaza o cheie cu o valoare
    std::map<std::string, int> frecventaProduse;///ora
    std::map<std::string, int> frecventaIngrediente;///nume
    std::map<int, int> comenziPerOra;

public:
    Gestiune();
    ~Gestiune();

    /// Incarcare date din fisiere
    void incarcaIngrediente(const std::string& fisier);
    void incarcaToppinguri(const std::string& fisier);
    void incarcaMeniuBauturi(const std::string& fisier);
    void incarcaMeniuPatiserie(const std::string& fisier);
    void incarcaMeniuSandwich(const std::string& fisier);

void vizualizareComenziRecente() const;
    /// Operatii Meniu (Client)
    void afisareMeniu(bool doarDisponibile = false) const;
    void afisareMeniuPersonalizat(bool vegan, bool fZahar, bool fLactoza, const std::string& numeIng = "", bool saContina = true) const;
    void cautaSiAfiseazaProdus(const std::string& nume) const;
void afisareLogisticaPreparare();
    /// Operatii Manager
    void afisareStocuriIngrediente() const;
    void reaprovizionare(const std::string& numeIng, int cantitate);
    void verificaPatiserieExpirata();
    void verificaSandwichExpirat();
    void afisareRapoarte() const;
    void afisareToateStocurile() const;      /// Pentru Barista (toate)
    void afisareIngredienteCritice() const; /// Pentru Manager (doar < 5)
    void afisareProduseCriticePatiserieSandwich() const;

    /// pentru cautare
    Produs* gasesteProdusDupaNume(const std::string& nume) const;
    Ingredient* gasesteIngredient(const std::string& nume);
    Ingredient* gasesteTopping(const std::string& nume);

    /// Procesare
    void proceseazaComanda(std::shared_ptr<Produs> p, int ora);
    static float getProfitTotal() { return profitTotal; }
};

#endif
