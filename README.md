## 名前 - Name
 DepthProjectionMapping

## 概要 - Overview
DepthProjectionMappingはintel RealSense D4XX seriesで取得した深度画像をプロジェクションマッピングできます。  
DepthProjectionMapping is able to project depth image using intel RealSense D4XX series and a projector.

## デモ - Demo
### 1. プロジェクターの投影位置の特定とポーズ推定  
Recognize projection position and estimate pose.
![demo1](https://raw.github.com/wiki/tes2840/DepthProjectionMapping/images/poseEstimation.png)

### 2. プロジェクションマッピング  
Projection mapping.  
![demo2](https://raw.github.com/wiki/tes2840/DepthProjectionMapping/images/DepthProjectionMapping.gif)

## 必要な環境 - Requirement  
### Libraries
OpenCV  
aruco of OpenCV's extra module  
openFrameworks   

### Equipment
Projector
intel RealSense D4XX series

## 使い方 - Usage
このプロジェクトをVisual Studio 2017以降のバージョンでビルドし実行してください。その後、1~3を実行してください。  
1.カメラをキャリブレートする  
   一度キャリブレートすれば次回以降は実行されません  
2.プロジェクターで投影されるマーカーに対して、描画される枠が一致するようにカメラ位置を調整し、'q'を押してください  
3.投影される深度画像にノイズが多ければプロジェクト内にあるsetting.jsonを環境に合わせて修正してください  
    "param-depthclampmax": "3400",	// 深度を取得する最大の距離(depthunits * depthclampmax)  
    "param-depthclampmin": "0",		// 深度画像を取得する最小の距離(depthunits * depthclampmin)  
    "param-depthunits": "1000",		// 深度単位[μm]  

Build this project since Visual Studio 2017 and run 1~3.  
1. Calibrate camera.  
   Note: Once the camera has been calibrated, it will not run from next time on.  
2. Adjust the position of the camera so that the drawn frame matches the marker projected by the projector and press 'q'.  
3. If the projected depth image is noisy, please modify setting.json in the project according to the environment.  
    "param-depthclampmax": "3400",	// Maximum distance to get depth(depthunits * depthclampmax)  
    "param-depthclampmin": "0",		// Minimum distance to get depth(depthunits * depthclampmin)  
    "param-depthunits": "1000",		// the depth unit value, in micrometre  

## インストール方法 - Install  
このプロジェクトをあなたがインストールしたopenFrameworksのapps/myAppsにクローンしてください。  
This project clone to your installed openFrameworks/apps/myApps.   

## Author
[tes2840](https://github.com/tes2840/)
