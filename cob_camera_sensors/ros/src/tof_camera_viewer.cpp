/*
 * Copyright 2017 Fraunhofer Institute for Manufacturing Engineering and Automation (IPA)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0

 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


//##################
//#### includes ####

// ROS includes
#include <ros/ros.h>
#include <cv_bridge/CvBridge.h>
#include <image_transport/image_transport.h>

// ROS message includes
#include <sensor_msgs/Image.h>

// External includes
#include <opencv/cv.h>
#include <opencv/highgui.h>

#include <cob_vision_utils/VisionUtils.h>
#include <sstream>

//####################
//#### node class ####
class CobTofCameraViewerNode
{
private:
	ros::NodeHandle m_NodeHandle;   ///< Node handle

	image_transport::ImageTransport image_transport_;       ///< Image transport instance
	image_transport::Subscriber xyz_image_subscriber_;	///< Subscribes to xyz image data
	image_transport::Subscriber grey_image_subscriber_;	///< Subscribes to gray image data

	sensor_msgs::CvBridge cv_bridge_0_;
	sensor_msgs::CvBridge cv_bridge_1_;

	IplImage* xyz_image_32F3_;	/// OpenCV image holding the 32bit point cloud
	cv::Mat xyz_mat_8U3_;		/// OpenCV image holding the transformed 8bit RGB point cloud
	IplImage* grey_image_32F1_;	/// OpenCV image holding the 32bit amplitude values
	cv::Mat grey_mat_8U3_;	/// OpenCV image holding the transformed 8bit RGB amplitude values

	int grey_image_counter_;

public:
	/// Constructor.
	/// @param node_handle Node handle instance
        CobTofCameraViewerNode(const ros::NodeHandle& node_handle)
        : m_NodeHandle(node_handle),
          image_transport_(node_handle),
          xyz_image_32F3_(0),
          xyz_mat_8U3_(cv::Mat()),
          grey_image_32F1_(0),
          grey_mat_8U3_(cv::Mat()),
	  grey_image_counter_(0)
        {
					///Void
        }

	/// Destructor.
        ~CobTofCameraViewerNode()
        {
		/// Do not release <code>m_GrayImage32F3</code>
		/// Do not release <code>xyz_image_32F3_</code>
		/// Image allocation is managed by Cv_Bridge object

		if(cvGetWindowHandle("z data"))cvDestroyWindow("z data");
		if(cvGetWindowHandle("grey data"))cvDestroyWindow("grey data");
        }

	/// initialize tof camera viewer.
        /// @return <code>false</code> on failure, <code>true</code> otherwise
	bool init()
	{
		/// Create viewer windows
		cvStartWindowThread();
		cv::namedWindow("z data");
		cv::namedWindow("grey data");

		xyz_image_subscriber_ = image_transport_.subscribe("image_xyz", 1, &CobTofCameraViewerNode::xyzImageCallback, this);
		grey_image_subscriber_ = image_transport_.subscribe("image_grey", 1, &CobTofCameraViewerNode::greyImageCallback, this);

		return true;
	}

	/// Topic callback functions.
	/// Function will be called when a new message arrives on a topic.
	/// @param grey_image_msg The gray values of point cloud, saved in a 32bit, 1 channel OpenCV IplImage
	void greyImageCallback(const sensor_msgs::ImageConstPtr& grey_image_msg)
	{
		/// Do not release <code>m_GrayImage32F3</code>
		/// Image allocation is managed by Cv_Bridge object
		ROS_INFO("Grey Image Callback");

		try
		{
			grey_image_32F1_ = cv_bridge_0_.imgMsgToCv(grey_image_msg, "passthrough");

			cv::Mat grey_mat_32F1 = grey_image_32F1_;
			ipa_Utils::ConvertToShowImage(grey_mat_32F1, grey_mat_8U3_, 1, 0, 800);
			cv::imshow("grey data", grey_mat_8U3_);
			int c = cvWaitKey(500);
			std::cout << c << std::endl;
			if (c=='s' || c==536871027)
			{
				std::stringstream ss;
				char counterBuffer [50];
				sprintf(counterBuffer, "%04d", grey_image_counter_);
				ss << "greyImage8U3_";
				ss << counterBuffer;
				ss << ".bmp";
				cv::imwrite(ss.str().c_str(),grey_mat_8U3_);
				std::cout << "[tof_camera_viewer] Image " << grey_image_counter_ << " saved." << std::endl;
				grey_image_counter_++;
			}
		}
		catch (sensor_msgs::CvBridgeException& e)
		{
			ROS_ERROR("[tof_camera_viewer] Could not convert from '%s' to '32FC1'.", grey_image_msg->encoding.c_str());
		}
		ROS_INFO("Image Processed");
	}

	/// Topic callback functions.
	/// Function will be called when a new message arrives on a topic.
	/// @param xyz_image_msg The point cloud, saved in a 32bit, 3 channel OpenCV IplImage
	void xyzImageCallback(const sensor_msgs::ImageConstPtr& xyz_image_msg)
	{
		/// Do not release <code>xyz_image_32F3_</code>
		/// Image allocation is managed by Cv_Bridge object

		try
		{
			xyz_image_32F3_ = cv_bridge_1_.imgMsgToCv(xyz_image_msg, "passthrough");

			cv::Mat xyz_mat_32F3 = xyz_image_32F3_;
			ipa_Utils::ConvertToShowImage(xyz_mat_32F3, xyz_mat_8U3_, 3);
			cv::imshow("z data", xyz_mat_8U3_);
		}
		catch (sensor_msgs::CvBridgeException& e)
		{
			ROS_ERROR("[tof_camera_viewer] Could not convert from '%s' to '32FC1'.", xyz_image_msg->encoding.c_str());
		}

	}
};

//#######################
//#### main programm ####
int main(int argc, char** argv)
{
        /// initialize ROS, spezify name of node
        ros::init(argc, argv, "tof_camera_viewer");

        /// Create a handle for this node, initialize node
        ros::NodeHandle nh;

        /// Create camera node class instance
        CobTofCameraViewerNode camera_viewer_node(nh);


        /// initialize camera node
        if (!camera_viewer_node.init()) return 0;

        ros::spin();

        return 0;
}


/*
 int numPoints = image->width*image->height;
 sensor_msgs::PointCloud pc_msg;
 pc_msg.header.stamp = ros::Time::now();
 pc_msg.points.resize(numPoints);

 /// Convert OpenCV image to PointCloud
 for (int i = 0; i < numPoints; i++)
 {
 msg.points[i].x = ((float*)image->imageData)[3*i + 0];
 msg.points[i].y = ((float*)image->imageData)[3*i + 1];
 msg.points[i].z = ((float*)image->imageData)[3*i + 2];
 }
*/
/*
IplImage* image = cvCreateImage(cvSize(176, 144), IPL_DEPTH_32F, 3);
 for (unsigned int i=0; i<msg->points.size(); i++)
 {
 ((float*)image->imageData)[3*i+0] = (msg->points[i]).x;
 ((float*)image->imageData)[3*i+1] = (msg->points[i]).y;
 ((float*)image->imageData)[3*i+2] = (msg->points[i]).z;
 }
 IplImage* image_show = cvCreateImage(cvSize(176, 144), IPL_DEPTH_8U, 3);
 ipa_Utils::ConvertToShowImage(image, image_show, 3);
*/
