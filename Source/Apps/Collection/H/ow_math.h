/* Mathematik für OW-Basic.
*/

#include <stdrom.h>
#include <math.h>

extern double wm;

double Sin(double value);
double Cos(double value);
double Tan(double value);
double Log(double value);
double Log2(double value);
double Ln(double value);
double Sqrt(double value);
double ASin(double value);
double ACos(double value);
double ATan(double value);
double Sinh(double value);
double Cosh(double value);
double Tanh(double value);
double ASinh(double value);
double ACosh(double value);
double ATanh(double value);
double ALog(double value);
double ALog2(double value);
double ALn(double value);
double Sqr(double value);
double Pow(double value, double val);
double Nrt(double value, double val);
double ATan2(double value,double value2);


double XCart(double r,double fi);
double YCart(double r,double fi);
double Radius(double x,double y);
double Phi(double x,double y);

/* Conversion from current mode (rad/degree/grad) to specified mode */

double Deg(double value);
double Rad(double value);
double Grad(double value);