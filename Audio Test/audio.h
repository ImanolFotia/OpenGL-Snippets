#ifndef AUDIO_H_INCLUDED
#define AUDIO_H_INCLUDED

#include <AL/al.h>
#include <AL/alc.h>
#include <vector>

typedef struct cVector3f
{
    float x;
    float y;
    float z;
} vec3;

using namespace::std;
typedef unsigned int UINT;
typedef char byte;

bool Init_Audio();

class Sound
{
public:

    Sound();

    virtual ~Sound() {}

    bool Play(UINT sourceid ,vec3 position);

    void loadSound(const char*, UINT);

    bool Stop(int id);

    char* loadWAV(const char* fn,int& chan,int& samplerate,int& bps,int& size);

    bool getState(int id);
private:

    struct SoundData
    {
        UINT id;
        int channel;
        int sampleRate;
        int bps;
        int size;
        byte* data;
        UINT bufferid;
        UINT format;
        UINT sourceid;

    };
    vector<SoundData> Sounds;
    ALCdevice* device;
    ALCcontext* context;

    int convertToInt(char* buffer,int len);




};

#endif // AUDIO_H_INCLUDED

