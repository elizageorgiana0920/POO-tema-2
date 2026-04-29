#include "BauturaSpeciala.h"
#include<utility>


BauturaSpeciala::BauturaSpeciala(std::string nume_, float pretPreparare_, int timp_,
    bool calda_, std::string descriere_, float pretExtra_)
    :Bautura(std::move(nume_), pretPreparare_, timp_, calda_),
descriere(std::move(descriere_)), pretExtra(pretExtra_){}


BauturaSpeciala::BauturaSpeciala(const BauturaSpeciala& other)
: Bautura(other), descriere(other.descriere), pretExtra(other.pretExtra){}

void swap(BauturaSpeciala& a, BauturaSpeciala& b) {
    using std::swap;
    swap(static_cast<Bautura&>(a), static_cast<Bautura&>(b));
    swap(a.descriere,  b.descriere);
    swap(a.pretExtra,  b.pretExtra);
}

BauturaSpeciala& BauturaSpeciala::operator=(BauturaSpeciala other) {
    swap(*this, other);
    return *this;
}

std::shared_ptr<Produs> BauturaSpeciala::clone() const {
    return std::make_shared<BauturaSpeciala>(*this);
}

float BauturaSpeciala::calculeazaPretFinal() const
{
    return Bautura::calculeazaPretFinal()+pretExtra;
}

void BauturaSpeciala::afisareDetalii(std::ostream& os) const
{
    os<<"Specificatii bautura custom: ";
    Bautura::afisareDetalii (os);
    os<<"\n Descriere: "<<descriere<<" | Cost aditional: +"<<pretExtra<<" RON\n";
}
