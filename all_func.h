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

bool is_KEYWORD( string label )
{/*  KEYWORDS : STATE LIST IS ALL HELP IN OUT END */
  static set<string> KEYWORDS = { "STATE", "LIST", "IS", "ALL", "HELP", "IN", "OUT", "END" };
  return KEYWORDS.find( label )!=KEYWORDS.end();
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
    status = OUT; //Need this in case in_justs == out_justs == {}
    must_swap = true;
    cin >> label; // need to get label
  }
  else
  {
    status = IN;
    must_swap = false;
    label = str;
  }
  if( is_KEYWORD(label) )
  {
    cout << "The given label is a KEYWORD.\n";
    return true;
  }
  cin >> str;
  if( str=="IN" )
  {
    cin >> str;
    while( str!="OUT" )
    {
      in.push_back( string(str) );
      cin >> str;
    }
    cin >> str;
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
      cout << "Swapped in_justs and out_justs";
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


bool try_insert( string &label, vector<string> &in, vector<string> &out, enum STATUS &status )
{
  //bool problem = false;
  NODE node( label, status, in, out );
  vector<string> justs;

  justs = node.chk_exist_just();
  if( justs.size()>0 )
  {
    cout << "Missing justifications\n";
    for( string str : justs )
    {
      cout << str << ' ';
    }
    cout << '\n';
    return true;
  }

  if( nodes.find( node )!=nodes.end() )
  {
    if( node.chk_crculr()==true )
    {
      cout << "Creating circularities.\n";//Later add the created dependency
      return true;
    }
    cout << "Replacing old statement.\n";
    nodes.erase( node );
    nodes.insert( node );
    NODE* ptr = (NODE*)(void*)(&(*nodes.find( node )));
    propogate( ptr );
  }
  else
  {
    node.eval_height();
    if( in.size()!=0 || out.size()!=0 )
    {//If not a premise
      node.eval_status();
    }
    cout << "Inserting new statement.\n";
    nodes.insert( node );
  }
  return false;
}

void handle_LIST()
{//LIST label/ALL
  string label;

  cin >> label;
  if( label=="ALL" )
  {
    for( const NODE &node : nodes )
    {
      cout << node.ret_label() << ":(" << ( node.ret_status()==IN ? "IN" : "OUT" )<< ")\n";
      cout << "in_justs:";for( string in_just : node.ret_in() )cout << in_just << ' ';cout << '\n';
      cout << "out_justs:";for( string out_just : node.ret_out() )cout << out_just << ' ';cout << '\n';
    }
  }
  else
  {
    set<NODE>::iterator node_iter = nodes.find( NODE(label) );
    if( node_iter!=nodes.end() )
    {
      const NODE &node = *node_iter;
      cout << node.ret_label() << ":(" << ( node.ret_status()==IN ? "IN" : "OUT" )<< ")\n";
      cout << "in_justs:";for( string in_just : node.ret_in() )cout << in_just << ' ';cout << '\n';
      cout << "out_justs:";for( string out_just : node.ret_out() )cout << out_just << ' ';cout << '\n';
    }
    else
    {
      cout << label << " not found.\n";
    }
  }
}

void handle_HELP()
{
  cout << "HELP:\n"
          "LIST label/ALL\n"
          "STATE [NOT] label IN [just] OUT [just] END\n"
          "IS label\n"
          "QUIT\n";
}
