#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sndfile.h>
#include <fftw3.h>
#include <math.h>
#include "load_files.h"
#include "RMSE.h"

double calculateRMSE(const char *filename) {
    SNDFILE *sndfile;
    SF_INFO sfinfo;
    float *audioData;
    sf_count_t frames;

    printf("Processing file: %s\n", filename);

    // Open the audio file
    if ((sndfile = sf_open(filename, SFM_READ, &sfinfo)) == NULL) {
        printf("Error opening file: %s\n", sf_strerror(NULL));
        return 0.0;
    }
    printf("audio file opened\n");

    if (sfinfo.format == 0) {
        printf("Unsupported audio format: %s\n", filename);
        sf_close(sndfile);
        return 0.0;
    }

    // Allocate memory for audio data
    size_t buffer_size = sfinfo.frames * sfinfo.channels * sizeof(float);
    audioData = (float *)malloc(buffer_size);
    if (audioData == NULL) {
        printf("Memory allocation error\n");
        sf_close(sndfile);
        return 0.0;
    }

    printf("memory allocated\n");

    // Read audio data from the file
    frames = sf_readf_float(sndfile, audioData, sfinfo.frames);

    printf("number of frames: %ld\n", frames);
    if (frames < 0) {
        printf("Error reading audio data: %s\n", sf_strerror(sndfile));
        free(audioData);
        sf_close(sndfile);
        return 0.0;
    }

    // Calculate RMSE
    double sumSquared = 0.0;
    for (sf_count_t i = 0; i < frames; i++) {
        sumSquared += audioData[i] * audioData[i];
    }

    double rmse = sqrt(sumSquared / frames);

    // Clean up
    free(audioData);
    sf_close(sndfile);

    return rmse;
}

double getAverageRMSE(const char **fileList, int fileListSize) {
    double totalRMSE = 0.0;

    for (int i = 0; i < fileListSize; i++) {
        double rmse = calculateRMSE(fileList[i]);
        totalRMSE += rmse;
    }

    return totalRMSE / fileListSize;
}
