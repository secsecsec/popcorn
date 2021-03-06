#!/bin/sh

# Copyright 2013-2014 Antonio Barbalace, SSRG VT

LINUX_FILE_PARAMS="linux_boot_args_"
PENGUIN_FILE_PARAMS="penguin_boot_args_"
FILE_ARGS=".args"
FILE_PARAM=".param"
RAMDISK_OFFSET=512
SUPPRESS=0

CPUS=`cat /proc/cpuinfo | grep processor | awk '{print $3}'`

for CPU in $CPUS
do

  ARGS=`./create_bootargs.sh $CPU`
  if [ $? -ne 0 ]
  then
    if [ $SUPPRESS -ne 1 ]
    then
      echo "WARN if this is not clustering this is an ERROR: $ARGS"
      SUPPRESS=1
    fi
    ARGS=
  fi

if [ -n "$ARGS" ] 
then 
echo $ARGS > $LINUX_FILE_PARAMS$CPU$FILE_ARGS
echo $ARGS > $PENGUIN_FILE_PARAMS$CPU$FILE_ARGS
  for ELEM in $ARGS
  do

     case $ELEM in
     # *memmap=[0-9]*M\$*) echo "$ELEM" ;;
     *memmap=[0-9]*M@*) 
       SIZE=${ELEM#memmap=}
       START=${ELEM#memmap*@}
       echo "FOUND $ELEM ${SIZE%%M*M} ${START%M}" 
     ;;
     *present_mask=*)
       CPUUU=${ELEM#present_mask=}
       echo "FOUND $ELEM $CPUUU"
       if [ ${CPUUU%-*} -ne $CPU ]
       then
         echo "WARN CPUUU ${CPUUU%-*} while CPU $CPU"
       fi
     ;;
    esac
  done
  BOOT_ADDR=`printf "0x%x\n" $(( ${START%M} << 20 ))`
  RAMDISK_ADDR=`printf "0x%x\n" $(( ( ${START%M} + $RAMDISK_OFFSET ) << 20 ))`

 # echo $ARGS > $FILE_PARAMS$CPU$FILE_ARGS
  echo "vmlinux.elf $LINUX_FILE_PARAMS$CPU$FILE_ARGS $CPU $BOOT_ADDR $RAMDISK_ADDR" > $LINUX_FILE_PARAMS$CPU$FILE_PARAM
PENGUIN_BOOT_ADDR=`printf "0x%x\n" $((BOOT_ADDR + 0x400000))`
  echo "kernel.elf $PENGUIN_FILE_PARAMS$CPU$FILE_ARGS $CPU $PENGUIN_BOOT_ADDR $RAMDISK_ADDR" > $PENGUIN_FILE_PARAMS$CPU$FILE_PARAM
fi
  START_ADDR=""

done
