#include "Produs.h"
#include <utility>

/// Constructorul clasei de baza
Produs::Produs(std::string nume, float pretPrep, int timpPrep)
    : nume(std::move(nume)), pretPreparare(pretPrep), timpPreparare(timpPrep) {}

///copy and swap
void swap(Produs& a, Produs& b)
{
    using std::swap;
    swap(a.nume, b.nume);
    swap(a.pretPreparare, b.pretPreparare);
    swap(a.timpPreparare, b.timpPreparare);
}

/// Implementarea interfetei non-virtuale (NVI)
void Produs::afiseaza(std::ostream& os) const
{
    os << "------------------------------------------\n";
    os << "Produs: " << nume << " (" << timpPreparare << "s preparare)\n";

    /// aici sunt detaliile specifice fiecărei clase (Bautura, Patiserie, Sandwich)
    afisareDetalii(os);
}

/// Implementare de baza pentru pretul final.
float Produs::calculeazaPretFinal() const
{
    return pretPreparare;
}

/// Operatorul << afisare pentru polimorfism
std::ostream& operator<<(std::ostream& os, const Produs& p)
{
    p.afiseaza(os);
    return os;
}
