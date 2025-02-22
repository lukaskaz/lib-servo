#pragma once

#include <cstdint>
#include <vector>

namespace servo
{

class ServoIf
{
  public:
    virtual ~ServoIf() = default;
    virtual bool movestart() = 0;
    virtual bool moveend() = 0;
    virtual bool moveto(int32_t) = 0;
    virtual bool movestart(uint32_t) = 0;
    virtual bool moveend(uint32_t) = 0;
    virtual bool moveto(uint32_t, int32_t) = 0;
};

} // namespace servo
