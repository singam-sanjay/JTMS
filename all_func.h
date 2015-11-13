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

int NODE::find_height()
{
  int height = 0;
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
  return height;
}

void NODE::correct_and_propogate_height()
{//Even this needs to follow the "tree"
  int old_height = height;
  height = find_height();
  if( height!=old_height )
  {
    for( const NODE &node : nodes )
    {
      if( node.isIn_in( label ) || node.isIn_out( label ) )
      {
        NODE* ptr = (NODE*)(void*)&node;//Cheated here
        ptr->correct_and_propogate_height();
      }
    }
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
  vector<PropNODE> heap_of_prop = { PropNODE(proponent) };

  for( const NODE &node : nodes )
  {
    NODE* ptr = (NODE*)(void*)&node;//Cheated here
    ptr->propogate_initialise();
  }
  while( heap_of_prop.size()>0 )
  {
    pop_heap( heap_of_prop.begin(), heap_of_prop.end() );
    PropNODE bottom = heap_of_prop.back();
    (bottom.it)->correct_and_propogate_height();
    string bottom_label = (bottom.it)->ret_label();

    enum STATUS oldStatus = (bottom.it)->ret_status();
    (bottom.it)->eval_status();
    if( )

    heap_of_prop.pop_back();

    for( const NODE &node : nodes )
    {
      if( node.isIn_in( bottom_label ) || node.isIn_out( bottom_label ) )
      {
        NODE* ptr = (NODE*)(void*)&node;//Cheated here
        if( (ptr->is_added_to_heap() ) == false )
        {
          heap_of_prop.push_back( PropNODE(ptr) );
          ptr->added_to_heap();
        }
      }
    }

    max_heap( heap_of_prop.begin(), heap_of_prop.end() );
  }

/*
  set<PropNODE> TreeofProp = { PropNODE(proponent) };//MinHeap of PropNODEs based on their height and label
  // Without the label, the data structure would resemble a subset of the dependency tree. read bool PropNODE::operator<(PropNODE)

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
      //Need to check if height changed
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
*/
}
