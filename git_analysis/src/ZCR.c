#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sndfile.h>
#include "ZCR.h"

double calculateZeroCrossRate(const char *filename) {
    SNDFILE *sndfile;
    SF_INFO sfinfo;
    float *audioData;
    sf_count_t frames;

    //printf("Processing file: %s\n", filename);

    // Open the audio file
    if ((sndfile = sf_open(filename, SFM_READ, &sfinfo)) == NULL) {
        printf("Error opening file: %s\n", sf_strerror(NULL));
        return 0.0;
    }
    //printf("audio file opened\n");

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

    //printf("memory allocated\n");

    // Read audio data from the file
    frames = sf_readf_float(sndfile, audioData, sfinfo.frames);

    //printf("number of frames: %ld\n", frames);
    if (frames < 0) {
        printf("Error reading audio data: %s\n", sf_strerror(sndfile));
        free(audioData);
        sf_close(sndfile);
        return 0.0;
    }

    // Calculate zero-crossing rate
    double zeroCrossRate = 0.0;

    for (sf_count_t i = 1; i < frames; i++) {
        if ((audioData[i - 1] >= 0 && audioData[i] < 0) || (audioData[i - 1] < 0 && audioData[i] >= 0)) {
            zeroCrossRate += 1.0;
        }
    }

    // Normalize by the duration of the signal
    zeroCrossRate /= frames;

    // Clean up
    free(audioData);
    sf_close(sndfile);

    return zeroCrossRate;
}

double getAverageZeroCrossRate(const char **fileList, int fileListSize) {
    double totalZeroCrossRate = 0.0;

    for (int i = 0; i < fileListSize; i++) {
        double zeroCrossRate = calculateZeroCrossRate(fileList[i]);
        totalZeroCrossRate += zeroCrossRate;
    }

    return totalZeroCrossRate / fileListSize;
}
