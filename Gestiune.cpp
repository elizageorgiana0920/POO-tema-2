
#include "Gestiune.h"
#include "Bautura.h"
#include "Patiserie.h"
#include "Sandwich.h"
#include "Exceptii.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>

///initiez membru static in afara clasei
float Gestiune::profitTotal = 0.0f;

///constructor
Gestiune::Gestiune()
{
    ///la pornire programul cauta fisierul profit_total pentru a continua unde a ramas

    std::ifstream f("profit_total.txt");
    if (f >> profitTotal) {}
    else
    {
        profitTotal = 0;
    }
}

///destructor
Gestiune::~Gestiune()
{

    ///salveaza datele la final daca programul s-a terminat cu return 0(corect)
    std::ofstream fprofit("profit_total.txt");
    if (fprofit.is_open())
    {
        fprofit << profitTotal;
        fprofit.close();
    }


    std::ofstream fing("ingrediente.txt");
    if (fing.is_open())
    {
        ///salvarea ingredientelor din fiser txt
        for (const auto& ing : listaIngrediente)
        {
            fing << ing.getNume() << " "
                 << ing.getStoc() << " "
                 << ing.getPret() << " "
                 << (ing.getVegan() ? 1 : 0) << " "
                 << (ing.getFaraZahar() ? 1 : 0) << " "
                 << (ing.getFaraLactoza() ? 1 : 0) << " "
                 << ing.getKcal() << "\n";
        }
        fing.close();
    }


    ///preluarea toppingurilor din fisier txt
    std::ofstream ftop("toppinguri.txt");
    if (ftop.is_open())
    {
        for (const auto& top : listaToppinguri)
        {
            ftop << top.getNume() << " " << top.getStoc() << " " << top.getPret() << " "
                 << (top.getVegan() ? 1 : 0) << " " << (top.getFaraZahar() ? 1 : 0) << " "
                 << (top.getFaraLactoza() ? 1 : 0) << " " << top.getKcal() << "\n";
        }
        ftop.close();
    }
}

///afiseaza istoricul comenzilor prin afisarea registrului, fisierul txt unde se salveaza bonurile
void Gestiune::vizualizareComenziRecente() const
{
    std::ifstream f("registru.txt");
    std::string linie;

    if (!f)
    {
        std::cout << "Nu exista comenzi inregistrate astazi.\n";
        return;
    }

    std::cout << "\n--- ULTIMELE COMENZI (GATA DE PREPARARE/RIDICARE) ---\n";

    while (std::getline(f, linie))
    {
        std::cout << linie << "\n";
    }
    f.close();
}

///preia ingredientele de baza din fisier
void Gestiune::incarcaIngrediente(const std::string& fisier)
{
    std::ifstream fin(fisier);
    if (!fin) throw DateInvalideException("Fisier ingrediente lipsa!");
    Ingredient temp;
    while (fin >> temp) listaIngrediente.push_back(temp);
}

///preia toppingurile din fisier
void Gestiune::incarcaToppinguri(const std::string& fisier)
{
    std::ifstream fin(fisier);
    if (!fin) return;
    Ingredient temp;
    while (fin >> temp) listaToppinguri.push_back(temp);
}

///preiau datele de patiserie si le voi pune in meniu cu pointeri spre Patiserie
void Gestiune::incarcaMeniuPatiserie(const std::string& fisier)
{
    std::ifstream fin(fisier);
    std::string nume;
    float pret;
    int timp, stoc, v, z, l;
    std::time_t expira;
    float kcal;
    while (fin >> nume >> pret >> timp >> expira >> stoc >> v >> z >> l >> kcal)
    {
        meniu.push_back(std::make_shared<Patiserie>(nume, pret, timp, expira, stoc, v!=0, z!=0, l!=0, kcal));
    }
}
///incarc datele de la sandwich si pe pun intr un meniu special cu pointeri la clasa Sandwich pentru a pastra corect specificatiile
void Gestiune::incarcaMeniuSandwich(const std::string& fisier)
{
    std::ifstream fin(fisier);
    std::string nume;
    float pret;
    int timp, stoc, v, z, l, incalz;
    std::time_t expira;
    float kcal;
    while (fin >> nume >> pret >> timp >> expira >> stoc >> v >> z >> l >> kcal >> incalz)
    {
        meniu.push_back(std::make_shared<Sandwich>(nume, pret, timp, expira, stoc, v!=0, z!=0, l!=0, kcal, incalz!=0));
    }
}

