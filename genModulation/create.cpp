#include "pch.h"
#define _USE_MATH_DEFINES
#include "create.h"
#include <math.h>
using namespace std;

void createFunction(std::vector<double>& t, std::vector<long long>& x, bool type)
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
			x[i] = A * sin(phase);
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
			x[i] = A * sin(phase + phase0);
		}
	}
}

int createWav(std::vector<long long> x)
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

modulator::modulator(double _znak, int _fm)
{
	znak = _znak;
	fm = _fm;
}

void modulator::FM(std::vector<int> bits, std::vector<long long>& exitSin, std::vector<double>& t, int N, std::vector<long long>& sinI, std::vector<long long>& cosQ)
{
	if (fm == 2)
	{
		FM2(bits, exitSin, t, N, 0);
		sinI = exitSin;
		cosQ.resize(N, 0);
	}
	else
		if (fm == 4)
		{
			std::vector<int> bitsNodd;
			std::vector<int> bitsEven;
			for (int i = 0; i < bits.size(); i++)
			{
				if ((i + 1) % 2 == 1)
					bitsNodd.push_back(bits[i]);
				else
					bitsEven.push_back(bits[i]);
			}
			std::vector<long long> SinNodd;
			std::vector<long long> CosEven;
			FM2(bitsNodd, SinNodd, t, N, 0);
			FM2(bitsEven, CosEven, t, N, M_PI / 2.);
			exitSin.resize(N);
			for (int i = 0; i < N; i++)
			{
				exitSin[i] = SinNodd[i] + CosEven[i];
			}
			sinI = SinNodd;
			cosQ = CosEven;
		}
}

void modulator::FM2(std::vector<int> bits, std::vector<long long>& exitSin, std::vector<double>& t, int N, double phase)
{
	exitSin.resize(N);
	t.resize(N);

	std::vector<int> newBits(N);
	for (int i = 0; i < newBits.size(); i++)
	{
		newBits[i] = bits[i / (N / bits.size())];
	}
	double dist = 0.001;
	for (int i = 0; i < N; i++)
	{
		//phase0 = newBits[i] == 0 ? 0 : M_PI;
		znak = newBits[i] == 0 ? -1 : 1;
		curPhase += 2 * M_PI * (f + dist) / exitFd;
		curT = t[i] = (double)i / exitFd;
		//exitSin[i] = A * sin(curPhase + phase0);
		exitSin[i] = znak * A * sin(curPhase + phase) * (i % 10 == 0 ? ((double)rand() / RAND_MAX) : 1);
	}
}

void modulator::SetFM(int _fm)
{
	fm = _fm;
}

void demodulator::Kostas(std::vector<long long>& exitSin, std::vector<int>& bits, std::vector<long long>& resultI, std::vector<long long>& resultQ)
{
	if (fm == 2)
	{
		resultQ.resize(exitSin.size(), 0);
		KostasFM2(exitSin, bits, resultI, 0);
	}
	else
		if (fm == 4)
		{
			KostasFM2(exitSin, bits, resultQ, M_PI / 2.);
			KostasFM2(exitSin, bits, resultI, 0);
		}
	drawDiagramma(resultI, resultQ);
}

void demodulator::KostasFM2(std::vector<long long>& exitSin, std::vector<int>& bits, std::vector<long long>& result, double phase)
{
	double curPhase = 0;
	auto curSig = exitSin;
	for (int i = 0; i < exitSin.size(); i++)
	{
		curPhase += 2 * M_PI * f / fd;
		curSig[i] = (double)exitSin[i] * sin(curPhase + phase);
	}
	filter fil;
	//fil.main(curSig, result, start_spectr, res_spectr);
	fil.FLF(curSig, result);
}

void demodulator::drawDiagramma(std::vector<long long>& resultI, std::vector<long long>& resultQ)
{
	ptsX.resize(resultI.size());
	ptsY.resize(resultQ.size());
	for (int i = 0; i < ptsX.size(); i++)
	{
		ptsX[i] = resultI[i];
		ptsY[i] = resultQ[i];
	}
}

std::vector<double> demodulator::GetptX()
{
	return ptsX;
}
std::vector<long long> demodulator::GetptY()
{
	return ptsY;
}

void demodulator::SetFM(int _fm)
{
	fm = _fm;
}

filter::filter()
{
	double t = 1. / (2. * f);
	h.push_back(2 * f);
	for (double i = 1. / fd; i < t; i += 1. / fd)
	{
		h.push_back(Calch(i));
	}
	signal = vector<long long>(h.size() - 1, 0);
}

void filter::FLF(std::vector<long long>& sig, std::vector<long long>& result)
{
	int sizeSig = sig.size();
	int sizeSignal = signal.size();
	int sizeH = h.size();

	result.resize(sizeSig);
	signal.insert(signal.end(), sig.begin(), sig.end());

	vector<long long> buffer(sizeSig + (sizeH - 1) * 2, 0);
	for (int i = 0; i < sizeSig + sizeH - 1; i++)
	{
		buffer[i] = signal[sizeSignal - (sizeH - 1) + i];
	}

	for (int i = 0; i < sizeSig; i++)
	{
		result[i] = 0;
		for (int j = -(sizeH - 1); j < sizeH; j++)
		{
			result[i] += (double)buffer[sizeH - 1 + i + j] * (double)h[abs(j)] / (double)h[0];
		}
		result[i] /= 2 * sizeH - 1;
	}
}

long long filter::Calch(double t)
{
	return /*(double)A **/ (double)sin(2. * f * t * M_PI) / (M_PI * t);
}

void fft(std::vector<base>& a, bool invert)
{
	int n = (int)a.size();
	if (n == 1)  return;

	vector<base> a0(n / 2), a1(n / 2);
	for (int i = 0, j = 0; i < n; i += 2, ++j) {
		a0[j] = a[i];
		a1[j] = a[i + 1];
	}
	fft(a0, invert);
	fft(a1, invert);

	double ang = 2 * M_PI / n * (invert ? -1 : 1);
	base w(1), wn(cos(ang), sin(ang));
	for (int i = 0; i < n / 2; ++i) {
		a[i] = a0[i] + w * a1[i];
		a[i + n / 2] = a0[i] - w * a1[i];
		if (invert)
			a[i] /= 2, a[i + n / 2] /= 2;
		w *= wn;
	}
}

void filter::main(std::vector<long long>& sig, std::vector<long long>& result, std::vector<long long>& start_spectr, std::vector<long long>& res_spectr)
{
	FLF(sig, result);
	vector<long long> buf(result.begin(), result.end());
	buf.insert(buf.end(), 24, 0);
	vector<base> spec(buf.begin(), buf.end());
	fft(spec, true);
	vector<base> spec_start(sig.begin(), sig.end());
	spec_start.insert(spec_start.end(), 24, 0);
	fft(spec_start, true);

	start_spectr.resize(1024);
	res_spectr.resize(1024);
	for (int i = 0; i < result.size(); i++)
	{
		start_spectr[i] = sqrt(spec_start[i].real() * spec_start[i].real() + spec_start[i].imag() * spec_start[i].imag());
		res_spectr[i] = sqrt(spec[i].real() * spec[i].real() + spec[i].imag() * spec[i].imag());
	}
}
