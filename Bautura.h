#ifndef BAUTURA_H
#define BAUTURA_H

#include "Produs.h"
#include "Ingredient.h"
#include <vector>
#include <string>
#include <memory>


///Bautura is a Produs
class Bautura : public Produs
{
private:
    std::vector<Ingredient*> listaIngrediente; // Reteta de baza din fisier
    std::vector<Ingredient*> toppinguriExtra;  // Topping-uri adaugate de client
    bool esteCalda;

public:
    explicit Bautura(std::string nume = "", float pretPrep = 0.0f, int timpPrep = 0, bool calda = true);

    /// Regula celor trei, constructor de copiere, operator de atribuire,
    Bautura(const Bautura& other);
    Bautura& operator=(Bautura other);

    friend void swap(Bautura& a, Bautura& b);

    ~Bautura() override = default;///default deoarece nu detine obiecte Ingr doar le foloseste

    /// Implementari functii virtuale
    bool esteVegan() const override;
    bool esteFaraZahar() const override;
    bool esteFaraLactoza() const override;
    float calculeazaKcalTotal() const override;
    bool esteDisponibil() const override;

    ///copie  a bauturii cand este adaugata in cos
    std::shared_ptr<Produs> clone() const override;

    /// Metode pentru adaugare
    void adaugaIngredient(Ingredient* ing);
    void adaugaToppingExtra(Ingredient* top);

    const std::vector<Ingredient*>& getIngrediente() const override
    {
        return listaIngrediente; /// returneaza pointerul catre vectorul tau real
        ///fiecare ingredient din listaIngredient are referinta spre clasa Ingredient
    }

protected:
    ///interva non virtuala
    float calculeazaPretFinal() const override;
    void afisareDetalii(std::ostream& os) const override;
};

#endif
