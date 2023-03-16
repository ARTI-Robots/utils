/// \file
/// \author Alexander Buchegger
/// \date 2023-01-19
/// \copyright ARTI - Autonomous Robot Technology GmbH. All rights reserved.
#include <arti_ros_param/yaml.h>
#include <gtest/gtest.h>
#include <sstream>
#include <xmlrpcpp/XmlRpcValue.h>

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

static void expect_eq(const XmlRpc::XmlRpcValue& expected, const XmlRpc::XmlRpcValue& actual)
{
  if (expected != actual)
  {
    std::ostringstream msg;
    msg << "expected " << getTypeName(expected.getType()) << '(' << expected << ')' << " != "
        << "actual " << getTypeName(actual.getType()) << '(' << actual << ')';
    GTEST_NONFATAL_FAILURE_(msg.str().c_str());
  }
}

TEST(TestYaml, testNull)
{
  std::istringstream yaml{"null"};
  expect_eq(XmlRpc::XmlRpcValue{}, arti_ros_param::loadYaml(yaml).getValue());
}

TEST(TestYaml, testIntegerScalar)
{
  std::istringstream yaml{"42"};
  expect_eq(XmlRpc::XmlRpcValue{42}, arti_ros_param::loadYaml(yaml).getValue());
}

TEST(TestYaml, testDoubleScalar1)
{
  std::istringstream yaml{"42.0"};
  expect_eq(XmlRpc::XmlRpcValue{42.0}, arti_ros_param::loadYaml(yaml).getValue());
}

TEST(TestYaml, testDoubleScalar2)
{
  std::istringstream yaml{"42e3"};
  expect_eq(XmlRpc::XmlRpcValue{42e3}, arti_ros_param::loadYaml(yaml).getValue());
}

TEST(TestYaml, testStringScalar)
{
  std::istringstream yaml{"'42'"};
  expect_eq(XmlRpc::XmlRpcValue{"42"}, arti_ros_param::loadYaml(yaml).getValue());
}

TEST(TestYaml, testEmptySequence)
{
  std::istringstream yaml{"[]"};
  XmlRpc::XmlRpcValue expected;
  expected.setSize(0);  // Make array
  expect_eq(expected, arti_ros_param::loadYaml(yaml).getValue());
}

TEST(TestYaml, testFlowStyleSequence1)
{
  std::istringstream yaml{"[42]"};
  XmlRpc::XmlRpcValue expected;
  expected[0] = 42;
  expect_eq(expected, arti_ros_param::loadYaml(yaml).getValue());
}

TEST(TestYaml, testFlowStyleSequence2)
{
  std::istringstream yaml{"[1, 2, 3]"};
  XmlRpc::XmlRpcValue expected;
  expected[0] = 1;
  expected[1] = 2;
  expected[2] = 3;
  expect_eq(expected, arti_ros_param::loadYaml(yaml).getValue());
}

TEST(TestYaml, testBlockStyleSequence)
{
  std::istringstream yaml{
    "- 1\n"
    "- 2\n"
    "- 3\n"
  };
  XmlRpc::XmlRpcValue expected;
  expected[0] = 1;
  expected[1] = 2;
  expected[2] = 3;
  expect_eq(expected, arti_ros_param::loadYaml(yaml).getValue());
}

TEST(TestYaml, testEmptyMap)
{
  std::istringstream yaml{"{}"};
  XmlRpc::XmlRpcValue expected;
  expected.begin();  // Make struct
  expect_eq(expected, arti_ros_param::loadYaml(yaml).getValue());
}

TEST(TestYaml, testFlowStyleMap1)
{
  std::istringstream yaml{"{a: 1}"};
  XmlRpc::XmlRpcValue expected;
  expected["a"] = 1;
  expect_eq(expected, arti_ros_param::loadYaml(yaml).getValue());
}

TEST(TestYaml, testFlowStyleMap2)
{
  std::istringstream yaml{"{a: 1, b: 2, c: 3}"};
  XmlRpc::XmlRpcValue expected;
  expected["a"] = 1;
  expected["b"] = 2;
  expected["c"] = 3;
  expect_eq(expected, arti_ros_param::loadYaml(yaml).getValue());
}

TEST(TestYaml, testBlockStyleMap)
{
  std::istringstream yaml{
    "a: 1\n"
    "b: 2\n"
    "c: 3\n"
  };
  XmlRpc::XmlRpcValue expected;
  expected["a"] = 1;
  expected["b"] = 2;
  expected["c"] = 3;
  expect_eq(expected, arti_ros_param::loadYaml(yaml).getValue());
}

TEST(TestYaml, testNestedContainers)
{
  std::istringstream yaml{
    "int: 1\n"
    "struct:\n"
    "  double: 2.0\n"
    "  sequence:\n"
    "    - 1\n"
    "    - 2\n"
    "  struct:\n"
    "    brackets: '[]'\n"
    "string: \"null\""
  };
  XmlRpc::XmlRpcValue expected;
  expected["int"] = 1;
  expected["struct"]["double"] = 2.0;
  expected["struct"]["sequence"][0] = 1;
  expected["struct"]["sequence"][1] = 2;
  expected["struct"]["struct"]["brackets"] = "[]";
  expected["string"] = "null";
  expect_eq(expected, arti_ros_param::loadYaml(yaml).getValue());
}

TEST(TestYaml, testInvalidYaml1)
{
  std::istringstream yaml{"{]"};
  arti_ros_param::LogTypeError log_silently{ros::console::Level::Debug};
  expect_eq(XmlRpc::XmlRpcValue{}, arti_ros_param::loadYaml(yaml, {}, log_silently).getValue());
}

TEST(TestYaml, testInvalidYaml2)
{
  std::istringstream yaml{"[a]: [b]"};
  arti_ros_param::LogTypeError log_silently{ros::console::Level::Debug};
  expect_eq(XmlRpc::XmlRpcValue{}, arti_ros_param::loadYaml(yaml, {}, log_silently).getValue());
}

int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
