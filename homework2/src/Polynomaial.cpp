#include "Polynomial.h"
#include <algorithm>

Polynomial::Polynomial(){
    capacity = 1;
    terms = 0;
    termArray = new Term[capacity];
}

Polynomial::~Polynomial(){
    delete[] termArray;
}

Polynomial::Polynomial(const Polynomial& poly){
    capacity = poly.capacity;
    terms = poly.terms;
    termArray = new Term[capacity];

    for(int i = 0; i < terms; i++)
        termArray[i] = poly.termArray[i];
}

Polynomial& Polynomial::operator=(const Polynomial& poly){
    if(this == &poly)
        return *this;

    Term* newArray = new Term[poly.capacity];

    for(int i = 0; i < poly.terms; i++)
        newArray[i] = poly.termArray[i];

    delete[] termArray;

    termArray = newArray;
    capacity = poly.capacity;
    terms = poly.terms;

    return *this;
}

void Polynomial::NewTerm(const float theCoeff, const int theExp){
    if(terms == capacity){
        capacity *= 2;
        Term* temp = new Term[capacity];

        copy(termArray, termArray + terms, temp);
        delete[] termArray;
        termArray = temp;
    }

    termArray[terms].coef = theCoeff;
    termArray[terms++].exp = theExp;
}