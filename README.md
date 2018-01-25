**QSTLink2**
========

[![GitHub version](https://badge.fury.io/gh/fpoussin%2Fqstlink2.svg)](https://badge.fury.io/gh/fpoussin%2Fqstlink2)  
[![Travis Build Status](https://travis-ci.org/fpoussin/QStlink2.svg?branch=master)](https://travis-ci.org/fpoussin/QStlink2) Travis - Ubuntu 14.04 - Qt5.2.1  
[![Jenkins Build Status](http://jenkins.netyxia.net/buildStatus/icon?job=QStlink2)](http://jenkins.netyxia.net/job/QStlink2/) Jenkins - Ubuntu 16.04 - Qt5.5.1  

**MOD**

else:LIBS += -L$$PWD -llibusb-1.0

**Description**  

QSTLink2 is a cross-platform STLinkV2 GUI.

It can perform the following actions:
 - Write
 - Read
 - Verify
 - Erase

**Downloads**

Windows binaries:  
 - https://github.com/fpoussin/QStlink2/releases  

Ubuntu PPA: 
 - https://launchpad.net/~fpoussin/+archive/ppa  

**Building on Linux**

*Build dependencies:*
 - libusb-1.0
 - QtUSB (included)
 - qt5-qmake
 - qtbase5-dev
 - libqt5gui5
 - libqt5xml5
 
*Build steps:*

    git clone git@github.com:fpoussin/QStlink2.git
    git submodule update --init
    qt5-qmake
    make
    sudo make install  # Optional


**Building in Windows**

You will need to run these commands from MSVC's CLI (I use MSVC2015 community)
You might need to install windows driver SDK (WDK) to get WinUSB libs/headers.
You can also build from Qt Creator, which is a lot easier.

I recommend that you build/download a static version of Qt 5 for Windows.  
You can find a guide here: https://github.com/fpoussin/Qt5-MSVC-Static

*Dependencies:*
 - MSVC 2010 or better (community/express editions work fine)
 - WinUsb (WDK)
 - QtUSB (included)
 - Qt 5.2 SDK or newer

*Build steps:*

    git clone git@github.com:fpoussin/QStlink2.git
    git submodule update --init
    qmake
    nmake

