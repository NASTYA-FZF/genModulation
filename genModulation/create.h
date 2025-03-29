#pragma once
#include <vector>
#include <fstream>
#include <iostream>
#include <complex>

const long long A = 0xFFFF / 2;
const double f = 110;
const double fd = 1000;

typedef std::complex<double> base;

void createFunction(std::vector<double>& t, std::vector<long long>& x, bool type);

typedef struct WAV_HEADER {
    /* RIFF Chunk Descriptor */
    uint8_t RIFF[4] = { 'R', 'I', 'F', 'F' }; // RIFF Header Magic header
    uint32_t ChunkSize;                     // RIFF Chunk Size
    uint8_t WAVE[4] = { 'W', 'A', 'V', 'E' }; // WAVE Header
    /* "fmt" sub-chunk */
    uint8_t fmt[4] = { 'f', 'm', 't', ' ' }; // FMT header
    uint32_t Subchunk1Size = 16;           // Size of the fmt chunk
    uint16_t AudioFormat = 1; // Audio format 1=PCM,6=mulaw,7=alaw,     257=IBM
    // Mu-Law, 258=IBM A-Law, 259=ADPCM
    uint16_t NumOfChan = 1;   // Number of channels 1=Mono 2=Sterio
    uint32_t SamplesPerSec = 1000;   // Sampling Frequency in Hz
    uint32_t bytesPerSec = 1000 * 2; // bytes per second
    uint16_t blockAlign = 2;          // 2=16-bit mono, 4=16-bit stereo
    uint16_t bitsPerSample = 16;      // Number of bits per sample
    /* "data" sub-chunk */
    uint8_t Subchunk2ID[4] = { 'd', 'a', 't', 'a' }; // "data"  string
    uint32_t Subchunk2Size;                        // Sampled data length
} wav_hdr;

int createWav(std::vector<long long> x);

class modulator
{
    double curPhase = 0.;
    double znak;
    double curT = 0.;
    double exitFd = fd;
    int fm;

public:
    modulator() {}
    modulator(double _phase0, int _fm);
    void FM(std::vector<int> bits, std::vector<long long>& exitSin, std::vector<double>& t, int N, std::vector<long long>& sinI, std::vector<long long>& cosQ);
    void FM2(std::vector<int> bits, std::vector<long long>& exitSin, std::vector<double>& t, int N, double phase);
    void SetFM(int _fm);
};

class demodulator {
    double nTact = 100;
    std::vector<double> ptsX;
    std::vector<long long> ptsY;
    int fm;
public:
    void Kostas(std::vector<long long>& exitSin, std::vector<int>& bits, std::vector<long long>& resultI, std::vector<long long>& resultQ);

    void KostasFM2(std::vector<long long>& exitSin, std::vector<int>& bits, std::vector<long long>& result, double phase);

    void drawDiagramma(std::vector<long long>& resultI, std::vector<long long>& resultQ);

    std::vector<double> GetptX();
    std::vector<long long> GetptY();

    void SetFM(int _fm);
};

class filter {
    std::vector<long long> h;
    std::vector<long long> signal;

public:
    filter();

    void FLF(std::vector<long long>& sig, std::vector<long long>& result);

    long long Calch(double t);

    void main(std::vector<long long>& sig, std::vector<long long>& result, std::vector<long long>& start_spectr, std::vector<long long>& res_spectr);
};

void fft(std::vector<base>& a, bool invert);