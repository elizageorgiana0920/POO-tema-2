#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include <limits>
#include <vector>

#include "Gestiune.h"
#include "Comanda.h"
#include "Autentificare.h"
#include "Exceptii.h"
#include "Bautura.h"
#include "Sandwich.h"
#include "Patiserie.h"

///pentru a marca ora la care s-a facut comanda
static int getOraActuala()
{
    std::time_t t = std::time(nullptr);
    struct tm* now = std::localtime(&t);
    return now->tm_hour;
}

static void meniuClient(Gestiune& g)
{
    int optiune = -1;
    while (optiune != 0)
    {
        std::cout << "\n--- 1. MENIU CLIENT ---\n"
                  << "1. VEZI MENIU COMPLET\n"
                  << "2. VEZI CE OPTIUNI AVEM DISPONIBILE MOMENTAN\n"
                  << "3. ALEGE UN MENIU PERSONALIZAT\n"
                  << "4. CAUTA UN PRODUS\n"
                  << "5. COMANDA ACUM\n"
                  << "0. INAPOI\nOptiunea: ";


        ///daca s-a citit altceva se continua pentru a nu se bloca, curata memoria temporara
        if (!(std::cin >> optiune))
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        if (optiune == 0) break;
        try
        {
            switch (optiune)
            {
            case 1:
                g.afisareMeniu(false);
                break;
            case 2:
                g.verificaPatiserieExpirata();
                g.afisareMeniu(true);
                break;
            case 3:
            {
                ///meniu personalizat
                bool v, z, l, cond;
                std::string ing;
                std::cout << "Vegan? (1-Da, 0-Nu): ";
                std::cin >> v;
                std::cout << "Fara zahar? (1-Da, 0-Nu): ";
                std::cin >> z;
                std::cout << "Fara lactoza? (1-Da, 0-Nu): ";
                std::cin >> l;
                std::cout << "Ingredient specific (sau 'nu'): ";
                std::cin >> ing;
                if (ing != "nu")
                {
                    std::cout << "Sa contina (1) sau sa NU contina (0)?: ";
                    std::cin >> cond;
                    g.afisareMeniuPersonalizat(v, z, l, ing, cond);
                }
                else
                    g.afisareMeniuPersonalizat(v, z, l);
                break;
            }
            case 4:
            {
                std::string nume;
                std::cout << "Introduceti numele produsului cautat: ";
                std::cin >> nume;
                g.cautaSiAfiseazaProdus(nume);
                break;
            }
            case 5:
            {
                g.verificaPatiserieExpirata();
                Comanda comanda;
                std::string numeP, raspuns = "da";
                bool comandaAreProduse = false; /// Indicator pentru finalizare

                while (raspuns == "da")
                {
                    Produs* p = nullptr;
                    while (p == nullptr)
                    {
                        std::cout << "Ce doriti sa comandati? (Nume): ";
                        std::cin >> numeP;
                        p = g.gasesteProdusDupaNume(numeP);
                        if (!p) std::cout << "[!] Produs inexistent. Incearcati din nou.\n";
                    }

                    try
                    {
                        ///veriifc disponibilitate
                        if (!p->esteDisponibil())
                        {
                            throw StocInsuficientException(p->getNume());
                        }

                        ///copiez produsul
                        auto pComandat = p->clone();
                        bool produsValid = true; /// indicator pentru produsul curent

                        ///daca este sandwich atunci verific daca vrea incalzit
                        auto s = std::dynamic_pointer_cast<Sandwich>(pComandat);
                        if (s)
                        {
                            std::string incalz;
                            std::cout << "Doriti sandwich-ul incalzit? (da/nu): ";
                            std::cin >> incalz;
                            if (incalz == "da")
                            {
                                try
                                {
                                    s->cereIncalzit();
                                }
                                catch (const OperatiuneInvalidaException& e)
                                {
                                    std::cout << "[!] " << e.what() << " Se va servi RECE.\n";

                                }
                            }
                        }

                        /// daca este bautura verific daca vrea topping
                        auto b = std::dynamic_pointer_cast<Bautura>(pComandat);
                        if (b)
                        {
                            std::string extra;
                            std::cout << "Doriti topping extra? (da/nu): ";
                            std::cin >> extra;
                            if (extra == "da")
                            {
                                std::string numeTop;
                                std::cout << "Nume topping: ";
                                std::cin >> numeTop;
                                Ingredient* tRef = g.gasesteTopping(numeTop);
                                if (tRef)
                                {
                                    try
                                    {
                                        b->adaugaToppingExtra(tRef);
                                    }
                                    catch (const CafeneaException& e)
                                    {
                                        std::cout << "[!] Eroare topping: " << e.what();
                                        std::cout << "[!] Produsul se adauga fara topping.\n";
                                    }
                                }
                                else
                                {
                                    std::cout << "[!] Topping-ul nu a fost gasit. Se adauga bautura simpla.\n";
                                }
                            }
                        }

                        ///daca n am erori pot sa adaug produsele la comandate
                        comanda.adaugaProdus(pComandat);
                        comandaAreProduse = true;
                        std::cout << "[OK] " << pComandat->getNume() << " a fost adaugat in cos.\n";

                    }
                    catch (const StocInsuficientException& e)
                    {
                        std::cout << "[!] EROARE STOC: " << e.what();
                        std::cout << "[!] Produsul NU a fost adaugat la bon.\n";
                    }
                    catch (const CafeneaException& e)
                    {
                        std::cout << "[!] Eroare: " << e.what() << "\n";
                    }

                    std::cout << "Mai adaugati ceva? (da/nu): ";
                    std::cin >> raspuns;
                }

                ///daca am cel putin un  produs valid finalizez comanda
                if (comandaAreProduse)
                {
                    comanda.afisareSumarConsola();
                    comanda.finalizeazaComanda(g);
                }
                else
                {
                    std::cout << "\n[!] Comanda anulata. Nu s-a adaugat niciun produs valid la bon.\n";
                }
                break;
            }
            default:
                std::cout << "Optiune invalida!\n";
            }
        }
        catch (const CafeneaException& e)
        {
            std::cout << "Eroare: " << e.what() << "\n";
        }
    }
}


