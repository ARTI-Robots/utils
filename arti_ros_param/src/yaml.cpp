/// \file
/// \author Alexander Buchegger
/// \date 2023-01-18
/// \copyright ARTI - Autonomous Robot Technology GmbH. All rights reserved.
#include <arti_ros_param/yaml.h>
#include <fstream>
#include <limits>
#include <stack>
#include <stdexcept>
#include <vector>
#include <yaml-cpp/eventhandler.h>
#include <yaml-cpp/exceptions.h>
#include <yaml-cpp/parser.h>

namespace arti_ros_param
{
namespace detail
{

class YamlToXmlRpcValueEventHandler : public YAML::EventHandler
{
public:
  XmlRpc::XmlRpcValue& getValue()
  {
    return value_;
  }

  void OnDocumentStart(const YAML::Mark& /*mark*/) override
  {
    // Ignored.
  }

  void OnDocumentEnd() override
  {
    // Ignored.
  }

  void OnNull(const YAML::Mark& mark, YAML::anchor_t anchor) override
  {
    OnScalar(mark, {}, anchor, "null");
  }

  void OnAlias(const YAML::Mark& /*mark*/, YAML::anchor_t /*anchor*/) override
  {
    // Ignored.
  }

  void OnScalar(
    const YAML::Mark& mark, const std::string& tag, YAML::anchor_t /*anchor*/,
    const std::string& value) override
  {
    if (container_stack_.empty())
    {
      std::ostringstream msg;
      msg << mark.line << ':' << mark.column << ": scalar cannot be added after end of document";
      throw std::logic_error(msg.str());
    }

    switch (container_stack_.top()->getType())
    {
      case XmlRpc::XmlRpcValue::TypeInvalid:
        // This means it is a map value, or the root value:
        parse(tag, value, *container_stack_.top());
        container_stack_.pop();
        break;

      case XmlRpc::XmlRpcValue::TypeArray:
        parse(tag, value, (*container_stack_.top())[container_stack_.top()->size()]);
        break;

      case XmlRpc::XmlRpcValue::TypeStruct:
      {
        // The current value is the map key; push a container for the map value:
        XmlRpc::XmlRpcValue& new_container = (*container_stack_.top())[value];
        container_stack_.emplace(&new_container);
        break;
      }

      default:
      {
        std::ostringstream msg;
        msg << mark.line << ':' << mark.column << ": value cannot be assigned to variable of type "
            << getTypeName(container_stack_.top()->getType());
        throw std::logic_error(msg.str());
      }
    }
  }

  void OnSequenceStart(
    const YAML::Mark& mark, const std::string& /*tag*/, YAML::anchor_t /*anchor*/,
    YAML::EmitterStyle::value /*style*/) override
  {
    pushContainer(mark);
    container_stack_.top()->setSize(0);  // Make array
  }

  void OnSequenceEnd() override
  {
    if (!container_stack_.empty() && container_stack_.top()->getType() == XmlRpc::XmlRpcValue::TypeArray)
    {
      container_stack_.pop();
    }
    else
    {
      throw std::logic_error("invalid end of sequence");
    }
  }

  void OnMapStart(
    const YAML::Mark& mark, const std::string& /*tag*/, YAML::anchor_t /*anchor*/,
    YAML::EmitterStyle::value /*style*/) override
  {
    pushContainer(mark);
    container_stack_.top()->begin();  // Make struct
  }

  void OnMapEnd() override
  {
    if (!container_stack_.empty() && container_stack_.top()->getType() == XmlRpc::XmlRpcValue::TypeStruct)
    {
      container_stack_.pop();
    }
    else
    {
      throw std::logic_error("invalid end of map");
    }
  }

protected:
  static const char* getTypeName(const XmlRpc::XmlRpcValue::Type type)
  {
    switch (type)
    {
#define X(name) case XmlRpc::XmlRpcValue::Type::name: return &(#name)[4];  // Return name without the "Type" prefix
      X(TypeInvalid)
      X(TypeBoolean)
      X(TypeInt)
      X(TypeDouble)
      X(TypeString)
      X(TypeDateTime)
      X(TypeBase64)
      X(TypeArray)
      X(TypeStruct)
#undef X
    }
    return "UNKNOWN";
  }

