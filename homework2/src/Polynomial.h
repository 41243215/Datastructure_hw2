#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H
#include <iostream>

using namespace std;

class Polynomial;
class Term{
    friend Polynomial;

private:
    float coef;
    int exp;
};

class Polynomial{
public:
    Polynomial();
    Polynomial(const Polynomial& poly);
    ~Polynomial();

    Polynomial& operator=(const Polynomial& poly);

    Polynomial Add(Polynomial poly);
    Polynomial Mult(Polynomial poly);
    float Eval(float f);

    friend ostream& operator<<(ostream& out, const Polynomial& poly);
    friend istream& operator>>(istream& in, Polynomial& poly);

private:
    Term* termArray;
    int capacity;
    int terms;

    void NewTerm(const float theCoeff, const int theExp);
};

#endif