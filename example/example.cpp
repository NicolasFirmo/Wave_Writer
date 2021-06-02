#ifdef _WIN32
#define _USE_MATH_DEFINES
#endif

#include <cmath>
#include <vector>

#include "wave_writer.hpp"

static const size_t duration			= 2;
static const size_t sampleRate			= 44100;
static const size_t bitDepth			= sizeof(short) * 8;

static float wave_frequency				= 20;
static const float modulation_frequency = 0.5f;
static const float wave_amplitude		= 0.5f;

int main() {
	std::vector<short> buffer;

	float audioAngle = 0;
	float modAngle	 = 0;

	for (size_t i = 0; i < sampleRate * duration; i++) {
		wave_frequency *= 1.00005f;

		float audioDelta = 2 * M_PI * wave_frequency / sampleRate;
		float modDelta	 = 2 * M_PI * modulation_frequency / sampleRate;
		audioAngle		 = std::fmod(audioAngle + audioDelta, 2 * M_PI);
		modAngle		 = std::fmod(modAngle + modDelta, 2 * M_PI);

		float val1		 = std::sin(modAngle) * std::sin(audioAngle);
		float val2		 = std::cos(modAngle) * std::sin(audioAngle);

		buffer.push_back(short(val1 * INT16_MAX));
		buffer.push_back(short(val2 * INT16_MAX));
	}

	WaveWriter::write("audio", {1, 2},
					  reinterpret_cast<const char *>(buffer.data()),
					  buffer.size() * sizeof(short));
}
