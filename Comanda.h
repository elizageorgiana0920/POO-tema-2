#ifndef COMANDA_H
#define COMANDA_H

#include<vector>
#include<string>
#include<memory>

class Produs;
class Gestiune;

class Comanda
{
private:
    static int numarComenzi;
    static int timpOcupatBarista;

    int idComanda;
    std::vector<std::shared_ptr<Produs>> produseComandate;
    float totalPlata;
    int timpPregatireComandaCurenta;
    std::string dataOraComanda;
    std::string codRidicare;

public:
    Comanda();

    void adaugaProdus(std::shared_ptr<Produs> p);
    int calculeazaTimpAsteptareTotal() const;
    void finalizeazaComanda(Gestiune& g);
    void afisareSumar() const;
    static int getNumarComenzi() { return numarComenzi; }
};

#endif
