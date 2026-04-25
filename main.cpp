#include<iostream>
#include<cstring>
void antet();
class Ingredient
{
private:
    char* nume;///nume ingredient
    int stoc;///stoc disponibil
    float pret;///pretul cu care a fost cumparat ingredientul
    bool vegan;///vegan=1=>ingredient vegan, vegan=0=>ingredient non-vegan
    bool faraZahar;///faraZahar=1=>ingredient fara zahar, faraZahar=0=>ingredient cu zahar
    float kcal;///numarul de kilocalorii pe care il are ingredientul
public:
    ///Constructor implicit
    Ingredient():nume(nullptr), stoc(0), pret(0.0f), vegan(false), faraZahar(false), kcal(0.0f) {}
    ///Constructor de initializare
    Ingredient(const char* nume_, int stoc_, float pret_, bool vegan_, bool faraZahar_, float kcal_) :stoc{stoc_}, pret{pret_}, vegan{vegan_}, faraZahar{faraZahar_}, kcal{kcal_}{
        if (nume_){
            nume=new char[std::strlen(nume_)+1];
            std::strcpy(nume, nume_);
        }
        else nume=nullptr;
    }
    ///Constructor cu un singur parametru
    explicit Ingredient (const char *nume_): stoc(0), pret(0.0f), vegan(false), faraZahar(false), kcal(0.0f){
        if (nume_){
            nume=new char[std::strlen(nume_)+1];
            std::strcpy(nume, nume_);
        }
        else nume=nullptr;
    }
    ///Constructor de copiere (CC)
    Ingredient(const Ingredient& other):stoc(other.stoc), pret(other.pret), vegan(other.vegan), faraZahar(other.faraZahar), kcal(other.kcal){
        if (other.nume){
            nume=new char[std::strlen(other.nume)+1];
            std::strcpy(nume, other.nume);
        }
        else nume=nullptr;
    }
    ///Operator de copiere (op=)
    Ingredient& operator=(const Ingredient& other){
        ///eliberez memoria
        if (this!=&other){
            delete[] nume;
            ///copiez datele vechi
            stoc=other.stoc;
            pret=other.pret;
            vegan=other.vegan;
            faraZahar=other.faraZahar;
            kcal=other.kcal;
            ///aloc spatiu si copiez noul nume
            if (other.nume){
                nume=new char[std::strlen(other.nume)+1];
                std::strcpy(nume, other.nume);
            }
            else nume=nullptr;
        }
        return *this;
    }
    ///Destructor
    ~Ingredient(){delete[] nume;}
    inline void reaprovizionare(int stocAdaugat){stoc+=stocAdaugat;}///la apelarea functiei se mareste stocul
    inline void consumaStoc(){stoc--;}///folosita dupa prepararea unei bauturi pentru scaderea stocului ingredientelor folosite
    //Getteri
    const char* getNume() const { return nume;}
    int getStoc() const {return stoc;}
    float getPret() const {return pret;}
    bool getVegan() const{return vegan;}
    bool getFaraZahar() const{return faraZahar;}
    float getKcal() const{return kcal;}
    void setNume(const char* numeNou)///sett-er
    {
        if (numeNou){
            delete[] nume;
            nume = new char[strlen(numeNou) + 1];
            strcpy(nume, numeNou);
        }
    }
    ///Friend pentru operatori
    friend std::istream& operator>>(std::istream& is, Ingredient& ing){
        char copie[300];
        if (is>>copie>>ing.stoc>>ing.pret>>ing.vegan>>ing.faraZahar>>ing.kcal){
            delete[] ing.nume;
            ing.nume=new char[std::strlen(copie)+1];
            std::strcpy(ing.nume, copie);
        }
        return is;
    }
    friend std::ostream& operator<<(std::ostream& os, const Ingredient& ing){
        os<<"nume: " << ing.nume <<", stoc: " <<ing.stoc <<", pret: "<<ing.pret<<", vegan: "<<ing.vegan<<", fara zahar: "<<ing.faraZahar<<", kcal:"<<ing.kcal<<"\n";
        return os;
    }
};

