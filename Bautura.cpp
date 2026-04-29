#include "Bautura.h"
#include "Ingredient.h"
#include <utility>

Bautura::Bautura(std::string nume_, float pretPreparare_, int timp_, bool calda_)
    : Produs(std::move(nume_), pretPreparare_),
      timpPreparare_(timp_), esteCalda(calda_)
{
}

Bautura::Bautura(const Bautura& other) :
    Produs(other),
    timpPreparare_(other.timpPreparare_),
    esteCalda(other.esteCalda)
{
}

void swap(Bautura& a, Bautura& b)
{
    using std::swap;
    swap(a.nume, b.nume);
    swap(a.pretPreparare, b.pretPreparare);
    swap(a.ingrediente, b.ingrediente);
    swap(a.timpPreparare_, b.timpPreparare_);
    swap(a.esteCalda, b.esteCalda);
}

Bautura& Bautura::operator=(Bautura other)
{
    swap(*this, other);
    return *this;
}

std::shared_ptr<Produs> Bautura::clone() const
{
    return std::make_shared<Bautura>(*this);
}

float Bautura::calculeazaPretFinal() const
{
    float total = pretPreparare;
    for (const auto* ing : ingrediente)
        if (ing != nullptr)
            total += ing->getPret();

    if (esteVegan())
        total *= 0.8f;

    return total;
}

bool Bautura::esteDisponibil() const
{
    if (ingrediente.empty())
        return true;
    for (const auto* ing : ingrediente)
        if (ing && !ing->esteInStoc())
            return false;
    return true;
}

void Bautura::afisareDetalii(std::ostream& os) const
{
    os << "Bautura: " << nume << " | Temperatura: ";
    if (esteCalda)
        os << "Calda ";
    else
        os << "Rece ";

    os << "| Timp: " << timpPreparare_ << "s" << " | Pret Final: " << pretFinal() << " RON | Kcal: " << getKcal();
    if (esteVegan()) os << " | VEGAN ";
    if (esteFaraZahar()) os << " | FARA ZAHAR ";
    if (esteFaraLactoza()) os << " | FARA LACTOZA ";
    if (!esteDisponibil()) os << " | STOC EPUIZAT !!!";
}
