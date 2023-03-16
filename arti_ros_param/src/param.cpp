/// \file
/// \author Alexander Buchegger
/// \date 2021-12-26
/// \copyright ARTI - Autonomous Robot Technology GmbH. All rights reserved.
#include <arti_ros_param/param.h>
#include <iostream>
#include <ros/node_handle.h>

namespace arti_ros_param
{

const XmlRpc::XmlRpcValue Param::INVALID_VALUE;

Param::Param(Path path, const XmlRpc::XmlRpcValue& value, const TypeErrorStrategy& type_error_strategy)
  : path_(std::move(path)), value_(value), type_error_strategy_(type_error_strategy)
{
}

const Param::Path& Param::getPath() const
{
  return path_;
}

const XmlRpc::XmlRpcValue& Param::getValue() const
{
  return value_;
}

XmlRpc::XmlRpcValue::Type Param::getType() const
{
  return value_.getType();
}

size_t Param::getSize() const
{
  return value_.size();
}

bool Param::exists() const
{
  return value_.valid();
}

bool Param::isScalar() const
{
  switch (value_.getType())
  {
    case XmlRpc::XmlRpcValue::TypeInvalid:
      break;

    case XmlRpc::XmlRpcValue::TypeBoolean:
    case XmlRpc::XmlRpcValue::TypeInt:
    case XmlRpc::XmlRpcValue::TypeDouble:
    case XmlRpc::XmlRpcValue::TypeString:
    case XmlRpc::XmlRpcValue::TypeDateTime:
    case XmlRpc::XmlRpcValue::TypeBase64:
      return true;

    case XmlRpc::XmlRpcValue::TypeArray:
    case XmlRpc::XmlRpcValue::TypeStruct:
      break;
  }

  return false;
}

bool Param::isCollection() const
{
  return value_.getType() == XmlRpc::XmlRpcValue::TypeArray || value_.getType() == XmlRpc::XmlRpcValue::TypeStruct;
}

bool Param::isArray() const
{
  return value_.getType() == XmlRpc::XmlRpcValue::TypeArray;
}

bool Param::isStruct() const
{
  return value_.getType() == XmlRpc::XmlRpcValue::TypeStruct;
}

Param Param::operator[](size_t array_index) const
{
  if (value_.getType() == XmlRpc::XmlRpcValue::TypeArray && array_index < static_cast<size_t>(value_.size()))
  {
    return {{&path_, std::to_string(array_index)}, value_[static_cast<int>(array_index)], type_error_strategy_};
  }

  return {{&path_, std::to_string(array_index)}, INVALID_VALUE, type_error_strategy_};
}

Param Param::operator[](std::string struct_member_name) const
{
  if (value_.getType() == XmlRpc::XmlRpcValue::TypeStruct && value_.hasMember(struct_member_name))
  {
    const XmlRpc::XmlRpcValue& value = value_[struct_member_name];
    return {{&path_, std::move(struct_member_name)}, value, type_error_strategy_};
  }

  return {{&path_, std::move(struct_member_name)}, INVALID_VALUE, type_error_strategy_};
}

Param Param::operator[](const XmlRpc::XmlRpcValue::const_iterator& struct_iterator) const
{
  if (value_.getType() == XmlRpc::XmlRpcValue::TypeStruct && struct_iterator != value_.end())
  {
    return {{&path_, struct_iterator->first}, struct_iterator->second, type_error_strategy_};
  }

  return {{&path_, "-"}, INVALID_VALUE, type_error_strategy_};
}

Param::Iterator Param::begin() const
{
  switch (value_.getType())
  {
    case XmlRpc::XmlRpcValue::TypeInvalid:
    case XmlRpc::XmlRpcValue::TypeBoolean:
    case XmlRpc::XmlRpcValue::TypeInt:
    case XmlRpc::XmlRpcValue::TypeDouble:
    case XmlRpc::XmlRpcValue::TypeString:
    case XmlRpc::XmlRpcValue::TypeDateTime:
    case XmlRpc::XmlRpcValue::TypeBase64:
      break;

    case XmlRpc::XmlRpcValue::TypeArray:
      return {this, 0};

    case XmlRpc::XmlRpcValue::TypeStruct:
      return {this, value_.begin()};
  }

  // Return invalid iterator:
  return {this, -1};
}

Param::Iterator Param::end() const
{
  switch (value_.getType())
  {
    case XmlRpc::XmlRpcValue::TypeInvalid:
    case XmlRpc::XmlRpcValue::TypeBoolean:
    case XmlRpc::XmlRpcValue::TypeInt:
    case XmlRpc::XmlRpcValue::TypeDouble:
    case XmlRpc::XmlRpcValue::TypeString:
    case XmlRpc::XmlRpcValue::TypeDateTime:
    case XmlRpc::XmlRpcValue::TypeBase64:
      break;

    case XmlRpc::XmlRpcValue::TypeArray:
      return {this, value_.size()};

    case XmlRpc::XmlRpcValue::TypeStruct:
      return {this, value_.end()};
  }

  // Return invalid iterator:
  return {this, -1};
}

void Param::handleTypeError(const std::string& error_message) const
{
  type_error_strategy_(*this, error_message);
}


std::string Param::Path::toString() const
{
  std::ostringstream s;
  s << *this;
  return s.str();
}

std::ostream& operator<<(std::ostream& out, const Param::Path& path)
{
  if (path.parent != nullptr)
  {
    out << *path.parent << '/' << path.key;
  }
  else
  {
    // Because a path starts with a ROS namespace, it already contains an initial '/', so we don't add another one:
    out << path.key;
  }
  return out;
}


bool Param::Iterator::operator==(const Param::Iterator& other) const
{
  return container_ == other.container_ && array_index_ == other.array_index_
         && struct_iterator_ == other.struct_iterator_;
}

bool Param::Iterator::operator!=(const Param::Iterator& other) const
{
  return !(*this == other);
}

Param Param::Iterator::operator*() const
{
  return container_->isArray() ? (*container_)[array_index_] : (*container_)[struct_iterator_];
}

void Param::Iterator::operator++()
{
  if (container_->isArray())
  {
    ++array_index_;
  }
  else
  {
    ++struct_iterator_;
  }
}

Param::Iterator::Iterator(const Param* container, int array_index)
  : container_(container), array_index_(array_index)
{
}

Param::Iterator::Iterator(const Param* container, const XmlRpc::XmlRpcValue::const_iterator& struct_iterator)
  : container_(container), array_index_(0), struct_iterator_(struct_iterator)
{
}


RootParam::RootParam(std::string path, const XmlRpc::XmlRpcValue& value, const TypeErrorStrategy& type_error_strategy)
  : Param({nullptr, std::move(path)}, value_copy_, type_error_strategy), value_copy_(value)
{
}

RootParam::RootParam(const XmlRpc::XmlRpcValue& value, const TypeErrorStrategy& type_error_strategy)
  : RootParam({}, value, type_error_strategy)
{
}

RootParam::RootParam(const ros::NodeHandle& node_handle, const TypeErrorStrategy& type_error_strategy)
  : RootParam(node_handle.getNamespace(), node_handle.param<XmlRpc::XmlRpcValue>({}, {}), type_error_strategy)
{
}

RootParam::RootParam(const Param& param)
  : RootParam(param.getPath().toString(), param.getValue(), param.type_error_strategy_)
{
}

}
