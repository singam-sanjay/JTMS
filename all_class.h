#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <list>
#include <set>

using namespace std;

enum STATUS { IN, OUT};

class NODE
{
  string label;
  vector<string> in, out; // Could change to set<string> to speed up search, although with costlier insertion, insertion happens only once
  int height;
  enum STATUS status;
public:
  NODE( string _label )
  {
    label = _label;
  }
  NODE( string _label, enum STATUS _status, vector<string> _in, vector<string> _out )
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
  bool isIn_in( string in_just ) const
  {
    return (find( in.begin(), in.end(), in_just )==in.end());
  }
  bool isIn_out( string out_just ) const
  {
    return (find( out.begin(), out.end(), out_just )==out.end());
  }
  string ret_label() const;
  int ret_height() const;
  void find_height();    // Finds the height of the node based on its justifications
  enum STATUS ret_status() const;
  void eval_status();
  bool chk_crculr();    // Check for circularities
  vector<string> chk_exist_just();// Check if the justifications exist, return those that don't exist
  friend class PropNODE;
};

set<NODE> nodes;

class PropNODE
{
  NODE *it;
public:
  bool operator<( PropNODE other ) const
  {
    return it->height < other.it->height || (it->height==other.it->height && it->label < other.it->label );
    /* Could have sorted just according to height, but those nodes with the same height will be treated equal
       and new nodes with the same height will not be inserted.*/
  }
  PropNODE( NODE* it_ )
  {
    it = it_;
  }
  friend void propogate( NODE *proponent );
};

void propogate( NODE *proponent );
