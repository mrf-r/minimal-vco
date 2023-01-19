# Minimal STM32 oscillator project

### STM32G030F6P6

Pinout:

Analog controls (0 to 3.3 V):

-  7 - PA0 - ADC_0 - base pitch (0 to 5 octaves)
-  8 - PA1 - ADC_1 - gen1 octave crossfade (x1-x2-x3-x4)
-  9 - PA2 - ADC_2 - gen1 amplitude (-1 to 1)
- 10 - PA3 - ADC_3 - gen2 sync phase
- 11 - PA4 - ADC_4 - gen2 pitch offset (0 to 3 octaves)
- 12 - PA5 - ADC_5 - sync (from gen x1 to zero, from sync phase to rand)

Analog outputs (PWM 0 to 3.3 V):

- 13 - PA6 - TIM3_CH1 - gen2 + gen1 * amplitude
- 14 - PA7 - TIM3_CH2 - gen1
- 15 - PB0 - TIM3_CH3 - reserved

Digital input:

- 16 - PA11 - GPIOA_11 - pitch cv calibration trig

Digital output:

- 17 - PA12 - GPIOA_12 - led

System IO:

- 18 - PA13 - SWDIO
- 19 - PA14 - SWCLK/BOOT0 - tie 10k to GND
- 20 - PB6 - UART1_TX - MIDI OUT (reserved)
- 01 - PB7 - UART1_RX - MIDI IN (reserved)

# build:


```bash
# python3 and modules for the st ip debug only and can be omitted
sudo apt install gcc-arm-none-eabi openocd gdb-multiarch python3
pip install -U pip cmsis-svd terminaltables
# build target
make all -j
#flash target
openocd -f openocd-flash.cfg
```

# usefull:

approximate execution time without debugging (in case of linear code):

```bash
arm-none-eabi-objdump --disassemble=vcoTap build/g0synth.elf > disasm.txt
```
