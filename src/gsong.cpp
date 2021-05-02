#include "gsong.hpp"
#include <cstdio>
#include <cstring>


namespace {

uint8_t fread8(FILE *file) {
    uint8_t b;
    fread(&b, 1, 1, file);
    return b;
}

} // namespace


void gt::Song::count_pattern_lengths() {
    highestusedpattern = 0;
    highestusedinstr   = 0;
    for (int c = 0; c < MAX_PATT; c++) {
        int d;
        for (d = 0; d <= MAX_PATTROWS; d++) {
            if (pattern[c][d * 4] == ENDPATT) break;
            if (pattern[c][d * 4] != REST ||
                pattern[c][d * 4 + 1] ||
                pattern[c][d * 4 + 2] ||
                pattern[c][d * 4 + 3])
            {
                highestusedpattern = c;
            }
            if (pattern[c][d * 4 + 1] > highestusedinstr) highestusedinstr = pattern[c][d * 4 + 1];
        }
        pattlen[c] = d;
    }

    for (int e = 0; e < MAX_SONGS; e++) {
        for (int c = 0; c < MAX_CHN; c++) {
            int d;
            for (d = 0; d < MAX_SONGLEN; d++) {
                if (songorder[e][c][d] >= LOOPSONG) break;
                if (songorder[e][c][d] < REPEAT &&
                    songorder[e][c][d] > highestusedpattern)
                {
                    highestusedpattern = songorder[e][c][d];
                }
            }
            songlen[e][c] = d;
        }
    }
}

bool gt::Song::load(char const* filename) {
    FILE* file = fopen(filename, "rb");
    if (!file) return false;
    char ident[4];
    fread(ident, 4, 1, file);
    if (memcmp(ident, "GTS5", 4)) {
        fclose(file);
        return false;
    }

    // read infotexts
    fread(songname,      sizeof(songname), 1, file);
    fread(authorname,    sizeof(authorname), 1, file);
    fread(copyrightname, sizeof(copyrightname), 1, file);

    // read songorderlists
    int amount = fread8(file);
    for (int d = 0; d < amount; d++) {
        for (int c = 0; c < MAX_CHN; c++) {
            int loadsize = fread8(file) + 1;
            fread(songorder[d][c], loadsize, 1, file);
        }
    }
    // read instruments
    amount = fread8(file);
    for (int c = 1; c <= amount; c++) {
        instr[c].ad        = fread8(file);
        instr[c].sr        = fread8(file);
        instr[c].ptr[WTBL] = fread8(file);
        instr[c].ptr[PTBL] = fread8(file);
        instr[c].ptr[FTBL] = fread8(file);
        instr[c].ptr[STBL] = fread8(file);
        instr[c].vibdelay  = fread8(file);
        instr[c].gatetimer = fread8(file);
        instr[c].firstwave = fread8(file);
        fread(&instr[c].name, MAX_INSTRNAMELEN, 1, file);
    }
    // read tables
    for (int c = 0; c < MAX_TABLES; c++) {
        int loadsize = fread8(file);
        fread(ltable[c], loadsize, 1, file);
        fread(rtable[c], loadsize, 1, file);
    }
    // read patterns
    amount = fread8(file);
    for (int c = 0; c < amount; c++) {
        int length = fread8(file) * 4;
        fread(pattern[c], length, 1, file);
    }
    fclose(file);

    count_pattern_lengths();

    return true;
}


