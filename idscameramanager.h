/*!
 * \file    idscameramanager.h
 * \author  IDS Imaging Development Systems GmbH
 * \date    2019-05-01
 * \since   1.0.0
 *
 * \version 1.0.0
 *

 */

#ifndef IDSCAMERAMANAGER_H
#define IDSCAMERAMANAGER_H

#include "acquisitionworker.h"

#include <peak_ipl/peak_ipl.hpp>

#include <peak/peak.hpp>

#include <cstdint>

#include "opencv2/opencv.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/videoio.hpp>

class IdsCameraManager{

public:
    explicit IdsCameraManager();
    ~IdsCameraManager();
    cv::Mat getFrame();

private:
    std::shared_ptr<peak::core::Device> m_device;
    std::shared_ptr<peak::core::DataStream> m_dataStream;
    std::shared_ptr<peak::core::NodeMap> m_nodemapRemoteDevice;


    AcquisitionWorker* m_acquisitionWorker;

    void DestroyAll();

    bool OpenDevice();
    void CloseDevice();
    
    void onCounterUpdated(unsigned int frameCounter, unsigned int errorCounter);

};

#endif // IDSCAMERAMANAGER_H
