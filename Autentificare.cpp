#include "Autentificare.h"
#include "Exceptii.h"
#include<iostream>

const std::string Autentificare::PAROLA_BARISTA = "barista123";
const std::string Autentificare::PAROLA_MANAGER = "manager456";

Rol Autentificare::selecteazaRol()
{
    int optiune;
    while (true)
    {
        std::cout << "\n===== ELIZA'S COFFEE SHOP =====\n";
        std::cout << "1. Client\n2. Barista\n3. Manager\n0. Iesire\n";
        std::cout << "Selectati rolul: ";

        if (!(std::cin >> optiune))
        {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            continue;
        }
        switch (optiune)
        {
        case 1: return Rol::CLIENT;
        case 2: return Rol::BARISTA;
        case 3: return Rol::MANAGER;
        case 0: exit(0);
        default:
            std::cout << "\nEroare! Optiune invalida!\n";
        }
    }
}

void Autentificare::verificaParola(Rol rol)
{
    if (rol == Rol::CLIENT)
        return;
    std::string parola;
    std::cout << "Introduceti parola pentru " << numeRol(rol) << ": ";
    std::cin >> parola;
    if (rol == Rol::BARISTA && parola != PAROLA_BARISTA)
        throw AccesInterzisException();
    if (rol == Rol::MANAGER && parola != PAROLA_MANAGER)
        throw AccesInterzisException();
}

std::string Autentificare::numeRol(Rol rol)
{
    switch (rol)
    {
    case Rol::CLIENT: return "Client";
    case Rol::BARISTA: return "Barista";
    case Rol::MANAGER: return "Manager";
    default:
        return "Necunoscut";
    }
}
