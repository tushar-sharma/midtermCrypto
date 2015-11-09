#include <iostream>
#include <cstdlib>
#include <stdint.h>
#include <vector>
#include "gcd.h"
#include "squareMultiply.h"
#include "moduloInverse.h"
#include <cmath>
#include <stdio.h>
#include <cstring>
using namespace std;
/* 
 * Purpose : Implements 
 *    - Diffie-Hellman key-exchange
 *    - El Gamal
 *    - RSA
 *    - El Gamal Digital Signature
 * 
 * Last Modified : Nov 9, 15
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
           if (squareMultiply(i, (totient / pPrime[j]), p) == 1) 
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
    cout<<"\nPlease enter Alice's secret key between 1 to 226\n";
    cin>>a; 

    BigInt aliceKey = getKey(a, g, p);
    cout<<"Alice public key is "<<aliceKey<<endl;


    //Get Bob's public key 
    cout<<"\nPlease enter Bob's secret key between 1 to 226\n";
    cin>>b; 

    BigInt bobKey = getKey(b, g, p);
    cout<<"Bob public key is "<<bobKey<<endl;


    //alice send her public key to bob
    cout<<"\nAlice sends "<<aliceKey<<" to bob\n";
    BigInt bobSharedKey = getSharedKey(aliceKey, b, p);
    cout<<"Bob computes his shared key as "<<bobSharedKey<<endl;

    //bob send his public key to alice
    cout<<"\nBob sends "<<bobKey<<" to Alice\n";
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
    cout<<"\nEnter message less than p \n";
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

BigInt getE(BigInt phi) 
{
    for (BigInt i = 2; i < phi; i++) {
        if (gcd(i, phi) == 1) return i;
    }
    return (BigInt) -1;
}

bool encodePlain(string msg, BigInt r,  vector<BigInt>& num, BigInt n)
{
   
   BigInt temp = 0, count = r;
   int sSize = count -1;

   for (BigInt i = 0; i < msg.size(); i++) {
       BigInt sValue = (BigInt) (msg[i] - 65); 
       BigInt exp = pow(26, sSize);

       temp = temp + (sValue * exp);

       sSize--;
       count--; 

       if (count == 0) {
          //checking block size length 
	  //TODO uncommment 
	  /*
           if (temp > n) {
	       return false;
	   }*/
           count = r; 
	   sSize = count -1;
	   num.push_back(temp);
	   temp = 0;
       }
   }
   return true;
}
void decodePlain(vector<BigInt> dmsg) 
{
    BigInt N, L = 26, a;
    string msg = "";
    vector<string> temp;

    char c;
    cout<<"\nDecoding \n";
    for (BigInt i = 0 ; i < dmsg.size(); i++) {
        N = dmsg[i];
        while (N > L) {
            a = N % L;
	    N = (N - a) / L; 
	    c = (char) (a + 65);
	    msg = c + msg;
	    //msg = string(c) + msg;
	    //msg.push_back(c);
	 }
	 a = N;
	 c = (char) (a + 65);
	 msg = c + msg;
	 //cout<<"msg is "<<msg<<endl;
	 temp.push_back(msg);
	 msg = "";
	 //msg.push_back(c);
    }
    cout<<"\nDecoded Message is\n";
    for (int i = 0; i < temp.size(); i++) {
        cout<<temp[i];
    }
    cout<<endl;
}


void rsa() 
{
    BigInt p, q; 

    cout<<"\nPlease enter two large numbers\n";
    cin>>p>>q;

    BigInt n = p * q; 
    cout<<"\nn is "<<n<<endl; 

    //calculate phi
    BigInt phi = (p - 1) *  (q - 1);
    cout<<"\ntotient function is "<<phi<<endl; 

    BigInt e = getE(phi);
    if (e == (BigInt) -1) {
        cout<<"cannot find e coprime to "<<phi<<endl;
	exit(-1);
    }
    cout<<"\ne is "<<e<<" which is coprime to "<<phi<<endl; 

    BigInt d = moduloInverse(e, phi);
    cout<<"\nd is "<<d<<endl;

    //encryption
    BigInt  r;
    string msg;
    cout<<"Enter message\n";
    cin>>msg;
    cout<<"\nEnter block size\n";
    cin>>r; 

    vector<BigInt> num, c, dmsg;
    bool isBlock = true;
    isBlock = encodePlain(msg, r, num,  n);

    if (isBlock == false) {
        cout<<"\nBlock size not correct.\n";
	exit(-1);
    }
    for (BigInt i = 0; i < num.size(); i++) {
        cout<<(i + 1)<<"st encoded block "<<num[i]<<endl;
    }
    cout<<"\nEncoding for each block\n";
    for (BigInt i = 0; i < num.size(); i++) {
       c.push_back(squareMultiply(num[i], e, n));
    }

    for (BigInt i = 0; i < c.size(); i++) {
        cout<<(i + 1)<<"st encrypted block "<<c[i]<<endl;
    }

    cout<<"\nDecoding for each block\n";
    for (BigInt i = 0; i < c.size(); i++) {
       dmsg.push_back(squareMultiply(c[i], d, n));
    }

    for (BigInt i = 0; i < dmsg.size(); i++) {
        cout<<(i + 1)<<"st decrypted block "<<dmsg[i]<<endl;
    }
    cout<<endl;

    decodePlain(dmsg);

/* 
    BigInt c = squareMultiply(msg, e, n);
    cout<<"\nCipher is "<<c<<endl;
  
    BigInt dmsg = squareMultiply(c, d, n);
    cout<<"\nDecrypted Message is "<<dmsg<<endl;
*/
}


