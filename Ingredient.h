#ifndef INGREDIENT_H
#define INGREDIENT_H

#include<string>
#include<iostream>

class Ingredient
{
private:
    std::string nume;
    int stoc;
    float pret;
    bool vegan;
    bool faraZahar;
    bool faraLactoza;
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
    void consumaStoc();
    void reaprovizionare(int cantitate);
    bool atentieStocMic() const;///<5
    bool esteInStoc() const;///>0

    ///Operator de afisare
    friend std::ostream& operator<<(std::ostream& os, const Ingredient& ing);
    friend std::istream& operator>>(std::istream& is, Ingredient& ing);
};

#endif
