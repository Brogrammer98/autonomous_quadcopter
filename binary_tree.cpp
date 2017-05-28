#include <cstddef>
#include <functional>
#include <queue>
#include <vector>
#include <list>
#include <deque>

#include <time.h>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <stdlib.h>


using namespace std;


float  map_data[250000];

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
       		void insert_keyval(int key,int fval,int gval,int hval,node *rot);
        	void insert_f(int key,int fval,int gval,int hval,node *rot);
        	node *minnode(node *rot);
        	bool search_bool(int key,node *rot);
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

void btree::insert_keyval(int key,int fval,int gval,int hval,node *rot)
{

  if(rot==NULL)
	{
		cout<<"inserting at root"<<endl;
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
		cout<<"inserting at root"<<endl;
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
      			rot->left->route_parent=rot;
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
      			rot->right->route_parent=rot;
      			return;
    		}
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

		if(!rot->left&&!rot->right)
		{
			rot=NULL;
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
			destroy_leaf_keyval(seed->key_value,rot->right);
			return;
			}
		}
    }
}

int main()
{

btree opentree_f,closedtree,opentree_keyval;

}