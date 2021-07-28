#include <iostream>
#include <vector>
#include <string>
#include <ctime>

 
using namespace std;
/* _________________________________________________________________________DIRECTORY TREE____________________________________________________________________________*/

class DirectoryTree {
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

        DirectoryTree *modify_contents(DirectoryTree *node) {
            //modify the contents, so chnage the file table
            //called from the 'File' class everytime a commit is made
        }
};

/*_________________________________________________________________________BST for VERSION CONTROL___________________________________________________________________*/
//This class is meant to store an array of file links to the file BST's
class FileArray {

};

//default public inheritence?
class File : FileArray {
    private:
        //file features can be stored as private objects for now
        string fileName;
        std::time_t timestamp;
        string hashValue;
        string type;
        int size;
        int index;
    public:
        //Implement version control - with BST
        File *lchild;
        File *rchild;
        File *parent;
        //technically creates nodes to the file BST's
        void create_file(int index, File *location, string name, string type, int size) {
           //assign private variables their values
           this->fileName = name;
           this->type = type;
           this->size = size;
            //generate hash value for file being created
           this->hashValue = generate_hash(location, size);



        }

        void delete_file(string location) {

        }

        string generate_hash(File *location, int size) {

        }

};