# Little pong

Building on mac, linux:

```
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
```
You will get Pong.app on mac and Pong on linux

Building for android:
Download sdk, ndk, cmd-tools
```
cd android/org.cyao.pong
./gradlew installRelease
```

Lol dunno how to make a good randint
