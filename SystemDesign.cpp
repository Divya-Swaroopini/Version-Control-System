#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <chrono>
#include <stdlib.h>
#include <experimental/filesystem>
#include <fstream>
#include <bits/stdc++.h>



namespace filesys = std::experimental::filesystem;
 
using namespace std;

/*_________________________________________________________________________BST for VERSION CONTROL___________________________________________________________________*/

class File {
    public:
        //file features can be stored as private objects for now
        //removed hash value for simplicity sake, since our BST sorting is based on file size
        string versionName;
        fstream *stream_obj;
        int size;
        int version;
        File* location;
        std::chrono::system_clock::time_point timestamp = std::chrono::system_clock::now();
        //Implement version control - with BST
        File *lchild;
        File *rchild;
        File *parent;
        //technically creates nodes to the file BST's
        //location is a node initialized in the directory class and sent here to be inserted into the BST
    
        void create_version(string fname, File *node, int version) {
            //create a version node
            File *new_version;
            string buffer;
            fstream org, output;
            output.open(fname, ios::out);
            org.open(node->versionName, ios::in);
            if(org.is_open()) {
                while(std::getline(org, buffer)) {
                    output << buffer << '\n';
                }
            } 
            org.close(); output.close();

            //update version and version name
            new_version -> version = version ++;
            new_version -> versionName = fname;
            
            //update new version sie
            ifstream in_file(fname, ios::binary);
            in_file.seekg(0, ios::end);
            new_version -> size = in_file.tellg(); 
             

            //inserting version into the BST
            new_version -> parent = node;
            if(new_version -> size < node -> size)
                node->lchild = new_version;
            else
                node->rchild = new_version;

        }
        File *insert_file(File *location, int size, fstream *stream_obj, int version) {
            if(location == NULL) {
                //assign private variables their values
                this->stream_obj = stream_obj;
                //assign stream object pointer to the stream object of the file class.
                //So pointer to file location is technically just copied
                this->size = size;
                //save version number here
                this->version = version;
            }

            //rchild (less than)
            else if(location -> size < size) {
                location -> rchild = insert_file(location -> rchild, size, stream_obj, version);
            }

            //lchild (greater than or equal to)
            else {
                location -> lchild = insert_file(location -> rchild, size, stream_obj, version);
            }
            return location;
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
            return NULL;
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


        int height(File *node){
        if (node == NULL)
            return 0;
        else
            {
            /* compute the height of each subtree */
            int lheight = height(node->lchild);
            int rheight = height(node->rchild);
 
            /* use the larger one */
            if (lheight > rheight)
            {
                return(lheight + 1);
            }
            else {
            return(rheight + 1);
            }
            }
        }

        File *find_version(File *node, string filename){
            int h = height(node);
            for (int i = 1; i <= h; i++)
                if (node->versionName == filename)
                    return node;
            return NULL;
        }
};



/* _________________________________________________________________________DIRECTORY TREE____________________________________________________________________________*/
//Search Directory function yet to be implemented
class DirectoryTree {
    public:
        //Multiple instances of this structure are created to store file details.
        //each instance can be accessed via a vector and an index is used to trace the files
        struct FileData{
            int size;
            int version;
            std::chrono::system_clock::time_point timestamp = std::chrono::system_clock::now();
            string filename;
            unsigned char type[4]; //first 512 bytes of a file I guess(magic number)
            File *file;
        };
        
        int total_no_of_files = 1;
        vector<FileData> new_file;
        string DirName;
        
        //create pointers to child and parent directory
        DirectoryTree *dirParent;
        DirectoryTree *dirChild;
};

class DirectoryFunctions : DirectoryTree {

    public:
        DirectoryTree *insert_dir(DirectoryTree *node) {

            if (node == NULL)
                //insert root directory
                node = new DirectoryTree;
            else {
                node -> dirChild = insert_dir(node -> dirChild);
            }
            return node;
        }

