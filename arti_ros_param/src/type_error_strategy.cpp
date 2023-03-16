/// \file
/// \author Alexander Buchegger
/// \date 2021-12-26
/// \copyright ARTI - Autonomous Robot Technology GmbH. All rights reserved.
#include <arti_ros_param/type_error_strategy.h>
#include <arti_ros_param/param.h>
#include <sstream>

namespace arti_ros_param
{

LogTypeError::LogTypeError(ros::console::Level level) noexcept
  : level_(level)
{
}

void LogTypeError::operator()(const Param& param, const std::string& error_message)
{
  switch (level_)
  {
#define X(level) \
  case ros::console::Level::level: \
  { ROS_LOG_STREAM(ros::console::Level::level, ROSCONSOLE_DEFAULT_NAME, \
                   "parameter " << param.getPath() << " has wrong type: " << error_message); break; }
    X(Debug)
    X(Info)
    X(Warn)
    X(Error)
    case ros::console::Level::Count:
    X(Fatal)
#undef X
  }
}

LogTypeError LOG_TYPE_ERROR{ros::console::Level::Warn};


std::string detail::makeWhat(const Param& param, const std::string& error_message)
{
  std::ostringstream what;
  what << "parameter " << param.getPath() << " has wrong type: " << error_message;
  return what.str();
}

ThrowTypeError<std::invalid_argument> THROW_TYPE_ERROR;

}
