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
  vector<string> in, out; // Could change to set<string> to speed up search, although with costlier insertion, insertion happens only once
  int height;
  bool status;
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
  bool isIn_in( string in_just )
  {
    return (find( in.begin(), in.end(), in_just )==in.end());
  }
  bool isIn_out( string out_just )
  {
    return (find( out.begin(), out.end(), out_just )==out.end());
  }
  bool chk_crculr();    // Check for circularities
  vector<string> chk_exist_just();// Check if the justifications exist, return those that don't exist
  void find_height();
  friend class PropNODE;
};

set<NODE> nodes;

class PropNODE
{
  set<NODE>::iterator it;
public:
  bool operator<( PropNODE other )
  {
    return it->height < other.it->height || (it->height==other.it->height && it->label < other.it->label );
  }
};
