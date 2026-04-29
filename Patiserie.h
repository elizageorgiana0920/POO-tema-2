#ifndef PATISERIE_H
#define PATISERIE_H

#include"Produs.h"
#include <ctime>

class Patiserie : public Produs
{
private:
    std::time_t dataFabricatiei;
    int valabilitate; ///este in ore
    void afisareDetalii(std::ostream& os) const override;

public:
    Patiserie(std::string nume = "", float pret = 0.0f, int valabilitate = 0);

    Patiserie(const Patiserie& other);
    Patiserie& operator=(Patiserie other);
    friend void swap(Patiserie& a, Patiserie& b);

    std::shared_ptr<Produs> clone() const override;

    float calculeazaPretFinal() const override;
    int getTimpPreparare() const override { return 15; }
    bool esteDisponibil() const override;

    bool esteProaspat() const;
    void marcheazaExpirat(); ///pune stocul pe 0
};

#endif
