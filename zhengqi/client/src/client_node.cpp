#include<ros/ros.h>
#include "client/Crist.h"
#include<iostream>
#include"client/topic_tim.h"

int main(int argc,char **argv)
{
    //初始化ros节点
    ros::init(argc,argv,"client_node");
    //创建节点句柄
    ros::NodeHandle node;
    //等待服务建立
    ros::service::waitForService("/show_name");
    //创建一个服务客户端，连接名为/show_name的服务
    ros::ServiceClient name_client=node.serviceClient<client::Crist>("/show_name");
    //初始化client::Crist的数据内容
    client::Crist srv;
    srv.request.name="Crist";
    //调用服务
    if(name_client.call(srv))
    {
        ROS_INFO("result: %s", srv.response.result.c_str());
        ROS_INFO("Succeed in calling service show_name");
    }
    else
    {
        ROS_ERROR("Failed to call service show_name");
        return 1;
    }
    //创建publisher，数据类型为client::topic_tim,发布名为"info_msg",队列长度10
    ros::Publisher inform_msg_pub=node.advertise<client::topic_tim>("info_msg",10);
    //设置循环频率
    ros::Rate loop_rate(1);
    while(ros::ok())
    {
        //初始化client::topic_tim的数据
        client::topic_tim information;
        information.name="crist";
        information.time_now=ros::Time::now();
        //发布消息
        inform_msg_pub.publish(information);

        ROS_INFO("publish information:  name:%s  ,time:%lld",information.name.c_str(),information.time_now.sec);
        //按照循环频率延时
        loop_rate.sleep();
    }
    return 0;
}