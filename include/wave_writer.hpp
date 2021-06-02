#include <fstream>
#include <type_traits>

class WaveWriter {
public:
	struct FormatData {
		int16_t audioFormat	  = 1;
		int16_t numChannels	  = 1;
		int32_t sampleRate	  = 44100;
		int16_t bitsPerSample = 16;
		int16_t blockAlign	  = numChannels * bitsPerSample / 8;
		int32_t byteRate	  = sampleRate * blockAlign;
	};

	static void write(const std::string &name, const FormatData &formatData,
					  const char *audio_data, std::streamsize audio_len) {
		std::ofstream outFile(name + ".wav", std::ios::binary);

		// RIFF chunk
		outFile << "RIFF";			// ID
		write_bytes(outFile, 0, 4); // size
		outFile << "WAVE";			// format
		// ----------

		// fmt sub-chunk
		outFile << "fmt ";								   // ID
		write_bytes(outFile, 16, 4);					   // size
		write_bytes(outFile, formatData.audioFormat, 2);   // audio format
		write_bytes(outFile, formatData.numChannels, 2);   // channels
		write_bytes(outFile, formatData.sampleRate, 4);	   // sample rate
		write_bytes(outFile, formatData.byteRate, 4);	   // byte rate
		write_bytes(outFile, formatData.blockAlign, 2);	   // block align
		write_bytes(outFile, formatData.bitsPerSample, 2); // bps
		// -------------

		// data sub-chunk
		outFile << "data";			// ID
		write_bytes(outFile, 0, 4); // size
		// --------------

		int preDataPos = outFile.tellp();
		outFile.write(audio_data, audio_len);
		int postDataPos = outFile.tellp();

		outFile.seekp(preDataPos - 4);
		write_bytes(outFile, postDataPos - preDataPos, 4); // size

		outFile.seekp(4, std::ios::beg);
		write_bytes(outFile, postDataPos - 8, 4); // size
	}

private:
	template <typename Int>
	requires std::is_integral_v<Int>
	static void write_bytes(std::ofstream &outFile, Int value,
							std::streamsize len) {
		outFile.write(reinterpret_cast<const char *>(&value), len);
	}
};
