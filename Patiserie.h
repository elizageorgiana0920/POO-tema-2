#ifndef PATISERIE_H
#define PATISERIE_H

#include "Produs.h"
#include <ctime>
#include <memory>

class Patiserie : public Produs
{
private:
    std::time_t dataExpirarii; ///format unix timestamp
    int stoc;
    bool vegan;
    bool faraZahar;
    bool faraLactoza;
    float kcal;

protected:
    void afisareDetalii(std::ostream& os) const override;

public:

    explicit Patiserie(std::string nume = "", float pretPrep = 0.0f, int timpPrep = 15,
                       std::time_t expiraLa = 0, int stocInitial = 0,
                       bool veg = false, bool fZahar = false, bool fLactoza = false, float calorii = 0.0f);

    ///construct d ecopiere, op de atribuire, swap
    Patiserie(const Patiserie& other);
    Patiserie& operator=(Patiserie other);
    friend void swap(Patiserie& a, Patiserie& b);

    ///polimorgism, crearea unei copii identice
    std::shared_ptr<Produs> clone() const override;


    float calculeazaPretFinal() const override;///aduna pret per ingredient + pret preparare
    bool esteDisponibil() const override;
    bool esteVegan() const override
    {
        return vegan;
    }
    bool esteFaraZahar() const override
    {
        return faraZahar;
    }
    bool esteFaraLactoza() const override
    {
        return faraLactoza;
    }
    float calculeazaKcalTotal() const override
    {
        return kcal;
    }
    inline void scadeStoc(){if (stoc>0) stoc--;}
    bool esteExpirat() const;
    int getStoc() const { return stoc; }
    void marcheazaExpirat();///stoc=0 daca esteExpirat este true

    ///nu avem ingrediente care ne duc spre clasa ingrediente, asa ca initializez cu o lista goala
    const std::vector<Ingredient*>& getIngrediente() const override
    {
        static const std::vector<Ingredient*> listaVida;
        return listaVida;
    }
};

#endif
