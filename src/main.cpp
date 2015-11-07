#include <iostream>
#include <cstdlib>
#include <stdint.h>
#include <vector>
using namespace std;

typedef unsigned long long int BigInt;

template <typename T>
T squareMultiply(T base, T exp, T modulus) {
  base %= modulus;
  T result = 1;
  while (exp > 0) {
    if (exp & 1) result = (result * base) % modulus;
    base = (base * base) % modulus;
    exp >>= 1;
  }
  return result;
}

void getPrime(vector<int>& pPrime) 
{
    //I'm skipping the primality test and factorization
    //Hard coding the values 
    pPrime.push_back(2);
    pPrime.push_back(3);
    pPrime.push_back(17);
}

BigInt generator(BigInt p)
{
    vector<int> pPrime; 
    BigInt totient = p -1;

    //Get prime factor of `p'
    getPrime(pPrime);

    for (BigInt i = 1; i < p; i++) {
       int flag = 0;
       for (BigInt j = 0; j < pPrime.size(); j++) {
           if (squareMultiply(i, (p / pPrime[j]), p) == 1) 
	       flag++;
       }
       if (flag == 0) {
           //we got our generator 
	   return i;
       }
    }
    return -1;
}

BigInt getKey(BigInt k, BigInt g, BigInt p) 
{

    return squareMultiply(g, k, p);
}


BigInt getSharedKey(BigInt key, BigInt a, BigInt p) 
{
    return squareMultiply(key, a, p);
}

void diffieHellman() 
{
    BigInt p, a, b;
    cout<<"\nPlease enter modulus\n"; 
    cin>>p; 

    //Get the Generator 
    BigInt g = generator(p);

    if (g == -1) {
        cout<<"\nGenerator not found\n";
    } 
    cout<<"\nGenerator is "<<g<<endl;

    //Get Alice's public key 
    cout<<"\nEnter Alice's secret key\n";
    cin>>a; 

    BigInt aliceKey = getKey(a, g, p);
    cout<<"Alice public key is "<<aliceKey<<endl;

    //Get Bob's public key 
    cout<<"\nEnter Bob's secret key\n";
    cin>>b; 

    BigInt bobKey = getKey(b, g, p);
    cout<<"Bob public key is "<<bobKey<<endl;


    //alice send her public key to bob
    cout<<"\nAlice sends her public key to bob\n";
    BigInt bobSharedKey = getSharedKey(aliceKey, b, p);
    cout<<"Bob computes his shared key as "<<bobSharedKey<<endl;

    //bob send his public key to alice
    cout<<"\nBob sends his public key to Alice\n";
    BigInt aliceSharedKey = getSharedKey(bobKey, a, p);
    cout<<"Alice computes her shared key as "<<aliceSharedKey<<endl;

    if (aliceSharedKey == bobSharedKey) {
        cout<<"\nDiffie Hellman key exchange successful\n\n";
    }
    else cout<<"Diffie Hellman key exchange unsuccessful\n\n";

}

int main(void)
{
    int choice;

    cout << "\n"; //introduction
    cout << "*******************************************\n";
    cout << "*             Please enter :              *\n";
    cout << "*  -----------------------------------    *\n";
    cout << "*     1  (Diffie-Hellman key-exchange)    *\n";
    cout << "*******************************************\n";
    cout << "\n";

    cin>>choice; 

    switch(choice) {
        case 1:
	    diffieHellman();
            break;
    }
    return 0;
}
