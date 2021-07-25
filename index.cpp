/*
USER -> TERMINAL -> THIS PROGRAM -> DISK

What are the functionalities?
1)Create file
2)Modify file
3)Delete file
4)Duplicate file
5)Restore Versions
6)Open file
*/

#include <iostream>
#include <string>
#include <ctime>
#include <fstream>
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

using namespace std;

class BSTnode {
    private:
        string lineData;
        string fileData;
        string fileName;
        std::time_t timestamp;
        string hashValue;
        string type;
        int size;
        int index;
    
    public: 

        BSTnode * lchild;
        BSTnode * rchild;
        BSTnode * parent;
        
        BSTnode * create_node(int index, BSTnode * node, string filedata,
                        string name, string hash, string type, int size) {

            //create new root file
            if(node == NULL)
            {
                // What about opening a new file up now?   
                node = new BSTnode;
                node -> index = index;
                node -> fileData = filedata;
                node -> fileName = name;
                node -> hashValue = hash;
                node -> type = type;
                node -> index = index;
                node -> size = size;
            }

            //deal with versions of file
            else if(node ->  index < index) {
                node -> rchild = create_node(index, node -> rchild, filedata, name, hash, type, size);
                node -> rchild -> parent = node;
            }

            else {
                node -> lchild = create_node(index, node -> lchild, filedata, name, hash, type, size);
                node -> lchild -> parent = node;
            }
        }

        BSTnode * delete_file(int index, BSTnode * node) {
            //Delete BST node
            if(node == NULL || node->index == index) 
                return delete_root(node);
            BSTnode* current = node;
            while(1) {
                int x = current->index;
                if(index < x){
                    if(current->lchild == NULL || current->lchild->index == index) {
                        current->lchild = delete_root(current->lchild);
                        break;
                    }
                    current = current -> lchild;
                } 
                else {
                    if(current->rchild == NULL || 
                    current -> rchild -> index == index) {
                        current -> rchild = delete_root(current -> rchild);
                        break;
                    }
                    current = current -> rchild;
                }
            }
            return node;
        }

        BSTnode * delete_root(BSTnode * node) {
            if(!node || node -> index == 0)
                return NULL;
            if(node -> rchild == NULL) 
                return node -> lchild;
            BSTnode * x = node -> rchild;
            while(x -> lchild)
                x = x -> lchild;
            x -> lchild = node -> lchild;
            return node -> rchild;
        }
};





void create_file() {
    cout << "Creating file in current directory\n";
    //check for duplicate file name
    int f = 0;
    string fileName;
    while (f != 1) {
        //compare function to check every other file in the current directory
    }

    //B-tree node needs to be created with the new file
    //BSTNode * root = NULL; //created empty tree
    //call create_node()

    //Metadata - filename, size, type, timestamp, etc + hash value
    //call hash function

    //display path of file created

    //open text editor directly with new file created

    //when user is in editor, a bash scrit needs to be running to
    //check for file changes and notify the program.

}

string hash_compute() {
    //returns the hash value computed for the file
}

void edit_file() {
    //function called from bash script when the file has been edited
    //function also called when command line changes are executed

    //hash value re-computed
    //call hash_compute()

    //new file version stored in BST
    //compute an index value and send it to create node
    //call create_node()
}


void delete_file() {
    //when node is deleted, the BST needs to be reformed
}

int main() {
    //When terminal app runs, it should automatically begin from this code and display below message.
    cout << "\nApplication has Started:\n";
    string choice="";
    cin >> choice;
    while(choice!="exit") {

            //each if-else statement will be a function call to perform a certain task, 
            //depending on command recieved by user until the user chooses to exit. 
            //When user exits, terminal app should automatically terminate.
            if(choice == "ls -m") {
                //List the menu of commands
            }
            else if(choice == "Cfile") {
            //create file
                create_file();
            }
            else if(choice == "Ofile") {
            //Open file
            }
            else if(choice == "Dfile") {

            }
            else if(choice == "exit")
                cout << "\nExiting application\n";
            else 
                cout << "Invalid Command. To check menu type 'ls -m'.";
    }
}