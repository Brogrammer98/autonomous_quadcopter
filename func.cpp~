#include <cstddef>
#include <functional>
#include <queue>
#include <vector>
#include <list>
#include <deque>
#include <console_bridge/console.h>
#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <stdlib.h>



using namespace std;

const float pi = 3.14159265;
const float RAD_TO_DEG = 180.0/pi;
const float DEG_TO_RAD = pi/180.0;

int    width      = 500;
int    height     = 500;


float  map_data[250000];

int w1 = 10;
int w2 = 0;
int w3 =35;

struct node
{
  int key_value;
  node *left=NULL;
  node *right=NULL;
  int route_parent_key;
  node *tree_parent=NULL;
  int f=0,g=0,h=0;

};

class btree
{
    public:
       		btree();
       		void insert_keyval(int key,int fval,int gval,int hval,node *rot);
       		void insert_keyval(int key,int fval,int gval,int hval,int rp,node *rot);

        	void insert_f(int key,int fval,int gval,int hval,node *rot);
        	void destroy_leaf_f(int key,int fval , node *rot);

        	node *minnode(node *rot);

        	bool search_bool(int key,node *rot);
        	bool search_keyval_fval_comp(int key,int fval,node *rot);
        	node *search_node(int key,node *rot);
        	void destroy_leaf_keyval(int key , node *rot);

        	node *root;
};

btree::btree()
{
  root=(node*) malloc(sizeof(node));
  root=NULL;
}

node *btree::minnode(node *rot)
{
	if(rot->left==NULL)
		return rot;
	else
		return minnode(rot->left);
}
void btree::insert_keyval(int key,int fval,int gval,int hval,int rp,node *rot)
{

  if(rot==NULL)
	{
        rot= new node;
    	rot->key_value=key;
    	rot->f=fval;
    	rot->g=gval;
    	rot->h=hval;
    	rot->route_parent_key=rp;
    	return;
  	}
 else
 	{

		if(key<rot->key_value)
  			{
  				if(rot->left!=NULL)
    				{
     					insert_keyval(key,fval,gval,hval, rp,rot->left);
     					return;
     				}
    			else
    				{

     					rot->left=new node;
      					rot->left->key_value=key;
      					rot->left->f=fval;
      					rot->left->g=gval;
      					rot->left->h=hval;
      					rot->left->route_parent_key=rp;
      					return;
  	  				}
  			}

  		if(key>rot->key_value)
 			{
    			if(rot->right!=NULL)
    				{
      					insert_keyval(key,fval,gval,hval, rp, rot->right);
      					return;
   		 			}
    			else
    				{
    					rot->right=new node;
      					rot->right->key_value=key;
      					rot->right->f=fval;
      					rot->right->g=gval;
      					rot->right->h=hval;
      					rot->right->route_parent_key=rp;
      					return;
    				}
  			}

  		if(key==rot->key_value)
  			{
 				rot->f=fval;
			  	rot->g=gval;
  				rot->h=hval;
  				rot->route_parent_key=rp;
    			return;
  			}
	}
}

void btree::insert_keyval(int key,int fval,int gval,int hval,node *rot)
{

  if(rot==NULL)
	{
        rot= new node;
    	rot->key_value=key;
    	rot->f=fval;
    	rot->g=gval;
    	rot->h=hval;
    	return;
  	}
 else
 	{

		if(key<rot->key_value)
  			{
  				if(rot->left!=NULL)
    				{
     					insert_keyval(key,fval,gval,hval, rot->left);
     					return;
     				}
    			else
    				{

     					rot->left=new node;
      					rot->left->key_value=key;
      					rot->left->f=fval;
      					rot->left->g=gval;
      					rot->left->h=hval;
      					return;
  	  				}
  			}

  		if(key>rot->key_value)
 			{
    			if(rot->right!=NULL)
    				{
      					insert_keyval(key,fval,gval,hval, rot->right);
      					return;
   		 			}
    			else
    				{
    					rot->right=new node;
      					rot->right->key_value=key;
      					rot->right->f=fval;
      					rot->right->g=gval;
      					rot->right->h=hval;
      					return;
    				}
  			}

  		if(key==rot->key_value)
  			{
 				rot->f=fval;
			  	rot->g=gval;
  				rot->h=hval;
    			return;
  			}
	}
}

void btree::insert_f(int key,int fval,int gval,int hval,node *rot)
{
 if(rot==NULL)
	{
        rot= new node;
    	rot->key_value=key;
    	rot->f=fval;
    	rot->g=gval;
    	rot->h=hval;
    	return;
  	}
 else
 	{

		if(fval<rot->f)
  			{
  				if(rot->left!=NULL)
    				{
     					insert_f(key,fval,gval,hval, rot->left);
     					return;
     				}
    			else
    				{

     					rot->left=new node;
      					rot->left->key_value=key;
      					rot->left->f=fval;
      					rot->left->g=gval;
      					rot->left->h=hval;
      					rot->left->route_parent_key=rot->key_value;
      					return;
  	  				}
  			}

  		if(fval>rot->f)
 			{
    			if(rot->right!=NULL)
    				{
      					insert_f(key,fval,gval,hval, rot->right);
      					return;
   		 			}
    			else
    				{
    					rot->right=new node;
      					rot->right->key_value=key;
      					rot->right->f=fval;
      					rot->right->g=gval;
      					rot->right->h=hval;
      					rot->right->route_parent_key=rot->key_value;
      					return;
    				}
  			}
        if(fval==rot->f)
        {
            insert_f(key,fval,gval,hval,rot->right);
        }
	}
}

