#ifndef SPECTRAL_BANDWIDTH_H
#define SPECTRAL_BANDWIDTH_H

// Function to calculate spectral bandwidth for a given audio file
double calculateSpectralBandwidth(const char *filename);

// Function to calculate the average spectral bandwidth for a list of audio files
double getAverageSpectralBandwidth(const char **fileList, int fileListSize);

#endif
