#include <ruby.h>
#include <math.h>
#include <portaudio.h>

typedef struct
{
    int running;
} rsound;

#define TABLE_SIZE   (512)
typedef struct
{
    float amp[TABLE_SIZE];
    float freq;
    int left_phase;
    int right_phase;
    float volume;
} waveData;

static int pa_init(rsound *rsnd);
static int pa_setsp(rsound *rsnd, PaStreamParameters *sp);
static int set_sin_data(waveData *dp);
static int set_tri_data(waveData *dp);
static int set_saw_data(waveData *dp);
static int set_pulse_data(waveData *dp);
static int engine_start(PaStreamParameters *sp);
static int engine_end();
static int engine_play(PaStreamParameters *sp, waveData *dp);
void Init_roscil();
void pa_error_quit(PaError err);
