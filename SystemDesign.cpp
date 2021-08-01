#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <stdlib.h>
#include <experimental/filesystem>
#include <fstream>

namespace filesys = std::experimental::filesystem;
 
using namespace std;
/* _________________________________________________________________________DIRECTORY TREE____________________________________________________________________________*/
//Search Directory function yet to be implemented
class DirectoryTree {
    public:
        //Multiple instances of this structure are created to store file details.
        //each instance can be accessed via a vector and an index is used to trace the files
        struct FileData{
            int size;
            time_t timestamp;
            string filename;
            string type;
            File *file;
        };
        int total_no_of_files = 0;
        vector<FileData> new_file;

        
        //create pointers to child and parent directory
        DirectoryTree *dirParent;
        DirectoryTree *dirChild;
    
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
            FileData obj;
            //Send each file of this directory to a function -
            //- that deletes induvidual files
            for(int i = 0; i < obj.size; i++)
                //assuming the column '0' of dirTables holds pointer to file location on hard disk
                fobj.delete_file(obj.file);
        }

        DirectoryTree *createFilefromDir(DirectoryTree *node) {

            File fobj;
            fstream stream_obj;
            string fname;
            cout << "File name? with proper extension plz\n";
            cin >> fname;

            //additional code needs to be added to check if the file already exists in the system.
            //Hence search functions need to still be implemented

            //Write code to open up the file to a text editor
            stream_obj.open(fname, ios::out);
            //rest of this code should execute after the person hits ctrl + save


            //find all the file values and assign them to Filedata Structure. 
            //set filename
            new_file[total_no_of_files].filename = fname;
            //set file object
            new_file[total_no_of_files].file = &fobj;
            //some method to retrieve file size
                //new_file.size = fobj.find_size(file);
            //some method to find file extension ~ magic number
                //new_file.size = fobj.find_type(file);
            //some method to set timestamp at time of creation

            //call create file function the file values;
            fobj.create_file(&fobj, new_file[total_no_of_files].size, &stream_obj);

            //since we have created a new file :
            total_no_of_files ++;
        }

        DirectoryTree *modify_contents(DirectoryTree *node) {
            //modify the contents, so chnage the file table
            //called from the 'File' class everytime a commit is made
        }
};

/*_________________________________________________________________________BST for VERSION CONTROL___________________________________________________________________*/

/*Left to Implement:
1.Search File
2.Modify File
*/
class File {
    private:
        //file features can be stored as private objects for now
        //removed hash value for simplicity sake, since our BST sorting is based on file size
        fstream *stream_obj;
        int size;
    public:
        //Implement version control - with BST
        File *lchild;
        File *rchild;
        File *parent;
        //technically creates nodes to the file BST's
        //location is a node initialized in the directory class and sent here to be inserted into the BST
        File *create_file(File *location, int size, fstream *stream_obj) {
            if(location == NULL) {
                //assign private variables their values
                this->stream_obj = stream_obj;
                //assign stream object pointer to the stream object of the file class.
                //So pointer to file location is technically just copied
                this->size = size;
            }

            //rchild (less than)
            else if(location -> size < size) {
                location -> rchild = create_file(location -> rchild, size, stream_obj);
                location -> rchild -> parent = location;
            }

            //lchild (greater than or equal to)
            else {
                location -> lchild = create_file(location -> rchild, size, stream_obj);
                location -> lchild -> parent = location;
            }
        }

        //Clear out file data from BST nodes
        //called from directory class
        File *delete_file(File *node) {
            //compute file size()
            int size;
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
};




/*
Flow of content if user needs to create a File (First File in filesystem):

User gives command to Create Directory:
Object of type DirectoryTree created -> So root Directory created & vector<Filedata> initialized.
User must Enter into Directory:
User must give command to Create File:
DirectoryTree *createFilefromDir() ->  creates a file with fstream and opens a text editor
User saves file and exits:
DirectoryTree *createFilefromDir() -> sets file metaData into vector<Filedata> -> creates a File class object and initializes BST -> calls File *create_file()
File *create_file() -> saves the address of file into the fstream object of it's class

*/
