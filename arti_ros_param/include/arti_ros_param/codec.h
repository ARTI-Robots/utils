/// \file
/// \author Alexander Buchegger
/// \date 2021-12-26
/// \copyright ARTI - Autonomous Robot Technology GmbH. All rights reserved.
#ifndef ARTI_ROS_PARAM_CODEC_H
#define ARTI_ROS_PARAM_CODEC_H

#include <arti_ros_param/forward_declarations.h>
#include <boost/optional.hpp>
#include <limits>
#include <type_traits>
#include <XmlRpcValue.h>

namespace arti_ros_param
{

template<typename T>
boost::optional<T> decodeParam(const Param& param)
{
  return Codec<T>::decode(param);
}

template<>
boost::optional<XmlRpc::XmlRpcValue> decodeParam<XmlRpc::XmlRpcValue>(const Param& param);

template<>
boost::optional<bool> decodeParam<bool>(const Param& param);

template<>
boost::optional<double> decodeParam<double>(const Param& param);

template<>
boost::optional<int> decodeParam<int>(const Param& param);

template<>
boost::optional<std::string> decodeParam<std::string>(const Param& param);

template<typename T>
struct Codec<T, typename std::enable_if<std::is_integral<T>::value && std::is_signed<T>::value>::type>
{
  static boost::optional<T> decode(const Param& param)
  {
    const boost::optional<int> value = ::arti_ros_param::decodeParam<int>(param);
    if (value && std::numeric_limits<T>::lowest() <= *value && *value <= std::numeric_limits<T>::max())
    {
      return static_cast<T>(value);
    }
    return boost::none;
  }
};

template<typename T>
struct Codec<T, typename std::enable_if<std::is_integral<T>::value && !std::is_signed<T>::value>::type>
{
  static boost::optional<T> decode(const Param& param)
  {
    const boost::optional<int> value = ::arti_ros_param::decodeParam<int>(param);
    if (value && 0 <= *value && static_cast<unsigned int>(*value) <= std::numeric_limits<T>::max())
    {
      return static_cast<T>(*value);
    }
    return boost::none;
  }
};

template<typename T>
struct Codec<T, typename std::enable_if<std::is_floating_point<T>::value>::type>
{
  static boost::optional<T> decode(const Param& param)
  {
    const boost::optional<double> value = ::arti_ros_param::decodeParam<double>(param);
    if (value)
    {
      return static_cast<T>(*value);
    }
    return boost::none;
  }
};

}

#endif // ARTI_ROS_PARAM_CODEC_H