///creeaza băuturi și le asociază automat ingredientele necesare
///cauta in lista de ingrediente referintele pt fiecare bautura
void Gestiune::incarcaMeniuBauturi(const std::string& fisier)
{
    std::ifstream fin(fisier);
    std::string linie;
    while (std::getline(fin, linie))
    {
        if (linie.empty()) continue;
        std::stringstream ss(linie);
        std::string nume, numeIng;
        float pretBaza;
        int timp, calda;

        if (ss >> nume >> pretBaza >> timp >> calda)
        {
            auto b = std::make_shared<Bautura>(nume, pretBaza, timp, calda != 0);
            while (ss >> numeIng)
            {
                Ingredient* ref = gasesteIngredient(numeIng);
                if (ref)
                {
                    b->adaugaIngredient(ref);
                }
            }
            meniu.push_back(b);
        }
    }
}

///afiseaza toate produsele din cafenea in functie de disponibilitate sau indisponibilitate
void Gestiune::afisareMeniu(bool doarDisponibile) const
{
    std::cout << "\n========== MENIU COMPLET ==========\n";
    for (const auto& p : meniu)
    {
        if (doarDisponibile && !p->esteDisponibil()) continue;
        p->afiseaza(std::cout);
    }
}

///meniu de filtrare, cu parmaterii ce sunt furnizati de catre client
void Gestiune::afisareMeniuPersonalizat(bool vegan, bool fZahar, bool fLactoza, const std::string& numeIng, bool saContina) const
{
    std::cout << "\n========== FILTRARE PERSONALIZATA ==========\n";
    bool gasit = false;
    for (const auto& p : meniu)
    {
        if (vegan && !p->esteVegan()) continue;
        if (fZahar && !p->esteFaraZahar()) continue;
        if (fLactoza && !p->esteFaraLactoza()) continue;

        if (!numeIng.empty())
        {
            bool areIng = false;
            for (auto* ing : p->getIngrediente())
            {
                if (ing && ing->getNume() == numeIng)
                {
                    areIng = true;
                    break;
                }
            }
            if (saContina && !areIng) continue;
            if (!saContina && areIng) continue;
        }

        if (p->esteDisponibil())
        {
            p->afiseaza(std::cout);
            gasit = true;
        }
    }
    if (!gasit) std::cout << "Niciun produs nu corespunde filtrelor.\n";
}

///cauta un produs in meniu si il afiseaza daca il gaseste
void Gestiune::cautaSiAfiseazaProdus(const std::string& nume) const
{
    for (const auto& p : meniu)
    {
        if (p->getNume() == nume)
        {
            p->afiseaza(std::cout);
            return;
        }
    }
    std::cout << "Produsul nu a fost gasit.\n";
}

///afiseaza stocul pentru fiecrae ingredient si afiseaza atentionare daca este mai mic stocul de 5
void Gestiune::afisareToateStocurile() const
{
    std::cout << "\n========== STOC COMPLET INGREDIENTE ==========\n";
    for (const auto& ing : listaIngrediente)
    {
        std::cout << std::left << std::setw(20) << ing.getNume()
                  << ": " << ing.getStoc() << " unitati";
        if (ing.atentieStocMic()) std::cout << " [ATENTIE!]";
        std::cout << "\n";
    }
}

