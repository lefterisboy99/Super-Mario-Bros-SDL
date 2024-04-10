#include <common.h>
#include <SDL.h>
#include <vector>
#include <string>

namespace engine {
class AudioSystem {
public:
	void Initialize();
	void Shutdown();

	size_t LoadMusic(const std::string &filePath, bool autoPlay = false);
	void PlayMusic(size_t index, bool looping = false);

	void SetVolume(int volume, bool forMusic = true);

private:
	std::vector<void *> music;
};

}