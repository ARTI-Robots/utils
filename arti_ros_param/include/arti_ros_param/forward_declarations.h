/// \file
/// \author Alexander Buchegger
/// \date 2023-01-25
/// \copyright ARTI - Autonomous Robot Technology GmbH. All rights reserved.
#ifndef ARTI_ROS_PARAM_FORWARD_DECLARATIONS_H
#define ARTI_ROS_PARAM_FORWARD_DECLARATIONS_H

namespace arti_ros_param
{

class Param;

class RootParam;

template<typename T, typename Enable=void>
struct Codec;

}

#endif //ARTI_ROS_PARAM_FORWARD_DECLARATIONS_H
