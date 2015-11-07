#include <iostream>
#include "squareMultiply.h"
using namespace std; 

typedef unsigned long long BigInt; 

int main(int argc, char **argv) 
{
    BigInt a, b, p;
    //hard coding values
    cout<<"Please enter a, b, p\n";
    cin>>a>>b>>p;

    BigInt result = squareMultiply(a, b, p);
    cout<<"Result is "<<result<<endl;

    return 0;
}
