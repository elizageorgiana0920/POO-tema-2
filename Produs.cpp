#include "Produs.h"
#include <utility>

/// Constructorul clasei de baza
Produs::Produs(std::string nume, float pretPrep, int timpPrep)
    : nume(std::move(nume)), pretPreparare(pretPrep), timpPreparare(timpPrep) {}

/// Implementarea interfetei non-virtuale (NVI)
void Produs::afiseaza(std::ostream& os) const {
    os << "------------------------------------------\n";
    os << "Produs: " << nume << " (" << timpPreparare << "s preparare)\n";

    /// Apel polimorfic către detaliile specifice fiecărei clase (Bautura, Patiserie, Sandwich)
    afisareDetalii(os);

    /// Afișăm prețul final calculat polimorfic la sfârșit, pentru consistență
    os << "Pret final de vanzare: " << calculeazaPretFinal() << " RON\n";
}

/// Implementare de baza pentru pretul final.
/// In Bautura va fi: pretBaza + ingrediente
/// In Patiserie va fi: pretBaza * 1.4
float Produs::calculeazaPretFinal() const {
    return pretPreparare;
}

/// Operatorul << care sustine polimorfismul
std::ostream& operator<<(std::ostream& os, const Produs& p) {
    p.afiseaza(os);
    return os;
}