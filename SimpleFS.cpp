#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <chrono>
#include <stdlib.h>
#include <fstream>
#include <bits/stdc++.h>
 
using namespace std;

//store file data
 struct FileData {
            //Doubt : Do we store a stream object here?
            int size;
            std::chrono::system_clock::time_point timestamp = std::chrono::system_clock::now();
            string filename;
            unsigned char type[4]; //first 512 bytes of a file I guess(magic number)
};

//store diretory data
struct DirectoryStorage {
        //A* a = (A*)malloc(sizeof(A));
    
        //Multiple instances of the structure from header file FileData.h are created to store file details.
        //each instance can be accessed via a vector and an index is used to trace the files
        
        //each time a directory is created, the totoal no of files needs to be set at 0
        int total_no_of_files = 0;
        vector<FileData> files;

        //name of directory.
        string DirName;
        //create pointers to child and parent directory
        struct DirectoryStorage *dirParent;
        vector<DirectoryStorage*> dirChild;
};

class Directory : DirectoryStorage {

    private:
        DirectoryStorage *node;

    public:
    
        Directory() {
            node = NULL;
        }
        ~Directory() {
            clean_up();
        }

        void clean_up() {
            //function to clear out memory 
        }

        DirectoryStorage *insert_dir(DirectoryStorage *node, string dirname) {

            //struct DirectoryStorage *newnode = new DirectoryStorage();
            struct DirectoryStorage *newnode = (DirectoryStorage*)malloc(sizeof(DirectoryStorage));

            if(node == NULL) {
                //insert a new node after node                
                newnode -> DirName = dirname;
                node = newnode;
                free(newnode);
                newnode = NULL;
                return node;
            }
            else {  
                //first comes here
                newnode = NULL;
                node -> dirChild.push_back(newnode);
                newnode = insert_dir(newnode, dirname);
                return newnode;
            }
        }

        DirectoryStorage *DirExists(DirectoryStorage *current_dir, string dirname) {
            //Visit all the Child Directories of this directory.
            for(int i=0; i<current_dir -> dirChild.size(); i++) {
                if(current_dir -> dirChild[i] -> DirName == dirname) {
                    return current_dir -> dirChild[i];
                    cout << current_dir -> dirChild[i] -> DirName;
                }
            }
            return NULL;
        }

        void delete_dir(DirectoryStorage *dir) {
            //access child directories and send them through a file deletion function.
            if(dirChild.size() > 0) {
            for(int i=0; i<dir -> dirChild.size(); i++)
                delete_dir(dir -> dirChild[i]);
            }
            //finally send this directory through a deletion function
            delete_contents(dir);
            //Write the code to free up the space previously occupied by this node and it's pointers.
            return;
        }

        void delete_contents(DirectoryStorage *dir) {
            //This function will delete all the file Data and Directory Data of this Directory node.
            dir->files.clear();
            dir -> total_no_of_files = 0;
            free(dirParent);
            dir->dirChild.clear();
        }

        void delete_file(string filename, DirectoryStorage *dir) {
            //delete all refrences to this file
            FileData *new_pointer;
                for(int i=0; i< dir->files.size(); i++) {
                    new_pointer = &(dir-> files[i]);
                    if(new_pointer -> filename == filename)
                        dir->files.erase(dir->files.begin()+i);
            }
            free(new_pointer);
            //delete file from system
            const char *file = filename.c_str();
            if (remove(file) == 0)
                printf("Deleted successfully");
            else
                printf("Unable to delete the file");
        }

        FileData *search_file(DirectoryStorage *current_dir, string filename) {
            cout << "Searching for file : "<< filename << " in directory "<<current_dir -> DirName <<endl;
            cout << total_no_of_files;
            for(int i=0; i<current_dir -> total_no_of_files; i++) {
                if(current_dir -> files[i].filename == filename) {
                    cout<<"file found!\n";
                    return &(current_dir -> files[i]);
                }
            }
            return NULL;
        }

        void create_file(DirectoryStorage *current_dir) {
            fstream stream_obj;

            string fname;
            cout << "Enter the filename with a proper file extension.\n";
            cin >> fname;
            if(search_file(current_dir, fname) != NULL) {
                cout << "File Already Exists\n";
                return;
            }

            //To open up the file to a text editor
            stream_obj.open(fname, ios::out);
            //rest of this code should execute after the user hits ctrl + save

            //To retrieve file-size
            ifstream in_file(fname, ios::binary);
            in_file.seekg(0, ios::end);
            int size = in_file.tellg(); 

            //Save the file details
            files.push_back(FileData());

            //find all the file values and assign them to FileData Structure. 
            //set filename
            files[total_no_of_files].filename = fname;

            //set file size
            files[total_no_of_files].size = size;
             
            //some method to find file extension ~ magic number
            if(in_file.is_open()) {
                in_file.seekg(0, ios::beg);
                in_file.read((char*)files[total_no_of_files].type, sizeof(files[total_no_of_files].type));
            }

            //since we have created a new file
            total_no_of_files ++;
        }

};