class Bautura{
    char *numeBautura;///numele bauturii
    float pretPreparare;///costul prepararii bauturii de catre barista
    int timpPreparare;///timpul necesar prepararii bauturii
    Ingredient* listaIngrediente;///lista de ingrediente necesara fiecarui preparat este salvata cu ajutorul pointerilor spre tipul Ingredient pentru a retine si proprietatile din clasa Ingredient, nu doar numele
    int nrIngrediente;///numarul de ingrediente pentru fiecare bautura
    inline float calculPretIngrediente() const///aici calculez pretul bauturii, dar fara costul de preparare, strict suma ingredientelor
    {
        float total=0;
        for(int i=0; i<nrIngrediente; i++)
            total+=listaIngrediente[i].getPret();
         return total;
    }
public:
    ///Constructor implicit
    Bautura():numeBautura(nullptr), pretPreparare(0.0f), timpPreparare(0), listaIngrediente(nullptr), nrIngrediente(0) {}
    ///Constructor de initializare
    Bautura(const char* numeBautura_, float pretPreparare_, int timpPreparare_): pretPreparare{pretPreparare_}, timpPreparare{timpPreparare_}, listaIngrediente(nullptr), nrIngrediente(0){
        if (numeBautura_){
            numeBautura = new char[std::strlen(numeBautura_) + 1];
            std::strcpy(numeBautura, numeBautura_);
        }
        else numeBautura = nullptr;
    }
    ///Constructor de copiere(CC)
    Bautura(const Bautura& other):pretPreparare(other.pretPreparare), timpPreparare(other.timpPreparare), nrIngrediente(other.nrIngrediente){
        if (other.numeBautura){
            numeBautura = new char[std::strlen(other.numeBautura) + 1];
            std::strcpy(numeBautura, other.numeBautura);
        }
        else numeBautura = nullptr;
        if (other.listaIngrediente){
            listaIngrediente = new Ingredient[other.nrIngrediente];
            for (int i=0; i<other.nrIngrediente; i++)
                listaIngrediente[i]=other.listaIngrediente[i]; // Se bazeaza pe op= din Ingredient
        }
        else listaIngrediente = nullptr;
    }
    ///Operator de copiere(op=)
    Bautura& operator=(const Bautura& other){
        ///eliberez memoria
        if (this!=&other){
            delete[] numeBautura;
            delete[] listaIngrediente;
            ///copiez datele vechi
            pretPreparare=other.pretPreparare;
            timpPreparare=other.timpPreparare;
            nrIngrediente=other.nrIngrediente;
            ///aloc spatiu si copiez noul nume
            if (other.numeBautura){
                numeBautura = new char[std::strlen(other.numeBautura) + 1];
                std::strcpy(numeBautura, other.numeBautura);
            }
            else numeBautura = nullptr;
            if (other.listaIngrediente){
                listaIngrediente = new Ingredient[other.nrIngrediente];
                for (int i=0; i<other.nrIngrediente; i++)
                    listaIngrediente[i] = other.listaIngrediente[i]; // Se bazeaza pe op= din Ingredient
            }
            else listaIngrediente = nullptr;
        }
        return *this;
    }
    ///Destructor
    ~Bautura()  {  delete[] numeBautura;  delete[] listaIngrediente; }
    ///Getteri
    const char* getNumeBautura() const { return numeBautura; }
    int getTimpPreparare() const { return timpPreparare; }
    float getPretPreparare() const { return pretPreparare; }
    Ingredient* getListaIngrediente() const { return listaIngrediente; }
    int getNrIngrediente() const { return nrIngrediente; }
    ///sett-er
    void setNumeBautura(const char* numeBauturaNou){
        if (numeBauturaNou){
            delete[] numeBautura;
            numeBautura=new char[strlen(numeBauturaNou)+1];
            strcpy(numeBautura, numeBauturaNou);
        }
    }
    void setTimpPreparare(int timpPreparareNou){ timpPreparare=timpPreparareNou; }
    void setPretPreparare(float pretPreparareNou) {pretPreparare=pretPreparareNou;}
    void setNrIngrediente(int nrIngredienteNou) {
        if (nrIngredienteNou>0) {
            delete [] listaIngrediente;;
            nrIngrediente=nrIngredienteNou;
            listaIngrediente=new Ingredient[nrIngrediente];
        }
    }
    inline float calculeazaPret() const{return pretPreparare+calculPretIngrediente();} ///calculeaza pretul total al unei bauturi, cu tot cu pretul ingredientelor si a costului de preparare
    inline bool verificaVegan() const///verifica daca bautura este vegana=>true else false
    {  for(int i=0; i<nrIngrediente; i++){
            if(listaIngrediente[i].getVegan()==0)
                return false;
        }
        return true;
    }
    inline bool verificaFaraZahar() const///verifica daca bautura nu are zahar=>true else false
    {for(int i=0; i<nrIngrediente; i++){
            if(listaIngrediente[i].getFaraZahar()==0)
                return false;
        }
        return true;
    }
    bool verificaStoc() const///verifica daca bautura poate fi preparata(adica daca are toate ingredientele in stoc)
    {for(int i=0; i<nrIngrediente; i++){
            if(listaIngrediente[i].getStoc()==0)
               return false;
        }
        return true;
    }
    ///Friend pentru operatori
    friend std::istream& operator>>(std::istream& is, Bautura& bau){
        char copie[300];
        if(is>>copie>>bau.pretPreparare>>bau.timpPreparare>>bau.nrIngrediente){
            delete[] bau.numeBautura;
            bau.numeBautura=new char[std::strlen(copie)+1];
            std::strcpy(bau.numeBautura, copie);
            delete[] bau.listaIngrediente;
            bau.listaIngrediente=new Ingredient[bau.nrIngrediente];
            for(int i=0; i<bau.nrIngrediente; i++){
                char numeIng[100];
                is>>numeIng;
                bau.listaIngrediente[i].setNume(numeIng);
            }
        }
        return is;
    }
    friend std::ostream& operator<<(std::ostream& os, const Bautura& bau){
        os<<"Nume: " << bau.numeBautura <<", pret preparare: " <<bau.pretPreparare<<", timp preparare: "<<bau.timpPreparare<<", lista de ingrediente: ";
        for(int i=0; i<bau.nrIngrediente-1; i++)
            os<<bau.listaIngrediente[i].getNume()<<", ";
        os<<bau.listaIngrediente[bau.nrIngrediente-1].getNume()<<"\n";
        return os;
    }
};
class Comanda{
private:
    static double profitVanzari;///profitul total pe toate comenzile
    static int nrTotalComenzi;///numarul total de comenzi pe sesiune de vanzari
    char *numeClient;///numele clientului care initiaza o comanda
    int nrBauturiComandate;///numarul de bauturi pe care le coamanda
    Bautura* listaBauturi;///lista de bauturi cu pointer spre tipul Bautura pentru a retine bauturile comandate cu toat especificatiile din clasa Bautura
    float totalPlata;///total plata per comanda
    char *oraComandata;///ora la care se initiaza comanda, introdusa manual de catre client
    inline void actualizeazaProfit(float pretBautura, bool esteVegan)///calculeaza profitul per bautura in fucntie de specificatiile acesteia, in cazul in care este vegana profitul este mai mic=20%, altfel 40%
{       double profit;
        if(esteVegan)
            profit=pretBautura*0.2;
        else
            profit=pretBautura*0.4;
        profitVanzari+=profit;///se aduna la profitul total
    }
public:
    ///Constructor implicit
    Comanda(): numeClient(nullptr), nrBauturiComandate(0), listaBauturi(nullptr), totalPlata(0.0f), oraComandata(nullptr) {}
    ///Constructor de initializare
    Comanda(const char* numeClient_, int nrBauturiComandate_, float totalPlata_, const char* oraComandata_):  nrBauturiComandate(nrBauturiComandate_), totalPlata(totalPlata_){
        if (numeClient_){
            numeClient=new char[std::strlen(numeClient_)+1];
            std::strcpy (numeClient, numeClient_);
        }
        else numeClient=nullptr;
        if (oraComandata_){
            oraComandata=new char[std::strlen(oraComandata_)+1];
            std::strcpy (oraComandata, oraComandata_);
        }
        else oraComandata=nullptr;
        if(nrBauturiComandate_>0)
            listaBauturi=new Bautura[nrBauturiComandate_];
        else listaBauturi=nullptr;
    }
    ///Constructor de copiere (CC)
    Comanda(const Comanda& other):nrBauturiComandate(other.nrBauturiComandate), totalPlata(other.totalPlata){
        if (other.numeClient){
            numeClient=new char[std::strlen (other.numeClient)+1];
            std::strcpy (numeClient, other.numeClient);
        }
        else numeClient=nullptr;
        if (other.listaBauturi){
            listaBauturi=new Bautura[other.nrBauturiComandate];
            for (int i=0; i<other.nrBauturiComandate; i++)
                listaBauturi[i]=other.listaBauturi[i];
        }
        else listaBauturi=nullptr;
        if (other.oraComandata){
            oraComandata=new char[strlen(other.oraComandata) + 1];
            strcpy(oraComandata, other.oraComandata);
        }
        else oraComandata=nullptr;
    }
///Operator de copiere(op=)
    Comanda& operator=(const Comanda& other){
        if (this!=&other){
            delete [] numeClient;
            delete [] listaBauturi;
            delete [] oraComandata;
            ///copiez datele vechi
            nrBauturiComandate=other.nrBauturiComandate;
            totalPlata=other.totalPlata;
            ///aloc spatiu si copiez noul nume
            if (other.numeClient){
                numeClient=new char[std::strlen (other.numeClient)+1];
                std::strcpy (numeClient, other.numeClient);
            }
            else numeClient=nullptr;
            if (other.oraComandata){
                oraComandata=new char[strlen(other.oraComandata) + 1];
                strcpy(oraComandata, other.oraComandata);
            }
            else oraComandata=nullptr;
            if (other.listaBauturi){
                listaBauturi=new Bautura[other.nrBauturiComandate];
                for (int i=0; i<other.nrBauturiComandate; i++){
                    listaBauturi[i]=other.listaBauturi[i];
                }
            }
            else listaBauturi=nullptr;
        }
        return *this;
    }
///Destructor
    ~Comanda(){  delete[] numeClient;  delete[] listaBauturi;  delete[] oraComandata;}
///Gett-eri
const char* getNumeClient()const{return numeClient;}
int getNrBauturiComandate()const{return nrBauturiComandate;}
float getTotalPlata()const{return totalPlata;}
const char* getOraComandata()const{return oraComandata;}
Bautura* getListaBauturi()const{return listaBauturi;}
static int getNrTotalComenzi(){return nrTotalComenzi;}
static double getProfitVanzari(){return profitVanzari;}
    static void addComanda(){nrTotalComenzi++;}
    float calculeazaKcal() const///aceasta functie este apelata doar dupa ce s-a trimis o comanda
{float kcalTotal=0.0f;
        for (int i=0; i<nrBauturiComandate; i++)///se trece prin toate bauturile comandate
        {Ingredient* ingredienteProdus=listaBauturi[i].getListaIngrediente();///se creeaza o lista pentru a lua ingredientele corecte specifice fiecarei bauturi cu atributele implicite clasei Ingredient
            int n=listaBauturi[i].getNrIngrediente();
            float kcalBautura=0.0f;///aici se retin si kcal pentru fiecare bautura in parte
            for(int j=0; j<n; j++)
                kcalBautura+=ingredienteProdus[j].getKcal();///iau kcal de la fiecare ingredient continut
            kcalTotal+=kcalBautura;
            std::cout<<i+1<<". "<<listaBauturi[i].getNumeBautura()<<"  ==>  "<<kcalBautura<<" kcal\n";///afisez pentru fiecare bautura
        }
        return kcalTotal;///returnez kcal totale pe toata comanda
    }
    void bonFiscal()///aceasta functie genereaza un bon fiscal dupa ce s-a dat o comanda
    {double sumaFinala=0;///suma totala de plata per comanda
        int timpTotal=0;///timpul total de preparare per comanda
        antet();///pentru aspect
        std::cout<<"Nume Client: "<<numeClient<<"                      Ora Comenzii: "<<oraComandata<<"\n";
        std::cout<<"-----------------------------------------------------------------\n";
        std::cout<<"Numar Comanda: "<<nrTotalComenzi<<"\n";
        std::cout<<"-----------------------------------------------------------------\n";
        for (int i=0; i<nrBauturiComandate; i++){
            bool vegan=false;
            double pretBautura=listaBauturi[i].getPretPreparare();///base price
            Ingredient* ingredienteProdus=listaBauturi[i].getListaIngrediente();///!!!de verificat daca merge cu functie de calculare a pretului
            int n=listaBauturi[i].getNrIngrediente();
            for(int j=0; j<n; j++)
                pretBautura+=ingredienteProdus[j].getPret();
            std::cout<<i+1<<". "<<listaBauturi[i].getNumeBautura()<<"  ";
            if(listaBauturi[i].verificaFaraZahar())///verifica calitate Zero Zahar
                std::cout<<"*(zero-zahar)  ";
            if (listaBauturi[i].verificaVegan()){
                vegan=true;
                std::cout<<"*(vegan)-Aplica reducere 20%  ";///verifica calitate vegan+reduecere produs
                pretBautura-=pretBautura*20/100;

            }
            actualizeazaProfit(pretBautura, vegan);///se actualizeaza profitul
            std::cout<<" = "<<pretBautura<<" RON\n";///se afiseaza pret per bautura
            sumaFinala+=pretBautura;
            timpTotal+=listaBauturi[i].getTimpPreparare();
        }
        std::cout<<"-----------------------------------------------------------------\n";
        std::cout<<"TOTAL DE PLATA:                          "<<sumaFinala<<" RON\n";
        std::cout<<"Timp estimat de preparare:               "<<timpTotal/60<<" minute si "<<timpTotal%60<<" secunde\n";
        std::cout<<"-----------------------------------------------------------------\n";
        std::cout<<"--------------------------Va multumim!---------------------------\n";
    }
///Friend pentru operatori
    friend std::istream& operator>>(std::istream& is, Comanda& com){
        char copie[300];///numeclient+nrbauturi+ora coamndata
        if (is>>copie>>com.nrBauturiComandate>>com.oraComandata){
            delete[] com.numeClient;
            com.numeClient=new char[std::strlen(copie)+1];
            std::strcpy (com.numeClient, copie);
            delete[] com.listaBauturi;
            com.listaBauturi=new Bautura[com.nrBauturiComandate];
            for (int i=0; i<com.nrBauturiComandate; i++){
                char numeBau[100];
                is>>numeBau;
                com.listaBauturi[i].setNumeBautura(numeBau);
            }
        }
        return is;
    }
    friend std::ostream& operator<<(std::ostream& os, const Comanda& com){
        os<<"Nume client: "<<com.numeClient<<", comanda contine: ";
        for (int i=0; i<com.nrBauturiComandate-1; i++)
            os<<com.listaBauturi[i].getNumeBautura()<<", ";
        os<<com.listaBauturi[com.nrBauturiComandate-1].getNumeBautura()<<", ora la care s-a dat comanda: "<<com.oraComandata<<"\n";
        return os;
    }
};
void afisare1(){
    antet();
    std::cout << "----------------------- MENIU CAFENEA ---------------------\n";
    std::cout << "1. Detalii pentru angajati\n";
    std::cout << "2. Cauta o bautura care contine sau NU un anumit ingredient\n";
    std::cout << "3. Comanda\n";
    std::cout << "4. Indecis? Consulta ce avem pe stoc din meniu\n";
    std::cout << "5. Afla detalii despre un preparat\n";
    std::cout << "0. Iesire program\n";
    std::cout << "Introduceti optiunea: ";
}
void afisare2(){
    antet();
    std::cout << "1. Vezi bonul comenzii\n";
    std::cout << "2. Vezi valorile nutritionale pentru intreaga comanda\n";
    std::cout << "0. Intoarce-te la meniul principal\n";
    std::cout << "Introduceti optiunea: ";
}
void afisare3(){
    antet();
    std::cout << "1. Aprovizioneaza stoc\n";
    std::cout << "2. Calculeaza profit pe sesiune vanzari\n";
    std::cout << "0. Intoarce-te la meniul principal\n";
    std::cout << "Introduceti optiunea: ";
}
void antet(){
    std::cout<<"=================================================================\n";
    std::cout<<"                    Eliza's coffee shop                          \n";
    std::cout<<"=================================================================\n";
}
int Comanda::nrTotalComenzi=0;///atributele statice sunt initializate
double Comanda::profitVanzari=0.0d;

