#include <vector>

template <typename T>
T fish (T a, T p)
{
    //stage Northern 
    T t , b , count, h, f;
    t = p, b = a; 
    count = (T) 0;

    std::vector<T> store;

    do {
        f  = t / b;
	h = t - f * b;
	count = count + 1; 
	t = b; 
	b =  h;
	store.push_back(f);

    } while(h > 1) ;

    //if h = 0, no inverse
    if (h == (T) 0 ) {
        return (T) 1; 
    }

    for (T i = 0; i < store.size(); i++)
        std::cout<<store[i]<<"  ";
    std::cout<<"\n";

    //stage southern
    t = 0, b = 1 ; 
    T flag = (count + 1); 
    do {
        f  = store[count - 1];
	std::cout<<"f is "<<f<<"\n";
        h = (f * b  +  t); 
	t = b;
	b = h;
	count--;
    } while (count > 0); 

    if (flag %2 == 1) return h;

    else return (p - h);

}


template <typename T>
T moduloInverse(T a , T b) {
    std::cout<<"hello"<<"\n";
    /* calling gcd from "gcd.h" */
    if (gcd(a, b) != 1) {
        return -1;  //modulo inverse not found
    }else {
        T result =  fish(a, b); 
	std::cout<<"return "<<result<<"\n";
	return result;
    }
}
