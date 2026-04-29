#ifndef EXCEPTII_H
#define EXCEPTII_H

#include <stdexcept>
#include <string>

///baza ierarhiei de exceptii
class CafeneaException:public std::runtime_error
{
    using std::runtime_error::runtime_error;
};

class StocInsuficientException: public CafeneaException
{
public:
    explicit StocInsuficientException(const std::string& numeIngredient)
        : CafeneaException("EROARE: Ingredientul '" + numeIngredient + "' nu este in stoc!\n") {}
};

class DateInvalideException:public CafeneaException
{
public:
    explicit DateInvalideException(const std::string& detalii): CafeneaException("Eroare: "+detalii) {}
};

class OraInvalidaException : public CafeneaException
{
public:
    OraInvalidaException()
        :CafeneaException("EROARE: Ora trebuie sa fie in format HH:MM!\n") {}
};

class AccesInterzisException: public CafeneaException
{
public:
    AccesInterzisException()
        :CafeneaException("ACCES RESPINS: Parola introdusa este incorecta!\n") {}
};

class ProdusInexistentException:public CafeneaException
{
public:
    explicit ProdusInexistentException(const std::string& nume)
        : CafeneaException("EROARE: Produsul "+nume+" nu exista in meniu!\n") {}
};

class ProdusExpiratException : public CafeneaException
{
public:
    explicit ProdusExpiratException(const std::string& nume)
        :CafeneaException("EROARE: Produsul "+nume+" este expirat si nu poate fi vandut!\n") {}
};

// Exceptie specifica pentru operatiuni de preparare invalide
class OperatiuneInvalidaException : public CafeneaException {
public:
    explicit OperatiuneInvalidaException(const std::string& numeProdus, const std::string& operatiune)
        : CafeneaException("EROARE PREPARARE: Produsul '" + numeProdus + "' nu permite operatiunea: " + operatiune + "!") {}
};
#endif
