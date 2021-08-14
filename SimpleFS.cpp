#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <chrono>
#include <stdlib.h>
#include <fstream>
#include <bits/stdc++.h>
#include "FileData.h"
 
using namespace std;

class DirectoryStorage {
    public:
        //Multiple instances of the structure from header file FileData.h are created to store file details.
        //each instance can be accessed via a vector and an index is used to trace the files
        
        //each time a directory is created, the totoal no of files needs to be set at 0
        int total_no_of_files = 0;
        vector<FileData> files;

        //name of directory.
        string DirName;
        //create pointers to child and parent directory
        DirectoryStorage *dirParent;
        vector<DirectoryStorage> dirChild;
};

class DirectoryFunctions : DirectoryStorage {

    public:
        DirectoryStorage *insert_dir(DirectoryStorage *node, string dirname) {
            if(node == NULL) {
                //insert a new node after node
                //initialization
                node-> DirName = dirname;
                vector<DirectoryStorage> dirChild;
                vector<FileData> files;

                return node;
            }
            else {
                DirectoryStorage *newNode = new DirectoryStorage;
                newNode = NULL;
                node -> dirChild.push_back(*newNode);
                newNode = insert_dir(newNode, dirname);
                return newNode;
            }
        }

        DirectoryStorage *DirExists(DirectoryStorage *current_dir, string dirname) {
            //Visit all the Child Directories of this directory.
            for(int i=0; i<current_dir -> dirChild.size(); i++) {
                if(current_dir -> dirChild[i].DirName == dirname)
                    return &(current_dir -> dirChild[i]);
            }
            return NULL;
        }

        void delete_dir(DirectoryStorage *dir) {
            //access child directories and send them through a file deletion function.
            if(dirChild.size() > 0) {
            for(int i=0; i<dir -> dirChild.size(); i++)
                delete_dir(&(dir -> dirChild[i]));
            }
            //finally send this directory through a deletion function
            delete_contents(dir);
            //Write the code to free up the space previously occupied by this node and it's pointers.
            return;
        }

        void delete_contents(DirectoryStorage *dir) {
            //This function will delete all the file Data and Directory Data of this Directory node.

        }

        FileData *search_file(DirectoryStorage *current_dir, char filename[]) {
            for(int i=0; i<current_dir -> total_no_of_files; i++) {
                if(current_dir -> files[i].filename == filename)
                    return &(current_dir -> files[i]);
            }
            return NULL;
        }

        void create_file(DirectoryStorage *current_dir) {
            fstream stream_obj;

            char fname[20];
            cout << "Enter the filename with a proper file extension.\n";
            cin >> fname;
            if(search_file(current_dir, fname) != NULL) {
                cout << "File Already Exists\n";
                return;
            }

            //To open up the file to a text editor
            stream_obj.open(fname, ios::out);
            //rest of this code should execute after the user hits ctrl + save

            //To retrieve file size
            ifstream in_file(fname, ios::binary);
            in_file.seekg(0, ios::end);
            int size = in_file.tellg(); 

            //Save the file details
            files.push_back(FileData());

            //find all the file values and assign them to FileData Structure. 
            //set filename
            std::strcpy(files[total_no_of_files].filename, fname);

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

    //create objects to call File and DirectoryFunctions classes
    DirectoryFunctions dobj;

    //create root directory. Function return a new node.
    DirectoryStorage *RootDirectory = dobj.insert_dir(NULL, "root");
    //Now we have a total of 1 directory in the system.
    int total_no_of_dir = 1;
    //Set current directory to root
    DirectoryStorage *current_dir = RootDirectory;

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
        cout<<path<<"$";
        cin >> command;
            //each if-else statement will be a function call to perform a certain task, 
            //depending on command recieved by user until the user chooses to exit. 
            //When user exits, terminal app should automatically terminate.
            if(command == "help") {
                //List the menu of commands
                cout<<"crt: Create\n Options:\t -f -> create a file\t -d -> create a directory"<<endl
                    <<"des: Destroy\n Options:\t -f -> destroy a file\t -d -> destroy a directory"<<endl
                    <<"mod: Modify\n Options:\t -f -> modify a file\t"<<endl
                    <<"fnd: Find File"<<endl
                    <<"nav: Navigate to Directory"<<endl;
            }
            else if(command == "cr -d") {
                //create a new Directory
                string tempName;
                cout << "What will you name this directory?\n";
                cin >> tempName;
                if(dobj.DirExists(RootDirectory, tempName) != NULL) 
                    cout << "Directory Name already Exists :/";
                else {
                    DirectoryStorage *newDir = dobj.insert_dir(current_dir, tempName);
                    newDir -> dirParent = current_dir;
                    cout << "Directory has been created, navigate to it.\n";
                    ++total_no_of_dir;
                }
            }
            else if(command == "del -d") {
                //delete directory
                string dirName;
                cout << "Enter directory to be deleted\n Note: To delete a directory, you must navigate to it's parent directory before deletion.\n";
                cin >> dirName;
                DirectoryStorage *dir = dobj.DirExists(current_dir, dirName);
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
                    cout << "First create a Directory. Only root exists";
                else if(current_dir->DirName == dirName)
                    cout << "You are presently in the Directory : " << dirName;
                
                else {
                    DirectoryStorage *dir = dobj.DirExists(current_dir, dirName);
                    if(dir == NULL) 
                        cout << "The Directory you are trying to enter into does not exist";
                    else {
                        //user is trying to navigate to a directory, so update path variable
                        cout << "Entering directory : " << dirName;
                        path = path + '/' + dirName;
                        current_dir = dir;
                    }
                }
            }
            else if(command == "crt -f") {
                //create a file in present directory
                dobj.create_file(current_dir);
            }
            else if(command == "fnd") {
                char fileName[20];
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
            else {
                cout << "This command is not valid. Type 'help' to view command list\n";
            }
    }
}





/*REMEMEBER TO FREE ALL THE POINTERS AT THE END OF EXECUTION AND DELETE THE ROOT NODE AS WELL.*/