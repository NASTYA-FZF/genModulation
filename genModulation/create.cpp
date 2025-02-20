#include "pch.h"
#define _USE_MATH_DEFINES
#include "create.h"
#include <math.h>

void createFunction(std::vector<double>& t, std::vector<UINT16>& x, bool type)
{
	if (!t.empty()) t.clear();
	if (!x.empty()) x.clear();

	double minF = 110;
	double maxF = 220;
	double fd(1000);
	double f(minF);
	int N(1000);
	double phase(0);
	double phase0(0);
	double stepF = (maxF - minF) / N;
	UINT16 A = 0xFFFF / 2;
	std::vector<int> bits({
		0, 1, 0, 1, 0, 1, 0, 1, 0, 1
		});

	t.resize(N);
	x.resize(N);

	if (type)
	{
		for (int i = 0; i < N; i++)
		{
			phase += 2 * M_PI * f / fd;
			t[i] = (double)i / fd;
			x[i] = A * (sin(phase) + 1);
			f += stepF;
		}
	}
	else
	{
		for (int i = 0; i < N; i++)
		{
			phase0 = bits[i / (N / bits.size())] == 0 ? 0 : M_PI;
			phase += 2 * M_PI * f / fd;
			t[i] = (double)i / fd;
			x[i] = A * (sin(phase + phase0) + 1);
		}
	}
}

int createWav(std::vector<UINT16> x)
{
    static_assert(sizeof(wav_hdr) == 44, "");

    uint32_t fsize = x.size();

    printf("file size: %u\n", fsize);

    wav_hdr wav;
    wav.ChunkSize = fsize + sizeof(wav_hdr) - 8;
    wav.Subchunk2Size = fsize + sizeof(wav_hdr) - 44;

    std::ofstream out("test.wav", std::ios::binary);
    out.write(reinterpret_cast<const char*>(&wav), sizeof(wav));

    int16_t d;
    for (int i = 0; i < fsize; ++i) {
        // TODO: read/write in blocks
		d = (int16_t)x[i];
        out.write(reinterpret_cast<char*>(&d), sizeof(int16_t));
    }

    return 0;
}