int main()
{
    Comanda comandaCurenta;///un obiect pentru comanda curenta, acesta se rescrie la fiecare comanda noua
    int nrIngrediente=25;
    Ingredient* camara=new Ingredient[nrIngrediente];///declar o camara care are referinta spre tipul Ingredient pentru a salva ingredientele din fisier intr-un mod corect sa fie valorile atribuite corect tuturor atributelor
    camara[0] = Ingredient("Cafea_Arabica", 50, 2.5, 1, 1, 2);
    camara[1] = Ingredient("Cafea_Robusta", 3000, 1.8, 1, 1, 2);
    camara[2] = Ingredient("Matcha_Pudra", 500, 8.0, 1, 1, 30);
    camara[3] = Ingredient("Ciocolata_Neagra_Pudra", 1000, 3.5, 1, 0, 45);
    camara[4] = Ingredient("Ciocolata_Alba_Lichida", 1000, 4.0, 0, 0, 55);
    camara[5] = Ingredient("Ceai_Verde_Frunze", 300, 5.0, 1, 1, 1);
    camara[6] = Ingredient("Ceai_Negru_Earle_Grey", 300, 5.0, 1, 1, 1);
    camara[7] = Ingredient("Ceai_Fructe_Padure", 500, 4.0, 1, 1, 2);
    camara[8] = Ingredient("Lapte_Vaca", 10000, 1.2, 0, 0, 6);
    camara[9] = Ingredient("Lapte_Migdale", 2000, 3.5, 1, 1, 3);
    camara[10] = Ingredient("Lapte_Ovaz", 2000, 3.0, 1, 0, 5);
    camara[11] = Ingredient("Lapte_Soia", 2000, 2.8, 1, 1, 4);
    camara[12] = Ingredient("Frisca_Lichida", 1500, 2.5, 0, 0, 35);
    camara[13] = Ingredient("Sirop_Vanilie", 1000, 4.5, 1, 0, 30);
    camara[14] = Ingredient("Sirop_Caramel_Sarat", 1000, 4.8, 1, 0, 32);
    camara[15] = Ingredient("Sirop_Fistic", 1000, 6.0, 1, 0, 35);
    camara[16] = Ingredient("Sirop_Alune_Padure", 1000, 4.5, 1, 0, 31);
    camara[17] = Ingredient("Zahar_Alb", 5000, 0.2, 1, 0, 40);
    camara[18] = Ingredient("Zahar_Brun", 3000, 0.4, 1, 0, 38);
    camara[19] = Ingredient("Stevia", 200, 1.5, 1, 1, 0);
    camara[20] = Ingredient("Miere_Bio", 1000, 2.0, 1, 0, 30);
    camara[21] = Ingredient("Apa_Plata", 400, 0.1, 1, 1, 0);
    camara[22] = Ingredient("Scortisoara", 200, 1.5, 1, 1, 2);
    camara[23] = Ingredient("Pudra_Cardamom", 100, 2.5, 1, 1, 3);
    camara[24] = Ingredient("Mini_Marshmallows", 500, 3.0, 0, 0, 33);
    int nrBauturi=15;
    Bautura* meniu=new Bautura[nrBauturi];///fac la fel si pentru bauturi

    // 0. Espresso_Scurt
    meniu[0] = Bautura("Espresso_Scurt", 5.0, 30);
    meniu[0].setNrIngrediente(2);
    meniu[0].getListaIngrediente()[0].setNume("Cafea_Arabica");
    meniu[0].getListaIngrediente()[1].setNume("Apa_Plata");

    // 1. Cappuccino_Clasic
    meniu[1] = Bautura("Cappuccino_Clasic", 7.0, 120);
    meniu[1].setNrIngrediente(4);
    meniu[1].getListaIngrediente()[0].setNume("Cafea_Robusta");
    meniu[1].getListaIngrediente()[1].setNume("Apa_Plata");
    meniu[1].getListaIngrediente()[2].setNume("Lapte_Vaca");
    meniu[1].getListaIngrediente()[3].setNume("Scortisoara");

    // 2. Caffe_Latte
    meniu[2] = Bautura("Caffe_Latte", 8.0, 150);
    meniu[2].setNrIngrediente(3);
    meniu[2].getListaIngrediente()[0].setNume("Cafea_Arabica");
    meniu[2].getListaIngrediente()[1].setNume("Apa_Plata");
    meniu[2].getListaIngrediente()[2].setNume("Lapte_Vaca");

    // 3. Flat_White
    meniu[3] = Bautura("Flat_White", 9.0, 130);
    meniu[3].setNrIngrediente(2);
    meniu[3].getListaIngrediente()[0].setNume("Cafea_Arabica");
    meniu[3].getListaIngrediente()[1].setNume("Lapte_Vaca");

    // 4. Matcha_Latte_Vegan
    meniu[4] = Bautura("Matcha_Latte_Vegan", 12.0, 180);
    meniu[4].setNrIngrediente(4);
    meniu[4].getListaIngrediente()[0].setNume("Matcha_Pudra");
    meniu[4].getListaIngrediente()[1].setNume("Apa_Plata");
    meniu[4].getListaIngrediente()[2].setNume("Lapte_Migdale");
    meniu[4].getListaIngrediente()[3].setNume("Stevia");

    // 5. Oat_Vanilla_Latte
    meniu[5] = Bautura("Oat_Vanilla_Latte", 10.0, 160);
    meniu[5].setNrIngrediente(3);
    meniu[5].getListaIngrediente()[0].setNume("Cafea_Arabica");
    meniu[5].getListaIngrediente()[1].setNume("Lapte_Ovaz");
    meniu[5].getListaIngrediente()[2].setNume("Sirop_Vanilie");

    // 6. Ceai_Verde_Bio
    meniu[6] = Bautura("Ceai_Verde_Bio", 6.0, 240);
    meniu[6].setNrIngrediente(3);
    meniu[6].getListaIngrediente()[0].setNume("Ceai_Verde_Frunze");
    meniu[6].getListaIngrediente()[1].setNume("Apa_Plata");
    meniu[6].getListaIngrediente()[2].setNume("Miere_Bio");

    // 7. Cortado_Migdale
    meniu[7] = Bautura("Cortado_Migdale", 7.5, 90);
    meniu[7].setNrIngrediente(2);
    meniu[7].getListaIngrediente()[0].setNume("Cafea_Robusta");
    meniu[7].getListaIngrediente()[1].setNume("Lapte_Migdale");

    // 8. Choco_White_Snow
    meniu[8] = Bautura("Choco_White_Snow", 11.0, 200);
    meniu[8].setNrIngrediente(4);
    meniu[8].getListaIngrediente()[0].setNume("Ciocolata_Alba_Lichida");
    meniu[8].getListaIngrediente()[1].setNume("Lapte_Vaca");
    meniu[8].getListaIngrediente()[2].setNume("Frisca_Lichida");
    meniu[8].getListaIngrediente()[3].setNume("Mini_Marshmallows");

    // 9. Caramel_Macchiato
    meniu[9] = Bautura("Caramel_Macchiato", 10.5, 170);
    meniu[9].setNrIngrediente(4);
    meniu[9].getListaIngrediente()[0].setNume("Cafea_Arabica");
    meniu[9].getListaIngrediente()[1].setNume("Lapte_Vaca");
    meniu[9].getListaIngrediente()[2].setNume("Sirop_Caramel_Sarat");
    meniu[9].getListaIngrediente()[3].setNume("Zahar_Brun");

    // 10. Mocha_Fistic
    meniu[10] = Bautura("Mocha_Fistic", 12.5, 210);
    meniu[10].setNrIngrediente(4);
    meniu[10].getListaIngrediente()[0].setNume("Ciocolata_Neagra_Pudra");
    meniu[10].getListaIngrediente()[1].setNume("Cafea_Robusta");
    meniu[10].getListaIngrediente()[2].setNume("Lapte_Vaca");
    meniu[10].getListaIngrediente()[3].setNume("Sirop_Fistic");

    // 11. Hot_Choco_Cardamom
    meniu[11] = Bautura("Hot_Choco_Cardamom", 10.0, 190);
    meniu[11].setNrIngrediente(4);
    meniu[11].getListaIngrediente()[0].setNume("Ciocolata_Neagra_Pudra");
    meniu[11].getListaIngrediente()[1].setNume("Lapte_Vaca");
    meniu[11].getListaIngrediente()[2].setNume("Pudra_Cardamom");
    meniu[11].getListaIngrediente()[3].setNume("Zahar_Alb");

    // 12. London_Fog
    meniu[12] = Bautura("London_Fog", 9.5, 300);
    meniu[12].setNrIngrediente(4);
    meniu[12].getListaIngrediente()[0].setNume("Ceai_Negru_Earle_Grey");
    meniu[12].getListaIngrediente()[1].setNume("Apa_Plata");
    meniu[12].getListaIngrediente()[2].setNume("Lapte_Vaca");
    meniu[12].getListaIngrediente()[3].setNume("Sirop_Vanilie");

    // 13. Ceai_Fructe_Iarna
    meniu[13] = Bautura("Ceai_Fructe_Iarna", 7.0, 300);
    meniu[13].setNrIngrediente(4);
    meniu[13].getListaIngrediente()[0].setNume("Ceai_Fructe_Padure");
    meniu[13].getListaIngrediente()[1].setNume("Apa_Plata");
    meniu[13].getListaIngrediente()[2].setNume("Scortisoara");
    meniu[13].getListaIngrediente()[3].setNume("Miere_Bio");

    // 14. Americano
    meniu[14] = Bautura("Americano", 5.0, 60);
    meniu[14].setNrIngrediente(2);
    meniu[14].getListaIngrediente()[0].setNume("Cafea_Arabica");
    meniu[14].getListaIngrediente()[1].setNume("Apa_Plata");

    for(int i=0; i<nrBauturi; i++){
        int nrIngrBautura=meniu[i].getNrIngrediente();///retinem nr de ingrediente pe care le are bautura i din meniu
        Ingredient* listaIngrBautura=meniu[i].getListaIngrediente();///si cream o lista cu acestea, deoarece altfel in cadrul bauturilor din meniu ingredientele nu vor avea niciun atribut vor avea doar numele, iar in rest valorile care ne intereseaza vor ramane implicite(0)
        for(int j=0; j<nrIngrBautura; j++){
            const char *nume=listaIngrBautura[j].getNume();
            for(int k=0; k<nrIngrediente; k++)
                if(nume !=nullptr && strcmp(camara[k].getNume(), nume)==0)
                    listaIngrBautura[j]=camara[k];
        }
    }
    int ok=-1;
    while (ok!=0){
        afisare1();
        std::cin>>ok;
        switch (ok){
        case 1:{
            afisare3();
            int ok1=-1;
            while(ok1!=0){
                std::cin>>ok1;
                switch(ok1){
                case 1:{
                    antet();
                    for(int i=0; i<nrIngrediente; i++)///se afiseaza pentru vanzator, sa vada mai bine{
                        std::cout<<"cod produs: "<<i<<" ==> "<<camara[i]<<"\n";
                    std::cout<<"**********************************************************************************\n";
                    std::cout << "Pentru reaprovizionare te rugam sa introduci codul produsului si stocul adaugat!\n";
                    int cod, aprovizionare;///astea reprezinta numarul de ordine si cu cat sa fie suplimentat
                    std::cin>>cod>>aprovizionare;
                    camara[cod].reaprovizionare(aprovizionare);///se apeleaza functia care suplimenteaza stocul elementului camara[cod]
                    std::cout<<"\nIngredientul selectat a fost reaprovizionat!\n"<<camara[cod]<<"\n";
                    afisare3();
                    break;
                }
                case 2:{
                    antet();
                    std::cout<<"------------------------RAPORT FINANCIAR-------------------------\n";
                    std::cout<<std::endl;
                    std::cout<<"Profitul pentru aceasta sesiune de vanzari este in valoare de "<<Comanda::getProfitVanzari()<<" RON\n";///se afiseaza atributul static
                    std::cout<<std::endl;
                    afisare3();
                    break;
                }
                case 0:
                    break;
                }
            }
            break;
        }
        case 2:{
            antet();
            char raspuns[5];
            char ingredient[100];
            std::cout<<"Despre ce ingredient este vorba?\n";
            std::cin>>ingredient;
            std::cout<<"Doresti ca bautura sa contina sau nu ingredientul? Raspunde cu 'da' sau 'nu'.\n";
            std::cin>>raspuns;
            std::cout<<"Bauturi in meniu cu specificatiile dorite:\n";
            if(strcmp(raspuns, "da")==0)///in functie de raspuns
                {int ver=0;
                for(int i=0; i<nrBauturi; i++){
                    Ingredient* ingredienteProdus=meniu[i].getListaIngrediente();
                    int n=meniu[i].getNrIngrediente();
                    int verif=0;
                    for(int j=0; j<n; j++)
                        if(strcmp(ingredienteProdus[j].getNume(), ingredient)==0)///daca am gasit acest ingredient in bautura => afisez bautura
                            verif=1;
                    if(verif==1){
                        ver=1;
                        std::cout<<"-"<<meniu[i].getNumeBautura()<<"\n";
                    }
                }
                if(ver==0)///daca nu a fost gasit niciun preparat cu acest ingredient
                    std::cout<<"Momentan nu sunt disponibile produse cu aceste specificatii, ne pare rau!\n";
            }
            else{
                int ver=0;
                for(int i=0; i<nrBauturi; i++){
                    Ingredient* ingredienteProdus=meniu[i].getListaIngrediente();
                    int n=meniu[i].getNrIngrediente();
                    int verif=1;
                    for(int j=0; j<n; j++)
                        if(strcmp(ingredienteProdus[j].getNume(), ingredient)==0)///daca am gasit o bautura cu acest ingredient, automat excludem acest preparat
                            verif=0;
                    if(verif==1){
                        ver=1;
                        std::cout<<"-"<<meniu[i].getNumeBautura()<<"\n";///daca nu a fost gasim, afisam
                    }
                }
                if(ver==0)
                    std::cout<<"Momentan nu sunt disponibile produse cu aceste specificatii, ne pare rau!\n";///daca toate produsele contin ingredientul
            }
            break;
        }
        case 3:{
            antet();
            char nume[100], ora[10];
            int nr;
            std::cout<<"Completare date pentru comanda...\n"; std::cin.get();///se elibereaza memorie
            std::cout<<"Introduceti numele si prenumele: "; std::cin.getline(nume, 100);
            std::cout<<"Introduceti ora comenzii: "; std::cin>>ora;
            std::cout<<"Cate bauturi doriti sa comandati? ";  std::cin>>nr;
            comandaCurenta=Comanda(nume, nr, 0.0f, ora);
            Comanda::addComanda();///tinem evidenta la comanda
            for(int i=0; i<nr; i++){
                char numeBautura[100];
                std::cout<<"Bautura "<<i+1<<": ";
                std::cin>>numeBautura;
                bool exista=false;///pentru a verifica daca exista
                bool stoc=true;///si daca este in stoc
                for(int j=0; j<nrBauturi; j++){
                    if(strcmp(meniu[j].getNumeBautura(), numeBautura)==0){
                        exista=true;
                        Ingredient* ingredienteProdus=meniu[j].getListaIngrediente();
                        int n=meniu[j].getNrIngrediente();
                        bool stoc=true;
                        for(int k=0; k<n; k++)
                            if(ingredienteProdus[k].getStoc()==0) {
                                stoc=false;
                                break;
                            }
                        if(stoc==false){
                            std::cout<<"Din pacate produsul introdus nu este momentan pe stoc! Va rugam introduceti alta bautura!\n";
                            i--;
                        }
                        else{
                            comandaCurenta.getListaBauturi()[i]=meniu[j];///aici pentru a consuma din ingrediente
                            for(int k=0; k<n; k++){
                                for(int m=0; m<nrIngrediente; m++){
                                    if(strcmp(camara[m].getNume(), ingredienteProdus[k].getNume())==0){
                                        camara[m].consumaStoc();
                                        break;
                                    }
                                }
                            }

                        }
                        exista=true;
                        break;
                    }
                }
                if(exista==false){
                    std::cout<<"Ne pare rau! Nu avem inca aceasta bautura in meniu! Incercati din nou!\n";
                    i--;
                }
            }
            std::cout<<"COMANDA INREGISTRATA CU SUCCES!\n";
            afisare2();
            int ok2=-1;
            while(ok2!=0){
                std::cin>>ok2;
                switch(ok2){
                case 1:{
                    comandaCurenta.bonFiscal();///generare bon fiscal prin apelare functie
                    afisare2();
                    break;
                }
                case 2:{
                    std::cout<<"--------------------------------------------------\n";
                    std::cout<<"Valorile nutritionale pentru comanda dvs sunt disponibile!\n";
                    int total=comandaCurenta.calculeazaKcal();
                    std::cout<<"Pentru toata comanda aveti "<<total<<" kilocalorii.\n";
                    afisare2();
                    break;
                }
                case 0:
                    break;
                }
            }
            break;
        }
        case 4:{
            antet(); ///bauturi care sunt in stoc;
            std::cout<<"OPTIUNILE NOASTE: \n";
            std::cout<<std::endl;
            for(int i=0; i<nrBauturi; i++){
                Ingredient* ingredienteProdus=meniu[i].getListaIngrediente();
                int n=meniu[i].getNrIngrediente();
                bool stoc=true;
                for(int j=0; j<n; j++)
                    if(ingredienteProdus[j].getStoc()==0)
                        stoc=false;
                if(stoc==true)
                    std::cout<<meniu[i].getNumeBautura()<<"\n";
            }
            break;
        }
        case 5:{
            antet(); ///detalii preparat: nume, vegan, fara zahar, ingrediente, pret, kcal
            std::cout<<"Introduceti va rog numele preparatului despre care doriti sa vedeti detalii: ";
            char numePreparat[100];
            std::cin>>numePreparat;
            bool ok=false;///verificam daca exista
            for(int i=0; i<nrBauturi; i++){
                if(strcmp(meniu[i].getNumeBautura(), numePreparat)==0){
                    ok=true;
                    float pret=0.0f, kcal=0.0f;
                    bool stoc=true;
                    std::cout<<meniu[i].getNumeBautura()<<": \n";
                    if(meniu[i].verificaFaraZahar())
                        std::cout<<"*fara zahar*\n";
                    else
                        std::cout<<"*contine zahar*\n";
                    if(meniu[i].verificaVegan())
                        std::cout<<"*vegan*\n";
                    else
                        std::cout<<"*non-vegan*\n";
                    pret=meniu[i].calculeazaPret();
                    Ingredient* ingredienteProdus=meniu[i].getListaIngrediente();
                    int n=meniu[i].getNrIngrediente();
                    for(int j=0; j<n; j++){
                        kcal+=ingredienteProdus[j].getKcal();
                        if(ingredienteProdus[j].getStoc()==0)
                            stoc=false;
                    }
                    std::cout<<"*"<<kcal<<" kcal*\n";
                    std::cout<<"*"<<pret<<" RON*\n";
                    if(stoc==true)
                        std::cout<<"*ESTE IN STOC*\n";
                    else
                        std::cout<<"*NU ESTE IN STOC*\n";
                }
            }
            if(ok==false)
                std::cout<<"Ne pare rau! Nu avem inca aceasta bautura in meniu!\n";
            break;
        }
        case 0:
            break;
        }
    }
    delete[] camara;
    delete[] meniu;
    return 0;
}