//ROS typedefs
#include "ros/ros.h"
#include <visualization_msgs/MarkerArray.h>
#include <nav_msgs/Odometry.h>
#include <cob_srvs/SetInt.h>

/* protected region user include files on begin */
#include <sick_flexisoft_client/flexiclient.h>
#include <boost/static_assert.hpp>
/* protected region user include files end */

class safety_controller_config
{
public:
		std::string port;
		std::string host;

};

class safety_controller_data
{
// autogenerated: don't touch this class
public:
//input data
    nav_msgs::Odometry in_odometry;
  
	
//output data
    	visualization_msgs::MarkerArray out_marker;
    	bool out_marker_active;
 

};

class safety_controller_impl
{
	/* protected region user member variables on begin */
	struct FlexiInput{
	    /*0.0*/ uint8_t:1;
	    /*0.1*/ uint8_t:1;
	    /*0.2*/ uint8_t :1;
	    /*0.3*/ uint8_t:1;
	    /*0.4*/ uint8_t:1;
	    /*0.5*/ uint8_t:1;
	    /*0.6*/ uint8_t em_stop_ur10_ok:1;
	    /*0.7*/ uint8_t:1;

	    /*1.0*/ uint8_t ext_permission:1;
	    /*1.1*/ uint8_t:1;
	    /*1.2*/ uint8_t enabling_switch_pressed:1;
	    /*1.3*/ uint8_t:1;
	    /*1.4*/ uint8_t:1;
	    /*1.5*/ uint8_t:1;
	    /*1.6*/ uint8_t manual_mode:1;
	    /*1.7*/ uint8_t:1;

	    /*2.0*/ uint8_t base_active:1;
	    /*2.1*/ uint8_t lift_active:1;
	    /*2.2*/ uint8_t arm_active:1;
	    /*2.3*/ uint8_t em_can_recover:1;
	    /*2.4*/ uint8_t em_stop_ok:1;
	    /*2.5*/ uint8_t hardware_stop_ok:1;
	    /*2.6*/ uint8_t laser_stop_ok:1;
	    /*2.7*/ uint8_t unlocked:1;

	    /*3.0*/ uint8_t connection_ok:1;
	    /*3.1*/ uint8_t arm_locked:1;
	    /*3.2*/ uint8_t edm_err_base:1;
	    /*3.3*/ uint8_t edm_err_lift:1;
	    /*3.4*/ uint8_t edm_err_arm:1;
	    /*3.5*/ uint8_t software_stop_ok:1;
	    /*3.6*/ uint8_t arm_in_laser:1;
	    /*3.7*/ uint8_t :1;

	    /*4.0*/ uint8_t xtio1_input_ok:1;
	    /*4.1*/ uint8_t xtio2_input_ok:1;
	    /*4.2*/ uint8_t:1;
	    /*4.3*/ uint8_t:1;
	    /*4.4*/ uint8_t:1;
	    /*4.5*/ uint8_t:1;
	    /*4.6*/ uint8_t:1;
	    /*4.7*/ uint8_t:1;

	    /*5.0*/ uint8_t:1;
	    /*5.1*/ uint8_t:1;
	    /*5.2*/ uint8_t:1;
	    /*5.3*/ uint8_t:1;
	    /*5.4*/ uint8_t gateway_input_ok:1;
	    /*5.5*/ uint8_t:1;
	    /*5.6*/ uint8_t:1;
	    /*5.7*/ uint8_t:1;

	    /*6.0*/ uint8_t xtio1_output_ok:1;
	    /*6.1*/ uint8_t xtio2_output_ok:1;
	    /*6.2*/ uint8_t:1;
	    /*6.3*/ uint8_t:1;
	    /*6.4*/ uint8_t:1;
	    /*6.5*/ uint8_t:1;
	    /*6.6*/ uint8_t:1;
	    /*6.7*/ uint8_t:1;

	    /*7.0*/ uint8_t:1;
	    /*7.1*/ uint8_t:1;
	    /*7.2*/ uint8_t:1;
	    /*7.3*/ uint8_t:1;
	    /*7.4*/ uint8_t gateway_output_ok:1;
	    /*7.5*/ uint8_t:1;
	    /*7.6*/ uint8_t:1;
	    /*7.7*/ uint8_t:1;

