#include "Autentificare.h"
#include "Exceptii.h"
#include <iostream>

///initializarea constantelor statice, parole pentru logare
const std::string Autentificare::PAROLA_BARISTA = "barista123";
const std::string Autentificare::PAROLA_MANAGER = "manager456";

Rol Autentificare::selecteazaRol()
{
    int optiune;
    while (true)
    {
        std::cout << "\n===== ELIZA'S COFFEE SHOP =====\n";
        std::cout << "1. Client\n2. Barista\n3. Manager\n0. Iesire\n";
        std::cout << "Alegeti rolul: ";

        ///verificam daca userul a introdus numere
        ///daca a intorodus litere -> eroare
        if (!(std::cin >> optiune))
        {
            std::cin.clear();///resetez starea de eroare
            std::cin.ignore(1000, '\n');///sterg continutul
            continue;
        }

        switch (optiune)
        {
        case 1:
            return Rol::CLIENT;
        case 2:
            return Rol::BARISTA;
        case 3:
            return Rol::MANAGER;
        case 0:
            return Rol::IESIRE;
        default:
            std::cout << "Optiune invalida!\n";
        }
    }
}

void Autentificare::verificaParola(Rol rol)
{
    ///pentru optiunile client si iesire nu se cere parola
    if (rol == Rol::CLIENT || rol == Rol::IESIRE) return;

    std::string parola;
    std::cout << "Introduceti parola pentru " << getNumeRol(rol) << ": ";
    std::cin >> parola;

    ///exceptii implementate in cazul introducerii unei parole gresite
    if (rol == Rol::BARISTA && parola != PAROLA_BARISTA) throw AccesInterzisException();
    if (rol == Rol::MANAGER && parola != PAROLA_MANAGER) throw AccesInterzisException();
}

///afisare useri
std::string Autentificare::getNumeRol(Rol rol)
{
    switch (rol)
    {
    case Rol::CLIENT:
        return "Client";
    case Rol::BARISTA:
        return "Barista";
    case Rol::MANAGER:
        return "Manager";
    default:
        return "Necunoscut";
    }
}
