/// \file
/// \author Alexander Buchegger
/// \date 2021-12-26
/// \copyright ARTI - Autonomous Robot Technology GmbH. All rights reserved.
#include <arti_ros_param/codec.h>
#include <arti_ros_param/param.h>

namespace arti_ros_param
{

template<>
boost::optional<XmlRpc::XmlRpcValue> decodeParam<XmlRpc::XmlRpcValue>(const Param& param)
{
  return param.getValue();
}

template<>
boost::optional<bool> decodeParam<bool>(const Param& param)
{
  switch (param.getType())
  {
    case XmlRpc::XmlRpcValue::TypeInvalid:
      return boost::none;

    case XmlRpc::XmlRpcValue::TypeBoolean:
      return param.cast<bool>();

    case XmlRpc::XmlRpcValue::TypeInt:
    {
      const int value = param.cast<int>();
      if (value == 0)
      {
        return false;
      }
      if (value == 1)
      {
        return true;
      }
      break;
    }

    case XmlRpc::XmlRpcValue::TypeDouble:
      break;

    case XmlRpc::XmlRpcValue::TypeString:
    {
      const auto& value = param.cast<std::string>();
      if (value == "true" || value == "True")
      {
        return true;
      }
      if (value == "false" || value == "False")
      {
        return false;
      }
      break;
    }

    case XmlRpc::XmlRpcValue::TypeDateTime:
    case XmlRpc::XmlRpcValue::TypeBase64:
    case XmlRpc::XmlRpcValue::TypeArray:
    case XmlRpc::XmlRpcValue::TypeStruct:
      break;
  }

  param.handleTypeError("expected boolean or convertible-to-boolean");
  return boost::none;
}

template<>
boost::optional<int> decodeParam<int>(const Param& param)
{
  if (param.getType() == XmlRpc::XmlRpcValue::TypeInt)
  {
    return param.cast<int>();
  }

  if (param.exists())
  {
    param.handleTypeError("expected integer");
  }

  return boost::none;
}

template<>
boost::optional<double> decodeParam<double>(const Param& param)
{
  switch (param.getType())
  {
    case XmlRpc::XmlRpcValue::TypeInvalid:
      return boost::none;

    case XmlRpc::XmlRpcValue::TypeBoolean:
      break;

    case XmlRpc::XmlRpcValue::TypeInt:
      return param.cast<int>();

    case XmlRpc::XmlRpcValue::TypeDouble:
      return param.cast<double>();

    case XmlRpc::XmlRpcValue::TypeString:
    {
      const auto& value = param.cast<std::string>();
      if (value == ".inf")
      {
        return std::numeric_limits<double>::infinity();
      }
      if (value == "-.inf")
      {
        return -std::numeric_limits<double>::infinity();
      }
      if (value == ".nan")
      {
        return std::numeric_limits<double>::quiet_NaN();
      }
      break;
    }

    case XmlRpc::XmlRpcValue::TypeDateTime:
    case XmlRpc::XmlRpcValue::TypeBase64:
    case XmlRpc::XmlRpcValue::TypeArray:
    case XmlRpc::XmlRpcValue::TypeStruct:
      break;
  }

  param.handleTypeError("expected floating-point or convertible-to-floating-point");
  return boost::none;
}

template<>
boost::optional<std::string> decodeParam<std::string>(const Param& param)
{
  switch (param.getType())
  {
    case XmlRpc::XmlRpcValue::TypeInvalid:
      return boost::none;

    case XmlRpc::XmlRpcValue::TypeString:
      return param.cast<std::string>();

    case XmlRpc::XmlRpcValue::TypeBoolean:
    case XmlRpc::XmlRpcValue::TypeInt:
    case XmlRpc::XmlRpcValue::TypeDouble:
    case XmlRpc::XmlRpcValue::TypeDateTime:
    case XmlRpc::XmlRpcValue::TypeBase64:
    case XmlRpc::XmlRpcValue::TypeArray:
    case XmlRpc::XmlRpcValue::TypeStruct:
    {
      std::ostringstream s;
      s << param.getValue();
      return s.str();
    }
  }

  param.handleTypeError("expected string or convertible-to-string");
  return boost::none;
}

}
