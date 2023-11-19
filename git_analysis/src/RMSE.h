#ifndef RMSE_FILTER_H
#define RMSE_FILTER_H

#ifdef __cplusplus
extern "C" {
#endif

double calculateRMSE(const char *filename);
double getAverageRMSE(const char **fileList, int fileListSize);

#ifdef __cplusplus
}
#endif

#endif // RMSE_FILTER_H
