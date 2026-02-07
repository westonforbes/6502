#!/usr/bin/env python3

def calculate_non_nop_percentage(filename):
    """Calculate the percentage of bytes that are not NOP (0xEA) instructions."""
    with open(filename, 'rb') as f:
        data = f.read()
    
    if len(data) == 0:
        return 0.0
    
    non_nop_count = sum(1 for byte in data if byte != 0xEA)
    percentage = (non_nop_count / len(data)) * 100
    
    return non_nop_count, len(data), percentage

if __name__ == '__main__':
    filename = 'assembly_programs/rom.bin'
    non_nop_count, total_bytes, percentage = calculate_non_nop_percentage(filename)
    print(f"Program Memory Utilization: {percentage:.2f}%")
    print(f"Bytes: {non_nop_count}/{total_bytes}")