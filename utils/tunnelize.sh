#!/bin/sh

TUN_ADDR=0x69c00000 # gigi

REPRESENTATIVE=`cat /proc/cpuinfo | grep processor | awk '{print $3}' | head -n 1`
K_INDEX=$(( $REPRESENTATIVE + 1 ))
echo tun_addr = $TUN_ADDR tun_cpu = $K_INDEX 

#NICE="nice -n 20"
TUNNEL="./bin/tunnel"

$TUNNEL $TUN_ADDR $REPRESENTATIVE &> /dev/null &
TUN_ID=""
while [ -z $TUN_ID ]
do
  TUN_ID=`ip -f inet link show |  awk '/tun[0-9]:/ {print $2}' | tail -n 1`
done

#TUN_DEV=${TUN_ID#tun}i
TUN_DEV=${TUN_ID%:}
echo "ifconfig $TUN_DEV 10.1.2.$K_INDEX up"
ifconfig $TUN_DEV 10.1.2.$K_INDEX up
route add -net 10.1.2.0 netmask 255.255.255.0 dev $TUN_DEV
echo "tunnel device $TUN_DEV setup on 10.1.2.$K_INDEX"
