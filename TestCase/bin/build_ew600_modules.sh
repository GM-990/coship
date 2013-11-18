#!/bin/sh
##########################################################################
# 编译和安装所有QT相关模块
# 参数： --sub      选择编译子模块
#        --all      编译所有模块
#        默认：     3秒后开始编译所有模块（可发SIGINT停止）
# 编译错误和提示信息保存在MMCP_HOME/build_error.log中
# Copyright (c) coship， lixiongfa @ 2011.6.30
# Modify: 
#   lixiongfa @ 2011.07.06:
#       向错误报告中输出所有提示信息
##########################################################################

if [ -z $MMCP_HOME ] || [ -z $MMCP_PLATFORM ] || [ -z $MMCP_COMPILING_MODE ]; then
    echo "ERROR: setupenv must be executed at first, exit!" | tee $PWD/build_error.log
    exit 0
fi

INSTALLING_MODULE=""
GENMAKEFILE=""
BUILD_LOG=$MMCP_HOME/build_error.log
#QT_INSTALL_PATH=/usr/local/qt_install/$MMCP_PLATFORM
QT_INSTALL_PATH=/usr/local/qt_install
QT_LIB_INSTALL_PATH=$MMCP_HOME/lib/$MMCP_PLATFORM/$MMCP_COMPILING_MODE/qt_lib
QT_SOURCE_CODE_DIR=$MMCP_HOME/QT/qt-everywhere-opensource-src-4.6.3

if [ -f $BUILD_LOG ] ; then
    rm -vf $BUILD_LOG
fi

if [ $MMCP_PLATFORM == bcm7019 ] ; then
    GENMAKEFILE=genMakefile.sh
elif [ $MMCP_PLATFORM == hi3716M ] || [ $MMCP_PLATFORM == hi3716H ]; then
    GENMAKEFILE=genMakefile_hisi3716.sh
fi

cd $MMCP_HOME

