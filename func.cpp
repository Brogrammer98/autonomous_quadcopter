struct node
{
  int key_value;
  node *left=NULL;
  node *right=NULL;
  node *route_parent=NULL;
  node *tree_parent=NULL;
  int f=0,g=0,h=0;

};

class btree
{
    public:
        btree();
        ~btree();

        void insert(node *leaf, node *rot);
        bool search_bool(int key_val,node searchnode);
        node *search_node(int key_val,node rot);
        node *minnode(node *rot);
        node *restructure_tree(int key,node *rot);
        void destroy_leaf();

        node *root;
};

node *btree::minnode(node *rot)
{
	if(rot->left==NULL)
		return rot;
	else
		return minnode(rot->left);
}

btree::btree()
{
  root=NULL;
}


void btree::insert(node *leaf,node *rot)
{
  if(leaf->key_value<rot->key_value)
  {
    if(rot->left!=NULL)
     insert(leaf, rot->left);
    else
    {
      rot->left=leaf;
      leaf->tree_parent=rot;
    }  
  }
  
  if(leaf->key_value>rot->key_value)
  {
    if(rot->right!=NULL)
      insert(leaf, rot->right);
    else
    {
      rot->right=leaf;
      leaf->tree_parent=rot;
    }
  }

  if(leaf->key_value==rot->key_value)
  {
  	rot->f=leaf->f;
  	rot->g=leaf->g;
  	rot->h=leaf->h;
  	
  }

}




bool btree::search_bool(node leaf,node *rot)
{
	if(leaf->key_value==rot->key_value)
	{
		return true;
	}
	else
	{
		if(rot->right==rot->left==NULL)
		{
			return false;
		}
     	else 
     	{
     		if(leaf->key_value>rot->key_value)
     		{
     			if(rot->right!=NULL)
     			{
     				return search_bool(leaf,rot->right);
     			}
     		}
     		else 
     		{
     			if(rot->left!=NULL)
     			{
     				return search_bool(leaf,rot->left);
     			}
     		}
     	}     	
	}
}

node *btree::search_return_node(node *leaf,node *rot)
{
	if(leaf->key_value==rot->key_value)
	{
		return rot;
	}
	else
	{
		if(rot->right==rot->left==NULL)
		{
			return NULL;
		}
     	else 
     	{
     		if(leaf->key_value>rot->key_value)
     		{
     			if(rot->right!=NULL)
     			{
     				return search_node(leaf,rot->right);
     			}
     		}
     		else 
     		{
     			if(rot->left!=NULL)
     			{
     				return search_node(leaf,rot->left);
     			}
     		}
     	}     	
	}
}

/*

************************************************************************************

the following some lines are some really crazy ass lines of function please look into it 
i am assuming that the tree is not self balancing 

***********************************************************************************

*/






void btree::destroy_leaf(node *leaf , node *rot)   
{

	if(leaf->key_value==rot->key_value)
		{	if(rot->parent)
			{
				root=rot->right;
				delete rot ;
				return;
			}
			else
			{
				if(rot->parent->left==rot)
					{
						rot->parent->left=rot->right;
						node *temps=minnode(rot->right);
						temps->left=rot->left;
						delete rot;
						delete leaf;
						return;

					}

			}
		}
	else if(leaf->key_value<rot->key_value)
	{
		destroy_leaf(leaf,rot->left);
		return;
	}	
}

node *current_node;
current_node->key_value=curr_xpos+curr_ypos*width;

btree opentree,closedtree;

int openlist_memcount=0,closedlist_memcount=0;

for(int i=0;i<height*width;i++)
{
	if(map_data[i]!=0)
	{
		node temp;
		temp.key_value=i;
		closedtree.insert(temp);

	}
	
}

float astar_w1=0,astar_w2=0;

int astar(int x,int y,int goal)
{
 
	node *temp;	
	temp->key_value=curr_xpos+curr_ypos*width;
	
	closedtree.insert(current_node);

    
	int t=0;

	for(int i=-1;i<=1;i++)
	{
		for(int j=-1;j<=1;j++)
		{
			if(curr_ypos+j>=0&&curr_xpos+i>=0&&curr_xpos+i<width&&curr_ypos+j<height)
			{ 
				node *tempo;

				tempo.keyval=curr_xpos+curr_ypos*width;curr_xpos+curr_ypos*width;
				tempo.g =temp.g + astar_w1*(sqrt(pow(i,2)+pow(j,2))) ; // add distance here 
				tempo.h = astar_w2*( abs(curr_xpos+i-x,2) + abs(curr_ypos+j-y,2) ); // distance from sucesso 
				tempo.f=tempo.g+tempo.h;
				node *searchresopen = opentree.search_node(tempo.keyval);
				node *searchresclosed = closedtree.search_node(tempo.keyval);

					

				if(opentree.search_bool(tempo)&& (searchresopen->f<tempo->f))
					goto ifexit; // add jump here
				if(closedtree.search_bool(tempo)&&(searchresopen->f<tempo->f))
					goto ifexit; // add jump here
				tempo->parent=current_node;			
				opentree.insert(tempo);

				ifexit:;
			}
		}
	} 
	
	current_node=opentree.minnode();
	if(opentree.min()==NULL)
		break;
	else
	astar(   current_node->keyval%width, current_node->keyval-((current->keyval)%(width,goal))  ); 				   	
}

