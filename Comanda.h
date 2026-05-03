#ifndef COMANDA_H
#define COMANDA_H

#include <vector>
#include <string>
#include <memory>

class Produs;
class Gestiune;

class Comanda
{
private:

    static int numarComenzi;
    static int timpOcupatBarista;

    int idComanda;
    /// agregare polimorfică: o comandă conține pointeri către clasa de bază Produs
    std::vector<std::shared_ptr<Produs>> produseComandate;
    float totalPlata;
    std::string dataOraComanda;
    std::string codRidicare;

public:
    Comanda();

    /// Getters
    const std::string& getCodRidicare() const
    {
        return codRidicare;
    }
    int getId() const
    {
        return idComanda;
    }
    float getTotalPlata() const
    {
        return totalPlata;
    }

    static void scadeTimpOcupat(int secunde)
    {
        timpOcupatBarista -= secunde;
        if (timpOcupatBarista < 0) timpOcupatBarista = 0;
    }

    const std::vector<std::shared_ptr<Produs>>& getProduseComandate() const {
        return produseComandate;
    }
    void adaugaProdus(std::shared_ptr<Produs> p);
    int calculeazaTimpPregatirePropriu() const;

    void finalizeazaComanda(Gestiune& g);
    void afisareSumarConsola() const;


};

#endif
