
template <typename T>
T squareMultiply(T u, T m, T p) {
  u %= p;
  T A = 1;
  while (m > 0) {
    if (m % 2 == 1) A = (A * u) % p;
    u = (u * u) % p;
    //m = (m - b) 
    // divide m / 2
    m = m / 2;
  }
  return A;
}
