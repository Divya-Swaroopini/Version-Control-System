#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <chrono>
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
            int version;
            std::chrono::system_clock::time_point timestamp = std::chrono::system_clock::now();
            string filename;
            unsigned char type[4]; //first 512 bytes of a file I guess(magic number)
            File *file;
        };
        int total_no_of_files = 0;
        vector<FileData> new_file;
        string DirName;
        
        //create pointers to child and parent directory
        DirectoryTree *dirParent;
        DirectoryTree *dirChild;
};

class DirectoryFunctions : DirectoryTree {

    public:
        DirectoryTree *insert_dir(DirectoryTree *node) {
            //code to create node
            if(node == NULL)
                //this node will inherit the empty 'files' matrix
                node = new DirectoryTree;
            else {
                node -> dirChild = insert_dir(node -> dirChild);
                node -> dirParent -> dirParent = node;
            }
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

            //calling delete_files, which deletes all files of directory
            delete_files(node);

        }

        void delete_files(DirectoryTree *node) {
            File fobj;
            FileData obj;
            //Send each file of this directory to a function -
            //- that deletes induvidual files
            for(int i = 0; i < obj.size; i++)
                //assuming the column '0' of dirTables holds pointer to file location on hard disk
                fobj.delete_file(obj.file);
        }


        //create new file (Version 0)
        void createFilefromDir(DirectoryTree *node) {

            File fobj;
            fstream stream_obj;
            string fname;

            //check if the file already exists in the system before accepting.
            do{
                cout << "Enter the filename with a proper file extension.\n";
                cin >> fname;
            }while(search_file(node, fname) == true);


            //Write code to open up the file to a text editor
            stream_obj.open(fname, ios::out);
            //rest of this code should execute after the user hits ctrl + save


            //find all the file values and assign them to Filedata Structure. 
            //set filename
            new_file[total_no_of_files].filename = fname;

             //set version
            new_file[total_no_of_files].version = 0;

            //set file object
            new_file[total_no_of_files].file = &fobj;

            //to retrieve file size
            ifstream in_file(fname, ios::binary);
            in_file.seekg(0, ios::end);
            new_file[total_no_of_files].size = in_file.tellg(); 
             
            //some method to find file extension ~ magic number
            if(in_file.is_open()) {
                in_file.seekg(0, ios::beg);
                in_file.read((char*)new_file[total_no_of_files].type, sizeof(new_file[total_no_of_files].type));
            }

            //call create file function the file values;
            fobj.create_file(&fobj, new_file[total_no_of_files].size, &stream_obj, 0);

            //since we have created a new file
            total_no_of_files ++;
        }

        bool search_file(DirectoryTree *node, string filename) {
            for(int i=0; i < node->total_no_of_files; i++) {
                if(node -> new_file[i].filename == filename){
                    //printfile Details

                    //compute timestamp
                    time_t t;
                    t = std::chrono::system_clock::to_time_t (new_file[i].timestamp);

                    cout << "This file exists:\n" << new_file[i].filename << new_file[i].size << new_file[i].type << ctime(&t);
                    return true;
                }
            }
            return false;
        }
 
     //pass the filename as the param
<<<<<<< HEAD
        void open_file(DirectoryTree *node, string filename) {
            //open file in read write mode on some editor
            //Implement after implementing file watcher
         system("gedit filename");
=======
        void open_file(DirectoryTree *node,string filename) {
            //open file in read write mode on some editor
            //Implement after implementing file watcher
         system(("gedit "+ filename).c_str());
>>>>>>> 140ad56ed4a451607ff4d15d6d3ebabddc91c80c
        }

        //Create root/ navigate to root
        void root_directory() {

        }

        //meant to check if directory belongs to directory tree and return node pointer
        DirectoryTree *DirExists(string dirName) {
        //bfs or dfs search algorithm

        }
};

/*_________________________________________________________________________BST for VERSION CONTROL___________________________________________________________________*/

/*Left to Implement:
1.Search File
2.Modify File
*/
class File {
    public:
        //file features can be stored as private objects for now
        //removed hash value for simplicity sake, since our BST sorting is based on file size
        string versionName;
        fstream *stream_obj;
        int size;
        int version;
        //Implement version control - with BST
        File *lchild;
        File *rchild;
        File *parent;
        //technically creates nodes to the file BST's
        //location is a node initialized in the directory class and sent here to be inserted into the BST
        void create_version(string fname, File *node, int version, int size) {
            //create a version node
            File *new_version;
    
            string buffer;
            fstream org, output;
            output.open(fname, ios::out);
            org.open(node->versionName, ios::in);
            if(fname.is_open()) {
                while(getline(node->versionName, buffer)) {
                    output << buffer << '\n';
                }
            }
            org.close(); output.close();

            //update version and version name
            new_version -> version = version ++;
            new_version -> versionName = fname;

            //inserting version into the BST
            new_version -> parent = node;
            if(new_version -> size < node -> size)
                node->lchild = new_version;
            else
                node->rchild = new_version;

        }
        File *create_file(File *location, int size, fstream *stream_obj, int version) {
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


int main() {
    //Call the root_directory function to create a root directory and navigate into it if already exists

    //then set all default variables
    DirectoryFunctions obj;
    obj.root_directory();
    DirectoryTree *current_dir = obj.DirExists("root");
    bool indir = true;
    string commit_file = "";


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
            else if(command[0] == '/') {
                //user is trying to navigate to a directory, so update path variable
                string dirName = command.substr(1, command.size());
                DirectoryTree *dir = obj.DirExists(dirName);
                if(dir == NULL) 
                    cout << "The Directory you are trying to enter into does not exist";
                else {
                    path = path + '/' + dirName;
                    indir = true;
                    current_dir = dir;
                }
            }
            else if(command == "cr dir") {
                //create a new Directory
                string tempName;
                cout << "What will you name this directory?\n";
                cin >> tempName;
                if(obj.DirExists(tempName) == NULL) 
                    cout << "Directory Name already Exists :/";
                else {
                    DirectoryTree *newDir;
                    newDir -> DirName = tempName;
                    obj.insert_dir(newDir);
                }
            }
            else if(command == "cr file") {
                //create file
                //check if in directory
                if(indir) 
                    obj.createFilefromDir(current_dir);
                else
                    cout << "You must first Navigate into a directory\n";
            }
            else if(command == "del file") {
                //Open file
            }
            else if(command == "del dir") {

            }
            else if(command == "commit file"){
              cout<<"\n Enter the filename to be commited";
              cin>>commit_file;
              
             
            else if(command == "exit")
                cout << "\nExiting application\n";
            else 
                cout << "Invalid Command. To check menu type 'ls -m'.";
    }
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