        void delete_dir(DirectoryTree *node) {
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
        void delete_contents(DirectoryTree *node) {
            //delete the directory's file data 
            //after deleting each induvidual file
            File obj;
            for(int i=0; i < new_file.size(); i++)
                obj.delete_file(new_file[i].file);
            cout << "\nDirectory Contents deleted\n";
        }


        //create new file (Version 0)
        void createFilefromDir(DirectoryTree *node) {

            File fobj;
            fstream stream_obj;
            string fname;
            int size;

        

            new_file.push_back(FileData());
            
            

            //check if the file already exists in the system before accepting.
            do{
                cout << "Enter the filename with a proper file extension.\n";
                cin >> fname;
            }while(search_file(node, fname) != NULL);


            //Write code to open up the file to a text editor
            stream_obj.open(fname, ios::out);
            //rest of this code should execute after the user hits ctrl + save

            //to retrieve file size
            ifstream in_file(fname, ios::binary);
            in_file.seekg(0, ios::end);
            size = in_file.tellg(); 
             


            //call create file function the file values;
            //create new file node
            File *location = new File;
            
    
            fobj.insert_file(location, size, &stream_obj, 0);


            //find all the file values and assign them to Filedata Structure. 
            //set filename
            new_file[total_no_of_files].filename = fname;

             //set version
            new_file[total_no_of_files].version = 0;

            //set file object
            new_file[total_no_of_files].file = &fobj;

            //to retrieve file size
            new_file[total_no_of_files].size = size;
             
            //some method to find file extension ~ magic number
            if(in_file.is_open()) {
                in_file.seekg(0, ios::beg);
                in_file.read((char*)new_file[total_no_of_files].type, sizeof(new_file[total_no_of_files].type));
            }

            //since we have created a new file
            total_no_of_files ++;
        }

        //Also works like a print file details function
        File *search_file(DirectoryTree *node, string filename) {
            for(int i=0; i < node->total_no_of_files; i++) {
                if(node -> new_file[i].filename == filename){
                    //printfile Details

                    //compute timestamp
                    time_t t;
                    t = std::chrono::system_clock::to_time_t (new_file[i].timestamp);

                    cout << "This file exists:\n" << new_file[i].filename << new_file[i].size << new_file[i].type << ctime(&t);
                    return new_file[i].file;
                }
            }
            return NULL;
        }
 
     //pass the filename as the param
        void open_file(DirectoryTree *node,string filename) {
            //open file in read write mode on some editor
            //Implement after implementing file watcher
         system(("gedit "+ filename).c_str());
        }

        DirectoryTree *DirExists(DirectoryTree *root, string dirname, int n) {
            //Start at the root, push all the nodes into a stack, check the stack until it is empty
            cout << "checking if Directory exists\n";
            vector<DirectoryTree*> visited;
            bool found = false;
            while(found != true) {
                //loops ends if either all nodes have been traversed or if found = true
                if(visited.size() == n)
                    break;

                //This part is meant to keep track of the directory elements
                for(int i=0; i<visited.size(); i++) {
                    if(root == visited[i])
                        found = true;
                }
                if(found != true)
                    visited.push_back(root);
                
                found = false;

                
                //This part of the code is for traversing the tree
                if(root-> DirName == dirname)
                    found = true;
                else if(root -> dirChild)
                    root = root -> dirChild;
                else if(root -> dirParent)
                    root = root -> dirParent;
            }
            cout << "this function has executed\n";
            if(found = true) 
                return root;
            else return NULL;
        }

    void commit_file(DirectoryTree *dirnode, string file, string commitName) {
        //call search file to find existing file
        File object;
        File *fnode = search_file(dirnode, file);
        if(fnode != NULL) {
            //find the version of file that needs to be committed:
            File *commit = object.find_version(fnode, commitName);
            if(commit != NULL)
                add_file_to_dir(dirnode, fnode, commitName);
            else
                cout << "version not found!\n";
        }
        else
            cout << "The file name entered does not exist in directory";
    }

    void add_file_to_dir(DirectoryTree* node, File* fnode, string commitName) {
        node -> new_file[total_no_of_files].filename = commitName;
        node -> new_file[total_no_of_files].file = fnode;
        node -> new_file[total_no_of_files].size = fnode-> size;
        node -> new_file[total_no_of_files].version = fnode -> version; 

         //some method to find file extension ~ magic number
        ifstream in_file(commitName, ios::binary);
        if(in_file.is_open()) {
            in_file.seekg(0, ios::beg);
            in_file.read((char*)node -> new_file[total_no_of_files].type, sizeof(node -> new_file[total_no_of_files].type));
        }

        total_no_of_files++;
    }
        
};


/***********************************************************************MAIN***************************************************************************/


int main() {
    //Call the root_directory function to create a root directory and navigate into it if already exists

    //create objects to call File and DirectoryFunctions classes
    DirectoryFunctions dobj;
    File fobj;
    bool indir = false;
    int total_no_of_dir = 1;

    //set up root directory
    //root directory location always stored

    DirectoryTree *root = NULL;
    root = dobj.insert_dir(root);
    root -> DirName = "root";


    DirectoryTree *current_dir = dobj.DirExists(root, "root", total_no_of_dir);
    if(current_dir != NULL)
        indir = true;
    else {
        cout << "Failed to create root directory";
        return 0;
    }



    //When terminal app runs, it should automatically begin from this code and display below message.
    string path = "/root";
    cout << "\n**************************Application has Started**************************\n";
    string command="";
    while(command!="exit") {
        cout<<path<<"$";
        cin >> command;
            //each if-else statement will be a function call to perform a certain task, 
            //depending on command recieved by user until the user chooses to exit. 
            //When user exits, terminal app should automatically terminate.
            if(command == "help") {
                //List the menu of commands
            }
            else if(command == "nav") {
                cout << "Enter the directory name, to navigate there\n";
                //user is trying to navigate to a directory, so update path variable
                string dirName;
                cin >> dirName;
                
                DirectoryTree *dir = dobj.DirExists(root, dirName, total_no_of_dir);
                if(dir == NULL) 
                    cout << "The Directory you are trying to enter into does not exist";
                else {
                    cout << "Entering directory : " << dirName;
                    path = path + '/' + dirName;
                    indir = true;
                    current_dir = dir;
                }
            }
            else if(command == "cr-dir") {
                //create a new Directory
                string tempName;
                cout << "What will you name this directory?\n";
                cin >> tempName;
                if(dobj.DirExists(root, tempName, total_no_of_dir) == NULL) 
                    cout << "Directory Name already Exists :/";
                else {
                    DirectoryTree *newDir = new DirectoryTree;
                    newDir = dobj.insert_dir(newDir);
                    newDir -> DirName = tempName;
                    newDir -> dirParent = current_dir;
                    cout << "Directory has been created, navigate to it.\n";
                    ++total_no_of_dir;
                }
            }
            else if(command == "search-file") {
                string fileName;
                cout << "File name?\n";
                cin >> fileName;
                 //check if in directory
                if(indir) 
                    if(dobj.search_file(current_dir, fileName) == NULL) 
                        cout << "Can't find it\n";
                else
                    cout << "You must first Navigate into a directory\n";
            }
            else if(command == "cr-file") {
                //create file
                //check if in directory
                if(indir) 
                    dobj.createFilefromDir(current_dir);
                else
                    cout << "You must first Navigate into a directory\n";
            }
            else if(command == "del-file") {
                string filename;
                cout << "Enter filename to delete";
                cin >> filename;
                //check if in directory
                if(indir) {
                    File *node = dobj.search_file(current_dir, filename);
                    if(node != NULL)
                        fobj.delete_file(node);
                    else
                        cout << "File not found";
                }
                else
                    cout << "You must first Navigate into a directory\n";
            
            }
            else if(command == "del-dir") {
                string dirName;
                cout << "Enter directory to be deleted";
                cin >> dirName;
                DirectoryTree *dir = dobj.DirExists(root, dirName, total_no_of_dir);
                if(dir != NULL) 
                    dobj.delete_dir(dir);
                else 
                    cout << "That directory does not exist";
            }
            else if(command == "cr-ver") {
                string verName;
                //create a version of an existing file
                cout << "Enter name of file version from where you choose to branch out\n";
                cin >> verName;
                File *node = fobj.find_version(&fobj,verName);
                if (node == NULL)
                    cout << "No such version exists:";
                else {
                    cout << "Enter the name of the new version you wish to create\n";
                    cin >> verName;
                    fobj.create_version(verName, node, node->version);

                }
            }
            
            else if(command == "commit-file"){
                string commitFile;
                string filename;
                cout << "Enter filename of existing commit\n";
                cin >> filename;
                cout << "Enter the version name of file you choose to commit\n";
                cin >> commitFile;
                if(filename == commitFile)
                    cout << "File Version names cannot be the same\n";
                else
                    dobj.commit_file(current_dir, filename, commitFile);
            }
             
            else if(command == "exit") {
                cout << "\nExiting application\n";
            }
            else 
                cout << "Invalid Command. To check menu type 'help'.\n";
    }
    dobj.delete_dir(root);
    free(root);
    free(current_dir);
    return 0;
}


/*When System is Initialized : the root Directory must be automatically created if not exists, else user must navigate into root dir by default*/


/*
Flow of program if user needs to create a File:

User must Enter into Directory:
User must give command to Create File:
DirectoryTree *createFilefromDir() ->  creates a file with fstream and opens a text editor
User saves file and exits:
DirectoryTree *createFilefromDir() -> sets file metaData into vector<Filedata> -> creates a File class object and initializes BST -> calls File *create_file()
File *create_file() -> saves the address of file into the fstream object of it's class

*/

/*
Flow of program if user needs to Search for a file:

*User must navigate to a directory to search for file. Search for file must return 'not found' if file is not present in the existing directory.
Files search function is implemented in DirectoryTree class, cuz file metaData is stored there.
(No search for directory required to be implemented)*

User types the filename:
Search takes fileName and searches vector<FileData> for a matching filename. 
If filename is found return the file metadata, printed out in columns
Else return not found.
*/


/*Flow of program if a user needs to create a Directory:
User gives command to Create Directory:


*/

/*Flow of program if user needs to delete a directory 
User gives command to delete Directory:

*/

/*Flow of program if user needs to delete single file
User gives command to delete file:

*/
