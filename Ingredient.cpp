#include "Ingredient.h"
#include "Exceptii.h"
#include <utility>


Ingredient::Ingredient()
    : stoc(0), pret(0.0f), vegan(false), faraZahar(false), faraLactoza(false), kcal(0.0f)
{
}

Ingredient::Ingredient(std::string nume_, int stoc_, float pret_,
                       bool vegan_, bool faraZahar_, bool faraLactoza_, float kcal_)
    : nume(std::move(nume_)), stoc(stoc_), pret(pret_),
      vegan(vegan_), faraZahar(faraZahar_), faraLactoza(faraLactoza_), kcal(kcal_)
{
}

void Ingredient::consumaStoc()
{
    if (stoc > 0)
        stoc--;
    else
        throw StocInsuficientException(nume);
}

void Ingredient::reaprovizionare(int cantitate)
{
    if (cantitate >= 0)
        stoc += cantitate;
}

std::ostream& operator<<(std::ostream& os, const Ingredient& ing)
{
    os << "Ingredient: " << ing.nume << " | Stoc: " << ing.stoc << " | Pret: " << ing.pret << " RON | Kcal: " << ing.
        kcal;
    if (ing.vegan)
        os << " | Vegan ";
    if (ing.faraLactoza)
        os << "| Fara Lactoza | ";
    if (ing.faraZahar)
        os << "Fara Zahar";
    os << "\n";
    return os;
}

std::istream& operator>>(std::istream& is, Ingredient& ing)
{
    is >> ing.nume >> ing.stoc >> ing.pret >> ing.vegan >> ing.faraZahar >> ing.faraLactoza >> ing.kcal;
    return is;
}
