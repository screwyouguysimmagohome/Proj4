#ifndef SPECTRAL_ROLLOFF_FILTER_H
#define SPECTRAL_ROLLOFF_FILTER_H

double calculateSpectralRolloff(const char *filename);
double getAverageSpectralRolloff(const char **fileList, int fileListSize);
int compareDouble(const void *a, const void *b);

#endif // SPECTRAL_ROLLOFF_FILTER_H
