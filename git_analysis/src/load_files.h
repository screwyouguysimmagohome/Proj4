#ifndef LOAD_FILES_H
#define LOAD_FILES_H

typedef struct {
    char *filename;
    char *path;
} MP3FileInfo;

MP3FileInfo* listMP3Files(const char *directoryPath);

#endif
