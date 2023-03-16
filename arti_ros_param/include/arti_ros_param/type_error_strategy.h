/// \file
/// \author Alexander Buchegger
/// \date 2021-12-26
/// \copyright ARTI - Autonomous Robot Technology GmbH. All rights reserved.
#ifndef ARTI_ROS_PARAM_TYPE_ERROR_STRATEGY_H
#define ARTI_ROS_PARAM_TYPE_ERROR_STRATEGY_H

#include <functional>
#include <ros/console.h>
#include <stdexcept>
#include <string>

namespace arti_ros_param
{

class Param;

using TypeErrorStrategy = std::function<void(const Param& param, const std::string& error_message)>;

class LogTypeError
{
public:
  explicit LogTypeError(ros::console::Level level) noexcept;

  void operator()(const Param& param, const std::string& error_message);

protected:
  ros::console::Level level_;
};

extern LogTypeError LOG_TYPE_ERROR;


namespace detail
{
std::string makeWhat(const Param& param, const std::string& error_message);
}

template<typename T>
class ThrowTypeError
{
public:
  void operator()(const Param& param, const std::string& error_message)
  {
    throw T{detail::makeWhat(param, error_message)};
  }
};

extern ThrowTypeError<std::invalid_argument> THROW_TYPE_ERROR;

}

#endif // ARTI_ROS_PARAM_TYPE_ERROR_STRATEGY_H
