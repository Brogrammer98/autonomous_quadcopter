#include <unistd.h>-
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <stdlib.h>

int INT_MAX =9999999;
float a[8]={-499,-1,499,-500,500,-501,1,501};
float b[8]={1.4,1,1.4,1,1,1.4,1,1.4};
using namespace std;

const float pi = 3.14159265;
const float RAD_TO_DEG = 180.0/pi;
const float DEG_TO_RAD = pi/180.0;

int    width      = 500;
int    height     = 500;


int map_data[250000];

struct node
{
  int64_t key_value;
  int route_parent_key=0;
  float f=0,g=0,h=0;
  node *tree_parent=nullptr;
  node *left=nullptr;
  node *right=nullptr;
node()
  {
    left=nullptr;
    right=nullptr;
    tree_parent=nullptr;

    route_parent_key=0;
  }
~node()
  {

  }
};

class btree
{
    public:
          btree();
          void insert_keyval(int key,float fval,float gval,float hval,node *rot);
          void insert_keyval(int key,float fval,float gval,float hval,int rp,node *rot);
          void destroy_leaf_keyval(int key , node *rot);
          void insert_f(int key,float fval,float gval,float hval,node *rot);
          void insert_f(int key,float fval,float x,float gval,float hval,node *rot);
          void destroy_leaf_f(int key,float fval , node *rot);
          float return_f(int key,node *rot);
          bool search_bool(int key,node *rot);
          bool search_keyval_fval_comp(int key,float fval,node *rot);
          node *search_node(int key,node *rot);
          node *minnode(node *rot);
          node *root;
};

btree::btree()
{
  root=(node*) malloc(sizeof(node));
  root=nullptr;
}
float btree::return_f(int key,node *rot)
{
if(key==rot->key_value)
  return rot->f;
else
  {
     if(key>rot->key_value)
        return search_bool(key,rot->right);
     else
        return search_bool(key,rot->left);

  }
}
node *btree::minnode(node *rot)
{
  if(rot->left==nullptr)
    return rot;
  else
    return minnode(rot->left);
}
void btree::insert_keyval(int key,float fval,float gval,float hval,int rp,node *rot)
{

  if(rot==nullptr)
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
          if(rot->left!=nullptr)
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
                rot->left->tree_parent=rot;
                return;
              }
        }

      if(key>rot->key_value)
      {
          if(rot->right!=nullptr)
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
                rot->right->tree_parent=rot;
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

void btree::insert_keyval(int key,float fval,float gval,float hval,node *rot)
{
 if(rot==nullptr)
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
     if(rot->left!=nullptr)
      {
       insert_keyval(key,fval,gval,hval, rot->left);
       return;
      }
     else
     { // cout<<"an insertion   ("<<key%width<<","<<key/width<<")"<<endl;
       rot->left=new node;
       rot->left->key_value=key;
       rot->left->f=fval;
       rot->left->g=gval;
       rot->left->h=hval;
       rot->left->tree_parent=rot;
       return;
      }
    }
   if(key>rot->key_value)
    {
     if(rot->right!=nullptr)
      {
       insert_keyval(key,fval,gval,hval, rot->right);
       return;
      }
     else
      {//cout<<"an insertion   ("<<key%width<<","<<key/width<<")"<<endl;
       rot->right=new node;
       rot->right->key_value=key;
       rot->right->f=fval;
       rot->right->g=gval;
       rot->right->h=hval;
       rot->right->tree_parent=rot;
       return;
      }
    }

    if(key==rot->key_value)
    {//cout<<"an insertion   ("<<key%width<<","<<key/width<<")"<<endl;
    rot->f=fval;
     rot->g=gval;
     rot->h=hval;
     return;
    }
  }
}

void btree::insert_f(int key,float fval,float gval,float hval,node *rot)
{
 if(rot==nullptr)
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
     if(rot->left!=nullptr)
      {
       insert_f(key,fval,gval,hval,rot->left);
       return;
      }
     else
      {
       rot->left=new node;
       rot->left->key_value=key;
       rot->left->f=fval;
       rot->left->g=gval;
       rot->left->h=hval;
       rot->left->tree_parent=rot;
       rot->left->route_parent_key=rot->key_value;
       return;
      }
    }
   if(fval>rot->f)
    {
     if(rot->right!=nullptr)
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
       rot->right->tree_parent=rot;
       rot->right->route_parent_key=rot->key_value;
       return;
      }
    }
   if(fval==rot->f)
     if(key!=rot->key_value)
     insert_f(key,fval,gval,hval,rot->right);
     else
     {  rot->f=fval;
        rot->g=gval;
        rot->h=hval;
     }
  }
}

