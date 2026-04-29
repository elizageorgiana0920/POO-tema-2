#include "Produs.h"
#include "Ingredient.h"

Produs::Produs(std::string nume_, float pret_)
    : nume(nume_), pretPreparare(pret_)
{
}

void Produs::afiseaza(std::ostream& os) const
{
    afisareDetalii(os);
}

float Produs::pretFinal() const
{
    return calculeazaPretFinal();
}

float Produs::getKcal() const
{
    return calculeazaKcalTotal();
}

int Produs::timpPregatire() const
{
    return getTimpPreparare();
}

void Produs::afisareDetalii(std::ostream& os) const
{
    os << "Produs: " << nume << " | Pret Preparare: " << pretPreparare << " RON";
}

void Produs::adaugaIngredient(Ingredient* ing)
{
    if (ing != nullptr)
    {
        ingrediente.push_back(ing);
    }
}

bool Produs::contineIngredient(const std::string& numeIng) const
{
    for (const auto* ing : ingrediente)
        if (ing && ing->getNume() == numeIng)
            return true;

    return false;
}

bool Produs::esteVegan() const
{
    for (const auto* ing : ingrediente)
        if (ing && !ing->getVegan())
            return false;

    return true;
}

bool Produs::esteFaraZahar() const
{
    for (const auto* ing : ingrediente)
        if (ing && !ing->getFaraZahar())
            return false;

    return true;
}

bool Produs::esteFaraLactoza() const
{
    for (const auto* ing : ingrediente)
        if (ing && !ing->getFaraLactoza())
            return false;

    return true;
}


bool Produs::verificareDisponibilitate() const
{
    return esteDisponibil();
}

float Produs::calculeazaKcalTotal() const
{
    float total = 0.0f;
    for (const auto* ing : ingrediente)
    {
        if (ing) total += ing->getKcal();
    }
    return total;
}

std::ostream& operator<<(std::ostream& os, const Produs& p)
{
    p.afiseaza(os);
    return os;
}
