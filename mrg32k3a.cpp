/* Implements L'Ecuyer's MRG */

#include "mrg32k3a.h"
#include <ctime>
#include <math.h>

RandomStream::RandomStream()
{
  // use default seeds
  a = 12345;
  b = 56789;
  c = 89012;
  
  d = 98765;
  e = 54321;
  f = 32109;
  
   /* a = x_1_i-2; b = x_1_i-1; c = x_1_i
     * d = x_2_i-2; e = x_2_i-1; f = x_2_i
     */
}

void RandomStream::reset()
{
  // reset stream
  a = 12345;
  b = 56789;
  c = 89012;
  
  d = 98765;
  e = 54321;
  f = 32109;
}

double RandomStream::nextVar()
{
  double u;
  unsigned long z;
  unsigned long x1i, x2i;
  static const unsigned long modulo1 = 4294967087; //pow(2,32) - 209
  static const unsigned long modulo2 = 4294944443; //pow(2,32) - 22853
  x1i = (1403580*b - 810728*a) % modulo1;
  x2i = (527612*f - 1370589*d) % modulo2;
  z = (x1i - x2i) % modulo1;
  u = (double)z / (double)modulo1;
  // update
  current_var = u;
  a = b; b = c; c = x1i;
  d = e; e = f; f = x2i;
  return u;
}
