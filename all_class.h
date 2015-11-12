#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <list>
#include <set>

using namespace std;

enum STATUS { in, out};

class NODE
{
  string label;
  vector<string> in, out;
  bool status;
  bool chk_crculr();    // Check for circularities
  vector<NODE> chk_exist_just();// Check if the justifications exist, return those that don't exist
public:
  NODE( string _label )
  {
    label = _label;
  }
  NODE( string _label, bool _status, vector<string> _in, vector<string> _out )
  {
    label = _label;
    status = _status;
    in = _in; // try std::move ?
    out = _out;
  }
  bool operator<( NODE other ) const
  {
    return label < other.label;
  }
  bool operator==( NODE other ) const
  {
    return label == other.label;
  }
};

set<NODE> nodes;
