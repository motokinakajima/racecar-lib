# ar_marker.py

## 0. ARマーカーとは
ARマーカーとは当初ARなどで使われるデジタルコンテンツの表示の目印として開発されたもので、見た目はピクセル数が少ないQRコードのようなものです。
ARマーカーはcv2がデフォルトで認識するライブラリを提供しており、瞬時に画像のARマーカーの四つ角の座標とそのIDを認識することが可能となっています。  
ここではそのARマーカーの認識が楽にするための関数やクラスをそろえたのでぜひ使ってください。

## 1. 使い方
### prerequisite
前提としてcv2とnumpyをインストールしている必要があります。  
cv2：```4.5.0```以降  
numpy：```1.19.3```以降  
をインストールするようにしてください。condaを使うと色々便利です。体感venvは少し不安定です。
```shell
pip3 install opencv-python
pip3 install numpy
```
もしエラーが出たら
```shell
pip install opencv-python
pip install numpy
```
を実行してみてください。

### import
```python
from ar_marker import *
```
このようにしてimportしてください。ファイルの位置が違ったりする場合はsysか何かで上手にimportしてください。

### initialize
```python
myDetector = ArMarkerDetector()
```
これでmyDetectorオブジェクトを作成できます。デフォルトでは```cv2.aruco.DICT_6X6_250```を使用しています。6x6のARマーカーを検出するモデルです。
4x4など別のARマーカーを検出したい場合には
```python
import cv2
myDetector = ArMarkerDetector(aruco_dictionary=cv2.aruco.DICT_4x4_250)
```
このようにします。

### get_all_markers
```python
import cv2
image = cv2.imread('test.jpg')
markerCorners, markerIds = myDetector.get_all_markers(image)
```
get_all_markersはimageにあるARマーカーを全て検出して、一つ目の戻り値にマーカーの角の座標(3次元配列)、二つ目の戻り値にマーカーのid(1次元配列)が出てきます。
引数にはcv2の画像型を入力します。

### get_largest_marker
```python
import cv2
image = cv2.imread('test.jpg')
markerCorner, markerId = myDetector.get_largest_marker(image)
```
get_largest_markerはimageにあるARマーカーのうち最も面積が大きいものを返します。一つ目の戻り値にはマーカーの角の座標(2次元配列)、二つ目の戻り値にはマーカーのid(整数型)が出てきます。
引数にはcv2の画像型を入力します。

### find_marker
```python
import cv2
image = cv2.imread('test.jpg')
target_id = 2
markerCorner, markerId = myDetector.find_marker(image, target_id)
```
find_markerはimageにあるARマーカーのうちidが特定のものを一つ返します。一つ目の戻り値にはマーカーの角の座標(2次元配列)、二つ目の戻り値にはマーカーのid(整数型)が出てきます。
引数の一つ目にはcv2の画像型、二つ目には整数型を入力します。二つ目の引数には探したいidを入力します。この例の場合はidが2のマーカーを出力します。

### get_marker_center
```python
import cv2
image = cv2.imread('test.jpg')
target_id = 2
x, y = myDetector.get_marker_center(image, targe t_id)
```
get_marker_centerはimageにあるARマーカーのうちidが特定のものの中心点の座標を返します。一つ目の戻り値にはマーカーの中心のx座標、二つ目の戻り値にはマーカーの中心のy座標が出てきます。
引数の一つ目にはcv2の画像型、二つ目には整数型を入力します。二つ目の引数には探したいidを入力します。この例の場合はidが2のマーカーの中心を出力します。

### get_marker_area
```python
import cv2
image = cv2.imread('test.jpg')
target_id = 2
area = myDetector.get_marker_area(image, target_id)
```
get_marker_centerはimageにあるARマーカーのうちidが特定のものの面積を返します。戻り値には面積が出力されます。
引数の一つ目にはcv2の画像型、二つ目には整数型を入力します。二つ目の引数には探したいidを入力します。この例の場合はidが2のマーカーの面積を出力します。