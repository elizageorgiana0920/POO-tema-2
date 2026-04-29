#ifndef PRODUS_H
#define PRODUS_H

#include<string>
#include<vector>
#include<memory>
#include<iostream>

class Ingredient;

class Produs
{
protected:
    std::string nume;
    float pretPreparare;
    std::vector<Ingredient*> ingrediente; ///pointeri pentru camara

    Produs(const Produs& other) = default;
    Produs& operator=(const Produs& other) = default;

    virtual float calculeazaPretFinal() const =0;
    virtual int getTimpPreparare() const =0;
    virtual bool esteDisponibil() const =0;
    virtual float calculeazaKcalTotal() const;
    virtual void afisareDetalii(std::ostream& os) const;

public:
    Produs(std::string nume = "", float p = 0.0f);
    virtual ~Produs() = default;


    ///interfata non-virtuala
    void afiseaza(std::ostream& os) const;
    float pretFinal() const;
    int timpPregatire() const;
    bool verificareDisponibilitate() const;

    ///clone (constructor virtual)
    virtual std::shared_ptr<Produs> clone() const = 0;

    ///non-virtuale comune
    void adaugaIngredient(Ingredient* ing);
    bool contineIngredient(const std::string& numeIng) const;
    bool esteVegan() const;
    bool esteFaraZahar() const;
    bool esteFaraLactoza() const;

    const std::string& getNume() const { return nume; }

    float getPretPreparare() const { return pretPreparare; }
    const std::vector<Ingredient*>& getIngrediente() const { return ingrediente; }

    float getKcal() const;
    friend std::ostream& operator<<(std::ostream& os, const Produs& p);
};

#endif
