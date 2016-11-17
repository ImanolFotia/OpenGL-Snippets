#include <iostream>
#include "audio.h"
#include "helpers.h"

using namespace std;

int main()
{

    Sound s;

    s.loadSound("audio.wav", 1);
    s.loadSound("audio2.wav", 2);

    alListener3f(AL_POSITION, 0,0,0);

    alListener3f(AL_ORIENTATION, 0,0,0);

    vec3 Position = {0,0,0};

    bool playing = true, playing2 = true;
    string line;
    bool running = true;
    getline(cin, line);

    while(1)
    {
        if(line == "ruido")
            s.Play(0, Position);

        if(line == "tono")
            s.Play(1, Position);

        if(line == "quit")
            running = false;

        line = "";
    }

    return 0;
}