/*void rsa() 
{
    BigInt p, q; 

    cout<<"\nPlease enter two large numbers\n";
    cin>>p>>q;

    BigInt n = p * q; 
    cout<<"\nn is "<<n<<endl; 

    //calculate phi
    BigInt phi = (p - 1) *  (q - 1);
    cout<<"\ntotient function is "<<phi<<endl; 

    BigInt e = getE(phi);
    if (e == (BigInt) -1) {
        cout<<"cannot find e coprime to "<<phi<<endl;
	exit(-1);
    }
    cout<<"\ne is "<<e<<" which is coprime to "<<phi<<endl; 

    BigInt d = moduloInverse(e, phi);
    cout<<"\nd is "<<d<<endl;

    //encryption
    BigInt msg;
    cout<<"Enter message\n";
    cin>>msg;
 
    BigInt c = squareMultiply(msg, e, n);
    cout<<"\nCipher is "<<c<<endl;
  
    BigInt dmsg = squareMultiply(c, d, n);
    cout<<"\nDecrypted Message is "<<dmsg<<endl;

}
*/
BigInt getS(BigInt left, BigInt right)
{
    return rand()%(right-left + 1) + left;
}

BigInt getR(BigInt p)
{
    for (BigInt i = 2; i < p - 1; i++) {
        if (gcd(i, (p -1 )) == 1) return i;
    }
    return -1; 

}

void elgamalDigitalSignature() 
{
    BigInt p, g; 

    cout<<"\nStarting Elgamal Digital Signature\n";
    cout<<"\nPlease enter prime p & generator g\n";
    cin>>p>>g; 

    // randomly select number between 0 and p-1

    //BigInt r = getS((BigInt) 0, p - 1);
    BigInt r;
    cout<<"\nPlease enter value for r\n";
    cin>>r;

    cout<<"\nSender selects a random key between 0 and "<<(p - 1)<<" is "<<r<<endl; 
    BigInt K = squareMultiply(g, r, p);
    cout<<"\nKey is "<<K<<endl;

    cout<<"The sender sends to B (K, g, p) = ("<<K<<","<<g<<","<<p<<")"<<endl; 

    BigInt M;
    cout<<"\nPlease enter message\n";
    cin>>M;

    BigInt R  = getR(p);
    if (R == (BigInt) -1) {
        cout<<"\nRandom key R not found\n";
	exit(-1);
    }
    cout<<"\nRandom key R is "<<R<<endl; 


    BigInt X = squareMultiply(g, R, p);
    cout<<"\nX is "<<X<<endl;
  
    //This means that R has a modulo inverse 

    BigInt Y = ((M - (r * X ) % (p - 1)) * moduloInverse(R, (p -1) )) % (p -1);

    cout<<"\nY is "<<Y<<endl;

    cout<<"The signature of message M is ("<<X<<","<<Y<<")"<<endl;

    cout<<"\nVerification...\n";
    BigInt A =(squareMultiply(K, X, p) * squareMultiply(X, Y, p) )% p ;
    cout<<"A is "<<A<<endl;

    if (A == squareMultiply(g, M, p)) {
        cout<<"Verification successful\n";
    } else  cout<<"Verification unsuccessful\n";
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
    cout << "*     3  (RSA)                            *\n";
    cout << "*     4  (El Gamal Digital Signature )    *\n";
    cout << "*     5  (Generator)                      *\n";
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
        case 3:
	    rsa();
	    break;
        case 4:
            elgamalDigitalSignature();
	    break;
        case 5:
            BigInt p;
            cout<<"\nEnter p\n";
            cin>>p;
            generator(p);
            break;
	default:
	    cout<<"\nWrong choice. Please run program again.\n";
	    exit(-1);
    }
    return 0;
}
