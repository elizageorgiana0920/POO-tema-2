
#ifndef SANDWICH_H
#define SANDWICH_H

#include "Produs.h"
#include <ctime>
#include <memory>

class Sandwich : public Produs
{
private:
    std::time_t dataExpirarii;
    int stoc;
    bool vegan, faraZahar, faraLactoza;
    float kcal;
    bool poateFiIncalzit; /// Proprietate a retetei (din fisier)
    bool vreaIncalzit;    /// De la client (default false)

protected:
    void afisareDetalii(std::ostream& os) const override;

public:
    explicit Sandwich(std::string nume = "", float pretBaza = 0.0f, int timpPrep = 60,
                      std::time_t expiraLa = 0, int stocInitial = 0,
                      bool veg = false, bool fZahar = true, bool fLactoza = false,
                      float calorii = 0.0f, bool poateIncalzi = false);

    Sandwich(const Sandwich& other);
    Sandwich& operator=(Sandwich other);
    friend void swap(Sandwich& a, Sandwich& b);

    std::shared_ptr<Produs> clone() const override;

    float calculeazaPretFinal() const override;
    bool esteDisponibil() const override;

    /// Getters pentru NVI (interfata Produs)
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

    /// Metode specifice pentru fluxul de comanda
    bool esteExpirat() const;
    void cereIncalzit();

    ///nu are ingrediente
    const std::vector<Ingredient*>& getIngrediente() const override
    {
        static const std::vector<Ingredient*> listaVida; /// Un singur vector gol pentru toata clasa
        return listaVida;
    }
};

#endif
