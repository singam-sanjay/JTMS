#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

enum STATUS { in, out};

class NODE
{
  string label;
  vector<string> in, out;
  bool status;
};