##########################################################################
# build QT
##########################################################################
build_QT()
{
    INSTALLING_MODULE="<QT>"
    echo $INSTALLING_MODULE "Build" | tee -a $BUILD_LOG
    QT_INSTALL_SUCCESS=0
    cd $MMCP_HOME/QT/build
    if [ -f $GENMAKEFILE ] ; then
        echo "$INSTALLING_MODULE Generate Makefile..." | tee -a $BUILD_LOG
        chmod +x $GENMAKEFILE
        . $GENMAKEFILE
    else
        echo "ERROR: configure QT failed... $GENMAKEFILE is missing" | tee -a $BUILD_LOG
    fi
    
    cd $MMCP_HOME/QT/build/$MMCP_PLATFORM/$MMCP_COMPILING_MODE
    
    if [ -f Makefile ] ; then
        echo "$INSTALLING_MODULE Compiling..." | tee -a $BUILD_LOG
        echo "========================== $INSTALLING_MODULE ==========================" | tee -a $BUILD_LOG
        make clean && make -j4 | tee -a $BUILD_LOG && make install && QT_INSTALL_SUCCESS=1
    else
        echo "$INSTALLING_MODULE ERROR: Makefile is missing... genMakefile.sh maybe failed" | tee -a $BUILD_LOG
    fi
    
    if [ $QT_INSTALL_SUCCESS == 1 ] ; then
        echo "$INSTALLING_MODULE Copy headers of our classes..."  | tee -a $BUILD_LOG
        cp $QT_SOURCE_CODE_DIR/src/plugins/gfxdrivers/directfb/cs*.h $QT_INSTALL_PATH/include/QtGui
    else
        echo "$INSTALLING_MODULE build failed, exit!" | tee -a $BUILD_LOG
        exit 0
    fi
    
    echo "Installing QT libs..." | tee -a $BUILD_LOG
    if [ ! -d $QT_LIB_INSTALL_PATH ]; then 
          mkdir -p $QT_LIB_INSTALL_PATH
    else
          rm -rvf $QT_LIB_INSTALL_PATH/*
    fi 
    
    echo "pwd: $PWD" | tee -a $BUILD_LOG
    cp -ar lib/libQtGui.so* $QT_LIB_INSTALL_PATH
    cp -ar lib/libQtCore.so* $QT_LIB_INSTALL_PATH
    cp -ar lib/libQtNetwork.so* $QT_LIB_INSTALL_PATH
    cp -ar lib/libQtWebKit.so* $QT_LIB_INSTALL_PATH
    cp -ar lib/libQtScript.so* $QT_LIB_INSTALL_PATH
    cp -ar lib/libQtXml.so* $QT_LIB_INSTALL_PATH
    cp -ar plugins $QT_LIB_INSTALL_PATH
    
    if [ $MMCP_COMPILING_MODE == release ]; then
        if [ $MMCP_PLATFORM == "bcm7019" ]; then
            find $QT_LIB_INSTALL_PATH -name "lib*" | xargs mipsel-linux-strip
        elif [ $MMCP_PLATFORM == "hi3716M" ] || [ $MMCP_PLATFORM == "hi3716H" ] ; then
            find $QT_LIB_INSTALL_PATH -name "lib*" | xargs arm-hisiv200-linux-strip
        fi
    fi
    
    cd $MMCP_HOME
    
    if [ -f $MMCP_HOME/QT/build/$MMCP_PLATFORM/$MMCP_COMPILING_MODE/bin/qmake ]; then
        echo "$MMCP_HOME/QT/build/$MMCP_PLATFORM/$MMCP_COMPILING_MODE/bin/qmake exist, chmod" | tee -a $BUILD_LOG
        chmod +x $MMCP_HOME/QT/build/$MMCP_PLATFORM/$MMCP_COMPILING_MODE/bin/qmake
    else
        echo "$MMCP_HOME/QT/build/$MMCP_PLATFORM/$MMCP_COMPILING_MODE/bin/qmake not found, exit!" | tee -a $BUILD_LOG
        exit 1
    fi
    
    if [ -f $QT_INSTALL_PATH/bin/qmake ]; then
        echo "$QT_INSTALL_PATH/bin/qmake exist, QT install success" | tee -a $BUILD_LOG
        chmod +x $QT_INSTALL_PATH/bin/qmake
    else
        echo "$QT_INSTALL_PATH/bin/qmake not found, exit!" | tee -a $BUILD_LOG
        exit 1
    fi

}

##########################################################################
# build webkitshell
##########################################################################
build_webkitshell()
{
    INSTALLING_MODULE="<webkitshell>"
    echo "$INSTALLING_MODULE Build"  | tee -a $BUILD_LOG
    cd $MMCP_HOME/webkitshell
    if [ $MMCP_PLATFORM == "bcm7019" ]; then
        #$MMCP_HOME/QT/build/$MMCP_PLATFORM/$MMCP_COMPILING_MODE/bin/qmake -spec $MMCP_HOME/QT/qt-everywhere-opensource-src-4.6.3/mkspecs/qws/linux-mipsel-g++ -unix -o Makefile webkitshell.pro
        #qmake -spec $MMCP_HOME/QT/qt-everywhere-opensource-src-4.6.3/mkspecs/qws/linux-mipsel-g++ -unix -o Makefile webkitshell.pro
        $MMCP_HOME/QT/build/$MMCP_PLATFORM/$MMCP_COMPILING_MODE/bin/qmake webkitshell.pro
    elif [ $MMCP_PLATFORM == "hi3716M" ] || [ $MMCP_PLATFORM == hi3716H ]; then
        #$MMCP_HOME/QT/build/$MMCP_PLATFORM/$MMCP_COMPILING_MODE/bin/qmake -spec $MMCP_HOME/QT/qt-everywhere-opensource-src-4.6.3/mkspecs/qws/linux-arm-g++ -unix -o Makefile webkitshell.pro
        #qmake -spec $MMCP_HOME/QT/qt-everywhere-opensource-src-4.6.3/mkspecs/qws/linux-arm-g++ -unix -o Makefile webkitshell.pro
        $MMCP_HOME/QT/build/$MMCP_PLATFORM/$MMCP_COMPILING_MODE/bin/qmake webkitshell.pro
    fi
    
    if [ -f Makefile ]; then
         echo "$INSTALLING_MODULE Compiling..." | tee -a $BUILD_LOG
         echo "========================== $INSTALLING_MODULE ==========================" | tee -a $BUILD_LOG
         make clean && make -j4 2>&1 | tee -a $BUILD_LOG
    else
        echo "$INSTALLING_MODULE Makefile is missing" | tee -a $BUILD_LOG
    fi
    
    if [ -f libwebkitshell.a ]; then
        rm -vf ../lib/$MMCP_PLATFORM/$MMCP_COMPILING_MODE/libwebkitshell.a
        cp libwebkitshell.a ../lib/$MMCP_PLATFORM/$MMCP_COMPILING_MODE
        echo "$INSTALLING_MODULE build complete" | tee -a $BUILD_LOG
    else
          echo "Build $INSTALLING_MODULE failed" | tee -a $BUILD_LOG
    fi
    
    cd $MMCP_HOME
}

##########################################################################
# build QtApp/Murphytalk
##########################################################################
build_Murphytalk()
{
    INSTALLING_MODULE="<QtApp/Murphytalk>"
    echo "$INSTALLING_MODULE Build" | tee -a $BUILD_LOG
    cd $MMCP_HOME/QtApp/Murphytalk
    if [ $MMCP_PLATFORM == "bcm7019" ]; then
        #$MMCP_HOME/QT/build/$MMCP_PLATFORM/$MMCP_COMPILING_MODE/bin/qmake -spec $MMCP_HOME/QT/qt-everywhere-opensource-src-4.6.3/mkspecs/qws/linux-mipsel-g++ -unix -o Makefile Murphytalk.pro
        #qmake -spec $MMCP_HOME/QT/qt-everywhere-opensource-src-4.6.3/mkspecs/qws/linux-mipsel-g++ -unix -o Makefile Murphytalk.pro
        $MMCP_HOME/QT/build/$MMCP_PLATFORM/$MMCP_COMPILING_MODE/bin/qmake Murphytalk.pro
    elif [ $MMCP_PLATFORM == "hi3716M" ] || [ $MMCP_PLATFORM == hi3716H ]; then
        #$MMCP_HOME/QT/build/$MMCP_PLATFORM/$MMCP_COMPILING_MODE/bin/qmake -spec $MMCP_HOME/QT/qt-everywhere-opensource-src-4.6.3/mkspecs/qws/linux-arm-g++ -unix -o Makefile Murphytalk.pro
        #qmake -spec $MMCP_HOME/QT/qt-everywhere-opensource-src-4.6.3/mkspecs/qws/linux-arm-g++ -unix -o Makefile Murphytalk.pro
        $MMCP_HOME/QT/build/$MMCP_PLATFORM/$MMCP_COMPILING_MODE/bin/qmake Murphytalk.pro
    fi
    
    if [ -f Makefile ]; then
        echo "$INSTALLING_MODULE Compiling..."
        echo "========================== $INSTALLING_MODULE ==========================" | tee -a $BUILD_LOG
        make clean && make -j4 2>&1 | tee -a $BUILD_LOG
    else
        echo "$INSTALLING_MODULE Makefile is missing" | tee -a $BUILD_LOG
    fi
    
    if [ -f libinputmethod.so.1.0.0 ]; then
        rm -vf $QT_LIB_INSTALL_PATH/libinputmethod.so*
        cp -ar libinputmethod.so* $QT_LIB_INSTALL_PATH
        echo "$INSTALLING_MODULE build complete" | tee -a $BUILD_LOG
    else
          echo "Build $INSTALLING_MODULE failed" | tee -a $BUILD_LOG
    fi
    
    cd $MMCP_HOME
}

##########################################################################
# build QtApp/cuteext
##########################################################################
build_cuteext()
{
    INSTALLING_MODULE="<QtApp/cuteext>"
    echo "$INSTALLING_MODULE Build" | tee -a $BUILD_LOG
    cd $MMCP_HOME/QtApp/cuteext
    if [ $MMCP_PLATFORM == "bcm7019" ]; then
        #$MMCP_HOME/QT/build/$MMCP_PLATFORM/$MMCP_COMPILING_MODE/bin/qmake -spec $MMCP_HOME/QT/qt-everywhere-opensource-src-4.6.3/mkspecs/qws/linux-mipsel-g++ -unix -o Makefile Murphytalk.pro
        #qmake -spec $MMCP_HOME/QT/qt-everywhere-opensource-src-4.6.3/mkspecs/qws/linux-mipsel-g++ -unix -o Makefile Murphytalk.pro
        $MMCP_HOME/QT/build/$MMCP_PLATFORM/$MMCP_COMPILING_MODE/bin/qmake cuteext.pro
    elif [ $MMCP_PLATFORM == "hi3716M" ] || [ $MMCP_PLATFORM == hi3716H ]; then
        #$MMCP_HOME/QT/build/$MMCP_PLATFORM/$MMCP_COMPILING_MODE/bin/qmake -spec $MMCP_HOME/QT/qt-everywhere-opensource-src-4.6.3/mkspecs/qws/linux-arm-g++ -unix -o Makefile Murphytalk.pro
        #qmake -spec $MMCP_HOME/QT/qt-everywhere-opensource-src-4.6.3/mkspecs/qws/linux-arm-g++ -unix -o Makefile Murphytalk.pro
        $MMCP_HOME/QT/build/$MMCP_PLATFORM/$MMCP_COMPILING_MODE/bin/qmake cuteext.pro
    fi
    
    if [ -f Makefile ]; then
        echo "$INSTALLING_MODULE Compiling..."
        echo "========================== $INSTALLING_MODULE ==========================" | tee -a $BUILD_LOG
        make clean && make -j4 2>&1 | tee -a $BUILD_LOG
    else
        echo "$INSTALLING_MODULE Makefile is missing" | tee -a $BUILD_LOG
    fi
    
    cd lib
    
    if [ -f libcuteext.so.1.0.0 ]; then
        rm -vf $QT_LIB_INSTALL_PATH/libcuteext.so*
        cp -ar libcuteext.so* $QT_LIB_INSTALL_PATH
        echo "$INSTALLING_MODULE build complete" | tee -a $BUILD_LOG
    else
          echo "Build $INSTALLING_MODULE failed" | tee -a $BUILD_LOG
    fi
    
    cd $MMCP_HOME
}

##########################################################################
# build QtMicrowin
##########################################################################
build_QtMicrowin()
{
    INSTALLING_MODULE="<QtMicrowin>"
    echo "$INSTALLING_MODULE Build" | tee -a $BUILD_LOG
    cd $MMCP_HOME/QtMicrowin
    if [ $MMCP_PLATFORM == "bcm7019" ]; then
        #$MMCP_HOME/QT/build/$MMCP_PLATFORM/$MMCP_COMPILING_MODE/bin/qmake -spec $MMCP_HOME/QT/qt-everywhere-opensource-src-4.6.3/mkspecs/qws/linux-mipsel-g++ -unix -o Makefile qtmicrowin.pro
        #qmake -spec $MMCP_HOME/QT/qt-everywhere-opensource-src-4.6.3/mkspecs/qws/linux-mipsel-g++ -unix -o Makefile qtmicrowin.pro
        $MMCP_HOME/QT/build/$MMCP_PLATFORM/$MMCP_COMPILING_MODE/bin/qmake qtmicrowin.pro
    elif [ $MMCP_PLATFORM == "hi3716M" ] || [ $MMCP_PLATFORM == hi3716H ]; then
        #$MMCP_HOME/QT/build/$MMCP_PLATFORM/$MMCP_COMPILING_MODE/bin/qmake -spec $MMCP_HOME/QT/qt-everywhere-opensource-src-4.6.3/mkspecs/qws/linux-arm-g++ -unix -o Makefile qtmicrowin.pro
        #qmake -spec $MMCP_HOME/QT/qt-everywhere-opensource-src-4.6.3/mkspecs/qws/linux-arm-g++ -unix -o Makefile qtmicrowin.pro
        $MMCP_HOME/QT/build/$MMCP_PLATFORM/$MMCP_COMPILING_MODE/bin/qmake qtmicrowin.pro
    fi
    
    if [ -f Makefile ]; then
        echo "$INSTALLING_MODULE Compiling..." | tee -a $BUILD_LOG
        echo "========================== $INSTALLING_MODULE ==========================" | tee -a $BUILD_LOG
        make clean && make -j4 2>&1 | tee -a $BUILD_LOG
    else
        echo "$INSTALLING_MODULE Makefile is missing" | tee -a $BUILD_LOG
    fi
    
    if [ -f libqtmicrowin.a ]; then
        rm -vf ../lib/$MMCP_PLATFORM/$MMCP_COMPILING_MODE/libqtmicrowin.a
        cp -ar libqtmicrowin.a ../lib/$MMCP_PLATFORM/$MMCP_COMPILING_MODE
        echo "$INSTALLING_MODULE build complete" | tee -a $BUILD_LOG
    else
          echo "Build $INSTALLING_MODULE failed" | tee -a $BUILD_LOG
    fi
    
    cd $MMCP_HOME
}

##########################################################################
# build webkitPlatform
##########################################################################
build_webkitPlatform()
{
      INSTALLING_MODULE="<webkitPlatform>"
      echo "$INSTALLING_MODULE Build" | tee -a $BUILD_LOG
      cd $MMCP_HOME/webkitPlatform
      
      if [ -f Makefile ]; then
            echo "$INSTALLING_MODULE Compiling..." | tee -a $BUILD_LOG
            echo "========================== $INSTALLING_MODULE ==========================" | tee -a $BUILD_LOG
            make clean && make -j4 2>&1 | tee -a $BUILD_LOG
      else
            echo "$INSTALLING_MODULE Makefile is missing" | tee -a $BUILD_LOG
      fi
      
      cd $MMCP_HOME
}

###########################################################################
# build_sub
###########################################################################
build_sub()
{
    echo "Select module to install:"
    echo "    1. QT"
    echo "    2. webkitshell"
    echo "    3. QtApp/Murphytalk"
    echo "    4. QtApp/cuteext"
    echo "    5. QtMicrowin"
    echo "    6. webkitPlatform"
    read SELECT
    case $SELECT in
    1)
    build_QT
    exit 0
    ;;
    2)
    build_webkitshell
    exit 0
    ;;
    3)
    build_Murphytalk
    exit 0
    ;;
    4)
    build_cuteext
    exit 0
    ;;
    5)
    build_QtMicrowin
    exit 0
    ;;
    6)
    build_webkitPlatform
    exit 0
    ;;
    *)
    echo "Invalid param"
    ;;
    esac
}

###########################################################################
# build_all
###########################################################################
build_all()
{
    if [ ! $MMCP_PLATFORM == "hi3716M_hisiv200" ]; then
        build_QT
        build_webkitshell
        build_Murphytalk
        build_cuteext
        build_QtMicrowin
        build_webkitPlatform
    else
        if [ ! -d $MMCP_HOME/lib/$MMCP_PLATFORM/$MMCP_COMPILING_MODE ]; then
            mkdir -p $MMCP_HOME/lib/$MMCP_PLATFORM/$MMCP_COMPILING_MODE
        fi
        
        if [ -d $MMCP_HOME/lib/hi3716H/$MMCP_COMPILING_MODE ]; then
            cp -avf $MMCP_HOME/lib/hi3716H/$MMCP_COMPILING_MODE/libwebkitshell.a $MMCP_HOME/lib/$MMCP_PLATFORM/$MMCP_COMPILING_MODE
            cp -avf $MMCP_HOME/lib/hi3716H/$MMCP_COMPILING_MODE/libqtmicrowin.a $MMCP_HOME/lib/$MMCP_PLATFORM/$MMCP_COMPILING_MODE
            cp -avf $MMCP_HOME/lib/hi3716H/$MMCP_COMPILING_MODE/libwebkitPlatform.a $MMCP_HOME/lib/$MMCP_PLATFORM/$MMCP_COMPILING_MODE
        fi
    fi
}

###########################################################################
# defaultdo
###########################################################################
defaultdo()
{
    echo "-----------------------------------------------------------------"
    echo "|       Warnning: ALL modules will be built after 3 second      |"
    echo "-----------------------------------------------------------------"
    echo -ne "\n[Build ALL] will start in 3 sec"
    sleep 1
    echo -ne "\b\b\b\b\b2 sec"
    sleep 1
    echo -ne "\b\b\b\b\b1 sec"
    sleep 1
    clear
    echo "[Build ALL] Start                    "
    build_all
}

###########################################################################
# showusage
###########################################################################
showusage()
{
    echo "Usage : "
    echo "build_modules.sh [ -all | -sub MODULENAME ]"
    echo "        --all: build all of the modules"
    echo "        --sub: build sub module"
    echo "        default(no param): build all of the modules after 3 second"
}

###########################################################################
# main
###########################################################################
export PATH=$QT_INSTALL_PATH/bin:$PATH

#if [ "x$1" != "x" ];then
#  case $1 in
#  --sub)
#  build_sub
#	;;
#  --all)
#  build_all
#  ;;
#  *)
#  showusage
#  ;;
#  esac
#else
#  defaultdo
#fi

build_all
