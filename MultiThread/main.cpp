#include <iostream>
#include <string>
#include <thread>
#include <SOIL.h>
#include <vector>
#include <cmath>

using namespace std;

void loadImage(string name)
{
    cout << "loaded image " << name << endl;
    int channels, width, height;
    unsigned char* pixels = SOIL_load_image(name.c_str(), &width, &height, &channels, SOIL_LOAD_AUTO);
    SOIL_free_image_data(pixels);
    cout << width << endl;
}

void sum(int a)
{
    int j = 0;
    for(int i = 0; i < 90000000; i++)
        j += sqrt(sqrt(i+a+a+a));

    cout << j << endl;
}


int main()
{
    thread t1, t2;

    return 0;
}
