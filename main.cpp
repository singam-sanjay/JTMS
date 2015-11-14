#include "all_class.h"
#include "all_func.h"

int main()
{
  string inp;

  do
  {
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
      handle_STATE( in, out );
    }
  }while( true );
  return 0;
}
