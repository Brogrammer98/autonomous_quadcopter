void astar(node *current_node,int goal,btree &opentree_f,btree &opentree_keyval,btree &closedtree,btree &master_tree)
{

    int curr_xpos=current_node->key_value%width;
    int curr_ypos=current_node->key_value/width;
    cout<<"x and y "<<curr_xpos<<"    "<<curr_ypos<<endl;
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
              node temp;
              temp.key_value=(curr_xpos+i)+width*(curr_ypos+j);
              if(curr_ypos+j>=0&&curr_xpos+i>=0&&curr_xpos+i<width&&curr_ypos+j<height)
                {
                 temp.g =current_node->g +(sqrt(pow(i,2)+pow(j,2))) ;
                 temp.h = ( abs(i) + abs(j) ); // distance from sucesso
                 temp.f=temp.g+temp.h;
                 //cout<<"enetering astar "<<current_node->key_value<<"     "<<i<<"    "<<j<<endl;
                 usleep(400000);
                if(!opentree_keyval.search_keyval_fval_comp(temp.key_value,temp.f,opentree_keyval.root)&&!closedtree.search_keyval_fval_comp(temp.key_value,temp.f,closedtree.root))
                  {
                   opentree_keyval.insert_keyval(temp.key_value,temp.f,temp.g,temp.h,opentree_keyval.root);
                   opentree_f.insert_f(temp.key_value,temp.f,temp.g,temp.h,opentree_keyval.root);
                   master_tree.insert_keyval(temp.key_value,temp.f,temp.g,temp.h,temp.route_parent_key,master_tree.root);
                  }
                }
              }
          }
       closedtree.insert_keyval(current_node->key_value,current_node->f,current_node->g,current_node->h,current_node->route_parent_key,closedtree.root);
       opentree_keyval.destroy_leaf_keyval(current_node->key_value,opentree_keyval.root);
       opentree_f.destroy_leaf_f(current_node->key_value,current_node->f,opentree_f.root);

       node *temps;
       temps=opentree_f.minnode(opentree_f.root);
       
       if(!temps)
         cout<<"node not found"<<endl;
       else
       {cout<<"next node is"<<temps->key_value<<endl;
       

       astar(temps,goal,opentree_f,opentree_keyval,closedtree,master_tree);
       return;
       }
      }
}