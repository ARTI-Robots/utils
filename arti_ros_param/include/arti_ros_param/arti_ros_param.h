/// \file
/// \author Alexander Buchegger
/// \date 2021-12-09
/// \copyright ARTI - Autonomous Robot Technology GmbH. All rights reserved.
#ifndef ARTI_ROS_PARAM_ARTI_ROS_PARAM_H
#define ARTI_ROS_PARAM_ARTI_ROS_PARAM_H

#include <arti_ros_param/param.h>
#include <boost/optional.hpp>
#include <ros/macros.h>
#include <ros/node_handle.h>
#include <string>
#include <utility>
#include <xmlrpcpp/XmlRpcValue.h>

namespace arti_ros_param
{

template<typename T>
boost::optional<T> getParam(
  const XmlRpc::XmlRpcValue& value, const TypeErrorStrategy& type_error_strategy = LOG_TYPE_ERROR)
{
  return Param{{nullptr, {}}, value, type_error_strategy}.decode<T>();
}

template<typename T>
boost::optional<T> getParam(
  const XmlRpc::XmlRpcValue& value, const std::string& param_name,
  const TypeErrorStrategy& type_error_strategy = LOG_TYPE_ERROR)
{
  if (!value.hasMember(param_name))
  {
    return boost::none;
  }

  return Param{{nullptr, param_name}, value[param_name], type_error_strategy}.decode<T>();
}

template<typename T>
boost::optional<T> getParam(
  const ros::NodeHandle& node_handle, const TypeErrorStrategy& type_error_strategy = LOG_TYPE_ERROR)
{
  return Param{{nullptr, node_handle.getNamespace()}, node_handle.param<XmlRpc::XmlRpcValue>({}, {}),
               type_error_strategy}.decode<T>();
}

template<typename T>
boost::optional<T> getParam(
  const ros::NodeHandle& node_handle, const std::string& param_name,
  const TypeErrorStrategy& type_error_strategy = LOG_TYPE_ERROR)
{
  return ::arti_ros_param::getParam<T>(ros::NodeHandle{node_handle, param_name}, type_error_strategy);
}

template<typename T>
bool getParamInto(
  const ros::NodeHandle& node_handle, const TypeErrorStrategy& type_error_strategy, T& result)
{
  boost::optional<T> value = ::arti_ros_param::getParam<T>(node_handle, type_error_strategy);
  if (value)
  {
    result = std::move(*value);
    return true;
  }
  return false;
}

template<typename T>
bool getParamInto(const ros::NodeHandle& node_handle, T& result)
{
  return ::arti_ros_param::getParamInto<T>(node_handle, LOG_TYPE_ERROR, result);
}

template<typename T>
bool getParamInto(const XmlRpc::XmlRpcValue& xml_rpc_value, const TypeErrorStrategy& type_error_strategy, T& result)
{
  boost::optional<T> value = ::arti_ros_param::getParam<T>(xml_rpc_value, type_error_strategy);
  if (value)
  {
    result = std::move(*value);
    return true;
  }
  return false;
}

template<typename T>
bool getParamInto(const XmlRpc::XmlRpcValue& value, T& result)
{
  return ::arti_ros_param::getParamInto<T>(value, LOG_TYPE_ERROR, result);
}

template<typename T>
bool getParamInto(
  const XmlRpc::XmlRpcValue& xml_rpc_value, const std::string& param_name, const TypeErrorStrategy& type_error_strategy,
  T& result)
{
  boost::optional<T> value = ::arti_ros_param::getParam<T>(xml_rpc_value, param_name, type_error_strategy);
  if (value)
  {
    result = std::move(*value);
    return true;
  }
  return false;
}

template<typename T>
bool getParamInto(const XmlRpc::XmlRpcValue& value, const std::string& param_name, T& result)
{
  return ::arti_ros_param::getParamInto<T>(value, param_name, LOG_TYPE_ERROR, result);
}

template<typename T>
bool getParamInto(
  const ros::NodeHandle& node_handle, const std::string& param_name, const TypeErrorStrategy& type_error_strategy,
  T& result)
{
  boost::optional<T> value = ::arti_ros_param::getParam<T>(node_handle, param_name, type_error_strategy);
  if (value)
  {
    result = std::move(*value);
    return true;
  }
  return false;
}

template<typename T>
bool getParamInto(const ros::NodeHandle& node_handle, const std::string& param_name, T& result)
{
  return ::arti_ros_param::getParamInto<T>(node_handle, param_name, LOG_TYPE_ERROR, result);
}

template<typename T>
T getOptionalParam(
  const ros::NodeHandle& node_handle, const T& default_value,
  const TypeErrorStrategy& type_error_strategy = LOG_TYPE_ERROR)
{
  boost::optional<T> value = ::arti_ros_param::getParam<T>(node_handle, type_error_strategy);
  if (value)
  {
    return std::move(*value);
  }
  return default_value;
}

template<typename T>
T getOptionalParam(
  const ros::NodeHandle& node_handle, const std::string& param_name, const T& default_value,
  const TypeErrorStrategy& type_error_strategy = LOG_TYPE_ERROR)
{
  return ::arti_ros_param::getOptionalParam<T>(ros::NodeHandle{node_handle, param_name}, default_value,
                                               type_error_strategy);
}

template<typename T>
T getOptionalParam(
  const XmlRpc::XmlRpcValue& xml_rpc_value, const std::string& param_name, const T& default_value,
  const TypeErrorStrategy& type_error_strategy = LOG_TYPE_ERROR)
{
  boost::optional<T> value = ::arti_ros_param::getParam<T>(xml_rpc_value, param_name, type_error_strategy);
  if (value)
  {
    return std::move(*value);
  }
  return default_value;
}

template<typename T>
T getOptionalParam(
  const XmlRpc::XmlRpcValue& xml_rpc_value, const T& default_value,
  const TypeErrorStrategy& type_error_strategy = LOG_TYPE_ERROR)
{
  boost::optional<T> value = ::arti_ros_param::getParam<T>(xml_rpc_value, type_error_strategy);
  if (value)
  {
    return std::move(*value);
  }
  return default_value;
}

template<typename T>
void getOptionalParamInto(
  const ros::NodeHandle& node_handle, const T& default_value, const TypeErrorStrategy& type_error_strategy, T& result)
{
  result = ::arti_ros_param::getOptionalParam<T>(node_handle, default_value, type_error_strategy);
}

template<typename T>
void getOptionalParamInto(const ros::NodeHandle& node_handle, const T& default_value, T& result)
{
  return ::arti_ros_param::getOptionalParamInto<T>(node_handle, default_value, LOG_TYPE_ERROR, result);
}

template<typename T>
void getOptionalParamInto(
  const ros::NodeHandle& node_handle, const std::string& param_name, const T& default_value,
  const TypeErrorStrategy& type_error_strategy, T& result)
{
  result = ::arti_ros_param::getOptionalParam<T>(node_handle, param_name, default_value, type_error_strategy);
}

template<typename T>
void getOptionalParamInto(
  const ros::NodeHandle& node_handle, const std::string& param_name, const T& default_value, T& result)
{
  return ::arti_ros_param::getOptionalParamInto<T>(node_handle, param_name, default_value, LOG_TYPE_ERROR, result);
}

template<typename T>
void getOptionalParamInto(
  const XmlRpc::XmlRpcValue& xml_rpc_value, const T& default_value, const TypeErrorStrategy& type_error_strategy,
  T& result)
{
  result = ::arti_ros_param::getOptionalParam<T>(xml_rpc_value, default_value, type_error_strategy);
}

template<typename T>
void getOptionalParamInto(
  const XmlRpc::XmlRpcValue& xml_rpc_value, const T& default_value, T& result)
{
  return ::arti_ros_param::getOptionalParamInto<T>(xml_rpc_value, default_value, LOG_TYPE_ERROR, result);
}

template<typename T>
void getOptionalParamInto(
  const XmlRpc::XmlRpcValue& xml_rpc_value, const std::string& param_name, const T& default_value,
  const TypeErrorStrategy& type_error_strategy, T& result)
{
  result = ::arti_ros_param::getOptionalParam<T>(xml_rpc_value, param_name, default_value, type_error_strategy);
}

template<typename T>
void getOptionalParamInto(
  const XmlRpc::XmlRpcValue& xml_rpc_value, const std::string& param_name, const T& default_value, T& result)
{
  return ::arti_ros_param::getOptionalParamInto<T>(xml_rpc_value, param_name, default_value, LOG_TYPE_ERROR, result);
}

template<typename T>
T getRequiredParam(const ros::NodeHandle& node_handle, const TypeErrorStrategy& type_error_strategy = LOG_TYPE_ERROR)
{
  const XmlRpc::XmlRpcValue xml_rpc_value{node_handle.param<XmlRpc::XmlRpcValue>({}, {})};
  Param param{{nullptr, node_handle.getNamespace()}, xml_rpc_value, type_error_strategy};

  boost::optional<T> value = param.decode<T>();
  if (value)
  {
    return std::move(*value);
  }
  param.handleTypeError("required parameter not available in " + node_handle.getNamespace());

  return T();
}

template<typename T>
T getRequiredParam(
  const ros::NodeHandle& node_handle, const std::string& param_name,
  const TypeErrorStrategy& type_error_strategy = LOG_TYPE_ERROR)
{
  return std::move(::arti_ros_param::getRequiredParam<T>(ros::NodeHandle{node_handle, param_name}, type_error_strategy));
}

template<typename T>
T getRequiredParam(
  const XmlRpc::XmlRpcValue& xml_rpc_value, const TypeErrorStrategy& type_error_strategy = LOG_TYPE_ERROR)
{
  Param param{{nullptr, {}}, xml_rpc_value, type_error_strategy};

  boost::optional<T> value = param.decode<T>();
  if (value)
  {
    return std::move(*value);
  }
  param.handleTypeError("required parameter not available");

  return T();
}

template<typename T>
T getRequiredParam(
  const XmlRpc::XmlRpcValue& xml_rpc_value, const std::string& param_name,
  const TypeErrorStrategy& type_error_strategy = LOG_TYPE_ERROR)
{
  if (!xml_rpc_value.hasMember(param_name))
  {
    Param{{nullptr, param_name}, xml_rpc_value, type_error_strategy}.handleTypeError(
      "required parameter not available in " + param_name);
    return T();
  }

  Param param{{nullptr, param_name}, xml_rpc_value[param_name], type_error_strategy};

  boost::optional<T> value = param.decode<T>();
  if (value)
  {
    return std::move(*value);
  }
  param.handleTypeError("required parameter not available in " + param_name);

  return T();
}

template<typename T>
void getRequiredParamInto(const ros::NodeHandle& node_handle, const TypeErrorStrategy& type_error_strategy, T& result)
{
  result = std::move(::arti_ros_param::getRequiredParam<T>(node_handle, type_error_strategy));
}

template<typename T>
void getRequiredParamInto(const ros::NodeHandle& node_handle, T& result)
{
  return ::arti_ros_param::getRequiredParamInto<T>(node_handle, LOG_TYPE_ERROR, result);
}

template<typename T>
void getRequiredParamInto(
  const ros::NodeHandle& node_handle, const std::string& param_name, const TypeErrorStrategy& type_error_strategy,
  T& result)
{
  result = std::move(::arti_ros_param::getRequiredParam<T>(node_handle, param_name, type_error_strategy));
}

template<typename T>
void getRequiredParamInto(const ros::NodeHandle& node_handle, const std::string& param_name, T& result)
{
  ::arti_ros_param::getRequiredParamInto<T>(node_handle, param_name, LOG_TYPE_ERROR, result);
}

template<typename T>
void getRequiredParamInto(
  const XmlRpc::XmlRpcValue& xml_rpc_value, const TypeErrorStrategy& type_error_strategy, T& result)
{
  result = std::move(::arti_ros_param::getRequiredParam<T>(xml_rpc_value, type_error_strategy));
}

template<typename T>
void getRequiredParamInto(const XmlRpc::XmlRpcValue& xml_rpc_value, T& result)
{
  return ::arti_ros_param::getRequiredParamInto<T>(xml_rpc_value, LOG_TYPE_ERROR, result);
}

template<typename T>
void getRequiredParamInto(
  const XmlRpc::XmlRpcValue& xml_rpc_value, const std::string& param_name, const TypeErrorStrategy& type_error_strategy,
  T& result)
{
  result = std::move(::arti_ros_param::getRequiredParam<T>(xml_rpc_value, param_name, type_error_strategy));
}

template<typename T>
void getRequiredParamInto(const XmlRpc::XmlRpcValue& xml_rpc_value, const std::string& param_name, T& result)
{
  return ::arti_ros_param::getRequiredParamInto<T>(xml_rpc_value, param_name, LOG_TYPE_ERROR, result);
}

bool hasParam(const ros::NodeHandle& node_handle, const std::string& param_name);

bool hasParam(const XmlRpc::XmlRpcValue& xml_rpc_value, const std::string& param_name);

static inline bool hasValue(const ros::NodeHandle& node_handle, const std::string& param_name) ROS_DEPRECATED;

static inline bool hasValue(const ros::NodeHandle& node_handle, const std::string& param_name)
{
  return ::arti_ros_param::hasParam(node_handle, param_name);
}

static inline bool hasValue(const XmlRpc::XmlRpcValue& xml_rpc_value, const std::string& param_name) ROS_DEPRECATED;

static inline bool hasValue(const XmlRpc::XmlRpcValue& xml_rpc_value, const std::string& param_name)
{
  return ::arti_ros_param::hasParam(xml_rpc_value, param_name);
}

}

#endif //ARTI_ROS_PARAM_ARTI_ROS_PARAM_H
