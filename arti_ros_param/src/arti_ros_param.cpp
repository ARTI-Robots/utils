/// \file
/// \author Alexander Buchegger
/// \date 2021-12-09
/// \copyright ARTI - Autonomous Robot Technology GmbH. All rights reserved.
#include <arti_ros_param/arti_ros_param.h>
#include <limits>
#include <ros/console.h>
#include <sstream>

namespace arti_ros_param
{

bool hasParam(const ros::NodeHandle& node_handle, const std::string& param_name)
{
  return node_handle.hasParam(param_name);
}

bool hasParam(const XmlRpc::XmlRpcValue& xml_rpc_value, const std::string& param_name)
{
  return xml_rpc_value.hasMember(param_name);
}

}
