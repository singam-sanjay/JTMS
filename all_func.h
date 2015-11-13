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

int NODE::ret_height() const
{
  return height;
}

void NODE::find_height()
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
}

enum STATUS NODE::ret_status() const
{
  return status;
}

string NODE::ret_label() const
{
  return label;
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
  set<PropNODE> TreeofProp = { PropNODE(proponent) };//MinHeap of PropNODEs based on their height and label
  /* Without the label, the data structure would resemble a subset of the dependency tree. read bool PropNODE::operator<(PropNODE)*/

  set<PropNODE>::iterator bottom = TreeofProp.begin();
  TreeofProp.erase( bottom );
  for( const NODE &node : nodes )
  {
    if( node.isIn_in( bottom->it->ret_label() ) || node.isIn_out( bottom->it->ret_label() ) )
    {
      NODE* ptr = (NODE*)(void*)&node;//Cheated here
      ptr->find_height();
      TreeofProp.erase( PropNODE( ptr ) );//erase if present
      TreeofProp.insert( PropNODE( ptr ) );//insert to bring back the "min heap" condition
    }
  }
  while( TreeofProp.size()>0 )
  {
    set<PropNODE>::iterator bottom = TreeofProp.begin();
    TreeofProp.erase( bottom );
    enum STATUS oldStatus = (bottom->it)->ret_status();
    bottom->it->eval_status();
    if( oldStatus==(bottom->it)->ret_status() )
    {//Then nothing to propogate
      continue;
    }
    for( const NODE &node : nodes )
    {
      if( node.isIn_in( bottom->it->ret_label() ) || node.isIn_out( bottom->it->ret_label() ) )
      {
        NODE* ptr = (NODE*)(void*)&node;//Cheated here
        ptr->find_height();
        TreeofProp.erase( PropNODE( ptr ) );//erase if present
        TreeofProp.insert( PropNODE( ptr ) );//insert to bring back the "min heap" condition
      }
    }
  }

}
