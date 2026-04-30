#include "Bautura.h"
#include "Exceptii.h"
#include <utility>

Bautura::Bautura(std::string nume, float pretPrep, int timpPrep, bool calda)
    : Produs(std::move(nume), pretPrep, timpPrep), esteCalda(calda) {}

/// Constructor de copiere
Bautura::Bautura(const Bautura& other)
    : Produs(other),
      listaIngrediente(other.listaIngrediente),
      toppinguriExtra(other.toppinguriExtra),
      esteCalda(other.esteCalda) {}

void swap(Bautura& a, Bautura& b)
{
    using std::swap;
    swap(static_cast<Produs&>(a), static_cast<Produs&>(b));
    swap(a.listaIngrediente, b.listaIngrediente);
    swap(a.toppinguriExtra, b.toppinguriExtra);
    swap(a.esteCalda, b.esteCalda);
}

///operator de atribuire
Bautura& Bautura::operator=(Bautura other)
{
    swap(*this, other);
    return *this;
}

void Bautura::adaugaIngredient(Ingredient* ing)
{
    if (ing == nullptr) throw DateInvalideException("Ingredient invalid.");
    listaIngrediente.push_back(ing);
}

/// client: adauga topping si scade stocul imediat
void Bautura::adaugaToppingExtra(Ingredient* top)
{
    if (top == nullptr) throw DateInvalideException("Topping invalid.");
    if (!top->esteInStoc()) throw StocInsuficientException(top->getNume());

    toppinguriExtra.push_back(top);
    top->consumaStoc();
}

float Bautura::calculeazaPretFinal() const
{
    float total = pretPreparare;
    for (const auto* ing : listaIngrediente) total += ing->getPret();
    for (const auto* top : toppinguriExtra)  total += top->getPret(); /// Plus topping
    return total;
}

///functie de verificare a stocului ingredientelor pentru o reteta
bool Bautura::esteDisponibil() const
{
    if (listaIngrediente.empty()) return false;
    for (const auto* ing : listaIngrediente)
    {
        if (!ing->esteInStoc()) return false;
    }
    return true;
}

///kcal pe toata reteta+toppinguri
float Bautura::calculeazaKcalTotal() const
{
    float total = 0;
    for (const auto* ing : listaIngrediente) total += ing->getKcal();
    for (const auto* top : toppinguriExtra)  total += top->getKcal();
    return total;
}

///verifica specificatiile si pentru bautura si pentru toppinguri

///vegan bautura +vegan toppings
bool Bautura::esteVegan() const
{
    for (const auto* ing : listaIngrediente) if (!ing->getVegan()) return false;
    for (const auto* top : toppinguriExtra)  if (!top->getVegan()) return false;
    return true;
}

bool Bautura::esteFaraZahar() const
{
    for (const auto* ing : listaIngrediente) if (!ing->getFaraZahar()) return false;
    for (const auto* top : toppinguriExtra)  if (!top->getFaraZahar()) return false;
    return true;
}

bool Bautura::esteFaraLactoza() const
{
    for (const auto* ing : listaIngrediente) if (!ing->getFaraLactoza()) return false;
    for (const auto* top : toppinguriExtra)  if (!top->getFaraLactoza()) return false;
    return true;
}

std::shared_ptr<Produs> Bautura::clone() const
{
    return std::make_shared<Bautura>(*this);
}


///afisare
void Bautura::afisareDetalii(std::ostream& os) const
{

    os << "  Specificatii: " << (esteCalda ? "calda" : "rece") << " | ";
    os << (esteVegan() ? "vegan" : "") << " | ";
    os << (esteFaraZahar() ? "fara zahar" : "") << " | ";
    os << (esteFaraLactoza() ? "fara lactoza" : "") << " | ";
    os << "  Pret total: " << calculeazaPretFinal() << " RON | ";
    os << "  Calorii: " << calculeazaKcalTotal() << " kcal | ";
    os << "  Stoc: " << (esteDisponibil() ? "Disponibil" : "Stoc epuizat");
    if (!toppinguriExtra.empty())
    {
        os << " | Extra: ";
        for (size_t i = 0; i < toppinguriExtra.size(); ++i)
        {
            os << toppinguriExtra[i]->getNume() << (i == toppinguriExtra.size() - 1 ? "" : ", ");
        }
    }
    os << "\n";
}