bool btree::search_bool(int key,node *rot)
{
	if(key==rot->key_value)
        return true;

	else
	{
		if(rot->right==NULL&&rot->left==NULL)
		{
			return false;
		}
     	else
     	{
     		if(key>rot->key_value)
     		{
     			if(rot->right!=NULL)
     				return search_bool(key,rot->right);
                else
     			return false;
     		}
     		else
     		{
     			if(rot->left!=NULL)
     				return search_bool(key,rot->left);
     			else
     			return false;

     		}
     	}
	}
}

node *btree::search_node(int key,node *rot)
{

	if(key==rot->key_value)
        return rot;

	else
	{
		if(rot->right==NULL&&rot->left==NULL)
		{
			return NULL;
		}
     	else
     	{
     		if(key>rot->key_value)
     		{
     			if(rot->right!=NULL)
     				return search_node(key,rot->right);
                else
     			return NULL;
     		}
     		else
     		{
     			if(rot->left!=NULL)
     				return search_node(key,rot->left);
     			else
     			return NULL;

     		}
     	}
	}
}

void btree::destroy_leaf_keyval(int key , node *rot)
{
	if(key<rot->key_value)
	{
		if(rot->left!=NULL)
		{
			destroy_leaf_keyval(key,rot->left);
			return;
		}

		else
			return;
	}


	else
	{
		if(key>rot->key_value)
		{
			if(rot->right!=NULL)
			{
				destroy_leaf_keyval(key,rot->right);
				return;
			}

			else
			return;
        }

	}

	if(( key==rot->key_value ))
	{
        cout<<endl<<"a match found   "<<rot->key_value<<"  "<<key;
		if(!rot->left&&!rot->right)
		{
            cout<<endl<<"deleting it ";
             rot=NULL;

             delete rot;
             
			return;
		}
		else
		{   if(!rot->right)
				return;
			else
			{
			node *seed;
			seed=new node;
			seed=minnode(rot->right);
			rot->f=seed->f;
			rot->key_value=seed->key_value;
			rot->g=seed->g;
			rot->h=seed->h;
			//cout<<endl<<seed->key_value;
			destroy_leaf_keyval(seed->key_value,rot->right);

			return;
			}
		}
    }
}

void btree::destroy_leaf_f(int key,int fval , node *rot)
{
	if(fval<rot->f)
	{
		if(rot->left!=NULL)
		{
			destroy_leaf_f(key,fval,rot->left);
			return;
		}

		else
			return;
	}


	else
	{
		if(fval>rot->f)
		{
			if(rot->right!=NULL)
			{
				destroy_leaf_f(key,fval,rot->right);
				return;
			}

			else
			return;
        }

	}

	if(( fval==rot->f ))
	{
       if(key!=rot->key_value)
       {
            destroy_leaf_f(key,fval,rot->right);
            return;
       }
       else{
		   if(!rot->left&&!rot->right)
		     {
          //  cout<<endl<<rot->key_value;
                rot=NULL;
                cout<<endl<<"node deleted";
               // free(rot);
                return;
             }
           else
             {
                if(!rot->right)
				return;

                else
                {
                    node *seed;
                    seed=new node;
                    seed=minnode(rot->right);
                    rot->f=seed->f;
                    rot->key_value=seed->key_value;
                    rot->g=seed->g;
                    rot->h=seed->h;
			//cout<<endl<<seed->key_value;
                    destroy_leaf_keyval(seed->key_value,rot->right);
                    return;
                }
             }
        }
    }
}

bool btree::search_keyval_fval_comp(int key,int fval, node *rot)
{

	if(key==rot->key_value)
       {
       	if(fval>rot->f)
        return true;

    	else
    	return false;
       }

	else
	{
		if(rot->right==NULL&&rot->left==NULL)
		{
			return false;
		}
     	else
     	{
     		if(key>rot->key_value)
     		{
     			if(rot->right!=NULL)
     				return search_keyval_fval_comp(key,fval,rot->right);
                else
     			return false;
     		}
     		else
     		{
     			if(rot->left!=NULL)
     				return search_keyval_fval_comp(key,fval,rot->left);
     			else
     			return false;
     		}
     	}
	}
}
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
       temps=new node;
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
btree opentree_f,closedtree,opentree_keyval,master_tree;

int main()
{
int p=0;



	for(int i=0;i<250000;i++)
	{
		map_data[i]=0;
	}

	for (int i = 0; i < 250; ++i)
	{
		map_data[i+100000]=1;
	}
node temp[1000];
int t=0;
node first;
first.key_value=0;

closedtree.root=&first;
    while(t<250000)
    {
	 if(map_data[t]!=0)
	  {

		temp[p].key_value=t;
		//cout<<"the key value is "<<(temp[p]).key_value<<endl;
		closedtree.insert_keyval(t,-t,0,0,closedtree.root);
        p+=1;
		if(!closedtree.root)
		{
		cout<<"root is still null"<<endl;
		}
	  }
     t+=1;
    }
node *tx;
tx=new node;
tx=closedtree.minnode(closedtree.root);
cout<<"min node is "<<tx->key_value;
usleep(1000000);
closedtree.destroy_leaf_keyval(tx->key_value,closedtree.root);\
closedtree.destroy_leaf_keyval(100007,closedtree.root);
node *tmp;
tmp=closedtree.root;

while(tmp)
{
cout<<endl<<tmp->key_value<<endl;
tmp=tmp->right;
}

/*
node faker,faker2,faker3;
faker.key_value=0;
faker2.key_value=0;
faker3.key_value=0;
opentree_f.root=&faker;
opentree_keyval.root=&faker2;
master_tree.root=&faker3;
opentree_f.insert_f(200,-100,0,0,opentree_f.root);

astar(opentree_f.root,200000,opentree_f,opentree_keyval,closedtree,master_tree);*/
}


