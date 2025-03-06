#include <iostream>
//#include <bits/stdc++.h>
#include <fstream>

using namespace std;

ifstream file;

// Structure of a Binary Tree Node
struct Node {
    int data;
    struct Node *left, *right;
    Node(int v)
    {   
        data = v;
        left = right = NULL;
    }
};
 
// Function to print inorder traversal
void printInorder(struct Node* node){
    if (node == NULL)
        return;
 
    // First recur on left subtree
    printInorder(node->left);
 
    // Now deal with the node
    cout << node->data << " ";
 
    // Then recur on right subtree
    printInorder(node->right);

}
/*
int diverleft(Node* node){
  if(node->left==nullptr && node->left==nullptr){
    return node->data;   
  }
  else if (node->left!=nullptr){
     return diverleft(node->left);
    }
  else if (node->right!=nullptr){
     return diverleft(node->right);
    };
return 0;
}

// Function to solve problem
void solve(struct Node* node){
    if (node == NULL)
        return;
    int datal, datar;

    solve(node->left);  
    
    // Then recur on right subtree
    solve(node->right);

    if(node->left==nullptr && node->right!=nullptr){
        datar=diverleft(node->right);
       if( node->data > datar ){
           node->left = node->right;
           node->right=nullptr;
        }
    }   
    else if(node->left!=nullptr && node->right==nullptr){
            datal=diverleft(node->left);
           if( node->data < datal ){
               node->right = node->left;
               node->left=nullptr;
            }
        }   
    else if(node->left!=nullptr && node->right!=nullptr){
          Node* cache;

          datal=diverleft(node->left);
          datar=diverleft(node->right);

          if(datal>datar){
               cache = node->left;
               node->left = node->right;
               node->right=cache;
            }
        }
    
}
*/
int arrange (struct Node* node){
    if (node == NULL) return -1;

    int datal, datar;

   datal = arrange(node->left);
     
    // Then recur on right subtree
   datar = arrange(node->right);

    if(datal==-1 && datar!=-1){  // Αριστερα null δεξια οκ
       if( node->data > datar ){
           node->left = node->right;
           node->right=nullptr;

           return datar;
        }
        
        return node->data;
    }   
    else if(datal!=-1 && datar==-1){  // δεξια null αριστερα οκ       
       if( node->data < datal ){
           node->right = node->left;
           node->left=nullptr;

           return node->data;
        }
    return datal;
    }   
    else if(datal!=-1 && datar!=-1){ // και τα δυο οκ
        Node* cache;

        if(datal>datar){
            cache = node->left;
            node->left = node->right;
            node->right=cache;

            return datar;
        }

        return datal;
    }

  return node->data;  // και τα δυο null
}

// Function to read the imput
Node* readInorder(){
    int val;
    file>>val;
    if (val==0 ){
        return nullptr;
    }
    
    Node* root = new Node(val);
    
    // First recur on left subtree
    root->left=readInorder();
   
    // Then recur on right subtree
    root->right=readInorder();

    return root;
}

int main(int argc, char **argv){
    struct Node* myTree;
    int N;
 
    file.open(argv[1]);
    
    if (!file.is_open())
    {
        std::cerr << "Error opening file" << std::endl;
        return 1;
    }
    
    file >> N;

    myTree=readInorder();

   file.close();

   /* cout << "Inorder traversal of binary tree is: \n";
    printInorder(myTree);
    cout<<'\n';
   */

    arrange(myTree);

    printInorder(myTree);
    cout<<'\n';
 
    return 0;
}
