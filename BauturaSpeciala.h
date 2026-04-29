#ifndef BAUTURASPECIALA_H
#define BAUTURASPECIALA_H

#include "Bautura.h"
#include <string>
#include <iostream>

class BauturaSpeciala: public Bautura
{
private:
    std::string descriere;
    float pretExtra;
    void afisareDetalii(std::ostream& os) const override;

public:
    BauturaSpeciala(std::string nume="", float pretPreparare=0.0f,
        int timpPreparare=0, bool esteCalda=true, std::string descriere="", float pretExtra=0.0f);

    BauturaSpeciala(const BauturaSpeciala& other);
    BauturaSpeciala& operator=(BauturaSpeciala other);
    friend void swap(BauturaSpeciala& a, BauturaSpeciala& b);

    std::shared_ptr<Produs> clone() const override;
    float calculeazaPretFinal() const override;
};

#endif