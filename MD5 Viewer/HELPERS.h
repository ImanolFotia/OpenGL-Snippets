#ifndef HELPERS_H
#define HELPERS_H

#include <fstream>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

inline
void RemoveQuotes(std::string& input)
{
	size_t pos;
	while ( (pos = input.find('\"')) != std::string::npos )
	{
		input.erase(pos, 1);
	}
}

inline
void ComputeQuaternionW(glm::quat q)
{
	float w2 = 1.0f - q[0] * q[0] - q[1] * q[1] - q[2] * q[2];
	if(w2 < 0.0f)
	{
		q[3] = 0.0f;
	}
	else
	{
		q[3] = -sqrtf(w2);
	}
}


#endif //HELPERS_H
