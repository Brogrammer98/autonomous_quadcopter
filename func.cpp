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
			delete rot;
			return;
		}
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


float astar_w1=0,astar_w2=0;

void astar(node *current_node,int goal)
{
    int  curr_xpos=current_node->key_value%width;
    int curr_ypos=current_node->key_value/width;

  if(!current_node)
    return;
  else
  { for(int i=-1;i<=1;i++)
      {
    for(int j=-1;j<=1;j++)
    {
      if(curr_ypos+j>=0&&curr_xpos+i>=0&&curr_xpos+i<width&&curr_ypos+j<height)
      {
        node tempo,tempo_mirror;

        tempo.key_value=curr_xpos+curr_ypos*width;
        tempo_mirror.key_value=curr_xpos+curr_ypos*width;

        tempo.g =current_node->g + astar_w1*(sqrt(pow(i,2)+pow(j,2))) ;
        tempo_mirror.g =current_node->g + astar_w1*(sqrt(pow(i,2)+pow(j,2))) ; // add distance here

        tempo.h = astar_w2*( abs(i) + abs(j) ); // distance from sucesso
        tempo_mirror.h = astar_w2*( abs(i) + abs(j) ); // distance from sucesso

        tempo.f=tempo.g+tempo.h;
        tempo_mirror.f=tempo.g+tempo.h;


        node *searchresopen = opentree_keyval.search_node(&tempo,opentree_keyval.root);
        node *searchresclosed = closedtree.search_node(&tempo,closedtree.root);


        if(curr_xpos+curr_ypos*width==goal)
        {
          tempo.route_parent=current_node;
          closedtree.insert_keyval(&tempo,closedtree.root);


          node tempx;
          tempx=tempo;


          do
          {
            path_vector[path_vector_count++]=tempx.key_value;

          }while(tempx.route_parent!=NULL);

          for(int i=0;i<path_vector_count;i++)
          {
            cout<<path_vector[i]<<"   ";
          }

          return;
        }

        if(opentree_keyval.search_bool(&tempo,opentree_keyval.root)&& (searchresopen->f<tempo.f))
          goto ifexit; // add jump here
        if(closedtree.search_bool(&tempo,closedtree.root)&&(searchresclosed->f<tempo.f))
          goto ifexit; // add jump here
        tempo.route_parent=current_node;
        tempo_mirror.route_parent=current_node;

        opentree_keyval.insert_keyval(&tempo_mirror,opentree_keyval.root);
        opentree_f.insert_f(&tempo,opentree_f.root);

        ifexit:;
      }
    }
  }

  current_node=opentree_f.minnode(opentree_f.root);

  astar(current_node,goal);

    }
}

int main()
{

btree opentree_f,closedtree,opentree_keyval;

}
