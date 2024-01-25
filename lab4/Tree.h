//
// Created by Smile on 2019/4/8.
//

#ifndef C_BINARYDIMEN_TREE_H
#define C_BINARYDIMEN_TREE_H

#include <stdio.h>
#include <iostream>
#include <vector>
using namespace std;

/****************************************************************
 *                    Write your code below
 ****************************************************************/

class TreeNode
{
  friend ostream &operator<<(ostream &out, const TreeNode &b);
  friend class BinaryTree;
  friend class BinaryDimonTree;

private:
  /* data */
  long long int data[2];


public:
  /* methods */
  TreeNode(long long int a,long long int b)
  {
      data[0]=a;
      data[1]=b;
  }
    TreeNode(int a,int b,int c,TreeNode *x,TreeNode *y)
    {
        height = c;
        data[0] = a;
        data[1] = b;
        left = x;
        right = y;
    }
  long long int getX();  /* DO NOT CHANGE */
  long long int getY();  /* DO NOT CHANGE */
  ~TreeNode(); /* DO NOT CHANGE */
  TreeNode* left;
  TreeNode* right;
  long long int height;
};


class BinaryDimonTree
{
friend istream &operator>>(istream &in, BinaryDimonTree &tree); /* DO NOT CHANGE */

private:
  /* data */
  TreeNode *root;

public:
  /* methods */
  BinaryDimonTree();          /* DO NOT CHANGE */
  TreeNode *find_nearest_node(long long int x, long long int y);  /* DO NOT CHANGE */
  void clear(TreeNode* a);
  void recur_search(TreeNode *cur,long long int x,long long int y, long long int &min_distance, TreeNode **guess);
  ~BinaryDimonTree();
  void insert(TreeNode *&a,int x, int y,int height);
};

#endif //C_BINARYDIMEN_TREE_H
