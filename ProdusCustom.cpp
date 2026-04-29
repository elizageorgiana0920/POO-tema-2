#include "ProdusCustom.h"
#include "Ingredient.h"
#include<utility>

ProdusCustom::ProdusCustom(const Bautura& b) : Bautura(b)
{
}

ProdusCustom::ProdusCustom(const ProdusCustom& other)
    : Bautura(other), extras(other.extras)
{
}

void swap(ProdusCustom& a, ProdusCustom& b)
{
    using std::swap;
    swap(static_cast<Bautura&>(a), static_cast<Bautura&>(b));
    swap(a.extras, b.extras);
}

ProdusCustom& ProdusCustom::operator=(ProdusCustom other)
{
    swap(*this, other);
    return *this;
}

std::shared_ptr<Produs> ProdusCustom::clone() const
{
    return std::make_shared<ProdusCustom>(*this);
}

void ProdusCustom::adaugaExtra(Ingredient* ing)
{
    if (ing != nullptr)
        extras.push_back(ing);
}

float ProdusCustom::calculeazaPretFinal() const
{
    float total = Bautura::calculeazaPretFinal();
    for (const auto* e : extras)
        if (e) total += e->getPret();
    return total;
}

void ProdusCustom::afisareDetalii(std::ostream& os) const
{
    os << "Bautura: ";
    Bautura::afisareDetalii(os);
    if (!extras.empty())
    {
        os << "\n   + EXTRA: ";
        for (const auto* e : extras)
        {
            if (e)
                os << e->getNume() << " (+" << e->getPret() << " RON) ";
        }
    }
}
