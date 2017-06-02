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

int main()
{
int p=0;

btree opentree_f,closedtree,opentree_keyval;


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
		cout<<"the key value is "<<(temp[p]).key_value<<endl;
		closedtree.insert_keyval(t,-1*t,0,0,closedtree.root);
        p+=1;
		if(!closedtree.root)
		{
		cout<<"root is still null"<<endl;
		}
	  }
     t+=1;
    }

node *temps;

temps=closedtree.search_node(100020,closedtree.root);
cout<<temps->key_value;
/*
node *teps;
teps=new node;
teps=closedtree.minnode((closedtree.root));
int y=0;
cout<<teps->key_value;*/
/*
while(teps)
{
//cout<<y++;
cout<<teps->key_value<<"   ";
teps=teps->left;
}*/
/*
    node curr_node;
    curr_node.key_value=0;
  */
   // opentree_f.insert_f(&curr_node,opentree_f.root);
   // opentree_keyval.insert_keyval(&curr_node,opentree_keyval.root);
//astar(&curr_node,0);

}
