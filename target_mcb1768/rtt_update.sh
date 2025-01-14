ELFFILE=$1
ELFDIR=${1%/*} 
# LOADADDRESS="$(arm-none-eabi-objdump $ELFFILE -x | grep __Vectors | awk -F" " '{print $1;}')"
RTTADDRESS="$(arm-none-eabi-objdump $ELFFILE -x | grep _SEGGER_RTT | awk -F" " '{print $1;}')"
RTTSIZE="$(arm-none-eabi-objdump $ELFFILE -x | grep _SEGGER_RTT | awk -F" " '{print $5;}')"
echo monitor rtt setup 0x$RTTADDRESS 0x$RTTSIZE '"SEGGER RTT"' > $ELFDIR/.gdbrtt