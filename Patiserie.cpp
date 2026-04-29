#include "Patiserie.h"
#include <utility>
#include <ctime>

Patiserie::Patiserie(std::string nume, float pretPrep, int timpPrep, std::time_t expiraLa,
                   int stocInitial, bool veg, bool fZahar, bool fLactoza, float calorii)
    : Produs(std::move(nume), pretPrep, timpPrep),
      dataExpirarii(expiraLa),
      stoc(stocInitial),
      vegan(veg),
      faraZahar(fZahar),
      faraLactoza(fLactoza),
      kcal(calorii) {}

/// Adaos de 40% (Pret de baza * 1.4)
float Patiserie::calculeazaPretFinal() const {
    return pretPreparare * 1.4f;
}

bool Patiserie::esteExpirat() const {
    return std::time(nullptr) > dataExpirarii;
}

void Patiserie::marcheazaExpirat() {
    stoc = 0;
}

bool Patiserie::esteDisponibil() const {
    return stoc > 0 && !esteExpirat();
}

std::shared_ptr<Produs> Patiserie::clone() const {
    return std::make_shared<Patiserie>(*this);
}

void Patiserie::afisareDetalii(std::ostream& os) const {
    os << "  Specificatii: " << kcal << " kcal | "
       << (vegan ? "Vegan | "  : "")
       << (faraZahar ? "Fara Zahar | " : "")
       << (faraLactoza ? "Fara Lactoza | " : "")
       << (!esteExpirat() ? "In termen\n" : "Expirat!\n";
}

// Copy-and-Swap actualizat cu noile campuri
void swap(Patiserie& a, Patiserie& b) {
    using std::swap;
    swap(static_cast<Produs&>(a), static_cast<Produs&>(b));
    swap(a.dataExpirarii, b.dataExpirarii);
    swap(a.stoc, b.stoc);
    swap(a.vegan, b.vegan);
    swap(a.faraZahar, b.faraZahar);
    swap(a.faraLactoza, b.faraLactoza);
    swap(a.kcal, b.kcal);
}

Patiserie::Patiserie(const Patiserie& other)
    : Produs(other), dataExpirarii(other.dataExpirarii), stoc(other.stoc),
      vegan(other.vegan), faraZahar(other.faraZahar),
      faraLactoza(other.faraLactoza), kcal(other.kcal) {}

Patiserie& Patiserie::operator=(Patiserie other) {
    swap(*this, other);
    return *this;
}