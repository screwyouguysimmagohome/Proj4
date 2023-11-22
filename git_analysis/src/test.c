#include <stdio.h>
#include "load_files.h"  
#include "spectral_bandwidth.h"
#include "RMSE.h"
#include "rolloff.h"
#include "tempo.h"
#include "ZCR.h"
#include <stdlib.h>
#include <string.h>

int main() {
    // Directory path
    const char *directoryPath = "/home/jonasbo/proj4/classical";

// List MP3 files in the dir
    MP3FileInfo *fileList = listMP3Files(directoryPath);

    if (fileList == NULL) {
        printf("Error listing MP3 files from the directory.\n");
        return 1;
    }

    int fileCount = 0;
    while (fileList[fileCount].filename != NULL) {
        fileCount++;
    }

     // array for full file paths
    char **filePaths = (char **)malloc(fileCount * sizeof(char *));
    for (int i = 0; i < fileCount; i++) {
        // Allocate memory for the full file path
        filePaths[i] = (char *)malloc(strlen(fileList[i].path) + strlen(fileList[i].filename) + 2);
        // Combine the directory path and filename
        sprintf(filePaths[i], "%s/%s", fileList[i].path, fileList[i].filename);
    }
 
    //printf("Number of files: %d\n", fileCount);

    
    //printf("File paths:\n");

    //for (int i = 0; i < fileCount; i++) {
        //printf("%s\n", filePaths[i]);}



    
    double averageBandwidth = getAverageSpectralBandwidth((const char **)filePaths, fileCount);
    printf("Average Spectral Bandwidth: %f\n", averageBandwidth);

    double averageRMSE = getAverageRMSE((const char **)filePaths, fileCount);
    printf("Average Root Mean Square Energy: %f\n", averageRMSE);

    double averageSpectralRolloff = getAverageSpectralRolloff((const char **)filePaths, fileCount);
    printf("Average Spectral Rolloff: %f\n", averageSpectralRolloff);

    double averageZeroCrossRate = getAverageZeroCrossRate((const char **)filePaths, fileCount);
    printf("Average Zero Cross Rate: %f\n", averageZeroCrossRate);
    //fflush(stdout);
    //printf("about to start average tempo");

    //double averageTempo = getAverageTempo((const char **)filePaths, fileCount);
    //printf("Average Tempo: %f\n", averageTempo);

    // Clean up
     for (int i = 0; i < fileCount; i++) {
        free(fileList[i].filename);
        free(fileList[i].path);
        free(filePaths[i]);
    }
    
    free(fileList);
    free(filePaths);

    return 0;
}