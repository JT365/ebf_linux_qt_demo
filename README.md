# Wildfire qt app

## ReadMe

1. This is Qt5 code, not compatible with QT4 version.
2. The build environment is qt5.6.2 or above.
3. Under Windows, the compiler is MinGW, no Visual Studio compiler supported.
4. Fileapp.pro is a multi project directory, in which qtui is the custom basic control library of demo. Skin is a skin resource file. Both of them are necessary and important.
5. App is the executable program of demo, that is, the program runs on ARM board.
6. Annotated .pro projects are independent applications, which can be uncommented and compiled separately.

## Install SDK

**Download SDK**

SDK Links：<https://url.cn/5Iv5apg>

Install qt5 cross development SDK：
```bash
./fsl-imx-x11-glibc-x86_64-meta-toolchain-qt5-cortexa7hf-neon-toolchain-4.1.15-2.1.0.sh
```

Accept all default settings, SDK will be installed under /opt/fsl-imx-x11/4.1.15-2.1.0/ 

**Note**：Source environment settings on every shell session 
```bash
source /opt/fsl-imx-x11/4.1.15-2.1.0/environment-setup-cortexa7hf-neon-poky-linux-gnueabi 
```

Input:
```bash
qmake -v 
```

You will get following messages:

```bash
embedfire@dev1:~$ qmake -v
QMake version 3.0
Using Qt version 5.6.2 in /opt/fsl-imx-x11/4.1.15-2.1.0/sysroots/cortexa7hf-neon-poky-linux-gnueabi/usr/lib
```

> SDK installed successfully!

## Get source code

**clone**
```bash
git clone https://github.com/Embdefire/ebf_linux_qt_demo.git
```
## Build

```bash
./build.sh
```

Add privileges if `build.sh` fails to excute

```bash
chmod +x build.sh
```

## Deliverables

A 'run_dir' sub-directory will be created under current directory. There are 'App/libqui/libskin' files. App can run directly on the ARM board!
Meanwhile, a 'fire-app-xxxx.tar.bz2' file will be packed. You can copy it to the corresponding directory to extract and replace the old 'App'.

## Clean build folders

```bash
make distclean
```


