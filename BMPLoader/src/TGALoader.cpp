#include <TGAHeader.h>
#include <TGALoader.h>
#include <fstream>

TGAImage::TGAImage(std::string filePath)
{
    std::ifstream FILE(filePath.c_str(), std::ios::binary);

    if(!FILE.is_open())
    {
        std::cout << "Fail to open TGA file" << std::endl;
        return;
    }

    TGA::TGAHeader* header = new TGA::TGAHeader[sizeof(char) * 18];

    FILE.read((char*)&header[0], sizeof(char) * 18);

    std::cout << header->imagedescriptor << std::endl;
}
