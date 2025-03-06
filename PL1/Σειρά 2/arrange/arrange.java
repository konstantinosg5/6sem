import java.io.BufferedReader;
import java.io.FileReader;

class TreeNode {
    int data;
    TreeNode left;
    TreeNode right;

    public TreeNode(int data) {
        this.data = data;
        this.left = null;
        this.right = null;
    }
}
//===============================================================================
//=========================================  Class BinaryTree   =================
class BinaryTree extends Exception {
    TreeNode root;
    static int i=0;
    String line = null ;
    int N;
    BufferedReader reader;
    String[] arr;

//=========================================
public BinaryTree() {
    
}   
//=======================================================
public void insert(String filename) {
    try{
        this.root = null;
        reader = new BufferedReader(new FileReader (filename) );
    
        line =reader.readLine();
        N=Integer.parseInt(line);
        
        line =reader.readLine();
        arr = line.split("\\s+");
        reader.close();
    }catch ( Exception e ) { // Bad practice !
        System.out.println ( "Something went wrong: " + e ) ;
    }
    root = readInorder(root, 0);    
}


//===================================================
private TreeNode readInorder (TreeNode root, int value) {
    try{      
        int val; 
       
        val=Integer.parseInt(arr[i++]);
       // System.out.print(Integer.toString(val) + ' ');
        if (val==0 ){
            return root;
        }
        
        if (root == null) {
            root = new TreeNode(val);
            
        }
        
        // First recur on left subtree
        root.left=readInorder(root.left,val);
        
        // Then recur on right subtree
        root.right=readInorder(root.right,val);
    
        return root;
        

        
    }catch ( Exception e ) { // Bad practice !
        System.out.println ( "Something went wrong: " + e ) ;
        return root;
    }
}


//===========================================  solve
// Same as in C++ 
public int solve (TreeNode node){
    if (node == null) return -1;

    int datal, datar;

    datal = solve(node.left);
     
    // Then recur on right subtree
    datar = solve(node.right);

    if(datal==-1 && datar!=-1){  // Αριστερα null δεξια οκ
       if( node.data > datar ){
           node.left = node.right;
           node.right=null;

           return datar;
        }
        
        return node.data;
    }   
    else if(datal!=-1 && datar==-1){  // δεξια null αριστερα οκ       
       if( node.data < datal ){
           node.right = node.left;
           node.left=null;

           return node.data;
        }
    return datal;
    }   
    else if(datal!=-1 && datar!=-1){ // και τα δυο οκ
        TreeNode cache;

        if(datal>datar){
            cache = node.left;
            node.left = node.right;
            node.right=cache;

            return datar;
        }

        return datal;
    }

  return node.data;  // και τα δυο null
}

//========================================================
void inorderTraversal(TreeNode node) {
    if (node != null) {
        inorderTraversal(node.left);
        System.out.print(node.data + " ");
        inorderTraversal(node.right);
    }
}

}


//================================  MAIN  ====================================
public class arrange{
public static void main (String[] args) {
try {   
    BinaryTree tree = new BinaryTree();  
    tree.insert(args[0]); 
   // System.out.print("input: ");
   // tree.inorderTraversal(tree.root);
   // System.out.println();
    tree.solve(tree.root);
   // System.out.print("output:");
    tree.inorderTraversal(tree.root);
    System.out.println();
       
} catch ( Exception e ) { // Bad practice !
    System.out.println ( "Something went wrong: " + e ) ;
}

}//main

}//main class