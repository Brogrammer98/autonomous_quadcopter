void astar(node *current_node,int goal)
{
  if(current_node->key_value==goal)
  {
    cout<<"goal has been found ";
    return;
  }
  else
  {
    for(int i=-1;i<=1;i++)
    {
      for(int j=-1;j<=1;j++)
      {
        node *temp;
        temp=new node;
        temp->key_value=(current_node->key_value%width+i)+width*(current_node->key_value/width +j);
        if(curr_ypos+j>=0&&curr_xpos+i>=0&&curr_xpos+i<width&&curr_ypos+j<height)
        {
          temp.key_value=curr_xpos+curr_ypos*width;
          temp.g =current_node->g + astar_w1*(sqrt(pow(i,2)+pow(j,2))) ;
          temp.h = astar_w2*( abs(i) + abs(j) ); // distance from sucesso
          temp.f=tempo.g+tempo.h;
          
          if(opentree_keyval.search_keyval_fval_comp(temp->key_value,temp->f,opentree_keyval.root))
            goto kudo;
          if(closedtree.search_keyval_fval_comp(temp->key_value,temp->f,closedtree.root))
            goto kudo;
          
          opentree_keyval.insert_keyval(temp->key_value,temp->f,temp->g,temp->h,opentree_keyval.root);
          master_tree.insert_keyval(temp->key_value,temp->f,temp->g,temp->h,temp->route_parent_key,master_tree.root)
        }
      }
    }
   closedtree.insert_keyval(current_node->key_value,current_node->f,current_node->g,current_node->h,current_node->route_parent_key,closedtree.root);
   opentree_keyval.destroy_leaf_keyval(current_node->key_value,opentree_keyval.root);
   opentree_f.destroy_leaf_fval(current_node->key_value,current_node->f,opentree_f.root);
   
   node *temps;
   temps=opentree_f.minnode(opentree_f.root);
   astar(temps,goal);
   return;
  }
}