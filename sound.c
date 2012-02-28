#include "sound.h"

VALUE rb_mSound;
VALUE rb_eSoundDomainError;

#define Need_Float(x) do {if (!RB_TYPE_P(x, T_FLOAT)) {(x) = rb_to_float(x);}} while(0)

#define domain_error(msg) \
    rb_raise(rb_eSoundDomainError, "An error is occurred - " #msg);

#define NUM_SECONDS   (5)
#define SAMPLE_RATE   (44100)
#define FRAMES_PER_BUFFER  (64)

#ifndef M_PI
#define M_PI  (3.14159265)
#endif

static int paWavePlayCallback(const void *inputBuffer,
			    void *outputBuffer,
			    unsigned long framesPerBuffer,
			    const PaStreamCallbackTimeInfo* timeInfo,
			    PaStreamCallbackFlags statusFlags,
			    void *userData)
{
    waveData *data = (waveData*)userData;
    float *out = (float*)outputBuffer;
    unsigned long i;
    (void)inputBuffer;		/* not used */

    for (i = 0; i < framesPerBuffer; i++) {
	*out++ = data->amp[data->left_phase];
	*out++ = data->amp[data->right_phase];
	data->left_phase += (double)TABLE_SIZE*(double)data->freq / (double)SAMPLE_RATE;
	data->right_phase += (double)TABLE_SIZE*(double)data->freq / (double)SAMPLE_RATE;
	if (data->left_phase >= TABLE_SIZE)
	    data->left_phase -= TABLE_SIZE;
	if (data->right_phase >= TABLE_SIZE)
	    data->right_phase -= TABLE_SIZE;
    }

    return paContinue;
}

static void StreamFinished(void* userData)
{
    printf("Stream Completed\n");
}

static int pa_init(rsound *rsnd)
{
    char *s;
    int err;

    if (rsnd->running != 0) {
	return -1;
    } else {
	rsnd->running = 1;
	err = (int)Pa_Initialize();

	if ((s = (char*)Pa_GetVersionText()) != NULL)
	    printf("%s\n", s);

	return 0;
    }
}

static int pa_setsp(rsound *rsnd, PaStreamParameters *sp)
{
    sp->device = Pa_GetDefaultOutputDevice();
    if (sp->device == paNoDevice) {
	return -1;
    }
    sp->channelCount = 2; /* stereo output */
    sp->sampleFormat = paFloat32; /* 32bit floating point output */
    sp->suggestedLatency = Pa_GetDeviceInfo(sp->device)->defaultLowOutputLatency;

    return 0;
}

static int set_sin_data(waveData *dp)
{
    int i;

    for (i = 0; i < TABLE_SIZE; i++) {
	dp->amp[i] = (float)sin(((double)i/TABLE_SIZE) * M_PI * 2.0);
    }
    dp->left_phase = dp->right_phase = 0;

    return 0;
}

static int set_tri_data(waveData *dp)
{
    int i;

    for (i = 0; i < TABLE_SIZE; i++) {
	if (i < TABLE_SIZE/4) {
	    dp->amp[i] = (float)(4.0 * ((double)i/TABLE_SIZE));
	} else if (i < TABLE_SIZE*3/4) {
	    dp->amp[i] = (float)(-4.0 * ((double)i/TABLE_SIZE) + 2.0);
	} else {
	    dp->amp[i] = (float)(4.0 * ((double)i/TABLE_SIZE) - 3.0);
	}
    }
    dp->left_phase = dp->right_phase = 0;

    return 0;
}

static int set_saw_data(waveData *dp)
{
    int i;

    for (i = 0; i < TABLE_SIZE; i++) {
	dp->amp[i] = (float)(-2.0 * ((double)i/TABLE_SIZE) + 1.0);
    }
    dp->left_phase = dp->right_phase = 0;

    return 0;
}
static int set_pulse_data(waveData *dp)
{
    int i;

    for (i = 0; i < TABLE_SIZE; i++) {
	if (i < TABLE_SIZE/2) {
	    dp->amp[i] = 1.0;
	} else {
	    dp->amp[i] = -1.0;
	}
    }
    dp->left_phase = dp->right_phase = 0;

    return 0;
}


static int engine_start(PaStreamParameters *sp)
{
    rsound *rsnd;
    
    rsnd = (rsound *)malloc(sizeof(rsound));
    rsnd->running = 0;

    if (pa_init(rsnd) != 0) {
	Pa_Terminate();
	domain_error("pa_init");
    }

    if (pa_setsp(rsnd, sp) != 0) {
	Pa_Terminate();
	domain_error("pa_setsp: No default output device.");
    }

    return 0;
}

static int engine_end()
{
    Pa_Terminate();
    printf("PortAudio is terminated.\n");

    return 0;
}

static int engine_play(PaStreamParameters *sp, waveData *dp)
{
    PaStream *stream;
    PaError err;

    err = Pa_OpenStream(&stream, NULL, sp, SAMPLE_RATE, FRAMES_PER_BUFFER,
			paClipOff, paWavePlayCallback, dp);
    if (err != paNoError)
	pa_error_quit(err);

    err = Pa_SetStreamFinishedCallback(stream, &StreamFinished);
    if (err != paNoError)
	pa_error_quit(err);

    err = Pa_StartStream(stream);
    if (err != paNoError)
	pa_error_quit(err);

    Pa_Sleep(NUM_SECONDS*1000);

    err = Pa_StopStream(stream);
    if (err != paNoError)
	pa_error_quit(err);

    err = Pa_CloseStream(stream);
    if (err != paNoError)
	pa_error_quit(err);

    return 0;
}

static VALUE sound_sin(VALUE obj)
{
    PaStreamParameters *sp;
    int i;
    waveData *dp;

    dp = (waveData *)malloc(sizeof(waveData));
    set_sin_data(dp);
    dp->freq = 440.0;

    sp = (PaStreamParameters *)malloc(sizeof(PaStreamParameters));
    sp->hostApiSpecificStreamInfo = NULL;

    engine_start(sp);
    engine_play(sp, dp);
    engine_end();
}

static VALUE sound_osc(VALUE obj, VALUE freq)
{
    PaStreamParameters *sp;
    int i;
    waveData *dp;

    Need_Float(freq);

    dp = (waveData *)malloc(sizeof(waveData));
    set_sin_data(dp);
    dp->freq = NUM2DBL(freq);

    sp = (PaStreamParameters *)malloc(sizeof(PaStreamParameters));
    sp->hostApiSpecificStreamInfo = NULL;

    engine_start(sp);
    engine_play(sp, dp);
    engine_end();
}

static VALUE sound_tri(VALUE obj, VALUE freq)
{
    PaStreamParameters *sp;
    int i;
    waveData *dp;

    Need_Float(freq);

    dp = (waveData *)malloc(sizeof(waveData));
    set_tri_data(dp);
    dp->freq = NUM2DBL(freq);

    sp = (PaStreamParameters *)malloc(sizeof(PaStreamParameters));
    sp->hostApiSpecificStreamInfo = NULL;

    engine_start(sp);
    engine_play(sp, dp);
    engine_end();
}

static VALUE sound_saw(VALUE obj, VALUE freq)
{
    PaStreamParameters *sp;
    int i;
    waveData *dp;

    Need_Float(freq);

    dp = (waveData *)malloc(sizeof(waveData));
    set_saw_data(dp);
    dp->freq = NUM2DBL(freq);

    sp = (PaStreamParameters *)malloc(sizeof(PaStreamParameters));
    sp->hostApiSpecificStreamInfo = NULL;

    engine_start(sp);
    engine_play(sp, dp);
    engine_end();
}

static VALUE sound_pulse(VALUE obj, VALUE freq)
{
    PaStreamParameters *sp;
    int i;
    waveData *dp;

    Need_Float(freq);

    dp = (waveData *)malloc(sizeof(waveData));
    set_pulse_data(dp);
    dp->freq = NUM2DBL(freq);

    sp = (PaStreamParameters *)malloc(sizeof(PaStreamParameters));
    sp->hostApiSpecificStreamInfo = NULL;

    engine_start(sp);
    engine_play(sp, dp);
    engine_end();
}

void Init_sound()
{
    rb_mSound = rb_define_module("Sound");
    rb_eSoundDomainError = rb_define_class_under(rb_mSound, "DomainError", rb_eStandardError);

    rb_define_module_function(rb_mSound, "sin", sound_sin, 0);
    rb_define_module_function(rb_mSound, "osc", sound_osc, 1);
    rb_define_module_function(rb_mSound, "tri", sound_tri, 1);
    rb_define_module_function(rb_mSound, "saw", sound_saw, 1);
    rb_define_module_function(rb_mSound, "pulse", sound_pulse, 1);
}

void pa_error_quit(PaError err)
{
    Pa_Terminate();
    fprintf(stderr, "An error occurred while using the portaudio stream\n");
    fprintf(stderr, "Error number: %d\n", err);
    fprintf(stderr, "Error message: %s\n", Pa_GetErrorText(err));
}
