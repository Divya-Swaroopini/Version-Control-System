#include <iostream>
#include <vector>
#include <string>
#include <ctime>

 
using namespace std;

/*________________________________________________________________________Indexed Storage Allocation______________________________________________________________________*/
// File Class
class File {

public:
    //file permissions
    string permissions;
    //File_type - magic number
    string file_type;
    // Name of File
    string filename;
    // Size of file
    size_t size;
    // Partition no. is which part of file is present at a particular block of memory
    int partition;
};


// Block class
class Block {
    //Block is occupied by a file? initially set to false
    bool occupied = false;
  
    // File of the block
    File file;

    
    public:
        //assigns current block to file
         void set_file(File file) {
            this->file = file;
            occupied = true;
        }
    
        //return the filename of this block
        string get_fname() {
            return file.filename;
        }

        //return partition number
        int get_partition_num() {
            return file.partition;
        }

        //is block empty?
        bool is_empty() {
            return !occupied;
        }

        //free the block space
        void free_block() {
            occupied = false;
        }
};

//How many blocks of the memory are empty?
int empty_block_count(vector<Block> memory) {
    int sum = 0;
    vector<Block>::iterator slot;

    for(slot = memory.begin(); slot!=memory.end(); slot++)
        sum += (*slot).is_empty();
    return sum;
} 

//generate random indexes for files
int generate_index(vector<Block> memory)
{
    int index = -1;
  
    // Check if memory is full
    if (!empty_block_count(memory) == 0) {
  
        // Here it will generate index until the memory block at generated index is found to be empty
        do {
            index = rand() % memory.size();
            index = abs(index);
  
        } while (!memory.at(index).is_empty());
    }
    return index;
}

bool if_exists(vector<Block> memory, string name) {
    
    vector<Block>::iterator slot;
  
    for (slot = memory.begin(); slot != memory.end(); slot++) {
        if (!(*slot).is_empty() && (*slot).get_fname() == name) {
            return true;
        }
    }
  
    return false;
}

//set file in the memory
//push index for each partition
//push file index to main index vector
void set_indexed_memory(vector<Block>* memory, vector<vector<int> >* index_page, File file) {
    int index;

    vector<int> thisindex;

    if(if_exists((*memory), file.filename))
        cout << "File already exists\n";
    else {
        if(empty_block_count(*memory) >= file.size) {
            for(int i=0; i<file.size; i++) {
                //itteration on file size
                //first a random empty index is generated
                index = generate_index(*memory);
                file.partition = i;
                //now file is pushed to memory and index to newpage
                (*memory).at(index).set_file(file);
                thisindex.push_back(index);
            }

            (*index_page).push_back(thisindex);
            cout << "Allocation complete" << endl;
        }
        else
            cout << "No space available";
    }

}


//delete file from memory and it's index
void delete_from_memory(vector<Block>* memory, vector<vector<int> >* index_page, string file) {
    vector<int>::iterator slot;
    vector<vector<int> >::iterator it;
    int index, i = 0;

    // Check if file exists
    if (if_exists((*memory), file)) {
        // Iterate main index
        for (it = (*index_page).begin();
             it != (*index_page).end(); it++) {
            // Check for sub-index at
            // start location
            slot = (*it).begin();
  
            // Check if it equals filename
            if ((*memory)
                    .at(*slot)
                    .get_fname()
                == file) {
                // Set for index and break
                index = i;
                break;
            }
            i++;
        }

        // Set the memory flag as empty
        for(slot = (*index_page).at(index).begin(); slot != (*index_page).at(index).end(); slot++)
            (*memory).at(*slot).free_block();
  
        // Erase file index from main index page
        (*index_page).erase((*index_page).begin() + index);
        cout << "File " << file << " has been deleted" << endl;
    }
    else {
        cout << "File does not exist" << endl;
    }
}
/* _________________________________________________________________________DIRECTORY TREE____________________________________________________________________________*/

class DirectoryTree {
    private:
        vector<int> files;
    public:
        DirectoryTree * directory;
    
        DirectoryTree * create_dir() {
            //code to create node

            //if it is the Root Directory
            
            //else
        }
        DirectoryTree * delete_dir() {
            //code to delete directory
        }

        DirectoryTree * addFile() {
            //code to add file indexes to file vectors
            //call a create file function to allocate storage space for file
            //call a create BST function to start storing file versions
        }
};

/*_________________________________________________________________________BST for VERSION CONTROL___________________________________________________________________*/

