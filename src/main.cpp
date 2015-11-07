#include <iostream>
#include <cstdlib>
#include <stdint.h>
#include <vector>
#include "squareMultiply.h"
#include "moduloInverse.h"
#include "gcd.h"
using namespace std;
/* 
 * AUTHOR : Tushar Sharma <ts362.njit.edu>
*/

typedef unsigned long long int BigInt;

void getPrime(vector<int>& pPrime) 
{
    //I'm skipping the primality test and factorization
    //Hard coding the values 
    char c; 
    BigInt factor;
    cout<<"\nInserting prime factors\n";
    cout << "\nPlease enter y to insert. n to abort: ";
    cin>>c;
    while (c != 'n') {
        cout<<"\nEnter prime factor \n";
        cin>>factor;
        pPrime.push_back(factor);
        cout<<"\nPlease enter y to insert. n to abort: ";
        cin>>c;
    }
    cout<<"\n\nPrime Factors are \n";
    for (BigInt i = 0; i < pPrime.size(); i++) {
       cout<<pPrime[i]<<" ";
    }
    cout<<endl;
}

BigInt generator(BigInt p)
{
    vector<int> pPrime; 
    BigInt totient = p -1;

    //Get prime factor of `p'
    getPrime(pPrime);

    for (BigInt i = 2; i < p - 1; i++) {
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

void elgamal()
{

    BigInt p, a, b, k, unity = 1;
    cout<<"\nPlease enter modulus\n"; 
    cin>>p; 

    //Get the Generator 
    BigInt g = generator(p);
    g = 3;

    if (g == -1) {
        cout<<"\nGenerator not found\n";
    } 
    cout<<"\nGenerator is "<<g<<endl;

    cout<<"\nEnter Alice's private key\n";
    cin>>a;

    BigInt aliceKey = squareMultiply(g, a, p);
    cout<<"Alice public key is "<<aliceKey<<endl;

    //Get Bob's private
    cout<<"\nEnter Bob's private key\n";
    cin>>b; 

    BigInt bobKey = squareMultiply(g, b, p);
    cout<<"Bob public key is "<<bobKey<<endl;

    //Encryption by Alice 

    //Get Alice's secret key
    cout<<"\nEnter Alice's secret key\n";
    cin>>k; 

    BigInt aliceMask = squareMultiply(bobKey, k, p);
    cout<<"\nAlice mask is "<<aliceMask<<endl;

    if (aliceMask == unity) {
        cout<<"\nPlease choose a different Mask\n";
	exit(-1);
    }

    BigInt msg;
    cout<<"\nEnter message \n";
    cin>>msg;

    BigInt aliceCipher = squareMultiply((aliceMask * msg), unity, p);
    cout<<"\nAlice Cipher is "<<aliceCipher<<endl;

    BigInt aliceH = squareMultiply(g, k, p);
    cout<<"\nAlice computes H as "<<aliceH<<endl;

    cout<<"\nAlice sends Ciphertext : "<<aliceCipher<<" & H : "<<aliceH<<" to Bob"<<endl;

    /*************** Bob **************/
    BigInt bobQ = p - 1 - b; 
    cout<<"\nBob's Q is "<<bobQ<<endl; 

    BigInt bobR = squareMultiply(aliceH, bobQ, p);
    cout<<"\nBob's R is "<<bobR<<endl;

    BigInt bobDecrypt = squareMultiply((aliceCipher * bobR), unity, p); 
    cout<<"Bob's decrypted message is "<<bobDecrypt<<endl;

   
}
int main(void)
{
    int choice;

    cout << "\n"; //introduction
    cout << "*******************************************\n";
    cout << "*      Please enter your choice:          *\n";
    cout << "*  -----------------------------------    *\n";
    cout << "*     1  (Diffie-Hellman key-exchange)    *\n";
    cout << "*     2  (El Gamal)                       *\n";
    cout << "*******************************************\n";
    cout << "\n";

    cin>>choice; 

    switch(choice) {
        case 1:
	    diffieHellman();
            break;
        case 2:
            elgamal();
	    break;
	default:
	    cout<<"\nWrong choice. Please run program again.\n";
	    exit(-1);
    }
    return 0;
}
