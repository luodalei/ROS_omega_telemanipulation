#include "/home/olmo/jade_workspace/src/omega_telemanipulation/include/collision_engine_omega_telemanipulation_oo.h"

CE_OO::CE_OO(ros::NodeHandle *nh){
    n=nh;
    c_r_c_pub = n->advertise<geometry_msgs::PoseStamped>("set_pos_c_r_c", 1000);
    s_g_c_pub = n->advertise<geometry_msgs::PoseStamped>("set_pos_s_g_c", 1000);
    set_omega_pub = n->advertise<std_msgs::Int64>("set_omega", 1000);
    sub_c_r_c = n->subscribe("feed_pos_c_r_c", 1000, &CE_OO::positionCallbackR, this);
    sub_s_g_c = n->subscribe("feed_pos_s_g_c", 1000, &CE_OO::positionCallbackG, this);
    sub_omega = n->subscribe("feed_pos_omega", 1000, &CE_OO::positionCallbackOmega, this);
}

bool CE_OO::run(){
    int fc;
    ros::Rate r(100);
    while (ros::ok())
    {
        ros::spinOnce();
        if((s_g_c_msg.pose.position.x<=2.10)&&(s_g_c_msg.pose.position.x>=-2.10)&&
           (s_g_c_msg.pose.position.y<=2.10)&&(s_g_c_msg.pose.position.y>=-2.10)&&
           (s_g_c_msg.pose.position.z<=2.10)&&(s_g_c_msg.pose.position.z>=-2.10)){
            flag_collision.data = 1;
            fc=1;
        }
        else{
            flag_collision.data = 0;
            fc=0;
        }
        set_omega_pub.publish(flag_collision);
        printf ("Flag collision status: %d\n", fc);
        ROS_INFO_STREAM("Positions(" <<  s_g_c_msg.pose.position.x << "," << s_g_c_msg.pose.position.y << "," << s_g_c_msg.pose.position.z << ")");
        r.sleep();
    }
}

void CE_OO::positionCallbackR(const geometry_msgs::PoseStamped::ConstPtr &msg)
{
    c_r_c_msg=*msg;
}

void CE_OO::positionCallbackG(const geometry_msgs::PoseStamped::ConstPtr &msg)
{
    s_g_c_msg=*msg;
}

void CE_OO::positionCallbackOmega(const geometry_msgs::PoseStamped::ConstPtr &msg)
{
    omega_msg=*msg;
    s_g_c_pub.publish(omega_msg);
}

int main( int argc, char** argv )
{
    ros::init(argc, argv, "collision_engine_omega_telemanipulation");
    ros::NodeHandle n;
    CE_OO instance(&n);
    instance.run();
}
