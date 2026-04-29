#include<iostream>
#include<string>
#include<fstream>
#include<ctime>
#include "Gestiune.h"
#include "Comanda.h"
#include "ProdusCustom.h"
#include "Autentificare.h"
#include "Exceptii.h"
#include "BauturaSpeciala.h"
#include "Sandwich.h"

static int oraActuala()
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
        std::cout << "\n--- MENIU CLIENT ---\n"
            << "1. Vizualizeaza meniu complet\n"
            << "2. Vizualizeaza doar produse disponibile\n"
            << "3. Filtrare dupa ingredient (contine/nu contine)\n"
            << "4. Produse VEGANE\n"
            << "5. Produse FARA ZAHAR\n"
            << "6. Produse FARA LACTOZA\n"
            << "7. Cauta produs dupa nume\n"
            << "8. Plaseaza comanda\n"
            << "0. Inapoi\nOptiunea: ";

        std::cin >> optiune;
        if (optiune == 1)
            g.afisareMeniu(false);
        else if (optiune == 2)
        {
            g.verificaPatiserieExpirata();
            g.afisareMeniu(true);
        }
        else if (optiune == 3)
        {
            std::string ing, raspuns;
            std::cout << "Ingredient: ";
            std::cin >> ing;
            std::cout << "Contine? (da/nu): ";
            std::cin >> raspuns;
            g.afisareMeniuFiltrat(ing, raspuns == "da");
        }
        else if (optiune == 4)
            g.afisareMeniuVegan();
        else if (optiune == 5)
            g.afisareMeniuFaraZahar();
        else if (optiune == 6)
            g.afisareMeniuFaraLactoza();
        else if (optiune == 7)
        {
            std::string nume;
            std::cout << "Nume produs: ";
            std::cin >> nume;
            try
            {
                Produs* p = g.cautaProdus(nume);
                p->afiseaza(std::cout);
                std::cout << "\nKcal: " << p->getKcal();
                if (p->esteVegan())
                    std::cout << " VEGAN";
                if (p->esteFaraZahar())
                    std::cout << " FARA ZAHAR";
                if (p->esteFaraLactoza())
                    std::cout << " FARA LACTOZA";
                std::cout << "\n";
            }
            catch (const CafeneaException& e)
            {
                std::cout << e.what() << "\n";
            }
        }
        else if (optiune == 8)
        {
            g.verificaPatiserieExpirata();

            Comanda comanda;

            int nrProduse;
            std::cout << "Cate produse doriti sa comandati? ";
            std::cin >> nrProduse;
            for (int i = 0; i < nrProduse; i++)
            {
                std::string numeP;
                std::cout << "Produsul " << i + 1 << ": ";
                std::cin >> numeP;
                try
                {
                    Produs* p = g.cautaProdus(numeP);
                    Bautura* b = dynamic_cast<Bautura*>(p);
                    if (b != nullptr)
                    {
                        std::string vreiCustom;
                        std::cout << "Doriti persoanizare? (da/nu): ";
                        std::cin >> vreiCustom;
                        if (vreiCustom == "da")
                        {
                            auto custom = std::make_shared<ProdusCustom>(*b);
                            std::string extra;
                            std::cout << "Adaugati extra? da/nu, pentru da introduceti numele extraului: ";
                            while (std::cin >> extra && extra != "nu")
                            {
                                Ingredient* ing = g.gasesteIngredient(extra);
                                if (ing) custom->adaugaExtra(ing);
                                std::cout << "Alteceva? (nume ingredient/nu): ";
                            }
                            comanda.adaugaProdus(custom);
                            continue;
                        }
                    }
                    comanda.adaugaProdus(std::shared_ptr<Produs>(p->clone()));
                }
                catch (const CafeneaException& e)
                {
                    std::cout << e.what() << " Reincercati.\n";
                    i--;
                }
            }
            comanda.afisareSumar();
            g.inregistreazaOraComanda(oraActuala());
            comanda.finalizeazaComanda(g);
        }
    }
}

