#include <iostream>
#include "gcd.h"
#include "moduloInverse.h"
using namespace std; 

typedef unsigned long long BigInt; 

int main(int argc, char **argv) 
{
    BigInt a, b;
    //hard coding values
    cout<<"Please enter a, b\n";
    cin>>a>>b;

    BigInt result = moduloInverse(a, b);
    cout<<"Result is "<<result<<endl;

    return 0;
}
