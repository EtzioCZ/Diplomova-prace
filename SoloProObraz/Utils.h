#pragma once

inline int double2int(double d)
{
    return static_cast<int>(d);
}

inline int float2int(float f)
{
    return static_cast<int>(f);
}

inline unsigned char float2uchar(float f)
{
    return static_cast<unsigned char>(f);
}

inline float double2float(double d)
{
    return static_cast<float>(d);
}

inline float int2float(int i)
{
    return static_cast<float>(i);
}
