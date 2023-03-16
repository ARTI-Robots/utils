/// \file
/// \author Alexander Buchegger
/// \date 2021-12-26
/// \copyright ARTI - Autonomous Robot Technology GmbH. All rights reserved.
#ifndef ARTI_ROS_PARAM_COLLECTIONS_H
#define ARTI_ROS_PARAM_COLLECTIONS_H

#include <arti_ros_param/param.h>
#include <list>
#include <map>
#include <set>
#include <vector>

namespace arti_ros_param
{

template<typename T, typename Allocator>
struct Codec<std::list<T, Allocator>>
{
static boost::optional<std::list<T, Allocator>> decode(const Param& param)
{
  if (!param.isArray())
  {
    return boost::none;
  }

  std::list<T, Allocator> list;
  for (const Param item : param)
  {
    boost::optional<T> value = ::arti_ros_param::decodeParam<T>(item);
    if (!value)
    {
      return boost::none;
    }
    list.emplace_back(std::move(*value));
  }
  return list;
}
};

template<typename K, typename T, typename Compare, typename Allocator>
struct Codec<std::map<K, T, Compare, Allocator>>
{
static boost::optional<std::map<K, T, Compare, Allocator>> decode(const Param& param)
{
  if (!param.isStruct())
  {
    return boost::none;
  }

  std::map<K, T, Compare, Allocator> map;
  for (const Param item : param)
  {
    boost::optional<T> value = ::arti_ros_param::decodeParam<T>(item);
    if (!value)
    {
      return boost::none;
    }
    map.emplace(item.getPath().key, std::move(*value));
  }
  return map;
}
};

template<typename T, typename Compare, typename Allocator>
struct Codec<std::set<T, Compare, Allocator>>
{
static boost::optional<std::set<T, Compare, Allocator>> decode(const Param& param)
{
  if (!param.isArray())
  {
    return boost::none;
  }

  std::set<T, Compare, Allocator> set;
  for (const Param item : param)
  {
    boost::optional<T> value = ::arti_ros_param::decodeParam<T>(item);
    if (!value)
    {
      return boost::none;
    }
    set.emplace(std::move(*value));
  }
  return set;
}
};

template<typename T, typename Allocator>
struct Codec<std::vector<T, Allocator>>
{
static boost::optional<std::vector<T, Allocator>> decode(const Param& param)
{
  if (!param.isArray())
  {
    return boost::none;
  }

  std::vector<T, Allocator> vector;
  vector.reserve(param.getSize());
  for (const Param item : param)
  {
    boost::optional<T> value = ::arti_ros_param::decodeParam<T>(item);
    if (!value)
    {
      return boost::none;
    }
    vector.emplace_back(std::move(*value));
  }
  return vector;
}
};

}

#endif // ARTI_ROS_PARAM_COLLECTIONS_H
