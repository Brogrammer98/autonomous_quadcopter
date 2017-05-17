struct node
{
  int key_value;
  node *left;
  node *right;
  node *parent;
  int f=0,g=0,h=0;

};

class btree
{
    public:
        btree();
        ~btree();

        void insert(node key);
        node *search(node key_value);
        void destroy_tree();
        void parent_reorient(node curr, node parent );

    private:
        void destroy_tree(node *leaf);
        void insert(int key, node *leaf);
        node *search(int key, node *leaf);
        node *root;
};

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

void btree:parent_reorient(node curr, node parent)
{
	curr.parent=$parent;
}

node *btree::search(int key, node *leaf)
{
  if(leaf!=NULL)
  {
    if(key==leaf->key_value)
      return leaf;
    if(key<leaf->key_value)
      return search(key, leaf->left);
    else
      return search(key, leaf->right);
  }
  else return NULL;
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

node *btree::search(int key)
{
  return search(key, root);
}

void btree::destroy_tree()
{
  destroy_tree(root);
}
struct listnode
{
	listnode *prev,*next;
	int xval,yval,fval;
}; 

void pop(int x,int y,listnode node)
{
	listnode* temp;
	temp=node 	

  while(temp->next !=nullptr)
  {
  	if(temp->xval==x&&temp->yval==y)
  	{
  		listnode* tempo;
  		tempo=temp;
  		tempo=tempo->prev;
  		tempo->next=temp->next;
  		delete temp;
  	}
  }
}

node current_node;

btree bintree,opentree,closedtree;

int closedlist_memcount=0,closedlist_memcount=0;

for(int i=0;i<height*width;i++)
{
	if(map_data[i]!=0)
	{
		node temp;
		temp.key_value=i;
		closedtree.insert(temp);

	}
	
}


	
	

int astar(int x,int y,int goal)
{
 
node temp;	
	temp.key_value=curr_xpos+curr_ypos*width;
	
	closedtree.insert(current);

    node *tempo;
	int t=0;

	for(int i=-1;i<=1;i++)
	{
		for(int j=-1;j<=1;j++)
		{
			if(curr_ypos+j>=0&&curr_xpos+i>=0&&curr_xpos+i<width&&curr_ypos+j<height)
			{ 

				tempo.keyval=curr_xpos+curr_ypos*width;curr_xpos+curr_ypos*width
				tempo.g =temp.g + // add distance here 
				tempo.h = // distance from sucessor 
				tempo.f=tempo.g+tempo.h;
				node searchresopen = opentree.search_res(tempo.keyval);
				node searchresclosed = closedtree.search_res(tempo.keyval)
				if(opentree.search(tempo)&&searchresopen.f<tempo.f)
					continue;
				if(closedtree.search(tempo)&&searresclosed.f<tempo.f)
					continue;		
				opentree.insert(tempo);

			}
		}
	} 
	node next_node;
	next_node=opentree.min();
	if(opentree.min()==nullptr)
		break
	else
	astar(next_node.keyval%width, next_node_keyval-next_node.keyval%width,goal); 				   	
}

