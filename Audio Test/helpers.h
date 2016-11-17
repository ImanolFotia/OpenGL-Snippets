#pragma once

static bool isBigEndian()
{
    int a=1;
    return !((char*)&a)[0];
}
