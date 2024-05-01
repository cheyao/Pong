# Little pong

## Dependencies:
SDL3


## Building on mac and linux:

```
git clone https://github.com/cheyao/Pong
cd Pong
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
```
You will get Pong.app on mac and Pong on linux

Building for android:
Download sdk, ndk, cmd-tools from Google (Use Android Studio)

PS. You probably need java 17 to build this
```
git clone https://github.com/cheyao/Pong
cd Pong
# We need SDL3 to build for android
git submodule update --init --recursive
cd android-wrapper 
./gradlew installRelease
```
