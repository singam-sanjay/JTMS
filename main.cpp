#include "all_class.h"
#include "all_func.h"

int main()
{
  string inp;

  do
  {
    cin.ignore( std::numeric_limits<std::streamsize>::max(), '\n');
    cout << "> ";
    cin >> inp;
    if( inp=="QUIT" )
    {
      cout << "GB !!\n";
      break;
    }
    else if( inp=="HELP" )
    {
      handle_HELP();
    }
    else if( inp=="IS" )
    {
      handle_IS();
    }
    else if( inp=="LIST" )
    {
      handle_LIST();
    }
    else if( inp=="STATE" )
    {
      vector<string> in, out;
      string label;
      enum STATUS status;
      if( handle_STATE( label, in, out, status ) )
      {
        continue;
      }
      if( try_insert( label, in, out, status ) )
      {
        continue;
      }
    }
  }while( true );
  return 0;
}
