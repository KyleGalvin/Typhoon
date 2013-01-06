#include "libfreenect/libfreenect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <vector>
#include <cmath>
#include <pthread.h>
#include <opencv/cv.h>
#include <opencv/cxcore.h>
#include <opencv/highgui.h>

//using namespace cv;
//using namespace std;

template <typename T>
  string NumberToString ( T Number )
  {
     ostringstream ss;
     ss << Number;
     return ss.str();
  }

//from cannyThresh.cpp to store current threshold frame
cv::Mat src; cv::Mat src_gray;
int thresh = 100;
int max_thresh = 255;
cv::RNG rng(12345);

/** @function thresh_callback */
void thresh_callback(int, void* )
{
  cv::Mat canny_output;
  std::vector< std::vector< cv::Point > > contours;
  std::vector< cv::Vec4i > hierarchy;

  /// Detect edges using canny
  cv::Canny( src_gray, canny_output, thresh, thresh*2, 3 );
  /// Find contours
  cv::findContours( canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0) );

  /// Draw contours
  cv::Mat drawing = cv::Mat::zeros( canny_output.size(), CV_8UC3 );
  for( int i = 0; i< contours.size(); i++ )
     {
       cv::Scalar color = cv::Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
       cv::drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, cv::Point() );
     }

  /// Show in a window
  imshow( "RGBContours", drawing );
}

class Mutex {
public:
	Mutex() {
		pthread_mutex_init( &m_mutex, NULL );
	}
	void lock() {
		pthread_mutex_lock( &m_mutex );
	}
	void unlock() {
		pthread_mutex_unlock( &m_mutex );
	}
private:
	pthread_mutex_t m_mutex;
};

class MyFreenectDevice : public Freenect::FreenectDevice {
  public:
	MyFreenectDevice(freenect_context *_ctx, int _index)
		: Freenect::FreenectDevice(_ctx, _index), m_buffer_depth(FREENECT_DEPTH_11BIT),m_buffer_rgb(FREENECT_VIDEO_RGB), m_gamma(2048), m_new_rgb_frame(false), m_new_depth_frame(false),
		  depthMat(cv::Size(640,480),CV_16UC1), rgbMat(cv::Size(640,480),CV_8UC3,cv::Scalar(0)), ownMat(cv::Size(640,480),CV_8UC3,cv::Scalar(0))
	{
		for( unsigned int i = 0 ; i < 2048 ; i++) {
			float v = i/2048.0;
			v = std::pow(v, 3)* 6;
			m_gamma[i] = v*6*256;
		}
	}
	// Do not call directly even in child
	void VideoCallback(void* _rgb, uint32_t timestamp) {
		std::cout << "RGB callback" << std::endl;
		m_rgb_mutex.lock();
		uint8_t* rgb = static_cast<uint8_t*>(_rgb);
		rgbMat.data = rgb;
		m_new_rgb_frame = true;
		m_rgb_mutex.unlock();
	};
	// Do not call directly even in child
	void DepthCallback(void* _depth, uint32_t timestamp) {
		std::cout << "Depth callback" << std::endl;
		m_depth_mutex.lock();
		uint16_t* depth = static_cast<uint16_t*>(_depth);
		depthMat.data = (uchar*) depth;
		m_new_depth_frame = true;
		m_depth_mutex.unlock();
	}

	bool getVideo(cv::Mat& output) {
		m_rgb_mutex.lock();
		if(m_new_rgb_frame) {
			cv::cvtColor(rgbMat, output, CV_RGB2BGR);
			m_new_rgb_frame = false;
			m_rgb_mutex.unlock();
			return true;
		} else {
			m_rgb_mutex.unlock();
			return false;
		}
	}

	bool getDepth(cv::Mat& output) {
			m_depth_mutex.lock();
			if(m_new_depth_frame) {
				depthMat.copyTo(output);
				m_new_depth_frame = false;
				m_depth_mutex.unlock();
				return true;
			} else {
				m_depth_mutex.unlock();
				return false;
			}
		}

  private:
	std::vector<uint8_t> m_buffer_depth;
	std::vector<uint8_t> m_buffer_rgb;
	std::vector<uint16_t> m_gamma;
	cv::Mat depthMat;
	cv::Mat rgbMat;
	cv::Mat ownMat;
	Mutex m_rgb_mutex;
	Mutex m_depth_mutex;
	bool m_new_rgb_frame;
	bool m_new_depth_frame;
};



int main(int argc, char **argv) {
	bool die(false);
	cv::string filename("snapshot");
	cv::string suffix(".png");
	int i_snap(0),iter(0);

	cv::Mat depthMat(cv::Size(640,480),CV_16UC1);
	cv::Mat depthf  (cv::Size(640,480),CV_8UC1);
	cv::Mat rgbMat(cv::Size(640,480),CV_8UC3,cv::Scalar(0));
	cv::Mat ownMat(cv::Size(640,480),CV_8UC3,cv::Scalar(0));

        //The next two lines must be changed as Freenect::Freenect isn't a template but the method createDevice:
        //Freenect::Freenect<MyFreenectDevice> freenect;
        //MyFreenectDevice& device = freenect.createDevice(0);
        //by these two lines:
        Freenect::Freenect freenect;
        MyFreenectDevice& device = freenect.createDevice<MyFreenectDevice>(0);

	cv::namedWindow("rgb",CV_WINDOW_AUTOSIZE);
	cv::namedWindow("depth",CV_WINDOW_AUTOSIZE);

	for(thresh=50;thresh<=250; thresh+=50){
		//TODO: add number to string!
		cv::namedWindow( "RGBContours", CV_WINDOW_AUTOSIZE );
	}

	device.startVideo();
	device.startDepth();
    while (!die) {
    	device.getVideo(rgbMat);
    	device.getDepth(depthMat);
        cv::imshow("rgb", rgbMat);
    	depthMat.convertTo(depthf, CV_8UC1, 255.0/2048.0);
        cv::imshow("depth",depthf);

		//mixit
		// Convert image to gray and blur it
		cv::cvtColor( rgbMat, src_gray, CV_BGR2GRAY );
		cv::blur( src_gray, src_gray, cv::Size(3,3) );

		for(thresh = 50; thresh<=250; thresh+=50){
			std::cout <<"thresh="<<thresh<<"\n";
			thresh_callback(0,0);
		}

		char k = cvWaitKey(5);
		if( k == 27 ){
		    cvDestroyWindow("rgb");
		    cvDestroyWindow("depth");
			break;
		}
		if( k == 8 ) {
			std::ostringstream file;
			file << filename << i_snap << suffix;
			cv::imwrite(file.str(),rgbMat);
			i_snap++;
		}
    }

   	device.stopVideo();
	device.stopDepth();
	return 0;
}
