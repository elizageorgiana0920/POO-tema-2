#ifndef BAUTURA_H
#define BAUTURA_H

#include "Produs.h"
#include "Ingredient.h"
#include <vector>
#include <string>
#include <memory>

class Bautura : public Produs {
private:
    std::vector<Ingredient*> listaIngrediente; // Reteta de baza din fisier
    std::vector<Ingredient*> toppinguriExtra;  // Topping-uri adaugate de client
    bool esteCalda;

public:
    explicit Bautura(std::string nume = "", float pretPrep = 0.0f, int timpPrep = 0, bool calda = true);

    // Rule of Three actualizat pentru a gestiona ambele liste
    Bautura(const Bautura& other);
    Bautura& operator=(Bautura other);
    friend void swap(Bautura& a, Bautura& b);

    ~Bautura() override = default;

    // Implementari functii virtuale (acum includ si topping-urile in calcul)
    bool esteVegan() const override;
    bool esteFaraZahar() const override;
    bool esteFaraLactoza() const override;
    float calculeazaKcalTotal() const override;
    bool esteDisponibil() const override;

    std::shared_ptr<Produs> clone() const override;

    // Metode pentru adaugare
    void adaugaIngredient(Ingredient* ing); // Pentru fisierul bauturi.txt
    void adaugaToppingExtra(Ingredient* top); // Pentru dorinta clientului

    const std::vector<Ingredient*>& getIngrediente() const override {
        return listaIngrediente; // Returneaza pointerul catre vectorul tau real
    }

protected:
    float calculeazaPretFinal() const override;
    void afisareDetalii(std::ostream& os) const override;
};

#endif