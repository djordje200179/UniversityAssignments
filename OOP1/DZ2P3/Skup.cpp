#include "Skup.hpp"

using namespace std;

Skup::Skup(string inicijalizator) {
    for (char karakter : inicijalizator)
        *this += karakter;
}

Skup& Skup::operator+=(char karakter) {
    if (!(*this)(karakter))
        karakteri += karakter;

    return *this;
}

bool Skup::operator()(char karakter) const {
    for (char privremeni : karakteri)
        if (privremeni == karakter)
            return true;

    return false;
}