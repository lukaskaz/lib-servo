#pragma once

namespace servo
{

class ServoIf
{
  public:
    virtual ~ServoIf() = default;
    virtual bool movestart() = 0;
    virtual bool moveend() = 0;
    virtual bool moveto(double) = 0;
};

} // namespace servo
