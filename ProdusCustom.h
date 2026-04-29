#ifndef PRODUSCUSTOM_H
#define PRODUSCUSTOM_H

#include "Bautura.h"
#include <vector>

class ProdusCustom : public Bautura
{
private:
    std::vector<Ingredient*> extras;
    void afisareDetalii(std::ostream& os) const override;

public:
    explicit ProdusCustom(const Bautura& baza);

    ProdusCustom(const ProdusCustom& other);
    ProdusCustom& operator=(ProdusCustom other);
    friend void swap(ProdusCustom& a, ProdusCustom& b);

    std::shared_ptr<Produs> clone() const override;

    void adaugaExtra(Ingredient* ing);
    float calculeazaPretFinal() const override;

    friend ProdusCustom operator+(const ProdusCustom& pc, Ingredient* ing)
    {
        ProdusCustom copie(pc);
        copie.adaugaExtra(ing);
        return copie;
    }
};

#endif
