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
{

}
void handle_STATE( vector<string> &in, vector<string> &out )
{

}
void handle_LIST()
{

}
void handle_HELP()
{

}
