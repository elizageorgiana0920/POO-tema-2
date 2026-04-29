#ifndef BAUTURA_H
#define BAUTURA_H
#include "Produs.h"

class Bautura : public Produs
{
protected:
    int timpPreparare_;
    bool esteCalda;

private:
    void afisareDetalii(std::ostream& os) const override;

public:
    Bautura(std::string nume = "", float pretPreparare = 0.0f,
            int timpPreparare = 0, bool esteCalda = true);

    Bautura(const Bautura& other);
    Bautura& operator=(Bautura other);

    friend void swap(Bautura& a, Bautura& b);

    ///constructor virtual
    std::shared_ptr<Produs> clone() const override;

    float calculeazaPretFinal() const override;

    int getTimpPreparare() const override
    {
        return timpPreparare_;
    }

    bool esteDisponibil() const override;
};

#endif