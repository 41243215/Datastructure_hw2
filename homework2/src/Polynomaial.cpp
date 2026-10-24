#include "Polynomial.h"

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