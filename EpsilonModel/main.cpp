#include <string>
#include "EpsilonModel.h"

namespace EpsilonModel
{

eModel ImportModel(std::string path)
{
    eModel model;
    model.loadModel(path);

    return model;
}

}

