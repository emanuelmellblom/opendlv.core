/**
 * Velodyne16Decoder is used to decode VLP-16 data realized with OpenDaVINCI
 * Copyright (C) 2016 Hang Yin
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef VELODYNE16DECODER_H_
#define VELODYNE16DECODER_H_

#include <cmath>
#include <memory>

#include "opendavinci/generated/odcore/data/SharedPointCloud.h"
#include "opendavinci/generated/odcore/data/CompactPointCloud.h"
#include "opendavinci/odcore/data/Container.h"
#include "opendavinci/odcore/wrapper/SharedMemory.h"
#include <opendavinci/odcore/io/StringListener.h>
#include "automotivedata/generated/cartesian/Constants.h"

namespace opendlv {
namespace core {
namespace system {
namespace proxy {
using namespace odcore::wrapper;

// This class will handle bytes received via a UDP socket.
class Velodyne16Decoder : public odcore::io::StringListener {
   private:
    /**
         * "Forbidden" copy constructor. Goal: The compiler should warn
         * already at compile time for unwanted bugs caused by any misuse
         * of the copy constructor.
         */
    Velodyne16Decoder(const Velodyne16Decoder &);

    /**
         * "Forbidden" assignment operator. Goal: The compiler should warn
         * already at compile time for unwanted bugs caused by any misuse
         * of the assignment operator.
         */
    Velodyne16Decoder &operator=(const Velodyne16Decoder &);

   public:
    /**
         * Constructor.
         */
    //Use this constructor if the VLP-16 live feed is decoded and sent out as shared point cloud. The boolean parameter tells if compact point cloud is sent out as well.
    //The first uint8_t type parameter tells the intensity option of CPC; the second uint8_t type parameter tells number of bits for intensity in CPC;
    //the third uint8_t parameter tells distance encoding unit option
    Velodyne16Decoder(const std::shared_ptr< SharedMemory >, odcore::io::conference::ContainerConference &, const string &, const bool &, const uint8_t &, const uint8_t &, const uint8_t &);

    //Use this constructor if the VLP-16 live feed is decoded and sent out as compact point cloud only.  
    //The first uint8_t type parameter tells the intensity option of CPC; the second uint8_t type parameter tells number of bits for intensity in CPC;
    //the third uint8_t parameter tells distance encoding unit option
    Velodyne16Decoder(odcore::io::conference::ContainerConference &, const string &, const uint8_t &, const uint8_t &, const uint8_t &);

    virtual ~Velodyne16Decoder();

    virtual void nextString(const std::string &s);

   private:
    void readCalibrationFile();
    void initializeArraysCPC();
    void sendPointCloud();
   private:
    const uint32_t m_MAX_POINT_SIZE = 30000; //the maximum number of points per frame. This upper bound should be set as low as possible, as it affects the shared memory size and thus the frame updating speed.
    const uint32_t m_SIZE_PER_COMPONENT = sizeof(float);
    const uint8_t m_NUMBER_OF_COMPONENTS_PER_POINT = 4;                                           // How many components do we have per vector?
    const uint32_t m_SIZE = m_MAX_POINT_SIZE * m_NUMBER_OF_COMPONENTS_PER_POINT * m_SIZE_PER_COMPONENT; // What is the total size of the shared memory?

    uint8_t m_CPCIntensityOption; //Only used when CPC is enabled. 0: without intensity; 1: with intensity; 2: send a CPC container twice, one with intensity, and the other without intensity
    uint8_t m_numberOfBitsForIntensity; //Range 0-7. Only used when CPC is enabled. Currently recommendation when intensity is included in CPC: 2 bits for intensity and 14 bits for distance
    uint8_t m_distanceEncoding; //0: cm; 1: 2mm
    uint32_t m_pointIndexSPC; //current number of points of the current frame for shared point cloud 
    uint32_t m_pointIndexCPC; //current number of points of the current frame for compact point cloud
    uint32_t m_startID;
    float m_previousAzimuth;
    float m_currentAzimuth;
    float m_nextAzimuth;
    float m_deltaAzimuth;
    float m_distance;
    std::shared_ptr< SharedMemory > m_velodyneSharedMemory; //shared memory for the shared point cloud
    float *m_segment;                                       //temporary memory for transferring data of each frame to the shared memory
    odcore::io::conference::ContainerConference &m_velodyneContainer;
    odcore::data::SharedPointCloud m_spc; //shared point cloud
    float m_verticalAngle[16];           //Vertal angle of each sensor beam
    string m_calibration;  //name of the calibration file for VLP-16
    const float toRadian = static_cast<float>(M_PI) / 180.0f;  //degree to radian
    bool m_withSPC;  //if SPC is expected
    bool m_withCPC;  //if CPC is expected
        
    //For compact point cloud:
    float m_startAzimuth;
    const uint8_t m_ENTRIES_PER_AZIMUTH = 16;//For VLP-16, there are 16 points per azimuth
    std::stringstream m_distanceStringStreamNoIntensity; //The string stream with distance values for all points of one frame, excluding intensity
    std::stringstream m_distanceStringStreamWithIntensity; //The string stream with distance values for all points of one frame, including intensity
    bool m_isStartAzimuth;  //Indicate if an azimuth is the starting azimuth of a new frame
    uint8_t m_sensorOrderIndex[16];//Specify the order for each 16 points in the string with distance values
    uint16_t m_16SensorsNoIntensity[16];//Store the distance values of the current 16 sensors
    uint16_t m_16SensorsWithIntensity[16];//Store the distance values of the current 16 sensors
};
}
}
}
} // opendlv::core::system::proxy
#endif /*VELODYNE16DECODER_H_*/
