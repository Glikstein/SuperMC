#ifndef MRG32K3A_H
#define MRG32K3A_H

class RandomStream
{
  private:
    unsigned long a,b,c,d,e,f; 
    /* a = x_1_i-2; b = x_1_i-1; c = x_1_i
     * d = x_2_i-2; e = x_2_i-1; f = x_2_i
    
    uses default seeds
    a = 12345;
    b = 56789;
    c = 89012;
  
    d = 98765;
    e = 54321;
    f = 32109; */
  
  public:
    double current_var;
    double nextVar();
    RandomStream();
    void reset();
};
#endif