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
public:
  NODE( string _label, bool _status, vector<string> _in, vector<string> _out )
  {
    label = _label;
    status = _status;
    in = _in; // try std::move ?
    out = _out;
  }
  vector<NODE> chk_crculr();    // Check for circularities
  vector<NODE> chk_exist_just();// Check if the justifications exist
  vector<NODE> chk_contra();    // Check for contradictions
};

vector<NODE> nodes;
