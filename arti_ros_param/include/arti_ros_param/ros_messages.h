/// \file
/// \author Alexander Buchegger
/// \date 2021-12-10
/// \copyright ARTI - Autonomous Robot Technology GmbH. All rights reserved.
#ifndef ARTI_ROS_PARAM_ROS_MESSAGES_H
#define ARTI_ROS_PARAM_ROS_MESSAGES_H

#include <arti_ros_param/collections.h>
#include <arti_ros_param/param.h>
#include <ros/builtin_message_traits.h>
#include <ros/message_traits.h>
#include <ros/serialization.h>
#include <ros/time.h>

namespace arti_ros_param
{
namespace detail
{

class RosMessageDecodingStream
{
public:
  RosMessageDecodingStream(const Param& param, const char* definition);

  template<typename T>
  void next(T& result)
  {
    param_[getNextMemberName()].decodeInto<T>(result);
  }

protected:
  ///
  /// \brief Finds next member name in message definition.
  ///
  std::string getNextMemberName();

  const Param& param_;
  const char* remaining_definition_;
};

}


template<typename T>
struct Codec<T, typename std::enable_if<ros::message_traits::IsMessage<T>::value>::type>
{
  static boost::optional<T> decode(const Param& param)
  {
    if (param.isStruct())
    {
      T result;
      detail::RosMessageDecodingStream stream{param, ros::message_traits::definition<T>()};
      ros::serialization::deserialize(stream, result);
      return result;
    }

    if (param.exists())
    {
      param.handleTypeError("expected struct");
    }

    return boost::none;
  }
};

template<>
boost::optional<ros::Time> decodeParam<ros::Time>(const Param& param)
{
  // While ROS defines a read() function for Time, it is not declared as a ROS message and has no definition:
  ros::Time result;
  detail::RosMessageDecodingStream stream{param, "uint32 sec\nuint32 nsec"};
  ros::serialization::deserialize(stream, result);
  return result;
}

template<>
boost::optional<ros::Duration> decodeParam<ros::Duration>(const Param& param)
{
  // While ROS defines a read() function for Duration, it is not declared as a ROS message and has no definition:
  ros::Duration result;
  detail::RosMessageDecodingStream stream{param, "uint32 sec\nuint32 nsec"};
  ros::serialization::deserialize(stream, result);
  return result;
}

}

#endif // ARTI_ROS_PARAM_ROS_MESSAGES_H
