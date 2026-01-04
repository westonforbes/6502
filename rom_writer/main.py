# ROM WRITER FOR 65C02 PROCESSOR & AT28C256 EEPROM
# Weston Forbes, westonforbes@gmail.com
# Initial Revision: 2025-12-28

# Note: When adding to this script, remember memory is read in little-endian format!



def initialize_blank_rom()-> bytearray:
    """
    #### Description:
    Initializes a blank ROM image for a AT28C256 EEPROM and 65C02 processor.
    The ROM should be on the upper 32KB of the 64KB address space, as this
    method offsets vectors by 32KB accordingly.

    #### Returns:
        bytearray: The initialized ROM image.
    """
    
    # Define the rom size at 32KB (32768 bytes).
    # Defining in hex for clarity.
    rom_size = 0x8000

    # Initialize ROM with NOP instructions (0xEA).
    rom = bytearray([0xea] * rom_size)

    # Reset vector for the 65C02 processor is 0xFFFC (low byte) and 0xFFFD (high byte).
    # The 65C02 is able to address 64KB, we've offset the ROM to take up the upper 32KB.
    # Therefore, when working in rom scope, we need to subtract 0x8000 (32KB) from the reset vector address.
    reset_vector_lower_address = 0x7FFC # 0x7FFC = 0xFFFC - 0x8000 (32KB offset).
    reset_vector_upper_address = 0x7FFD # 0x7FFD = 0xFFFD - 0x8000 (32KB offset).

    # We want to set the reset vector to point to the start of our ROM (0x0000).
    # However, since this is a instruction for the processor, we need to add in the 32KB offset.
    # 0x0000 + 0x8000 = 0x8000.
    rom[reset_vector_lower_address] = 0x00 # Low byte of 0x8000.
    rom[reset_vector_upper_address] = 0x80 # High byte of 0x8000.

    # Return the initialized ROM that is filled with NOPs and has the reset vector set.
    return rom

def lda_example(rom: bytearray, address: int, value: int)-> None:
    """
    #### Description:
    Example function to add an LDA immediate instruction to the ROM image.

    #### Parameters:
        rom (bytearray): The ROM image to modify.
        address (int): The address in the ROM to insert the instruction.
        value (int): The immediate value to load into the accumulator.
    """
    
    # LDA immediate opcode for 65C02 is 0xA9.
    rom[address] = 0xA9       # Opcode for LDA immediate.
    rom[address + 1] = value  # Immediate value to load.
    rom[address + 2] = 0x8D   # Store Accumulator to absolute address.
    rom[address + 3] = 0x00   # Low byte of address 0x6000.
    rom[address + 4] = 0x60   # High byte of address 0x6000.

def save_rom_to_file(rom: bytearray, filename: str)-> None:
    """
    #### Description:
    Saves the ROM image to a binary file.

    #### Parameters:
        rom (bytearray): The ROM image to save.
        filename (str): The name of the file to save the ROM image to.
    """
    
    # Use "with" statement to safely open and write to the file.
    # "With" statement ensures the file is properly closed during normal operation and if exception occurs.
    with open(filename, 'wb') as file:

        # Write the ROM bytearray to the file.
        file.write(rom)

def clear_console()-> None:
    """
    #### Description:
    Clears the console screen.
    """
    
    import os

    # Clear command based on operating system.
    os.system('cls' if os.name == 'nt' else 'clear')

if __name__ == "__main__":
    
    # Clear the console at the start.
    clear_console()

    # Inform the user that ROM creation is starting.
    print("creating ROM image...", end='')

    # Initialize a blank ROM image.
    rom = initialize_blank_rom()

    # Load an example LDA immediate instruction at the start of the ROM.
    # We're loading the value 0x42 into the accumulator.
    lda_example(rom, 0x0000, 0x42) 

    # Save the ROM image to a binary file.
    save_rom_to_file(rom, 'rom_writer/rom.bin')

    # Indicate completion to the user.
    print("done.")

