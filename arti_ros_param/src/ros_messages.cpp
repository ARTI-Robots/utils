/// \file
/// \author Alexander Buchegger
/// \date 2021-12-10
/// \copyright ARTI - Autonomous Robot Technology GmbH. All rights reserved.
#include <arti_ros_param/ros_messages.h>
#include <cstring>
#include <ros/console.h>

namespace arti_ros_param
{

detail::RosMessageDecodingStream::RosMessageDecodingStream(const Param& param, const char* definition)
  : param_(param), remaining_definition_(definition)
{
}

std::string detail::RosMessageDecodingStream::getNextMemberName()
{
  while (remaining_definition_ != nullptr && *remaining_definition_ != 0)
  {
    // Skip whitespace:
    remaining_definition_ += std::strspn(remaining_definition_, " \t\r\n");
    if (*remaining_definition_ == 0 || *remaining_definition_ == '=')
    {
      // Reached string-terminating zero or end-of-definition marker "=====...".
      break;
    }
    else if (*remaining_definition_ == '#')
    {
      // Comment --> skip ahead to end of line:
      remaining_definition_ += std::strcspn(remaining_definition_, "\r\n");
    }
    else
    {
      // Else, this must be a type name --> skip ahead to space:
      remaining_definition_ += std::strcspn(remaining_definition_, " \t");

      // Skip whitespace:
      remaining_definition_ += std::strspn(remaining_definition_, " \t");
      const char* const start_of_member_name = remaining_definition_;

      // Skip ahead to end of member name:
      remaining_definition_ += std::strcspn(remaining_definition_, " \t\r\n#=");
      const char* const end_of_member_name = remaining_definition_;

      // Skip whitespace:
      remaining_definition_ += std::strspn(remaining_definition_, " \t");
      const bool is_constant_definition = *remaining_definition_ == '=';

      // Skip ahead to end of line:
      remaining_definition_ += std::strcspn(remaining_definition_, "\r\n");

      if (!is_constant_definition && start_of_member_name != end_of_member_name)
      {
        std::string member_name{start_of_member_name, end_of_member_name};
        return member_name;
      }
    }
  }

  ROS_ERROR_STREAM("message definition ended before decoding was finished, this should never happen");
  return {};
}

}
