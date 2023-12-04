#pragma once
#include <iostream>
#include <vector>
#include <jack/jack.h>
#include <jack/midiport.h>
#include <jack/ringbuffer.h>

class SbMidi {
    public:
        SbMidi();
        void TransmitMessage(std::vector<char> message);
        static int ProcessWrapper(jack_nframes_t nframes, void* arg);
        static int SetupTxBufferWrapper(jack_nframes_t buffersize, void* arg);

    private:
        int process(jack_nframes_t nframes, void* arg);
        int setupTxBuffer(jack_nframes_t buffersize, void* arg);
        jack_client_t* m_client;
        jack_port_t* m_outputPort;
        jack_ringbuffer_t* m_ringbuffer;
        int m_maxTx;
};
