### ROM Memory Map
- 0x0100 - 0x01FF: Stack.
- 0x6000 - 0x6003: LCD interface.
- 0x6004 - 0x7FFF: Unused, VIA is selected in this range.
- 0x8000 - 0xFFFB: Program memory.

### Prefix Notation
- $ = Hexadecimal prefix.
- % = Binary prefix.
- No prefix = Decimal number.

# ROM Writer
## Programmer Useage

### Windows
1. Use XGPro

### Linux (Minipro)

1. Install dependencies.
    ```shell
    sudo apt-get install build-essential pkg-config git libusb-1.0-0-dev
    ```

2. Clone the repository.
    ```shell
    git clone https://gitlab.com/DavidGriffith/minipro.git
    cd minipro
    ```
3. Build and install.
    ```shell
    make
    sudo make install
    ```

4. Set up udev rules (so you don't need sudo to use the programmer).
    ```shell
    sudo cp udev/*.rules /etc/udev/rules.d/
    sudo udevadm trigger
    ```

5. Add your user to the plugdev group.
    ```shell
    sudo usermod -a -G plugdev $USER
    ```

6. Install bash completion.
    ```shell
    sudo cp bash_completion.d/minipro /etc/bash_completion.d/
    ```

### Compile Assembly
```bash
./vasm6502_oldstyle -Fbin -dotdir assembly_programs/main.s -o assembly_programs/rom.bin
```

### Read Chip
```shell
minipro -p "AT28C256" -r assembly_programs/dump.bin
```

### Write Chip
```shell
minipro -p "AT28C256" -w assembly_programs/rom.bin
```

### Erase a chip.
```shell
minipro -p "AT28C256" -E
```
