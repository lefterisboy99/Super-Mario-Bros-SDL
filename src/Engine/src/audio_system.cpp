#include <audio_system.h>
#include <SDL_mixer.h>

namespace engine {

void AudioSystem::Initialize() {
	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 4096) < 0) {
		ASSERT(0);
	}
}

size_t AudioSystem::LoadMusic(const std::string &filePath, bool autoPlay) {
	Mix_Music *musicHandle = Mix_LoadMUS(filePath.c_str());
	ASSERT(musicHandle);
	music.push_back(musicHandle);

	if (autoPlay) ASSERT(Mix_PlayMusic(musicHandle, 1) == 0);

	return music.size() - 1;
}

void AudioSystem::PlayMusic(size_t index, bool looping) {
	if (index < music.size()) {
		Mix_Music *hnd = (Mix_Music *)music[index];
		ASSERT(Mix_PlayMusic(hnd, looping ? -1 : 1) == 0);
	}
}

void AudioSystem::SetVolume(int volume, bool forMusic) {
	if (forMusic)
		Mix_VolumeMusic(volume);
	else
		Mix_Volume(-1, volume);


}

void AudioSystem::Shutdown() {
	Mix_CloseAudio();
}

}