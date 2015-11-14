vector<string> NODE::chk_exist_just()
{
  vector<string> non_exstnt_just;
  auto nodes_end = nodes.end();
  for( auto just : in )
  {
    if( nodes.find( NODE(just) )==nodes_end )
    {
      non_exstnt_just.push_back(just);
    }
  }
  for( auto just : out )
  {
    if( nodes.find( NODE(just) )==nodes_end )
    {
      non_exstnt_just.push_back(just);
    }
  }
  return non_exstnt_just;
}

bool NODE::chk_crculr()
{
  list<string> DDB_labels;
  DDB_labels.insert(DDB_labels.begin(), out.begin(), out.end());
  DDB_labels.insert(DDB_labels.begin(), in.begin(), in.end());

  string just_label;
  auto nodes_end = nodes.end(), iter = nodes.begin();//Just for the initialisation part
  while( DDB_labels.size()>0 )
  {
    just_label = DDB_labels.front();
    DDB_labels.pop_front();
    if( just_label==label )
    {
      return true;
    }
    if( ( iter=nodes.find(NODE(just_label)) )!=nodes_end )
    {
      DDB_labels.insert( DDB_labels.end(), (iter->in).begin(), (iter->in).end() );
      DDB_labels.insert( DDB_labels.end(), (iter->out).begin(), (iter->out).end() );
    }
  }
  return false;
}

void NODE::eval_height()
{
  height = 0;
  for( string in_just : in )
  {
    height = max( height, (nodes.find( NODE(in_just) ))->ret_height() );
    // will always find in_just since node inserted only when supporting justifications are present
  }
  for( string out_just : out )
  {
    height = max( height, (nodes.find( NODE(out_just) ))->ret_height() );
  }
  if( height!=0 )
  {
    height++;
  }
}

void NODE::eval_status()
{
  for( string in_just : in )
  {
    if( (nodes.find( NODE(in_just) ))->ret_status() != IN )
    {  // will always find in_just since node inserted only when supporting justifications are present
      status = OUT;
      return;
    }
  }
  for( string out_just : out )
  {
    if( (nodes.find( NODE(out_just) ))->ret_status() != OUT )
    {
      status = OUT;
      return;
    }
  }
  status = IN;
}

void propogate( NODE *proponent )
{
  list<PropNODE> list_of_prop = { PropNODE(proponent) }, copy;
  vector<PropNODE> sorted_prop;
  string bottom_label;
  PropNODE bottom = PropNODE(NULL);

  for( const NODE &node : nodes )
  {
    NODE* ptr = (NODE*)(void*)&node;//Cheated here
    ptr->propogate_initialise();
  }

  //Get dependency tree
  while( list_of_prop.size()>0 )
  {
    bottom = list_of_prop.front();
    copy.push_back(bottom);
    bottom_label = (bottom.it)->ret_label();

    for( const NODE &node : nodes )
    {
      if( node.isIn_in( bottom_label ) || node.isIn_out( bottom_label ) )
      {
        NODE* ptr = (NODE*)(void*)&node;//Cheated here
        if( (ptr->is_added_to_heap() ) == false )
        {
          list_of_prop.push_back( PropNODE(ptr) );
          ptr->added_to_heap();
        }
      }
    }

    list_of_prop.pop_front();
  }

  move( begin(copy), end(copy), back_inserter(sorted_prop) );
  sort( begin(sorted_prop), end(sorted_prop) );
  // sort take cares of dependency

  for( PropNODE &node : sorted_prop )
  {
    (node.it)->eval_status();
    (node.it)->eval_height();
  }
}

void handle_IS()
{//IS label
  string label;
  set<NODE>::iterator search_iter;

  cin >> label;
  search_iter = nodes.find( NODE(label) );
  if( search_iter==nodes.end() )
  {
    cout << "\"" << label << "\" not found.\n";
  }
  else
  {
    cout << ( search_iter->ret_status()==IN ? "IN\n" : "OUT\n");
  }
  cin.ignore( std::numeric_limits<std::streamsize>::max(), '\n');
}

bool handle_STATE( string &label, vector<string> &in, vector<string> &out, enum STATUS &status )
{// STATE [NOT] label IN OUT END
  string str;
  bool must_swap;

  cin >> str;
  if( str=="NOT" )
  {
    must_swap = true;
    cin >> label; // need to get label
  }
  else
  {
    must_swap = false;
    label = str;
  }
  if( str=="IN" )
  {
    cin >> str;
    while( str!="OUT" )
    {
      in.push_back( string(str) );
      cin >> str;
    }
    while( str!="END" )
    {
      out.push_back( string(str) );
      cin >> str;
    }
    if( in.size()>0 && out.size()>0 )
    {
      vector<string> intrsc;
      set_intersection( in.begin(), in.end(), out.begin(), out.end(), back_inserter(intrsc) );
      if( intrsc.size()>0 )
      {
        cout << "Commons nodes in IN and OUT of " << label << ".\n";
        for( string common_just : intrsc )
        {
          cout << common_just << ' ';
        } cout << '\n';
        return true;
      }
    }
    if( must_swap )
    {//Since when notted, the in_justs become out_just and vice versa
      swap( in, out );
      cout << "Swapped in_justs and out_justs"
    }
    cout << "Non intersecting IN and OUT.\n";
  }
  else
  {
    cout << "Wrong format.\n";
    return true;
  }

  return false;
}


bool try_insert( string &label, vector<string> &in, vector<string> &out, enum STATUS status )
{
  bool problem = false;
  //Check if IN justs are present
  for( string str : in )
  {
    if( nodes.find( NODE(str) )==nodes.end() )
    {
      cout << "IN::" << label << " not found.\n";
      problem = true;
    }
  }
  //Check if OUT justs are present
  for( string str : out )
  {
    if( nodes.find( NODE(str) )==nodes.end() )
    {
      cout << "OUT::" << label << " not found.\n";
      problem =  true;
    }
  }
  if( problem )
  {
    return true;
  }

  //Need to insert over here
  return false;
}

void handle_LIST()
{

}
void handle_HELP()
{

}
