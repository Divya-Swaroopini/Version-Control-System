#include <iostream>
#include <vector>
#include <string>
#include <ctime>

 
using namespace std;
/* _________________________________________________________________________DIRECTORY TREE____________________________________________________________________________*/

class DirectoryTree : File {
    public:
        //files in the directory
        vector<vector<string>> files;
        //create pointers to child and parent directory
        DirectoryTree * dirParent;
        DirectoryTree * dirChild;
    
        DirectoryTree *create_dir(DirectoryTree *node) {
            //code to create node
            if(node == NULL)
                //this node will inherit the empty 'files' matrix
                node = new DirectoryTree;
            else {
                node -> dirChild = create_dir(node -> dirChild);
                node -> dirParent -> dirParent = node;
            }
        }

        DirectoryTree *delete_dir(DirectoryTree *node) {
            //Start at index of directory
            //if directory has a child, take pointer to child, else delete root
            DirectoryTree *TobeDeleted = node;
            if(node -> dirChild) {
                node = node -> dirChild;
                while(node != TobeDeleted) {
                    if(node -> dirChild)
                        node = node -> dirChild;
                    else {
                        delete_contents(node);
                        node = node -> dirParent;
                    }
                }
            }
            delete_contents(node);
            //final call to function to delete root

        }
        DirectoryTree *delete_contents(DirectoryTree *node) {
            //delete the directory's file data 
            //after deleting each induvidual file

            //calling delete_files, which deletes all files of directory
            delete_files(node);

        }

        DirectoryTree *delete_files(DirectoryTree *node) {
            File fobj;
            //Send each file of this directory to a function -
            //- that deletes induvidual files
            for (int i = 0; i < files.size(); i++)
                //assuming the column '0' of dirTables holds pointer to file location on hard disk
                fobj.delete_file(files[i][0]);
        }

        DirectoryTree *createFilefromDir(DirectoryTree *node) {
            //create an object of 'File' (BST type), creates place in memory fot this file 
            File new_file;
            //append the location of new_file to FileTable

            //generate index value for file
            int index;

            //find file size of temporary file;
            int size;

            //call create file function to assign the file values;
            create_file(index, size, std::addressof(new_file));
        }

        DirectoryTree *modify_contents(DirectoryTree *node) {
            //modify the contents, so chnage the file table
            //called from the 'File' class everytime a commit is made
        }
};

/*_________________________________________________________________________BST for VERSION CONTROL___________________________________________________________________*/

class File {
    private:
        //file features can be stored as private objects for now
        string hashValue;
        int size;
        int index;
    public:
        //Implement version control - with BST
        File *lchild;
        File *rchild;
        File *parent;
        //technically creates nodes to the file BST's
        File *create_file(int index, int size, File *location) {
            if(location == NULL) {
                //assign private variables their values
                this->index = index;
                this->size = size;
                //generate hash value for file being created
                this->hashValue = generate_hash(location);
            }

            //rchild
            else if(location -> size < size) {
                location -> rchild = create_file(index, size, location -> rchild);
                location -> rchild -> parent = location;
            }

            //lchild
            else {
                location -> lchild = create_file(index, size, location -> rchild);
                location -> lchild -> parent = location;
            }
        }

        //Clear out file data from BST nodes
        File *delete_file(string node) {
            //compute file size()
            int size = node.size();
            //Delete BST node
            if(node == NULL || node->size == size) 
                return delete_root(node);
            File *current = node;
            while(1) {
                int x = current->size;
                if(size < x){
                    if(current->lchild == NULL || current->lchild->size == size) {
                        current->lchild = delete_root(current->lchild);
                        break;
                    }
                    current = current -> lchild;
                } 
                else {
                    if(current->rchild == NULL || current -> rchild -> size == size) {
                        current -> rchild = delete_root(current -> rchild);
                        break;
                    }
                    current = current -> rchild;
                }
            }
            return node;
        }

        File * delete_root(File * node) {
            if(!node || node -> size == 0)
                return NULL;
            if(node -> rchild == NULL) 
                return node -> lchild;
            File * x = node -> rchild;
            while(x -> lchild)
                x = x -> lchild;
            x -> lchild = node -> lchild;
            return node -> rchild;
        }

        string generate_hash(File *location) {
            //Write hash function here
        }
};