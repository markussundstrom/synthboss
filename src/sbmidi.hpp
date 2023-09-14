#pragma once
#include <iostream>
#include <jack/jack.h>
#include <jack/midiport.h>
#include <jack/ringbuffer.h>

class SbMidi {
    public:
        SbMidi();
        static int ProcessWrapper(jack_nframes_t nframes, void* arg);
        static int SetupTxBufferWrapper(jack_nframes_t buffersize, void* arg);

    private:
        int process(jack_nframes_t nframes, void* arg);
        int setupTxBuffer(jack_nframes_t buffersize, void* arg);
        jack_client_t* client;
        jack_port_t* outputPort;
        jack_ringbuffer_t* ringbuffer;
        int maxTx;
};