static void meniuBarista(Gestiune& g)
{
    int optiune = -1;
    while (optiune != 0)
    {
        std::cout << "\n--- MENIU BARISTA ---\n"
            << "1. Vizualizeaza produse disponibile\n"
            << "2. Verifica stocuri critice\n"
            << "3. Vizualizeaza raport reaprovizionare\n"
            << "0. Inapoi\noptiuneiunea: ";
        std::cin >> optiune;

        if (optiune == 1)
        {
            g.verificaPatiserieExpirata();
            g.afisareMeniu(true);
        }
        else if (optiune == 2)
        {
            g.verificaStocuriCritice();
            std::cout << "Raport salvat in reaprovizionare.txt\n";
        }
        else if (optiune == 3)
        {
            std::ifstream f("reaprovizionare.txt");
            std::string linie;
            std::cout << "\n--- reaprovizionare.txt ---\n";
            while (std::getline(f, linie))
                std::cout << linie << "\n";
        }
    }
}

static void meniuManager(Gestiune& g)
{
    int optiune = -1;
    while (optiune != 0)
    {
        std::cout << "\n--- MENIU MANAGER ---\n"
            << "1. Vizualizeaza meniu complet\n"
            << "2. Reaprovizionare ingredient\n"
            << "3. Genereaza raport stocuri critice\n"
            << "4. Vizualizeaza raport reaprovizionare\n"
            << "5. Rapoarte business (vanzari, profit, ore varf)\n"
            << "6. Vizualizeaza registru comenzi\n"
            << "7. Profit total (sesiuni multiple)\n"
            << "0. Inapoi\noptiuneiunea: ";
        std::cin >> optiune;
        if (optiune == 1)
            g.afisareMeniu(false);
        else if (optiune == 2)
        {
            std::string numeIng;
            int cant;
            std::cout << "Ingredient: ";
            std::cin >> numeIng;
            std::cout << "Cantitate adaugata: ";
            std::cin >> cant;
            g.reaprovizionare(numeIng, cant);
        }
        else if (optiune == 3)
        {
            g.verificaPatiserieExpirata();
            std::cout << "Raport salvat in reaprovizonare.txt\n";
        }
        else if (optiune == 4)
        {
            std::ifstream f("reaprovizionare.txt");
            std::string linie;
            std::cout << "\n--- reaprovizionare.txt ---\n";
            while (std::getline(f, linie))
                std::cout << linie << "\n";
        }
        else if (optiune == 5)
            g.afisareRapoarte();
        else if (optiune == 6)
        {
            std::ifstream f("registru.txt");
            std::string linie;
            std::cout << "\n--- Registru Comenzi ---\n";
            while (std::getline(f, linie))
                std::cout << linie << "\n";
        }
        else if (optiune == 7)
            g.afisareProfitTotal();
    }
}

int main()
{
    Gestiune g;
    try
    {
        g.incarcaIngrediente("ingrediente.txt");
        g.incarcaMeniuBauturi("bauturi.txt");
        g.incarcaMeniuPatiserie("patiserie.txt");
        g.incarcaMeniuSandwich("sandwichuri.txt");
    } catch (const std::exception& e)
    {
        std::cerr<<"Eroare la incarcarea fisierelor: "<<e.what()<<"\n";
        return 1;
    }

    while (true)
    {
        try
        {
            Rol rol = Autentificare::selecteazaRol();
            Autentificare::verificaParola(rol);

            std::cout << "\nBine ai venit, "<<Autentificare::numeRol(rol) << "!\n";

            switch (rol)
            {
            case Rol::CLIENT: meniuClient(g);
                break;
            case Rol::BARISTA: meniuBarista(g);
                break;
            case Rol::MANAGER: meniuManager(g);
                break;
            }
        }
        catch (const AccesInterzisException& e)
        {
            std::cout << e.what() << "\n";
        }
        catch (const CafeneaException& e)
        {
            std::cout << "Eroare: " << e.what() << "\n";
        }
    }
    return 0;
}
