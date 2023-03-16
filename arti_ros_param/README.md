## General Description
The arti ros parameter package allow easy access to
parameters on the parameter server using the ros node handle
or using the xmprpc values used internally in ros for the
parameter updates.

There are several data types supported, primitive like bool, int, ... but also collections and even ros messages.
Therefore, if you want to retrieve a ros message from the paramter server you can simply use the ros message as type.
As structure on the parameter server the function assumes the structure of the ros message. 

### Usage
Below some simple examples:

First include the main file to retrieve data
```
#include <arti_ros_param/arti_ros_param.h>
```

If you want to retriev data from a collection you need also to include additionally
```
#include <arti_ros_param/collections.h>
```

#### Retrieve data
Now there are different methods to get data.

##### Retrieve required data
If you want to retrieve data which yield a fault if not existing you can use the following
```
XmlRpc::XmlRpcValue graph = arti_ros_param::getRequiredParam<XmlRpc::XmlRpcValue>(nh_, "graph");
```
This will retrieve the data with the name graph. In the example you can also see that you can retrieve xmlrpc values
allowing further queries

##### Retrieve optional data
If you want to retrieve optional values you have two options.
First option if you have a default vaue you can use the following
```
bool flag = arti_ros_param::getOptionalParam<bool>(graph, "flag", false);
```
Second option if you just want the value if exists and make other if not you can use the following
```
boost::optional<geometry_msgs::Point> origin = arti_ros_param::getParam<geometry_msgs::Point>(nh_, "origin");
```

#### Checking for data
There is also the option to check if data exists with the following call
```
bool has_sink = arti_ros_param::hasValue(root_node, "sink");
```



