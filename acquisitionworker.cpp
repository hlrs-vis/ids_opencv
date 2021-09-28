/*!
 * \file    acquisitionworker.cpp
 * \author  IDS Imaging Development Systems GmbH
 * \date    2019-05-01
 * \since   1.0.0
 *
 * \brief   The AcquisitionWorker class is used in a worker thread to capture
 *          images from the device continuously and do an image conversion into
 *          a desired pixel format.
 *
 * \version 1.0.1
 *
 * Copyright (C) 2019 - 2021, IDS Imaging Development Systems GmbH.
 *
 * The information in this document is subject to change without notice
 * and should not be construed as a commitment by IDS Imaging Development Systems GmbH.
 * IDS Imaging Development Systems GmbH does not assume any responsibility for any errors
 * that may appear in this document.
 *
 * This document, or source code, is provided solely as an example of how to utilize
 * IDS Imaging Development Systems GmbH software libraries in a sample application.
 * IDS Imaging Development Systems GmbH does not assume any responsibility
 * for the use or reliability of any portion of this document.
 *
 * General permission to copy or modify is hereby granted.
 */

#include "acquisitionworker.h"

#include <peak_ipl/peak_ipl.hpp>

#include <peak/converters/peak_buffer_converter_ipl.hpp>

#include <cmath>
#include <cstring>
#include <chrono>
#include <ctime>



AcquisitionWorker::AcquisitionWorker()
{
    m_running = false;
    m_frameCounter = 0;
    m_errorCounter = 0;
}

void AcquisitionWorker::Start()
{
    try
    {
        // Lock critical features to prevent them from changing during acquisition
        m_nodemapRemoteDevice->FindNode<peak::core::nodes::IntegerNode>("TLParamsLocked")->SetValue(1);

        // Start acquisition
        m_dataStream->StartAcquisition();
        m_nodemapRemoteDevice->FindNode<peak::core::nodes::CommandNode>("AcquisitionStart")->Execute();
        m_nodemapRemoteDevice->FindNode<peak::core::nodes::CommandNode>("AcquisitionStart")->WaitUntilDone();
    }
    catch (const std::exception& e)
    {
        //qDebug() << "Exception: " << e.what();
    }

    m_running = true;
}

cv::Mat AcquisitionWorker::GetFrame()
{
    /*std::clock_t c_start = std::clock();
    auto t_start = std::chrono::high_resolution_clock::now();
    */
    if (m_running)
    {

        try
        {
            // Get buffer from device's datastream
            const auto buffer = m_dataStream->WaitForFinishedBuffer(5000);

            //QImage qImage(m_imageWidth, m_imageHeight, QImage::Format_RGB32);
            cv::Mat cvImage(m_imageHeight, m_imageWidth, CV_8UC3);

            // OpenCV Image
            size_t imageSize = cvImage.total() * cvImage.elemSize();
            //const auto image = 
            peak::BufferTo<peak::ipl::Image>(buffer).ConvertTo(
                peak::ipl::PixelFormatName::BGR8, cvImage.data, cvImage.total() * cvImage.elemSize());
            
            //cv::Mat cvImage2 = cvImage.clone();


            // Queue buffer so that it can be used again
            m_dataStream->QueueBuffer(buffer);
            
            // Emit signal that the image is ready to be displayed
            // emit imageReceived(qImage);

            m_frameCounter++;
            /*
            std::clock_t c_end = std::clock();
            auto t_end = std::chrono::high_resolution_clock::now();
 
            std::cout << std::fixed << std::setprecision(2) << "CPU time used: "
                        << 1000.0 * (c_end - c_start) / CLOCKS_PER_SEC << " ms\n"
                        << "Wall clock time passed: "
                        << std::chrono::duration<double, std::milli>(t_end-t_start).count()
                        << " ms\n";*/
            return cvImage;
        }
        catch (const std::exception& e)
        {
            m_errorCounter++;

            //qDebug() << "Exception: " << e.what();
        }
    }

}

void AcquisitionWorker::Stop()
{
    m_running = false;
}

void AcquisitionWorker::SetDataStream(std::shared_ptr<peak::core::DataStream> dataStream)
{
    m_dataStream = dataStream;
    m_nodemapRemoteDevice = m_dataStream->ParentDevice()->RemoteDevice()->NodeMaps().at(0);

    m_imageWidth = static_cast<int>(
        m_nodemapRemoteDevice->FindNode<peak::core::nodes::IntegerNode>("Width")->Value());
    m_imageHeight = static_cast<int>(
        m_nodemapRemoteDevice->FindNode<peak::core::nodes::IntegerNode>("Height")->Value());
}

bool AcquisitionWorker::isRunning()
{
    return m_running;
}