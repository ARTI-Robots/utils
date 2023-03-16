/// \file
/// \author Alexander Buchegger
/// \date 2021-12-26
/// \copyright ARTI - Autonomous Robot Technology GmbH. All rights reserved.
#ifndef ARTI_ROS_PARAM_PARAM_H
#define ARTI_ROS_PARAM_PARAM_H

#include <arti_ros_param/codec.h>
#include <arti_ros_param/type_error_strategy.h>
#include <boost/optional.hpp>
#include <iosfwd>
#include <ros/forwards.h>
#include <string>
#include <utility>
#include <xmlrpcpp/XmlRpcValue.h>

namespace arti_ros_param
{

class Param
{
public:
  struct Path
  {
    const Path* parent;
    std::string key;

    std::string toString() const;

    friend std::ostream& operator<<(std::ostream& out, const Path& path);
  };

  class Iterator
  {
  public:
    bool operator==(const Iterator& other) const;
    bool operator!=(const Iterator& other) const;
    Param operator*() const;
    void operator++();

  protected:
    friend class Param;

    Iterator(const Param* container, int array_index);
    Iterator(const Param* container, const XmlRpc::XmlRpcValue::const_iterator& struct_iterator);

    const Param* container_;
    int array_index_;
    XmlRpc::XmlRpcValue::const_iterator struct_iterator_;
  };

  Param(Path path, const XmlRpc::XmlRpcValue& value, const TypeErrorStrategy& type_error_strategy);

  const Path& getPath() const;
  const XmlRpc::XmlRpcValue& getValue() const;
  XmlRpc::XmlRpcValue::Type getType() const;
  size_t getSize() const;

  bool exists() const;
  bool isScalar() const;
  bool isCollection() const;
  bool isArray() const;
  bool isStruct() const;

  template<typename T>
  const T& cast() const
  {
    // Ugly workaround for XmlRpcValue's missing const correctness:
    return static_cast<T&>(const_cast<XmlRpc::XmlRpcValue&>(value_));
  }

  template<typename T>
  boost::optional<T> decode() const
  {
    return decodeParam<T>(*this);
  }

  template<typename T>
  bool decodeInto(T& result)
  {
    boost::optional<T> value = decodeParam<T>(*this);
    if (value)
    {
      result = std::move(*value);
      return true;
    }
    return false;
  }

  Param operator[](size_t array_index) const;
  Param operator[](std::string struct_member_name) const;

  Iterator begin() const;
  Iterator end() const;

  void handleTypeError(const std::string& error_message) const;

protected:
  friend class RootParam;

  static const XmlRpc::XmlRpcValue INVALID_VALUE;

  Param operator[](const XmlRpc::XmlRpcValue::const_iterator& struct_iterator) const;

  Path path_;
  const XmlRpc::XmlRpcValue& value_;
  const TypeErrorStrategy& type_error_strategy_;
};


class RootParam : public Param
{
public:
  RootParam(
    std::string path, const XmlRpc::XmlRpcValue& value, const TypeErrorStrategy& type_error_strategy = LOG_TYPE_ERROR);
  explicit RootParam(const XmlRpc::XmlRpcValue& value, const TypeErrorStrategy& type_error_strategy = LOG_TYPE_ERROR);
  explicit RootParam(const ros::NodeHandle& node_handle, const TypeErrorStrategy& type_error_strategy = LOG_TYPE_ERROR);
  RootParam(const Param& param);

protected:
  XmlRpc::XmlRpcValue value_copy_;
};

}

#endif // ARTI_ROS_PARAM_PARAM_H
