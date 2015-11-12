#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

using namespace std;

enum STATUS { in, out};

class NODE
{
  string label;
  vector<string> in, out;
  bool status;
};