///afiseaza doar ingredientele cu stoc mai mic de 5
void Gestiune::afisareIngredienteCritice() const
{
    std::cout << "\n========== RAPORT STOCURI CRITICE (< 5) ==========\n";
    bool gasit = false;
    for (const auto& ing : listaIngrediente)
    {
        if (ing.atentieStocMic())   // Verificăm pragul de 5
        {
            std::cout << " (!) " << std::left << std::setw(20) << ing.getNume()
                      << " | Stoc actual: " << ing.getStoc() << "\n";
            gasit = true;
        }
    }
    if (!gasit)
    {
        std::cout << "Toate ingredientele sunt in parametri optimi.\n";
    }
}

///pt manager: poate sa mareaza stocul ingredientelor
void Gestiune::reaprovizionare(const std::string& numeIng, int cantitate)
{
    Ingredient* ing = gasesteIngredient(numeIng);
    if (ing) ing->reaprovizionare(cantitate);
}

///verifica toata patiseria si "arunca" ce este expirat
void Gestiune::verificaPatiserieExpirata()
{
    for (auto& p : meniu)
    {
        auto pat = std::dynamic_pointer_cast<Patiserie>(p);
        if (pat && pat->esteExpirat()) pat->marcheazaExpirat();
    }
}
///verifica toate sandwich-urile si "arunca" ce este expirat
void Gestiune::verificaSandwichExpirat()
{
    for (auto& p : meniu)
    {
        auto sand = std::dynamic_pointer_cast<Sandwich>(p);
        // Dacă este sandwich și data curentă a depășit dataExpirarii
        if (sand && sand->esteExpirat())
            sand->marcheazaExpirat();

    }
}

///Proceseaza vânzarea unui produs
///Scade stocurile de ingrediente, actualizează profitul, reține ora vânzării
///și salvează coamanda în registru

void Gestiune::proceseazaComanda(std::shared_ptr<Produs> p, int ora)
{
    if (!p->esteDisponibil()) throw StocInsuficientException(p->getNume());

    /// consum resursele (Ingrediente pentru Bautura / Stoc pentru Mancare)
    for (auto* ing : p->getIngrediente())
    {
        ing->consumaStoc();
        frecventaIngrediente[ing->getNume()]++;
    }

    auto patiserie = std::dynamic_pointer_cast<Patiserie>(p);
    if (patiserie) patiserie->scadeStoc();

    auto sandwich = std::dynamic_pointer_cast<Sandwich>(p);
    if (sandwich) sandwich->scadeStoc();

    /// actualizez profitul total
    ///pretul platit de client - pretul de preparare
    profitTotal += (p->getPretFinal() - p->getPretPreparare());

    /// salvez vanzarea în statistici.txt pentru produsul de top si ora de varf

    salveazaVanzareInIstoric(ora, p->getNume());

    /// actualizez datele in memorie pentru raportul curent
    ///frecventaProduse[p->getNume()]++;
    ///frecventaOre[ora]++;
}

///afiseaza un raport cu cel mai vandut produs, profitul total si determina care este ora la care s-au facut cele mai multe comenzi
void Gestiune::afisareRapoarte() const
{
    std::cout << "\n======== RAPORT BUSINESS ========\n";
    if (!frecventaProduse.empty())
    {
        auto top = std::max_element(frecventaProduse.begin(), frecventaProduse.end(),
                                    [](const auto& a, const auto& b)
        {
            return a.second < b.second;
        });
        std::cout << "Top Produs: " << top->first << " (" << top->second << ")\n";
    }
    std::cout << "Profit Total: " << profitTotal << " RON\n";
    if (!frecventaOre.empty())
    {
        int oraVarf = -1, maxComenzi = -1;
        for (auto const& [ora, nr] : frecventaOre)
        {
            if (nr > maxComenzi)
            {
                maxComenzi = nr;
                oraVarf = ora;
            }
        }
        std::cout << "Ora de varf: " << oraVarf << ":00 cu " << maxComenzi << " comenzi.\n";
    }
}

