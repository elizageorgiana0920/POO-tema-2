#ifndef AUTENTIFICARE_H
#define AUTENTIFICARE_H

#include <string>

enum class Rol { CLIENT, BARISTA, MANAGER, IESIRE };

class Autentificare
{
private:
    static const std::string PAROLA_BARISTA;
    static const std::string PAROLA_MANAGER;

public:
    static Rol selecteazaRol();
    static void verificaParola(Rol rol);
    static std::string getNumeRol(Rol rol);
};

#endif
