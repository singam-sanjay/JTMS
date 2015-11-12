vector<NODE> NODE::chk_exist_just()
{
  vector<NODE> non_exstnt_just;
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
