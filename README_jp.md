# DepthProjectionMapping
[English](README.md)   | Japanese 
## 名前
 DepthProjectionMapping

## 概要
DepthProjectionMappingはintel RealSense D4XX seriesで取得した深度画像をプロジェクションマッピングできます。  
また、投影領域を指定して動画をプロジェクションマッピングすることができます。

## デモ
### 1. プロジェクターの投影位置の特定とポーズ推定  
<img src="https://raw.github.com/wiki/tes2840/DepthProjectionMapping/images/DepthProjectionMapping_Pose_estimation.png" width="500">

### 2. 深度画像のプロジェクションマッピング  
![demo2](https://raw.github.com/wiki/tes2840/DepthProjectionMapping/images/DepthProjectionMapping.gif)

### 3. 投影領域を指定して動画をプロジェクションマッピング
<img src="https://raw.github.com/wiki/tes2840/DepthProjectionMapping/images/DepthProjectionMapping_Demo.gif" width="20%">

## 必要な環境
### Libraries
OpenCV  
aruco of OpenCV's extra module  
openFrameworks   
[cvui](https://github.com/Dovyski/cvui)

### Equipment
Projector
intel RealSense D4XX series

## 使い方  
このプロジェクトをVisual Studio 2017以降のバージョンでビルドし実行してください。その後、以下の手順を実行してください。  
1. カメラをキャリブレートする。  
   一度キャリブレートすれば次回以降は実行されません。  
2. プロジェクターで投影されたマーカーが、カメラの画角内に入るようにカメラ位置を調整し、'q'もしくは'd'を押してください。  
d: 深度画像のプロジェクションマッピングを開始  
q: 投影領域を指定して動画をプロジェクションマッピングを開始  
<img src="https://raw.github.com/wiki/tes2840/DepthProjectionMapping/images/DepthProjectionMapping_Pose_estimation.png" width="20%">

### 深度画像のプロジェクションマッピング(Press 'd')
1. 投影される深度画像にノイズが多ければプロジェクト内にあるsetting.jsonを環境に合わせて修正してください。  
    "param-depthclampmax": "3400",	// 深度を取得する最大の距離(depthunits * depthclampmax)  
    "param-depthclampmin": "0",		// 深度画像を取得する最小の距離(depthunits * depthclampmin)  
    "param-depthunits": "1000",		// 深度単位[μm]  

### 投影領域を指定して動画をプロジェクションマッピング(Press 'q')
1. 背景画像を指定します。Background imageのSetボタンを押して下さい。  
2. 投影したい対象物を投領域内に配置します。背景画像と差分がない箇所は黒色になります。その領域には映像が投影されません(マスク領域)。  
   <img src="https://raw.github.com/wiki/tes2840/DepthProjectionMapping/images/DepthProjectionMapping_Background.gif" width="50%">
3. マスク領域の調整をします。マスク領域はBinarization thresholdをスライドすることで調整可能です。  
    <img src="https://raw.github.com/wiki/tes2840/DepthProjectionMapping/images/DepthProjectionMapping_Binarization.gif" width="50%">
4. 投影したい対象物のみが表示されるようになったらMaskのSetボタンを押します。マスク領域を確定することができます。  
   <img src="https://raw.github.com/wiki/tes2840/DepthProjectionMapping/images/DepthProjectionMapping_FixMask.gif" width="50%">
5. マスク領域を修正します。画面上を右クリックでマスク領域から除外、左クリックでマスク領域に指定することが可能です。  
   また、Thick sizeのスライダーを調整することでブラシサイズが調整可能です。  
   <img src="https://raw.github.com/wiki/tes2840/DepthProjectionMapping/images/DepthProjectionMapping_ModifyMask.gif" width="50%">
6. 投影したい映像を"screen"ウィンドウ上で指定します。右クリックで開始地点を指定し、終了地点で離します。  
   <img src="https://raw.github.com/wiki/tes2840/DepthProjectionMapping/images/DepthProjectionMapping_CaptureArea.gif" width="20%">
7.  1~6.のステップをすべて設定できたらCompositionのStartボタンを押して下さい。  
    <img src="https://raw.github.com/wiki/tes2840/DepthProjectionMapping/images/DepthProjectionMapping_GUI.png" width="20%">
8.  プロジェクションマッピングが開始されます。  
    <img src="https://raw.github.com/wiki/tes2840/DepthProjectionMapping/images/DepthProjectionMapping_Demo.gif" width="20%">

## インストール方法  
openFrameworksのapps/myAppsにこのプロジェクトをクローンしてください。  

## Author
github : [tes2840](https://github.com/tes2840/)  
Twitter : [tes2840](https://twitter.com/tes2840)  
