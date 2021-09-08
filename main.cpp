/*!
 * \file    main.cpp
 * \author  IDS Imaging Development Systems GmbH
 * \date    2019-05-01
 * \since   1.0.0
 *
 * \brief   This application demonstrates how to use the IDS peak API
 *          combined with a QT widgets GUI to display images from Genicam
 *          compatible device.
 *
 * \version 1.0.0
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

#include "idscameramanager.h"


int main(int argc, char* argv[])
{    
    IdsCameraManager w;

    cv::String outFileName = "test" + std::to_string(0);
    outFileName.append(".jpg");
    cv::imwrite(outFileName,w.getFrame());
    

    return 0;
}
