/// \file
/// \author Alexander Buchegger
/// \date 2023-01-18
/// \copyright ARTI - Autonomous Robot Technology GmbH. All rights reserved.
#ifndef ARTI_ROS_PARAM_YAML_H
#define ARTI_ROS_PARAM_YAML_H

#include <arti_ros_param/param.h>
#include <arti_ros_param/type_error_strategy.h>
#include <iosfwd>
#include <string>

namespace arti_ros_param
{

/**
 * Loads parameters from a YAML-encoded stream (e.g. a YAML file or string stream).
 *
 * \param in the input stream providing the YAML-encoded parameters.
 * \param fake_path the path of the data source corresponding to the input stream (only used for error messages).
 * \param type_error_strategy called when an error occurs, e.g. during parsing or conversion from YAML to
 *   XmlRpc::XmlRpcValue.
 *
 * \returns loaded data as a RootParam (wrapping an XmlRpc::XmlRpcValue) if successful,
 *          or an invalid RootParam (check via RootParam::exists()) otherwise.
 */
RootParam loadYaml(
  std::istream& in, std::string fake_path = {}, const TypeErrorStrategy& type_error_strategy = LOG_TYPE_ERROR);

/**
 * Loads parameters from a YAML file.
 *
 * \param path the path of the YAML file.
 * \param type_error_strategy called when an error occurs, e.g. during parsing or conversion from YAML to
 *   XmlRpc::XmlRpcValue.
 *
 * \returns loaded data as a RootParam (wrapping an XmlRpc::XmlRpcValue) if successful,
 *          or an invalid RootParam (check via RootParam::exists()) otherwise.
 */
RootParam loadYaml(const std::string& path, const TypeErrorStrategy& type_error_strategy = LOG_TYPE_ERROR);

}

#endif // ARTI_ROS_PARAM_YAML_H
