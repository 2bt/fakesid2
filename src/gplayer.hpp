#pragma once
#include <cstdint>
#include <array>
#include "gsong.hpp"

namespace gt {


class Player {
public:
    gt::Song const& song;
    Player(gt::Song const& song);

    enum {
        PLAY_BEGINNING = 0x01,
        PLAY_POS       = 0x02,
        PLAY_PATTERN   = 0x03,
        PLAY_STOP      = 0x04,
        PLAY_STOPPED   = 0x80,
    };

    void initsong(int num, int mode, int pattpos = 0);
    void stopsong();
    void playroutine();


    // TODO: test
    void releasenote(int chnnum);
    void playtestnote(int note, int ins, int chnnum);

    void mutechannel(int chnnum) { channels[chnnum].mute ^= 1; }
    int  isplaying() const { return (songinit != PLAY_STOPPED); }

    std::array<uint8_t, 25> sidreg;

private:
    void sequencer(int c);

    struct Channel {
        uint8_t  trans;
        uint8_t  instr;
        uint8_t  note;
        uint8_t  lastnote;
        uint8_t  newnote;
        uint32_t pattptr;
        uint8_t  pattnum;
        uint8_t  songptr;
        uint8_t  repeat;
        uint16_t freq;
        uint8_t  gate;
        uint8_t  wave;
        uint16_t pulse;
        uint8_t  ptr[2];
        uint8_t  pulsetime;
        uint8_t  wavetime;
        uint8_t  vibtime;
        uint8_t  vibdelay;
        uint8_t  command;
        uint8_t  cmddata;
        uint8_t  newcommand;
        uint8_t  newcmddata;
        uint8_t  tick;
        uint8_t  tempo;
        uint8_t  mute;
        uint8_t  advance;
        uint8_t  gatetimer;
    };

    int      multiplier       = 1;      // for multi speed
    uint16_t adparam          = 0x0f00; // HR
    bool     optimizepulse    = true;
    bool     optimizerealtime = true;

    std::array<Channel, MAX_CHN> channels = {};
    uint8_t                      filterctrl   = 0;
    uint8_t                      filtertype   = 0;
    uint8_t                      filtercutoff = 0;
    uint8_t                      filtertime   = 0;
    uint8_t                      filterptr    = 0;
    uint8_t                      funktable[2];
    uint8_t                      masterfader = 0x0f;
    int                          psnum       = 0; // song number

    int songinit     = 0;
    int lastsonginit = 0;
    int startpattpos = 0;

    int espos[MAX_CHN];
    int esend[MAX_CHN];
    int epnum[MAX_CHN];
};


void playroutine();
void initsong(int num, int mode, int pattpos = 0);

} // namespace gt
