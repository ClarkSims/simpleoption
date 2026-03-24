#include <cmath>
#include <math.h>

double normalCNDF(double value)
{
  return 0.5 * erfc(-value * M_SQRT1_2);
}

#include <iostream>

using namespace std;

bool equal( double lhs, double rhs, double epsilon) {
  return fabs( lhs - rhs) / (fabs(lhs)+fabs(rhs)) < epsilon;
}

int number_errors = 0;

void test( double z, double cndf_z, double epsilon) {
  double Cndf_z = normalCNDF(z);
  if (!equal( Cndf_z, cndf_z, epsilon)) {
    ++number_errors;
    cout << "normaCNDF( " << z << ") = " << normalCNDF(z) << " correct answer = " << cndf_z << endl;
  }
}

int main() {
  double Epsilon = 1e-4;
  test( -2, 0.02275, Epsilon);
  test( -1, 0.15866, Epsilon);
  test( 0, .5, Epsilon);
  test( 1, 0.84134, Epsilon);
  test( 2, 0.97725, Epsilon);
  if (number_errors == 0) {
    cout << "All test passed!" << endl;
    return 0;
  } else {
    cout << number_errors << " test failed" << endl;
    return 1;
  }
}
