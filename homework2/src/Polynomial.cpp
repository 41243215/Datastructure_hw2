#include "Polynomial.h"
#include <algorithm>
#include <cmath>

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

Polynomial Polynomial::Add(Polynomial b){
    Polynomial c;
    int aPos = 0;
    int bPos = 0;
    
    while((aPos < terms) && (bPos < b.terms)){
        if(termArray[aPos].exp == b.termArray[bPos].exp){
            float t = termArray[aPos].coef + b.termArray[bPos].coef;

            if(t)
                c.NewTerm(t, termArray[aPos].exp);
            
            aPos++;
            bPos++;
        }
        else if(termArray[aPos].exp < b.termArray[bPos].exp){
            c.NewTerm(b.termArray[bPos].coef, b.termArray[bPos].exp);
            bPos++;
        }
        else{
            c.NewTerm(termArray[aPos].coef, termArray[aPos].exp);
            aPos++;
        }
    }
    for(; aPos < terms; aPos++)
        c.NewTerm(termArray[aPos].coef, termArray[aPos].exp);

    for(; bPos < b.terms; bPos++)
        c.NewTerm((b.termArray[bPos].coef), b.termArray[bPos].exp);

    return c;
}

float Polynomial::Eval(float f){
    float result = 0;
    
    for(int i = 0; i < terms; i++)
        result += termArray[i].coef * pow(f, termArray[i].exp);
    return result;
}

Polynomial Polynomial::Mult(Polynomial b){
    Polynomial c;
    for(int i = 0; i < terms; i++){
        Polynomial temp;
        
        for(int j = 0; j < b.terms; j++){
            float coefficient = termArray[i].coef * b.termArray[j].coef;
            int exponent = termArray[i].exp + b.termArray[j].exp;

            if(coefficient != 0)
                temp.NewTerm(coefficient, exponent);
        }

        c = c.Add(temp);
    }
    return c;
}

ostream& operator<<(ostream& out, const Polynomial& poly){
    if(poly.terms == 0)
        out << 0;
    
    else{
        for(int i = 0; i < poly.terms; i++){
            float coefficient = poly.termArray[i].coef;

            if(i == 0){
                if(coefficient < 0)
                    out << "-";
            }
            else{
                if(coefficient > 0)
                    out << " + ";
                else
                    out << " - ";
            }
            out << fabs(coefficient);  //取浮點數絕對值

            int exponent = poly.termArray[i].exp;
            
            if(exponent > 0){
                out << "x";
                if(exponent > 1){
                    out << "^" << exponent;
                }
            }
        }
    }
    return out;
}

istream& operator>>(istream& in, Polynomial& poly){
    int numberOfTerms;

    if(!(in >> numberOfTerms))
        return in;
    if(numberOfTerms < 0){
        in.setstate(ios::failbit);
        return in;
    }

    Polynomial temp;
    int previousExponent = 0;

    for(int i = 0; i < numberOfTerms; i++){
        float coefficient;
        int exponent;

        if(!(in >> coefficient >> exponent))
            return in;
        
        if(coefficient == 0 || exponent < 0){
            in.setstate(ios::failbit);
            return in;
        }
        if(i > 0 && exponent >= previousExponent){
            in.setstate(ios::failbit);
            return in;
        }
        previousExponent = exponent;
        temp.NewTerm(coefficient, exponent);
    }
    poly = temp;
    
    return in;
}