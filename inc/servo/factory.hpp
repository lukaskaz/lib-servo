#pragma once

#include "servo/interfaces/servo.hpp"

#include <memory>

namespace servo
{

class Factory
{
  public:
    template <typename T, typename C>
    static std::shared_ptr<ServoIf> create(const C& config)
    {
        return std::shared_ptr<T>(new T(config));
    }
};

} // namespace servo
