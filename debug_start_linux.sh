#IP="$(cat /etc/resolv.conf | grep nameserver | awk -F" " '{print $2;}')"
#IP="localhost"
#echo $IP
#echo bindto $IP > bindto.cfg
#echo target extended-remote $IP:3333 > .gdbremote

xterm -geometry +80+80 -e "openocd -f openocd.cfg" &
sleep 1
# xterm -geometry +600+100 -e "telnet $IP 3456" &
# putty -telnet -P 5678 $IP &
xterm -geometry 160x40+100+450 -e "gdb-multiarch -q" &  
#x-terminal-emulator -hold -e "gdb-multiarch -q" &
