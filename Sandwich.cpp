#include "Sandwich.h"
#include "Exceptii.h"
#include <utility>
#include <ctime>

Sandwich::Sandwich(std::string nume, float pretBaza, int timpPrep, std::time_t expiraLa,
                   int stocInitial, bool veg, bool fZahar, bool fLactoza,
                   float calorii, bool poateIncalzi)
    : Produs(std::move(nume), pretBaza, timpPrep),
      dataExpirarii(expiraLa), stoc(stocInitial), vegan(veg),
      faraZahar(fZahar), faraLactoza(fLactoza), kcal(calorii),
      poateFiIncalzit(poateIncalzi), vreaIncalzit(false) {}

///Se apeleaza doar daca clientul doreste incalzire
void Sandwich::cereIncalzit()
{
    if (!poateFiIncalzit)
    {
        /// Aruncam exceptia daca reteta nu permite incalzirea
        throw OperatiuneInvalidaException(this->nume, "Incalzire");
    }
    vreaIncalzit = true;
}

///Adaos 50% + 2 RON taxa de incalzire (daca este cazul)
float Sandwich::calculeazaPretFinal() const
{
    float pret = pretPreparare * 1.5f;
    if (vreaIncalzit)
    {
        pret += 2.0f;
    }
    return pret;
}

bool Sandwich::esteExpirat() const
{
    return std::time(nullptr) > dataExpirarii;
}

bool Sandwich::esteDisponibil() const
{
    return stoc > 0 && !esteExpirat();
}

std::shared_ptr<Produs> Sandwich::clone() const
{
    return std::make_shared<Sandwich>(*this);
}

void Sandwich::afisareDetalii(std::ostream& os) const
{
    os << "  Specificatii: " << kcal << " kcal | "
       << (vegan ? "Vegan | " : "")
       << (faraZahar ? "Fara Zahar | " : "")
       << (faraLactoza ? "Fara Lactoza | " : "")
       << (!esteExpirat() ? "In termen | " : "EXPIRAT! | ")
       << (vreaIncalzit ? "Servit Cald" : "Servit Rece");

    /// Timpul de preparare creste simbolic cu 60s daca se incalzeste
    int timpEfectiv = timpPreparare + (vreaIncalzit ? 60 : 0);
    os << "\n  Timp: " << timpEfectiv << "s | Stoc: " << stoc << " buc | Pret: " << calculeazaPretFinal() << " RON\n";
}

// Copy-and-Swap
void swap(Sandwich& a, Sandwich& b)
{
    using std::swap;
    swap(static_cast<Produs&>(a), static_cast<Produs&>(b));
    swap(a.dataExpirarii, b.dataExpirarii);
    swap(a.stoc, b.stoc);
    swap(a.vegan, b.vegan);
    swap(a.faraZahar, b.faraZahar);
    swap(a.faraLactoza, b.faraLactoza);
    swap(a.kcal, b.kcal);
    swap(a.poateFiIncalzit, b.poateFiIncalzit);
    swap(a.vreaIncalzit, b.vreaIncalzit);
}

Sandwich::Sandwich(const Sandwich& other)
    : Produs(other), dataExpirarii(other.dataExpirarii), stoc(other.stoc),
      vegan(other.vegan), faraZahar(other.faraZahar),
      faraLactoza(other.faraLactoza), kcal(other.kcal),
      poateFiIncalzit(other.poateFiIncalzit), vreaIncalzit(other.vreaIncalzit) {}

Sandwich& Sandwich::operator=(Sandwich other)
{
    swap(*this, other);
    return *this;
}
