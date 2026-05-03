# Sistem de Management pentru Cafenea
## Eliza's Coffee Shop

Acest proiect implementează un sistem orientat pe obiect (POO) pentru gestionarea activităților dintr-o cafenea. Aplicația acoperă procese esențiale precum gestionarea comenzilor, administrarea stocurilor, logistica de preparare și generarea de rapoarte de business.

---

## ️Structura Claselor și Metodele Utilizate

### 1. Clasa `Produs` (Clasa de Bază)
* `virtual std::shared_ptr<Produs> clone() const = 0;`
* `virtual float calculeazaPretFinal() const = 0;` // Calcul preț total pentru un produs
* `virtual bool esteDisponibil() const = 0;` // Verificare stoc și termen de valabilitate
* `void afiseaza(std::ostream& os) const;` // Interfață Non-Virtuală (NVI) pentru afișare
* `virtual bool esteVegan() const = 0;` // Getter pentru filtrare
* `virtual float calculeazaKcalTotal() const = 0;` // Calcul kilocalorii

### 2. Clasa `Bautura`
* `void adaugaIngredient(Ingredient* ing);` // Definire rețetă de bază
* `void adaugaToppingExtra(Ingredient* top);` // Personalizare comandă client
* `float calculeazaPretFinal() const override;` // Suma (PretBaza + Ingrediente + Toppings)
* `const std::vector<Ingredient*>& getIngrediente() const override;` // Returnează lista de ingrediente

### 3. Clasa `Sandwich`
* `void cereIncalzit();` // Opțiune încălzire
* `float calculeazaPretFinal() const override;` // Include taxa de încălzire (+2 RON)
* `bool esteExpirat() const;` // Verificare timestamp vs timp curent
* `void marcheazaExpirat();` // Setează stocul la 0 (scoatere forțată din meniu)
* `void scadeStoc();` // Actualizare inventar după vânzare

### 4. Clasa `Patiserie`
* `bool esteExpirat() const;` // Verificare valabilitate
* `void marcheazaExpirat();` // Setează stocul la 0
* `void scadeStoc();` // Actualizare stoc
* `int getStoc() const;` // Accesare cantitate disponibilă

### 5. Clasa `Gestiune` 
* `void incarcaIngrediente(string fisier);` // Citire bază de date materii prime
* `void incarcaMeniuSandwich(string fisier);` // Instanțiere obiecte Sandwich din fișier
* `void incarcaMeniuPatiserie(string fisier);` // Instanțiere obiecte Patiserie din fișier
* `void afisareMeniu(bool doarDisponibile) const;` // Vizualizare catalog produse
* `void verificaProduseExpirate();` // Curăță vitrina de produse expirate
* `void proceseazaComanda(shared_ptr<Produs> p, int ora);` // Scade stoc, adaugă profit, scrie registru
* `void afisareProduseCriticePatiserieSandwich() const;` // Alerta: stoc < 3 unități
* `void afisareRapoarte() const;` // Analiză date: Top Produs, Profit Total, Ora de Vârf

### 6. Clasa `Comanda`
* `void adaugaProdus(shared_ptr<Produs> p);` // Adăugare în coșul de cumpărături
* `void finalizeazaComanda(Gestiune& g);` // Confirmare plată și execuție procesare stoc
* `void afisareSumarConsola() const;` // Afișare bon fiscal înainte de plată

### 7. Clasa `Autentificare` (Sistemul de Securitate)
* `static Rol selecteazaRol();` // Gestionează meniul de start și validarea tipului de utilizator
* `static void verificaParola(Rol rol);` // Verifică credențialele și afișează excepția `AccesInterzisException` în caz de eșec
* `static string getNumeRol(Rol rol);` // Convertește tipul Enum în text lizibil pentru interfață

### 8. Clasa `Ingredient` (Gestiunea Materiilor Prime)
* `void consumaStoc();` // Scade stocul cu 1
* `bool atentieStocMic();` // Indicator pentru pragul critic de aprovizionare (< 5)
* `operator>>` / `operator<<` // Facilitează citirea din fișiere și afișarea detaliată a valorilor nutriționale
---

