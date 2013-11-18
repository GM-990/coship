#!/bin/bash

BOOT_CLASSES=$MMCP_HOME/jvm/build/$PLATFORM_JAVAVM/btclasses.zip:$MMCP_HOME/classes/lib/classes.jar:$MMCP_HOME/$MMCP_MIDDLEWARE/lib/$MMCP_MIDDLEWARE.jar

if [ "x$MMCP_MIDDLEWARE" != "xdtvm" ]; then
  BOOT_CLASSES=$BOOT_CLASSES:$MMCP_HOME/gem/lib/gem.jar
fi

size="-Dscreensize=720*576"
main="org.avmsc.dtvm.test.PackageMain"

while ([ "x$1" != "x" ])
do
  case $1 in
  -m)
	shift	
	main="$1"
	size=
	param=
	;;
  -r)
	shift
	if [ "x$1" = "xs" ]; then
	   size=-D"screensize=720*576"
	   param="generalsd"
	elif [ "x$1" = "xh" ]; then
	   size="-Dscreensize=1280*720"
	   param="generalhd"
	fi
	;;
  *)
        break
	;;
  esac
done

$MMCP_HOME/test/exec/$PLATFORM/dtvmtest.out -Duser.home=$WORK_DIR -Djava.class.path=.:$MMCP_HOME/test/dtvm/javacode -Xbootclasspath=$MMCP_HOME/jvm/build/$PLATFORM_JAVAVM/btclasses.zip:$MMCP_HOME/classes/lib/classes.jar:$MMCP_HOME/$MMCP_MIDDLEWARE/lib/$MMCP_MIDDLEWARE.jar:$MMCP_HOME/test/dtvm/lib/testsuite.jar $size $main $param