static void meniuBarista(Gestiune& g)
{
    int optiune = -1;
    while (optiune != 0)
    {
        std::cout << "\n--- 2. MENIU BARISTA ---\n"
                  << "1. VEZI PRODUSE PE STOC\n"
                  << "2. VEZI STOCURI CRITICE\n"
                  << "0. INAPOI\nOptiunea: ";
        std::cin >> optiune;
        if (optiune == 0) break;

        switch (optiune)
        {
        case 1:
            g.afisareToateStocurile();
            break;
        case 2:
            g.afisareIngredienteCritice();
            break;
        default:
            std::cout << "Optiune invalida!\n";
        }
    }
}


static void meniuManager(Gestiune& g)
{
    int optiune = -1;
    while (optiune != 0)
    {
        std::cout << "\n--- 3. MENIU MANAGER ---\n"
                  << "1. VEZI MENIU COMPLET\n"
                  << "2. VEZI STOC INGREDIENTE\n"
                  << "3. REAPROVIZIONEAZA\n"
                  << "4. INGREDIENTE CU STOC CRITIC\n"
                  << "5. PRODUSE CU STOC CRITIC (PATISERIE/SANDWICH)\n"
                  << "6. RAPOARTE BUSINESS\n"
                  << "7. VEZI TOATE COMENZILE (ISTORIC)\n"
                  << "0. INAPOI\nOptiunea: ";
        std::cin >> optiune;
        if (optiune == 0) break;

        switch (optiune)
        {
        case 1:
            g.afisareMeniu(false);
            break;
        case 2:
            g.afisareToateStocurile();
            break;
        case 3:
        {
            std::string nume;
            int cant;
            std::cout << "Nume ingredient: ";
            std::cin >> nume;
            std::cout << "Cantitate: ";
            std::cin >> cant;
            g.reaprovizionare(nume, cant);
            break;
        }
        case 4:
            g.afisareIngredienteCritice();
            break;
        case 5:
            g.afisareProduseCriticePatiserieSandwich();
            break;
        case 6:
            g.afisareRapoarte();
            break;
        case 7:
        {
            std::ifstream f("registru.txt");
            std::string linie;
            if (!f) std::cout << "Istoricul este gol.\n";
            while (std::getline(f, linie)) std::cout << linie << "\n";
            break;
        }
        default:
            std::cout << "Optiune invalida!\n";
        }
    }
}

int main()
{
    Gestiune cafea;

    try
    {
        cafea.incarcaIngrediente("ingrediente.txt");
        cafea.incarcaToppinguri("toppinguri.txt");
        cafea.incarcaMeniuBauturi("bauturi.txt");
        cafea.incarcaMeniuPatiserie("patiserie.txt");
        cafea.incarcaMeniuSandwich("sandwichuri.txt");
    }
    catch (const std::exception& e)
    {
        std::cerr << "Eroare: " << e.what() << "\n";
        return 1;
    }


    cafea.verificaPatiserieExpirata();
    cafea.verificaSandwichExpirat();

    while (true)
    {
        try
        {
            Rol rol = Autentificare::selecteazaRol();
            if (rol == Rol::IESIRE) break;

            Autentificare::verificaParola(rol);
            std::cout << "\nAcces permis: " << Autentificare::getNumeRol(rol) << "\n";

            switch (rol)
            {
            case Rol::CLIENT:
                meniuClient(cafea);
                break;
            case Rol::BARISTA:
                meniuBarista(cafea);
                break;
            case Rol::MANAGER:
                meniuManager(cafea);
                break;
            default:
                break;
            }
        }
        catch (const AccesInterzisException& e)
        {
            std::cout << e.what() << "\n";
        }
        catch (const CafeneaException& e)
        {
            std::cout << "Eroare cafenea: " << e.what() << "\n";
        }
    }

    std::cout << "Aplicatie inchisa. Profitul si istoricul au fost salvate.\n";
    return 0;
}