## Main.cpp

### Fluxul Principal
1. **Inițializare**: Constructorul `Gestiune` încarcă profitul salvat.
2. **Încărcare Date**: Citirea tuturor fișierelor `.txt` (Ingrediente, Bauturi, Patiserie, Sandwich-uri, Topping-uri).
3. **Verifica la început vitrina**: `verificaPatiserieExpirata()` și `verificaSandwichExpirat()` setează stocul la 0 pentru produsele vechi.
4. **Autentificare**: Selectare ROL (Client, Barista, Manager) prin clasa `Autentificare`.

### Structura Meniului Interactiv

| Rol | Acțiuni Principale                                                                |
| :--- |:----------------------------------------------------------------------------------|
| **CLIENT** | Vizualizare meniu, Filtrare dietetică, Căutare, Comandă                           |
| **BARISTA** | Verificare meniu disponibil, Vizualizare stocuri critice, Vede comenzi pe sesiune |
| **MANAGER** | Reaprovizionare, Rapoarte Business, Istoric Comenzi, Alerte Stoc Preparate        |


## 📄 Persistența Datelor
* `profit_total.txt`: Salvează profitul net la închiderea programului.
* `registru.txt`: Jurnalul tuturor tranzacțiilor (ora, produs, preț).
* `statistici.txt`: Tine evidenta orelor si produselor la care se comanda pentru alcatuirea orei de varf si a celui mai vandut produs.
* `ingrediente.txt` & `toppinguri.txt`: Stocuri actualizate automat de Destructor.


## Structura Generală

### 1. Ierarhia Produselor

**Clasă de bază:** `Produs` 
**Clase derivate:**
- `Bautura`
- `Patiserie`
- `Sandwich`  

---

### 2. Gestiunea Resurselor

- `Ingredient`  
  Monitorizează:
    - stocul disponibil
    - prețul de achiziție
    - valoarea calorică
    - proprietățile dietetice (Vegan, Fără Zahăr, Fără Lactoză)

- `Gestiune`  
  Funcționalitate:
    - coordonarea meniului
    - încărcarea datelor din fișiere
    - persistența stocurilor și a profitului între sesiuni

---

### 3. Logica de Tranzacționare

- `Comanda`  
  Gestionează fluxul comenzilor:
    - administrarea coșului de cumpărături
    - generarea codurilor unice de ridicare
    - calculul timpului de așteptare
    - emiterea bonurilor fiscale în format text

---

## Funcționalități pe Roluri

### Client
- Vizualizarea meniului (complet sau doar produsele disponibile)
- Filtrare avansată:
    - preferințe dietetice (Vegan, Fără Lactoză)
    - includerea/excluderea unor ingrediente
- Personalizarea comenzilor:
    - topping-uri pentru băuturi
    - opțiunea de încălzire pentru sandwich-uri
- Plasarea comenzilor:
    - generare automată cod de ridicare
    - estimare timp de așteptare

---

### Barista
- Monitorizarea stocurilor de produse
- Identificarea ingredientelor cu stoc redus
- Vizualizarea comenzilor care stau la coada una dupa alta
- Autentificare securizată pe bază de parolă

---

### Manager
- Rapoarte de business:
    - profit total
    - cel mai vândut produs
    - ora de vârf
- Management inventar:
    - identificarea stocurilor critice (sub 5 unități)
    - reaprovizionare ingrediente
- Controlul calității:
    - eliminarea produselor expirate
- Baza de date:
    - acces la istoricul complet al comenzilor
- Autentificare securizată pe bază de parolă

---

### Bibliografie
- https://stackoverflow.com/questions/54738445/how-do-i-use-setw-in-c-to-align-format-text-in-the-console
- https://www.geeksforgeeks.org/cpp/file-handling-c-classes/
- https://github.com/mcmarius/poo/tree/master/tema-2
- https://stackoverflow.com/questions/20015656/how-to-create-my-own-header-file-in-c
- https://stackoverflow.com/questions/25020129/cin-ignorenumeric-limitsstreamsizemax-n
- https://www.youtube.com/watch?v=LhJyQZVDQpQ

---