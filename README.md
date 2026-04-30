# Sistem de Management pentru Cafenea
## Eliza's Coffee Shop

Acest proiect implementează un sistem orientat pe obiect (POO) pentru gestionarea activităților dintr-o cafenea. Aplicația acoperă procese esențiale precum gestionarea comenzilor, administrarea stocurilor, logistica de preparare și generarea de rapoarte de business.

---

## Structura Generală

### 1. Ierarhia Produselor

**Clasă de bază:** `Produs` (abstractă)  
Definește interfața comună pentru toate produsele comercializate, folosind principiul NVI (Non-Virtual Interface).

**Clase derivate:**
- `Bautura`  
  Gestionează rețete complexe bazate pe mai multe ingrediente și permite adăugarea de topping-uri extra.

- `Patiserie`  
  Produse perisabile care au termen de expirare și un adaos comercial fix.

- `Sandwich`  
  Produse personalizabile care pot fi încălzite, influențând atât prețul, cât și timpul de preparare.

---

### 2. Gestiunea Resurselor

- `Ingredient`  
  Reprezintă unitatea de bază din inventar. Monitorizează:
    - stocul disponibil
    - prețul de achiziție
    - valoarea calorică
    - proprietățile dietetice (Vegan, Fără Zahăr, Fără Lactoză)

- `Gestiune`  
  Componenta centrală a sistemului ("creierul aplicației"). Responsabilități:
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
- Monitorizarea stocurilor de ingrediente
- Acces la istoricul comenzilor din `registru.txt`
- Identificarea ingredientelor cu stoc redus
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
- Audit:
    - acces la istoricul complet al comenzilor

---

## Concepte POO Implementate

- **Polimorfism Dinamic**  
  Utilizat pentru calculul prețului și afișarea detaliilor specifice produselor prin `std::shared_ptr<Produs>`.

- **NVI (Non-Virtual Interface)**  
  Metoda `afiseaza()` controlează structura generală, iar detaliile sunt implementate în clasele derivate.

- **Gestiunea Excepțiilor**  
  Ierarhie proprie:
    - `StocInsuficient`
    - `AccesInterzis`
    - `ProdusExpirat`

- **Persistența Datelor**  
  Salvarea automată în fișiere `.txt`:
    - stocuri
    - profit
    - registru de vânzări

- **Membri Statici**  
  Utilizați pentru:
    - ID-uri globale
    - monitorizarea timpului total de lucru al barista-ului
    - profitul total al cafenelei

---

