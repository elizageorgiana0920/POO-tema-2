#include "Patiserie.h"
#include "Ingredient.h"
#include <utility>

Patiserie::Patiserie(std::string nume_, float pret_, int valabilitate_)
    : Produs(std::move(nume_), pret_), valabilitate(valabilitate_)
{
    dataFabricatiei = std::time(nullptr);
}

Patiserie::Patiserie(const Patiserie& other)
    : Produs(other), dataFabricatiei(other.dataFabricatiei), valabilitate(other.valabilitate)
{
}

void swap(Patiserie& a, Patiserie& b)
{
    using std::swap;
    swap(a.nume, b.nume);
    swap(a.pretPreparare, b.pretPreparare);
    swap(a.ingrediente, b.ingrediente);
    swap(a.dataFabricatiei, b.dataFabricatiei);
    swap(a.valabilitate, b.valabilitate);
}

Patiserie& Patiserie::operator=(Patiserie other)
{
    swap(*this, other);
    return *this;
}

std::shared_ptr<Produs> Patiserie::clone() const
{
    return std::make_shared<Patiserie>(*this);
}

bool Patiserie::esteProaspat() const
{
    std::time_t acum = std::time(nullptr);
    return (acum - dataFabricatiei) < (static_cast<long>(valabilitate) * 3600);
}

bool Patiserie::esteDisponibil() const
{
    if (!esteProaspat()) return false;
    for (const auto* ing : ingrediente)
        if (ing && !ing->esteInStoc())
            return false;
    return true;
}

void Patiserie::marcheazaExpirat()
{
    if (!esteProaspat())
    {
        for (auto* ing : ingrediente)
        {
            if (ing != nullptr)
                ing->reaprovizionare(-ing->getStoc());
        }
    }
}

float Patiserie::calculeazaPretFinal() const
{
    if (!esteProaspat())
        return 0.0f;

    float pret = pretPreparare;
    std::time_t acum = std::time(nullptr);

    long secundeTrecute = acum - dataFabricatiei;
    long secundeTotal = static_cast<long>(valabilitate) * 3600;

    // Daca a trecut mai mult de 70% din timp, reducere 30%
    if (secundeTrecute > (secundeTotal * 0.7))
        pret *= 0.7f;

    return pret;
}

void Patiserie::afisareDetalii(std::ostream& os) const
{
    os << "Patiserie: " << nume << " | Pret: " << pretFinal() << " RON | Stare: ";
    if (!esteProaspat())
        os << "EXPIRAT!!!";
    else
    {
        os << "Proaspat ";
        if (pretFinal() < pretPreparare)
            os << "- Expira curand => reducere 30%";
    }
}
