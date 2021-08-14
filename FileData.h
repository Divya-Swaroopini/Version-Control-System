#include <ctime>
#include <chrono>
#include <string.h>

#ifndef FILE_DATA  //begin header guard
#define FILE_DATA

 struct FileData{
            //Doubt : Do we store a stream object here?
            int size;
            std::chrono::system_clock::time_point timestamp = std::chrono::system_clock::now();
            char filename[20] ="";
            unsigned char type[4]; //first 512 bytes of a file I guess(magic number)
};

#endif