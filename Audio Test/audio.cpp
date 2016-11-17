#include <iostream>
#include <fstream>
#include <cstring>
#include "audio.h"

#include "helpers.h"

Sound::Sound()
{
const char * devicename = alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER);

    this->device=alcOpenDevice(devicename);
    std::cout << devicename << std::endl;
    if(device==NULL)
    {
        std::cout << "cannot open sound card" << std::endl;
        return ;
    }
    context=alcCreateContext(device,NULL);
    if(context==NULL)
    {
        std::cout << "cannot open context" << std::endl;
        return ;
    }
    alcMakeContextCurrent(context);

}

void Sound::loadSound(const char* fn,UINT id)
{
    SoundData SoundD;
    SoundD.id = id;
    SoundD.data = loadWAV(fn,SoundD.channel,SoundD.sampleRate, SoundD.bps, SoundD.size);

    alGenBuffers(1,&SoundD.bufferid);

    if(SoundD.channel==1)
    {
        if(SoundD.bps==8)
        {
            SoundD.format=AL_FORMAT_MONO8;
        }
        else
        {
            SoundD.format=AL_FORMAT_MONO16;
        }
    }
    else
    {
        if(SoundD.bps==8)
        {
            SoundD.format=AL_FORMAT_STEREO8;
        }
        else
        {
            SoundD.format=AL_FORMAT_STEREO16;
        }
    }
    alBufferData(SoundD.bufferid,SoundD.format,SoundD.data,SoundD.size,SoundD.sampleRate);

    alGenSources(1,&SoundD.sourceid);
    std::cout << SoundD.sourceid << std::endl;
    UINT ID = SoundD.id;
    Sounds.push_back(SoundD);
}

int Sound::convertToInt(char* buffer,int len)
{
    int a=0;
    if(!isBigEndian())
        for(int i=0; i<len; i++)
            ((char*)&a)[i]=buffer[i];
    else
        for(int i=0; i<len; i++)
            ((char*)&a)[3-i]=buffer[i];
    return a;
}

char* Sound::loadWAV(const char* fn,int& chan,int& samplerate,int& bps,int& size)
{
    char buffer[4];
    std::ifstream in(fn,std::ios::binary);
    in.read(buffer,4);
    if(strncmp(buffer,"RIFF",4)!=0)
    {
        std::cout << "this is not a valid WAVE file"  << std::endl;
        return NULL;
    }
    in.read(buffer,4);
    in.read(buffer,4);      //WAVE
    in.read(buffer,4);      //fmt
    in.read(buffer,4);      //16
    in.read(buffer,2);      //1
    in.read(buffer,2);
    chan=convertToInt(buffer,2);
    in.read(buffer,4);
    samplerate=convertToInt(buffer,4);
    in.read(buffer,4);
    in.read(buffer,2);
    in.read(buffer,2);
    bps=convertToInt(buffer,2);
    in.read(buffer,4);      //data
    in.read(buffer,4);
    size=convertToInt(buffer,4);
    char* data=new char[size];
    in.read(data,size);
    return data;
}


ALfloat audioDir[6];

bool Sound::Play(UINT sourceid ,vec3 position)
{

    ALint source_state;
    alSourcei(Sounds[sourceid].sourceid,AL_LOOPING,AL_FALSE);
    alGetSourcei(Sounds[sourceid].sourceid, AL_SOURCE_STATE, &source_state);



    if(source_state != AL_PLAYING)
    {

        alSourcei(Sounds[sourceid].sourceid,AL_BUFFER,Sounds[sourceid].bufferid);

        alSource3f(Sounds[sourceid].sourceid,AL_POSITION,position.x,position.y,position.z);
        alSource3f (Sounds[sourceid].sourceid, AL_VELOCITY, 0,0,0);

        alSourcePlay(Sounds[sourceid].sourceid);
    }
    alGetSourcei(Sounds[sourceid].sourceid, AL_SOURCE_STATE, &source_state);

    if (source_state == AL_PLAYING)
        return true;
    else
    {
        Stop(sourceid);
        return false;
    }
}

bool Sound::getState(int id)
{
    ALint source_state;
    alGetSourcei(Sounds[id].sourceid, AL_SOURCE_STATE, &source_state);
    return (source_state == AL_PLAYING ? true : false);
}

bool Sound::Stop(int id)
{
    alSourceStop(Sounds[id].sourceid);
}
