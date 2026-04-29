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
    const std::string& getNume() const
    {
        return nume;
    }

    int getStoc() const
    {
        return stoc;
    }

    float getPret() const
    {
        return pret;
    }

    bool getVegan() const
    {
        return vegan;
    }

    bool getFaraZahar() const
    {
        return faraZahar;
    }

    bool getFaraLactoza() const
    {
        return faraLactoza;
    }

    float getKcal() const
    {
        return kcal;
    }

    ///Metode
    void consumaStoc();
    void reaprovizionare(int cantitate);

    bool atentieStocMic() const
    {
        return stoc < 5;
    }

    bool esteInStoc() const
    {
        return stoc > 0;
    }

    ///Operator de afisare
    friend std::ostream& operator<<(std::ostream& os, const Ingredient& ing);
    friend std::istream& operator>>(std::istream& is, Ingredient& ing);
};

#endif