int main() {
    //Call the root_directory function to create a root directory and navigate into it if already exists

    //create object of Directory class
    Directory dobj;
    char root[20] = {'r', 'o', 'o', 't'};
    //create root directory. Function return a new node.
    struct DirectoryStorage *RootDirectory = dobj.insert_dir(NULL, root);
    //Now we have a total of 1 directory in the system.
    int total_no_of_dir = 1;
    //Set current directory to root
    struct DirectoryStorage *current_dir = RootDirectory;

    //To be sure that root was created:
    if(current_dir == NULL) {
        cout << "Failed to create root directory\n Ending Application";
        return 0;
    }


    //When terminal app runs, it should automatically begin from this code and display below message.
    string path = "/root";
    cout << "\n**************************Application has Started**************************\n";
    string command="";
    while(command!="exit") {
        cout<<"\n"<<path<<"$";
        cin >> command;
            //each if-else statement will be a function call to perform a certain task, 
            //depending on command recieved by user until the user chooses to exit. 
            //When user exits, terminal app should automatically terminate.
            if(command == "help") {
                //List the menu of commands
                cout<<"crt: Create\n Options:\t $f -> create a file\t $d -> create a directory"<<endl
                    <<"des: Destroy\n Options:\t $f -> destroy a file\t $d -> destroy a directory"<<endl
                    <<"mod: Modify\n Options:\t $f -> modify a file\t"<<endl
                    <<"fnd: Find File"<<endl
                    <<"nav: Navigate to Directory"<<endl
                    <<"ls:  List all files in this directory"<<endl
                    <<"exit: Exit Application"<<endl;
            }
            else if(command == "crt$d") {
                //create a new Directory
                char tempName[20]="";
                cout << "What will you name this directory?\n";
                cin >> tempName;
                if(dobj.DirExists(RootDirectory, tempName) != NULL) 
                    cout << "Directory Name already Exists :/";
                else {
                    struct DirectoryStorage *newDir = dobj.insert_dir(current_dir, tempName);
                    newDir -> dirParent = current_dir;
                    cout << "Directory has been created, navigate to it.\n";
                    ++total_no_of_dir;
                }
            }
            else if(command == "des$d") {
                //delete directory
                string dirName;
                cout << "Enter directory to be deleted\n Note: To delete a directory, you must navigate to it's parent directory before deletion.\n";
                cin >> dirName;
                struct DirectoryStorage *dir = dobj.DirExists(current_dir, dirName);
                if(dir != NULL) 
                    dobj.delete_dir(dir);
                else 
                    cout << "That directory does not exist";
            }
            else if(command == "nav") {
                cout << "Enter the directory name, to navigate there\n";
                string dirName;
                cin >> dirName;

                if(total_no_of_dir == 1)
                    cout << "First create a Directory. Only root exists\n";
                else if(current_dir->DirName == dirName)
                    cout << "You are presently in the Directory : " << dirName;
                
                else {
                    //struct DirectoryStorage *dir = dobj.DirExists(current_dir, dirName);
                    //if(dir == NULL) 
                        //cout << "The Directory you are trying to enter into does not exist";
                    //else {
                        //user is trying to navigate to a directory, so update path variable
                        cout << "Entering directory : " << dirName;
                        path = path + '/' + dirName;
                        //current_dir = dir;
                    //}
                }
            }
            else if(command == "crt$f") {
                //create a file in present directory
                dobj.create_file(current_dir);
            }
            else if(command == "des$f") {
                //Delete the file from the directory storage and erase file from users system.
                string fileName;
                cout << "File name?\n";
                cin >> fileName;
                FileData *fileLocation = dobj.search_file(current_dir, fileName);
                if(fileLocation == NULL)
                    cout << "Can't find it\n";
                else {
                    dobj.delete_file(fileName, current_dir);
                    free(fileLocation);
                }

            }
            else if(command == "fnd") {
                string fileName;
                cout << "File name?\n";
                cin >> fileName;
                FileData *fileLocation = dobj.search_file(current_dir, fileName);
                if(fileLocation == NULL)
                    cout << "Can't find it\n";
                else {
                    //print file details.
                    cout<< "File Details:";
                }
            }
            else if(command == "exit") {
                cout << "Bye!" << endl << "**************Application Ending******************"<< endl;
                //call destructor to clean up memory space.
                dobj.~Directory();
            }
            else {
                cout << "This command is not valid. Type 'help' to view command list\n";
            }
    }
}





/*REMEMEBER TO FREE ALL THE POINTERS AT THE END OF EXECUTION AND DELETE THE ROOT NODE AS WELL.
Follow the link to make changes to current class structure. Rememeber to design a deconstructor too.
http://www.hamedkiani.com/coding-interview/a-class-implementation-of-binary-search-tree-in-c
*/

/*CURRENT ERRORS:
COMPARING TWO FILE NAMES OR TWO DIRECTORY NAMES IS CAUSING THE COMPILER TO HANG OR STOP EXECUTION.
REST OF THE CODE SEEMS TO WORK FINE
*/