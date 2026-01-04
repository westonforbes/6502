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

### Read Chip
```shell
minipro -p "AT28C256" -r dump.bin
```

### Write Chip
```shell
minipro -p "AT28C256" -w rom.bin
```

### Erase a chip.
```shell
minipro -p "AT28C256" -E
```
