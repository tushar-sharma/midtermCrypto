template <typename T>
T gcd(T a,T b)
{ 
   //a can be written as, a = bt + r,
   //So, gcd(a, b) = gcd(b, r).

    if (b == 0)
        return a;

    return gcd(b, (a%b));
}


