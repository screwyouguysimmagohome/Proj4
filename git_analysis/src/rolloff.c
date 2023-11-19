#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sndfile.h>
#include <fftw3.h>
#include <math.h>
#include "load_files.h"
#include "rolloff.h"

#define ENERGY_PERCENTAGE_THRESHOLD 0.85

int compareDouble(const void *a, const void *b) {
    return (*(double *)b > *(double *)a) - (*(double *)b < *(double *)a);
}

double calculateSpectralRolloff(const char *filename) {
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

    // Perform FFT
    printf("Before FFT\n");
    fftw_complex *in, *out;
    fftw_plan plan;
    in = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * frames);
    out = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * frames);

    if (in == NULL || out == NULL) {
        printf("Memory allocation error\n");
        free(audioData);
        sf_close(sndfile);
        fftw_free(in);
        fftw_free(out);
        return 0.0;
    }
    printf("Allocated memory for FFT\n");

    for (int i = 0; i < frames; i++) {
        in[i][0] = audioData[i];
        in[i][1] = 0.0;
    }

    plan = fftw_plan_dft_1d(frames, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
    if (plan == NULL) {
        printf("FFTW plan creation error\n");
        free(audioData);
        sf_close(sndfile);
        fftw_free(in);
        fftw_free(out);
        return 0.0;
    }
    printf("FFTW plan created\n");

    fftw_execute(plan);

    printf("FFT executed\n");

    // Calculate the magnitude spectrum
    double *magnitude = (double *)malloc(sizeof(double) * frames / 2);
    for (int i = 0; i < frames / 2; i++) {
        magnitude[i] = sqrt(out[i][0] * out[i][0] + out[i][1] * out[i][1]);
    }

    // Sort the magnitude spectrum
    qsort(magnitude, frames / 2, sizeof(double), compareDouble);

    // Calculate the energy threshold
    double total_energy = 0.0;
    for (int i = 0; i < frames / 2; i++) {
        total_energy += magnitude[i] * magnitude[i];
    }

    double energy_threshold = ENERGY_PERCENTAGE_THRESHOLD * total_energy;

    // Find the rolloff frequency
    
    double nyquist_frequency = sfinfo.samplerate / 2.0;
    double accumulated_energy = 0.0;
    double rolloff_frequency = 0.0;

    for (int i = 0; i < frames / 2; i++) {
        accumulated_energy += magnitude[i] * magnitude[i];
        if (accumulated_energy >= energy_threshold) {
            // Convert the index to a frequency value
            rolloff_frequency = i * nyquist_frequency / frames;
            break;
        }
    }

    // Clean up
    free(audioData);
    free(magnitude);
    sf_close(sndfile);
    fftw_destroy_plan(plan);
    fftw_free(in);
    fftw_free(out);

    return rolloff_frequency;
}

double getAverageSpectralRolloff(const char **fileList, int fileListSize) {
    double total_rolloff = 0.0;

    for (int i = 0; i < fileListSize; i++) {
        double rolloff = calculateSpectralRolloff(fileList[i]);
        total_rolloff += rolloff;
    }

    return total_rolloff / fileListSize;
}
