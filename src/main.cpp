#include <cstdio>
#include <cstring>
#include <algorithm>
#include <SDL2/SDL.h>
#include "gplayer.hpp"
#include "resid/sid.h"


enum {
    MIXRATE           = 44100,
    PALCLOCKRATE      = 985248,
    SAMPLES_PER_FRAME = MIXRATE / 50,
    BUFFER_SIZE       = MIXRATE / 50 * 2,
};


class SidEngine {
public:
    SidEngine() {
        m_sid.reset();
        m_sid.set_chip_model(MOS8580);
        m_sid.set_sampling_parameters(PALCLOCKRATE, SAMPLE_RESAMPLE_INTERPOLATE, MIXRATE);
    }
    void set_reg(int r, uint8_t value) {
        m_sid.write(r, value);
    }
    void mix(int16_t* buffer, int length) {
        int c = 999999999;
        m_sid.clock(c, buffer, length);
    }
private:
    mutable SID m_sid;
    uint8_t     m_regs[25] = {};
};


gt::Song   song;
gt::Player player(song);
SidEngine  sid_engine;


void tick() {
    player.play_routine();
    for (int i = 0; i < 25; ++i) {
        sid_engine.set_reg(i, player.regs[i]);
//        printf(" %02X", player.sidreg[i]);
    }
//    printf("\n");
}

void audio_callback(void* u, Uint8* stream, int bytes) {
//    auto then = std::chrono::steady_clock::now();

    int16_t* buffer = (int16_t*) stream;
    int      length = bytes / sizeof(int16_t);
    static int sample = 0;
    while (length > 0) {
        if (sample == 0) tick();
        int l = std::min(SAMPLES_PER_FRAME - sample, length);
        sample += l;
        if (sample == SAMPLES_PER_FRAME) sample = 0;
        length -= l;
//        if (playing)
        sid_engine.mix(buffer, l);
//        else for (int i = 0; i < l; ++i) buffer[i] = 0;
        buffer += l;
    }

//    // measure time
//    auto now = std::chrono::steady_clock::now();
//    size_t nano = std::chrono::duration_cast<std::chrono::microseconds>(now - then).count();
//    static size_t nano_acc = 0;
//    static size_t count = 0;
//    nano_acc += nano;
//    count += bytes / 2;
//    if (count >= MIXRATE) {
//        render_time = nano_acc / (count / float(MIXRATE));
//        nano_acc = 0;
//        count = 0;
//        printf("%10lu\n", render_time);
//    }
}



int main(int argc, char** argv) {

    if (!song.load("../sng/hyperspace.sng")) {
        printf("ERROR\n");
        return 1;
    }

    player.init_song(0, gt::Player::PLAY_BEGINNING);

    SDL_AudioSpec spec = { MIXRATE, AUDIO_S16, 1, 0, BUFFER_SIZE, 0, 0, &audio_callback };
    SDL_OpenAudio(&spec, nullptr);
    SDL_PauseAudio(0);
    getchar();

    return 0;
}
