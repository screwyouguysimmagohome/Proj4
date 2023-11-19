#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <aubio/aubio.h>
#include "tempo.h"


double calculateTempo(const char *filename) {
    printf("IN CALCULATE TEMPO");
    // Open the audio file
    const char *sound_file = filename;
    uint_t hop_size = 512;
    uint_t samplerate = 0;
    uint_t channels = 0;
    uint_t buflen = 0;
    aubio_source_t *source = NULL;
    aubio_tempo_t *tempo = NULL;
    fvec_t* buf = NULL;

    printf("audio file open");
    

    source = new_aubio_source(sound_file, samplerate, hop_size);
    if (source == NULL) {
        fprintf(stderr, "Error opening file %s\n", sound_file);
        return 0.0;
    }


    samplerate = aubio_source_get_samplerate(source);
    if (samplerate == 0) {
        // Default to a reasonable value if the samplerate couldn't be determined
        samplerate = 44100;
    }
  

    printf("source file open");

    samplerate = aubio_source_get_samplerate(source);
    channels = aubio_source_get_channels(source);
    buflen = hop_size * channels;


    // Create tempo detection
    tempo = new_aubio_tempo("default", buflen, hop_size, samplerate);
    if (tempo == NULL) {
        fprintf(stderr, "Error creating tempo detection\n");
        del_aubio_source(source);
        return 0.0;
    }

    printf("tempo detection open");

    // Allocate buffer
    buf = new_fvec(hop_size);
    if (buf == NULL) {
        fprintf(stderr, "Error allocating buffer\n");
        del_aubio_tempo(tempo);
        del_aubio_source(source);
        return 0.0;
    }

    printf("buffer allocated");

    double tempo_value = 0.0;

    // Process audio frames
    while (1) {
    if (!source || !tempo || !buf) {
        fprintf(stderr, "Error: NULL pointer encountered\n");
        break;
    }

    aubio_source_do(source, buf, NULL);
    aubio_tempo_do(tempo, buf, buf);

    if (aubio_source_get_duration(source) == 0.) {
        break;
    }
}
    printf("audio frames processed");

    // Clean up
    del_fvec(buf);
    del_aubio_tempo(tempo);
    del_aubio_source(source);

    printf("all cleaned up");

    return tempo_value;
}

double getAverageTempo(const char **fileList, int fileListSize) {
    double total_tempo = 0.0;
    printf("IN get average tempo");

    for (int i = 0; i < fileListSize; i++) {
        printf("IN forloop get average tempo");

        double tempo = calculateTempo(fileList[i]);
        total_tempo += tempo;
    }

    return total_tempo / fileListSize;
}