void btree::insert_f(int key,float fval,float x,float gval,float hval,node *rot)
{
 if(rot==nullptr)
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
     if(rot->left!=nullptr)
      {
       insert_f(key,fval,gval,hval,rot->left);
       return;
      }
     else
      {
       rot->left=new node;
       rot->left->key_value=key;
       rot->left->f=fval;
       rot->left->g=gval;
       rot->left->h=hval;
       rot->left->tree_parent=rot;
       rot->left->route_parent_key=rot->key_value;
       return;
      }
    }
   if(fval>rot->f)
    {
     if(rot->right!=nullptr)
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
       rot->right->tree_parent=rot;
       rot->right->route_parent_key=rot->key_value;
       return;
      }
    }
   if(fval==rot->f)
    {
     if(key!=rot->key_value)
     insert_f(key,fval,gval,hval,rot->right);
     else
     {  rot->f=x;
        rot->g=gval;
        rot->h=hval;
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
   if(rot->right==nullptr&&rot->left==nullptr)
      return false;

   else
    {
     if(key>rot->key_value)
      {
       if(rot->right!=nullptr)
        return search_bool(key,rot->right);
       else
        return false;
      }
     else
      {
       if(rot->left!=nullptr)
        return search_bool(key,rot->left);
       else
       {
        return false;
       }
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
   if(rot->right==nullptr&&rot->left==nullptr)
     return nullptr;
   else
    {
     if(key>rot->key_value)
      {
       if(rot->right!=nullptr)
        return search_node(key,rot->right);
       else
        return nullptr;
      }
     else
      {
       if(rot->left!=nullptr)
        return search_node(key,rot->left);
       else
        return nullptr;
      }
    }
  }
}

void btree::destroy_leaf_keyval(int key , node *rot)
{
 if(key<rot->key_value)
  {
   if(rot->left!=nullptr)
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
     if(rot->right!=nullptr)
      {
       destroy_leaf_keyval(key,rot->right);
       return;
      }
     else
      return;
    }
  }

 if(( key==rot->key_value ))
  {//cout<<"exec del"<<endl;
   node *seed;
   seed=new node;
   if(!rot->right)
    goto jumpo;

    seed=minnode(rot->right);
    rot->f=seed->f;
    rot->key_value=seed->key_value;
    rot->g=seed->g;
    rot->h=seed->h;
    destroy_leaf_keyval(seed->key_value,rot->right);

    goto pop;

    jumpo:
    if(rot->tree_parent->right==rot&&rot->tree_parent!=nullptr)
    rot->tree_parent->right=nullptr;
    if(rot->tree_parent->left==rot&&rot->tree_parent!=nullptr)
    rot->tree_parent->left=nullptr;
    pop:
    return;
  }
}


void btree::destroy_leaf_f(int key,float fval , node *rot)
{
 if(fval<rot->f)
  {
   if(rot->left!=nullptr)
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
     if(rot->right!=nullptr)
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
   else
    {
     if(( key==rot->key_value ))
      {
       node *seed;
       seed=new node;

       if(!rot->right)
       goto jumpo;

       seed=minnode(rot->right);
       rot->f=seed->f;
       rot->key_value=seed->key_value;
       rot->g=seed->g;
       rot->h=seed->h;

       destroy_leaf_f(seed->key_value,seed->f,rot->right);
       goto pop;
       jumpo:
       if((rot->tree_parent->right==rot)&&rot->tree_parent!=nullptr)
        rot->tree_parent->right=nullptr;
       if((rot->tree_parent->left==rot)&&rot->tree_parent!=nullptr)
        rot->tree_parent->left=nullptr;
       pop:
       return;
      }
    }
  }
}

bool btree::search_keyval_fval_comp(int key,float fval, node *rot)
{
 if(key==rot->key_value)
  {
   if(fval>=rot->f)
    return false;
   else
    return true;
  }
 else
  {
   if(rot->right==nullptr&&rot->left==nullptr)
    return true;
   else
    {
     if(key>rot->key_value)
      {
       if(rot->right!=nullptr)
        return search_keyval_fval_comp(key,fval,rot->right);
       else
        return true;
      }
     else
      {
       if(rot->left!=nullptr)
        return search_keyval_fval_comp(key,fval,rot->left);
       else
        return true;
      }
    }
  }
}