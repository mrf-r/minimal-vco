# pre: pip install -U cmsis-svd terminaltables
# disable gdb warning that i don't understand)
set pagination off
set print pretty on
# load registers viewer
source ../gdb-svd.py
svd LPC176x5x.svd
target extended-remote :3333

define reload
    monitor rtt stop
    monitor reset halt
    # monitor program build/g0synth.elf verify
    file ../build/MCB1768.elf
    load
    # main ingridient for LPC RAM
    set $psp=0
    set $msp=(int)__vectors_start__
    # update rtt RAM base for openocd 
    shell ./rtt_update.sh ../build/MCB1768.elf
    source ../build/.gdbrtt
    tb main
    continue
    monitor rtt start
    end

reload