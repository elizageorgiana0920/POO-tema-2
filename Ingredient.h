#ifndef INGREDIENT_H
#define INGREDIENT_H

#include<string>
#include<iostream>

class Ingredient
{
private:
    std::string nume;///nume ingredient
    int stoc;///cantitate disponibila
    float pret;
    bool vegan;///1-da 0-nu este vegan
    bool faraZahar;///1-nu are zahar 0-are
    bool faraLactoza;///1-nu are lactoza 0-are
    float kcal;

public:
    ///Constructori
    Ingredient();
    Ingredient(std::string nume, int stoc, float pret, bool vegan, bool faraZahar, bool faraLactoza, float kcal);

    ///Getteri
    const std::string& getNume() const;

    int getStoc() const;
    float getPret() const;
    bool getVegan() const;
    bool getFaraZahar() const;
    bool getFaraLactoza() const;
    float getKcal() const;

    ///Metode
    void consumaStoc();///la vanzare scade stocul cu 1
    void reaprovizionare(int cantitate);///adauga la stoc + cantitate
    bool atentieStocMic() const;///stoc<5
    bool esteInStoc() const;///stoc>0

    ///Operator de afisare
    friend std::ostream& operator<<(std::ostream& os, const Ingredient& ing);
    friend std::istream& operator>>(std::istream& is, Ingredient& ing);
};

#endif
