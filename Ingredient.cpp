#include "Ingredient.h"
#include "Exceptii.h"
#include <utility>

/// Constructor implicit
Ingredient::Ingredient()
    : nume(""), stoc(0), pret(0.0f), vegan(false),
      faraZahar(false), faraLactoza(false), kcal(0.0f)
{
}

/// Constructor de inițializare, folosesc std::move pentru eficienta la copierea string-ului
Ingredient::Ingredient(std::string nume, int stoc, float pret, bool vegan, bool faraZahar, bool faraLactoza, float kcal)
    : nume(std::move(nume)), stoc(stoc), pret(pret), vegan(vegan),
      faraZahar(faraZahar), faraLactoza(faraLactoza), kcal(kcal)
{
}

/// Getteri
const std::string& Ingredient::getNume() const
{
    return nume;
}

int Ingredient::getStoc() const
{
    return stoc;
}

float Ingredient::getPret() const
{
    return pret;
}

bool Ingredient::getVegan() const
{
    return vegan;
}

bool Ingredient::getFaraZahar() const
{
    return faraZahar;
}

bool Ingredient::getFaraLactoza() const
{
    return faraLactoza;
}

float Ingredient::getKcal() const
{
    return kcal;
}

///metode
void Ingredient::consumaStoc()
{
    if (stoc > 0)
        stoc--;
    else
        throw StocInsuficientException(nume);///daca stocul == 0 -> exceptie
}


void Ingredient::reaprovizionare(int cantitate)
{
    if (cantitate >= 0)
        stoc += cantitate;
}

bool Ingredient::atentieStocMic() const
{
    return stoc < 5;
}

bool Ingredient::esteInStoc() const
{
    return stoc > 0;
}

///operator de afisare
std::ostream& operator<<(std::ostream& os, const Ingredient& ing)
{
    os << "Ingredient: " << ing.nume
       << " | Stoc: " << ing.stoc
       << " | Pret: " << ing.pret << " RON"
       << " | Kcal: " << ing.kcal
       << "\n  - Vegan: " << (ing.vegan ? "Da" : "Nu")
       << " | Fara Zahar: " << (ing.faraZahar ? "Da" : "Nu")
       << " | Fara Lactoza: " << (ing.faraLactoza ? "Da" : "Nu");
    return os;
}

/// Operator de citire
std::istream& operator>>(std::istream& is, Ingredient& ing)
{
    is >> ing.nume >> ing.stoc >> ing.pret >> ing.vegan >> ing.faraZahar >> ing.faraLactoza >> ing.kcal;
    return is;
}
