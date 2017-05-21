struct node
{
  int key_value;
  node *left=nullptr;
  node *right=nullptr;
  int f=0,g=0,h=0;

};

class btree
{
    public:
        btree();
        ~btree();

        void insert(node key);
        bool searchbool(int key_val,node searchnode);
        node *search_return_node(int key_val,node root);
        void destroy_tree();
        void parent_reorient(node curr, node parent );
        node *minnode(btree tree);
    private:
        void destroy_tree(node *leaf);
        void insert(int key, node *leaf);
        node *search(int key, node *leaf);
        node *root;
};

node *btree::minnode(node *tree)
{
	if(tree->left==nullptr)
		return tree;
	else
	{
		return minnode(tree->left);
	
	}
}

btree::btree()
{
  root=NULL;
}

void btree::destroy_tree(node *leaf)
{
  if(leaf!=NULL)
  {
    destroy_tree(leaf->left);
    destroy_tree(leaf->right);
    delete leaf;
  }
}


void btree::insert(node *leaf)
{
  if(key< leaf->key_value)
  {
    if(leaf->left!=NULL)
     insert(key, leaf->left);
    else
    {
      leaf->left=new node;
      leaf->left->key_value=key;
      leaf->left->left=NULL;    //Sets the left child of the child node to null
      leaf->left->right=NULL;   //Sets the right child of the child node to null
    }  
  }
  else if(key>=leaf->key_value)
  {
    if(leaf->right!=NULL)
      insert(key, leaf->right);
    else
    {
      leaf->right=new node;
      leaf->right->key_value=key;
      leaf->right->left=NULL;  //Sets the left child of the child node to null
      leaf->right->right=NULL; //Sets the right child of the child node to null
    }
  }
}


void btree::insert(int key)
{
  if(root!=NULL)
    insert(key, root);
  else
  {
    root=new node;
    root->key_value=key;
    root->left=NULL;
    root->right=NULL;
  }
}

bool btree::searchbool(int key_val,node *searchnode)
{
	if(key_val==searchnode.key_value)
	{
		return true;
	}
	else
	{
		if(searchnode.right=searchnode.left==nullptr)
		{
			return false;
		}
     	else 
     	{
     		if(key_value>searchnode.key_value)
     		{
     			if(searchnode.right!=nullptr)
     			{
     				node* temp =searchnode.right;
     				return searchbool(key_val,temp);
     			}
     		}
     		else 
     		{
     			if(searchnode.left!=nullptr)
     			{
     				node* tmep= searchnode.left;
     				return searchbool(key_val,temp);
     			}
     		}

     	}     	
	}
}

node *btree::search_return_node(int key_val,node root)
{
	if(key_val==root.key_value)
	{
		return root;
	}
	else
	{
		if(root.right=root.left==nullptr)
		{
			return root;
		}
     	else 
     	{
     		if(key_value>root.key_value)
     		{
     			if(root.right!=nullptr)
     			{
     				node* temp =root.right;
     				return search_return_node(key_val,temp);
     			}
     		}
     		else 
     		{
     			if(root.left!=nullptr)
     			{
     				node* tmep= root.left;
     				return search_return_node(key_val,temp);
     			}
     		}

     	}     	
	}
}


void btree::destroy_tree()
{
  destroy_tree(root);
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
 
	node temp;	
	temp.key_value=curr_xpos+curr_ypos*width;
	
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
				node searchresopen = opentree.search_res(tempo.keyval);
				node searchresclosed = closedtree.search_res(tempo.keyval);

					

				if(opentree.search(tempo)&&searchresopen.f<tempo.f)
					goto ifexit; // add jump here
				if(closedtree.search(tempo)&&searresclosed.f<tempo.f)
					goto ifexit; // add jump here
				tempo.parent=&current_node;			
				opentree.insert(tempo);

				ifexit:;
			}
		}
	} 
	
	current_node=opentree.minnode();
	if(opentree.min()==nullptr)
		break
	else
	astar(current_node.keyval%width, current_node_keyval-current.keyval%width,goal); 				   	
}

