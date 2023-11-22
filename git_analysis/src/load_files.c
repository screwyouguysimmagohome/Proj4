#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "load_files.h"

MP3FileInfo* listMP3Files(const char *directoryPath) {
    DIR *dir;
    struct dirent *entry;
    dir = opendir(directoryPath);

    if (dir == NULL) {
        perror("opendir");
        return NULL;
    }

    int fileCount = 0;
    MP3FileInfo *fileList = NULL;

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) {
            char *extension = strrchr(entry->d_name, '.');
            if (extension != NULL && strcmp(extension, ".wav") == 0) {
                fileList = (MP3FileInfo *)realloc(fileList, (fileCount + 1) * sizeof(MP3FileInfo));
                fileList[fileCount].filename = strdup(entry->d_name);
                fileList[fileCount].path = strdup(directoryPath);
                fileCount++;
            }
        }
    }

    closedir(dir);

    //for (int i = 0; i < fileCount; i++) {
      //  printf("File %d:\n", i + 1);
        //printf("Path: %s\n", fileList[i].path);
        //printf("Filename: %s\n", fileList[i].filename);
    //}
    
    return fileList;
}