///afiseaza daca patiserie si sandwich au stoc mai mic de 3
void Gestiune::afisareProduseCriticePatiserieSandwich() const
{
    std::cout << "\n========== ALERTA STOC CRITIC PREPARATE (< 3) ==========\n";
    bool gasit = false;

    for (const auto& p : meniu)
    {
        /// Verificăm dacă obiectul este Patiserie
        auto pat = std::dynamic_pointer_cast<Patiserie>(p);
        if (pat && pat->getStoc() < 3)
        {
            std::cout << " [!] PATISERIE: " << pat->getNume() << " | Stoc: " << pat->getStoc() << "\n";
            gasit = true;
        }

        /// Verificăm dacă obiectul este Sandwich
        auto sand = std::dynamic_pointer_cast<Sandwich>(p);
        if (sand && sand->getStoc() < 3)
        {
            std::cout << " [!] SANDWICH:  " << sand->getNume() << " | Stoc: " << sand->getStoc() << "\n";
            gasit = true;
        }
    }

    if (!gasit)
    {
        std::cout << "Toate preparatele de patiserie si sandwich-urile au stoc suficient.\n";
    }
    std::cout << "--------------------------------------------------------\n";
}
///cauta dupa nume un pointer in Ingredient
Ingredient* Gestiune::gasesteIngredient(const std::string& nume)
{
    for (auto& ing : listaIngrediente) if (ing.getNume() == nume) return &ing;
    return nullptr;
}

///identifica un topping specific în lista de topping-uri
Ingredient* Gestiune::gasesteTopping(const std::string& nume)
{
    for (auto& top : listaToppinguri) if (top.getNume() == nume) return &top;
    return nullptr;
}

///cauta in meniu dupa nume si returneaza adresa acestuia
Produs* Gestiune::gasesteProdusDupaNume(const std::string& nume) const
{
    for (const auto& p : meniu) if (p->getNume() == nume) return p.get();
    return nullptr;
}

void Gestiune::incarcaStatistici(const std::string& fisier)
{
    std::ifstream fin(fisier);
    if (!fin) return;

    int ora;
    std::string nume;
    /// citim perechi de tip: Ora Nume_Produs
    while (fin >> ora >> nume)
    {
        frecventaOre[ora]++;
        frecventaProduse[nume]++;
    }
    fin.close();
}

void Gestiune::salveazaVanzareInIstoric(int ora, const std::string& numeProdus) {

    std::ofstream fout("statistici.txt", std::ios::app);

    if (fout.is_open()) {
        fout << ora << " " << numeProdus << "\n";
        fout.close();
    }

    frecventaOre[ora]++;
    frecventaProduse[numeProdus]++;
}

void Gestiune::afiseazaRaportBusiness() const
{
    std::cout << "===== RAPORT BUSINESS =====\n";
    std::cout << "Profit total: " << profitTotal << " RON\n";

    ///calculez ora de varf
    if (!frecventaOre.empty())
    {
        auto varf = std::max_element(frecventaOre.begin(), frecventaOre.end(),
                                     [](const auto& a, const auto& b)
        {
            return a.second < b.second;
        });
        std::cout << "Ora de varf: " << varf->first << ":00 (" << varf->second << " comenzi)\n";
    }

    /// calculez top produs
    if (!frecventaProduse.empty())
    {
        auto top = std::max_element(frecventaProduse.begin(), frecventaProduse.end(),
                                    [](const auto& a, const auto& b)
        {
            return a.second < b.second;
        });
        std::cout << "Cel mai vandut produs: " << top->first << " (" << top->second << " unitati)\n";
    }
}

void Gestiune::adaugaComandaInSesiune(std::shared_ptr<Comanda> c) {
    comenziSesiuneCurenta.push_back(c);
}

void Gestiune::afisareComenziSesiune() const {
    std::cout << "\n--- COMENZI NOI (SESIUNEA CURENTA) ---\n";
    if (comenziSesiuneCurenta.empty()) {
        std::cout << "Nu sunt comenzi noi de preparat.\n";
        return;
    }
    for (const auto& com : comenziSesiuneCurenta) {
        com->afisareSumarConsola();
        std::cout << "--------------------------------------\n";
    }
}
