#pragma once

namespace servo
{

class ServoIf
{
  public:
    virtual ~ServoIf() = default;
    virtual bool moveleft() = 0;
    virtual bool moveright() = 0;
    virtual bool movecenter() = 0;
    virtual bool moveto(double) = 0;
};

} // namespace servo
