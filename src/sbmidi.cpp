#include "sbmidi.hpp"

SbMidi::SbMidi() {
    jack_options_t options = JackNullOption;
    jack_status_t status;

        
    m_client = jack_client_open("Synthboss", options, &status);
    if (m_client == nullptr) {
        std::cerr << "jack_client_open() failed. Status 0x" << status << std::endl;
        exit(1);
    }

    m_ringbuffer = jack_ringbuffer_create(16384);

    if (m_ringbuffer == NULL) {
        std::cerr << "Cannot allocate ringbuffer" << std::endl;
        exit(1);
    }

    m_maxTx = (31250 / (jack_get_sample_rate(m_client) / 
                jack_get_buffer_size(m_client))) / 8;

    jack_set_process_callback(m_client, ProcessWrapper, this);
    jack_set_buffer_size_callback(m_client, SetupTxBufferWrapper, this);
    
    m_outputPort = jack_port_register(m_client, "out", 
        JACK_DEFAULT_MIDI_TYPE, JackPortIsOutput, 0);

    if (jack_activate(m_client)) {
        std::cerr << "Cannot activate client" << std::endl;
        exit(1);
    }
}

int SbMidi::ProcessWrapper(jack_nframes_t nframes, void* arg) {
    SbMidi* sbmidi = static_cast<SbMidi*>(arg);
    return sbmidi->process(nframes, 0);
}

int SbMidi::process(jack_nframes_t nframes, void* arg) {
    void* portBuf = jack_port_get_buffer(m_outputPort, nframes);
    jack_midi_data_t* txBuffer;
    jack_midi_clear_buffer(portBuf);
    
    int readspace = jack_ringbuffer_read_space(m_ringbuffer);
    readspace = (readspace > m_maxTx) ? m_maxTx : readspace;
    if (readspace > 0) {
        if ((txBuffer = jack_midi_event_reserve(portBuf, 0, readspace))) {
            if (readspace > jack_ringbuffer_read(m_ringbuffer,
                        reinterpret_cast<char*>(txBuffer), readspace)) {
                std::cerr << "Fewer bytes than expected " <<
                            "read from ringbuffer" << std::endl;
            }
        } else {
            std::cerr << "Couldn't allocate MIDI event buffer" << std::endl;
        }
    }
    return 0;
}

void SbMidi::TransmitMessage(std::vector<char> message) {
    int toWrite = message.size();
    if (toWrite <= jack_ringbuffer_write_space(m_ringbuffer)) {
        if ( toWrite > jack_ringbuffer_write(m_ringbuffer, &message[0], toWrite)) {
            std::cerr << "SbMidi: Error writing to ringbuffer" << std::endl;
        }
    } else {
        std::cerr << "SbMidi: Message to large for ringbuffer" << std::endl;
    }
}

int SbMidi::SetupTxBufferWrapper(jack_nframes_t buffersize, void* arg) {
    SbMidi* sbmidi = static_cast<SbMidi*>(arg);
    return sbmidi->setupTxBuffer(buffersize, 0);
}

int SbMidi::setupTxBuffer(jack_nframes_t buffersize, void* arg) {
    m_maxTx = (31250 / (jack_get_sample_rate(m_client) / buffersize)) / 8;
    return 0;
}
