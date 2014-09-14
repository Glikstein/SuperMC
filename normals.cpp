#include <math.h>
#include "normals.h"
#include "mrg32k3a.h"

double invertNormal(double u)
{
  
static double a[4];
static double b[4];
static double c[9];

c[0]= 0.3374754822726147;
a[0] = 2.50662823884;
c[1] = 0.9761690190917186;
a[1] = -18.61500062529;
c[2] = 0.1607979714918209;
a[2] = 41.39119773534;
c[3] = 0.0276438810333863;
a[3] = -25.44106049637;
c[4] = 0.0038405729373609;
b[0] = -8.47351093090;
c[5] = 0.0003951896511919;
b[1] = 23.08336743743;
c[6] = 0.0000321767881768;
b[2]= -21.06224101826;
c[7] = 0.0000002888167364;
b[3] = 3.13082909833;
c[8] = 0.0000003960315187;


  double sumN = 0;	// numerator sum
  double sumD = 0;	// denominator sum
  if(u<1 && u >= 0.92)
  {
    for(int i = 0; i<=8; i++)
      sumN += c[i] * pow( log(-log(1-u)) , i );
    
    return sumN;
  }
  if(u >= 0.5 && u < 0.92)
  {
    for(int i = 0; i<= 3; i++)
    {
      sumN += a[i] * pow( (u - 0.5), (2*i + 1) );
      sumD += b[i] * pow( (u - 0.5), (2*i + 2) );
    }
    sumD += 1;
    return sumN / sumD;
  }
  if(u > 0 && u < 0.5)
    return -invertNormal(1-u);
  else
    return sumN/sumD; // 0/0 throws NaN;
}

    