	    /*8.0*/ uint8_t:1;
	    /*8.1*/ uint8_t:1;
	    /*8.2*/ uint8_t:1;
	    /*8.3*/ uint8_t:1;
	    /*8.4*/ uint8_t laser_front_ok:1;
	    /*8.5*/ uint8_t:1;
	    /*8.6*/ uint8_t:1;
	    /*8.7*/ uint8_t:1;

	    /*9.0*/ uint8_t:1;
	    /*9.1*/ uint8_t:1;
	    /*9.2*/ uint8_t:1;
	    /*9.3*/ uint8_t:1;
	    /*9.4*/ uint8_t laser_rear_ok:1;
	    /*9.5*/ uint8_t:1;
	    /*9.6*/ uint8_t:1;
	    /*9.7*/ uint8_t:1;
	}  __attribute__ ((__packed__));
	BOOST_STATIC_ASSERT_MSG(sizeof(FlexiInput) <= sizeof(flexi::FlexiInputData::data), "FlexiInput does not fit into payload");

	struct FlexiOutput{
	    /*0.0*/ uint8_t laser_case:5;
	    /*0.5*/ uint8_t far_front:1;
	    /*0.6*/ uint8_t far_left:1;
	    /*0.7*/ uint8_t far_right:1;

	    /*1.0*/ uint8_t enable_base:1;
	    /*1.1*/ uint8_t enable_torso:1;
	    /*1.2*/ uint8_t:1;
	    /*1.3*/ uint8_t:1;
	    /*1.4*/ uint8_t:1;
	    /*1.5*/ uint8_t:1;
	    /*1.6*/ uint8_t:1;
	    /*1.7*/ uint8_t:1;
	    }  __attribute__ ((__packed__));
	BOOST_STATIC_ASSERT_MSG(sizeof(FlexiOutput) <= sizeof(flexi::FlexiOutputData::data), "FlexiOutput does not fit into payload");

	boost::scoped_ptr <flexi::FlexiClient> flexi_client_;
	FlexiInput flexi_input_;
	boost::mutex flexi_input_mutex_;
	FlexiOutput flexi_output_;

	/* protected region user member variables end */

public:
    safety_controller_impl() 
    {
        /* protected region user constructor on begin */
    	flexi_client_.reset(new flexi::FlexiClient(boost::bind(&safety_controller_impl::handle_input, this, _1)));
    	memset(&flexi_output_, sizeof(flexi_output_), 0);
    	memset(&flexi_input_, sizeof(flexi_input_), 0);
    	flexi_output_.enable_base = 1;
    	flexi_output_.enable_torso = 1;
		/* protected region user constructor end */
    }
    void configure(safety_controller_config config) 
    {
        /* protected region user configure on begin */
    	if (!flexi_client_->connect(config.host, config.port))
    	{
    		exit (EXIT_FAILURE);
    	}
    	flexi_client_->start_worker();
    	flexi_client_->start_worker();
		/* protected region user configure end */
    }
    void update(safety_controller_data &data, safety_controller_config config)
    {
        /* protected region user update on begin */
    	double direction = atan2(data.in_odometry.twist.twist.linear.y, data.in_odometry.twist.twist.linear.x);
    	double lin_velocity = sqrt(data.in_odometry.twist.twist.linear.y*data.in_odometry.twist.twist.linear.y + data.in_odometry.twist.twist.linear.x*data.in_odometry.twist.twist.linear.x);
    	//=(D5+F$3)/F$3
    	//D5=winkel
    	//F3 = 15deg
    	// TODO: if lin vel > limit then safety field=far

		/* protected region user update end */
    }

	bool callback_set_mode(cob_srvs::SetInt::Request  &req, cob_srvs::SetInt::Response &res , safety_controller_config config)
	{
		/* protected region user implementation of service callback for set_mode on begin */
		/* protected region user implementation of service callback for set_mode end */
		return true;
	}
    


    
    /* protected region user additional functions on begin */
	void handle_input(const flexi::FlexiInputData &input)
	{
		boost::mutex::scoped_lock lock(flexi_input_mutex_);
	    flexi_input_ = *(FlexiInput*)input.data;
	}
	/* protected region user additional functions end */
    
};
