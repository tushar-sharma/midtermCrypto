template <typename T>
T calPower(T a, T y, T m) 
{
    if (y == 0) 
        return 1; 

    T  p = calPower(a, y / 2 , m) % m ; 
    p = (p * p) % m; 
   
    if (y % 2 == 1) {
        p = (p * a) % m;
    }

    return p;
}


template <typename T>
T moduloInverse(T a , T b) {

    /* calling gcd from "gcd.h" */
    if (gcd(a, b) != 1) {
        return -1;  //modulo inverse not found
    }else {
        return calPower(a, b - 2, b);
    }
}
