# pre: pip install -U cmsis-svd terminaltables

# disable gdb warning that i don't understand)
set pagination off
set print pretty on
# load registers viewer
source gdb-svd.py
svd STM32G030.svd
# IP address can be changed in debug startup script
target extended-remote :3333
# file obj/BOOT.elf
define reload
    # monitor rtt stop
    monitor reset halt
    file obj/BLUEPILL.elf
    # shell ./rtt_update.sh
    # load
    monitor program build/g0synth.elf verify
    monitor reset halt
    # source obj/.gdbrtt
    tb main
    continue
    # monitor rtt start
    end
reload