  void pushContainer(const YAML::Mark& mark)
  {
    if (container_stack_.empty())
    {
      std::ostringstream msg;
      msg << mark.line << ':' << mark.column << ": collection (sequence or map) cannot be added after end of document";
      throw std::logic_error(msg.str());
    }

    switch (container_stack_.top()->getType())
    {
      case XmlRpc::XmlRpcValue::TypeInvalid:
        // This means it is a map value, or the root value.
        break;

      case XmlRpc::XmlRpcValue::TypeArray:
      {
        XmlRpc::XmlRpcValue& new_container = (*container_stack_.top())[container_stack_.top()->size()];
        container_stack_.emplace(&new_container);
        break;
      }

      default:
      {
        std::ostringstream msg;
        msg << mark.line << ':' << mark.column << ": collection (sequence or map) cannot be a map key";
        throw std::invalid_argument(msg.str());
      }
    }
  }

  static void parse(const std::string& tag, const std::string& value, XmlRpc::XmlRpcValue& result)
  {
    // Try to determine type of scalar and canonicalize special values (see
    // https://yaml.org/spec/1.2.2/#1032-tag-resolution):
    if (tag == "!")
    {
      // A verbatim string stays a string:
      result = value;
    }
    else if (value.empty())
    {
      // An empty string stays an empty string:
      result = value;
    }
    else if (value == "null" || value == "Null" || value == "NULL" || value == "~")
    {
      // Null becomes an invalid value:
      result = XmlRpc::XmlRpcValue{};
    }
    else if (value == "true" || value == "True" || value == "TRUE")
    {
      result = true;
    }
    else if (value == "false" || value == "False" || value == "FALSE")
    {
      result = false;
    }
    else if (value == ".nan" || value == ".NaN" || value == ".NAN")
    {
      result = std::numeric_limits<double>::quiet_NaN();
    }
    else if (value == ".inf" || value == ".Inf" || value == ".INF")
    {
      result = std::numeric_limits<double>::infinity();
    }
    else if ((value[0] == '+' || value[0] == '-')
             && (value.compare(1, std::string::npos, ".inf") == 0 || value.compare(1, std::string::npos, ".Inf") == 0
                 || value.compare(1, std::string::npos, ".INF") == 0))
    {
      result = (value[0] == '+') ? std::numeric_limits<double>::infinity() : -std::numeric_limits<double>::infinity();
    }
    else if (!parseNumber(value, result))
    {
      // Everything else is deemed a string:
      result = value;
    }
  }

  static bool parseNumber(const std::string& value, XmlRpc::XmlRpcValue& result)
  {
    if (value.empty())
    {
      return false;
    }

    if (value.find_first_of(".eE") != std::string::npos)
    {
      char* end_of_number{nullptr};
      const double number = std::strtod(value.c_str(), &end_of_number);
      if (end_of_number == value.c_str() + value.size())
      {
        result = number;
        return true;
      }
    }
    else
    {
      char* end_of_number{nullptr};
      const long number = std::strtol(value.c_str(), &end_of_number, 10);
      if (end_of_number == value.c_str() + value.size())
      {
        result = static_cast<int>(number);
        return true;
      }
    }
    return false;
  }

  std::stack<XmlRpc::XmlRpcValue*, std::vector<XmlRpc::XmlRpcValue*>> container_stack_{{&value_}};
  XmlRpc::XmlRpcValue value_;
};

}

RootParam loadYaml(std::istream& in, std::string fake_path, const TypeErrorStrategy& type_error_strategy)
{
  detail::YamlToXmlRpcValueEventHandler event_handler;
  try
  {
    YAML::Parser yaml_parser;
    yaml_parser.Load(in);
    if (!yaml_parser.HandleNextDocument(event_handler))
    {
      throw std::invalid_argument("syntax error");
    }
  }
  catch (const YAML::Exception& ex)
  {
    static const std::string prefix = "failed to parse YAML: ";
    RootParam result{std::move(fake_path), {}, type_error_strategy};
    result.handleTypeError(prefix + ex.what());
    return result;
  }
  catch (const std::exception& ex)
  {
    static const std::string prefix = "failed to convert YAML: ";
    RootParam result{std::move(fake_path), {}, type_error_strategy};
    result.handleTypeError(prefix + ex.what());
    return result;
  }
  catch (...)
  {
    RootParam result{std::move(fake_path), {}, type_error_strategy};
    result.handleTypeError("unknown error when loading YAML");
    return result;
  }
  return {std::move(fake_path), event_handler.getValue(), type_error_strategy};
}

RootParam loadYaml(const std::string& path, const TypeErrorStrategy& type_error_strategy)
{
  std::ifstream file_stream{path};
  return loadYaml(file_stream, path + ':', type_error_strategy);
}

}
