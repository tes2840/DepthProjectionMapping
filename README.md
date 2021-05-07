# DepthProjectionMapping  
English | [Japanese](README_jp.md)   
## Name
 DepthProjectionMapping

## Overview
DepthProjectionMapping is able to project depth image using intel RealSense D4XX series and a projector.  
Also, project video by specifying the projection area.  

## Demo
### 1. Recognize projection position and estimate pose  
<img src="https://raw.github.com/wiki/tes2840/DepthProjectionMapping/images/DepthProjectionMapping_Pose_estimation.png" width="500">

### 2. Projection mapping of depth image  
![demo2](https://raw.github.com/wiki/tes2840/DepthProjectionMapping/images/DepthProjectionMapping.gif)   

### 3. Projection mapping of video by specifying the projection area 
<img src="https://raw.github.com/wiki/tes2840/DepthProjectionMapping/images/DepthProjectionMapping_Demo.gif" width="20%">

## Requirement  
### Libraries
OpenCV  
aruco of OpenCV's extra module  
openFrameworks   
[cvui](https://github.com/Dovyski/cvui)  

### Equipment
Projector
intel RealSense D4XX series

## Usage
Build this project since Visual Studio 2017 and run.  
1. Calibrate camera.  
   Note: Once the camera has been calibrated, it will not run from next time on.  
2. Adjust the camera position so that the marker projected by the projector is within the angle of view of it, and press 'q' or 'd'.  
d: Start projection mapping of depth image  
q: Start projection mapping of video by specifying the projection area  

### Projection mapping of depth image(Press 'd')  
1. If the projected depth image is noisy, please modify setting.json in the project according to the environment.  
    "param-depthclampmax": "3400",	// Maximum distance to get depth(depthunits * depthclampmax)  
    "param-depthclampmin": "0",		// Minimum distance to get depth(depthunits * depthclampmin)  
    "param-depthunits": "1000",		// the depth unit value, in micrometre  

### Projection mapping of video by specifying the projection area(Press 'q')
1. Specify the background image. Click the Set button of "Background image".    
2. Place the object to be projected in the projection area. The area where there is no difference from the background image will be black.  
   The image will not be projected on the area(mask area).  
   <img src="https://raw.github.com/wiki/tes2840/DepthProjectionMapping/images/DepthProjectionMapping_Background.gif" width="50%">
3. Adjust the mask area. The mask area can be adjusted by sliding the Binarization threshold.  
    <img src="https://raw.github.com/wiki/tes2840/DepthProjectionMapping/images/DepthProjectionMapping_Binarization.gif" width="50%">
4. Press the Set button of "Mask" when only the object you want to project is displayed. The mask area can be fixed.  
   <img src="https://raw.github.com/wiki/tes2840/DepthProjectionMapping/images/DepthProjectionMapping_FixMask.gif" width="50%">
5. Modify the mask area. You can right-click on the screen to exclude the point from the mask area, or left-click to specify it as the mask area.  
   The brush size can be adjusted by adjusting the Thick size slider.  
   <img src="https://raw.github.com/wiki/tes2840/DepthProjectionMapping/images/DepthProjectionMapping_ModifyMask.gif" width="50%">
6. In the "screen" window, specify the image you want to project. Right-click to specify the start point, and release at the end point.    
   <img src="https://raw.github.com/wiki/tes2840/DepthProjectionMapping/images/DepthProjectionMapping_CaptureArea.gif" width="20%">
7.  When all the steps 1~6 are set, press the ""Start" button of "Composition".   
    <img src="https://raw.github.com/wiki/tes2840/DepthProjectionMapping/images/DepthProjectionMapping_GUI.png" width="20%">
8.  Projection mapping will begin.  
    <img src="https://raw.github.com/wiki/tes2840/DepthProjectionMapping/images/DepthProjectionMapping_Demo.gif" width="20%">

## Install  
This project clone to your installed openFrameworks/apps/myApps.   

## Author
github : [tes2840](https://github.com/tes2840/)  
Twitter : [tes2840](https://twitter.com/tes2840)  
