// Convert mnemonic string to hex opcode
// Returns 0xFF if not found
byte opcodeToHex(String mnemonic) {
    mnemonic.toUpperCase();
    
    // Row 0
    if (mnemonic == "BRK") return 0x00;
    if (mnemonic == "ORA_ZP_X") return 0x15;
    if (mnemonic == "TSB_ZP") return 0x04;
    if (mnemonic == "ORA_ZP") return 0x05;
    if (mnemonic == "ASL_ZP") return 0x06;
    if (mnemonic == "RMB0_ZP") return 0x07;
    if (mnemonic == "PHP") return 0x08;
    if (mnemonic == "ORA_IMM") return 0x09;
    if (mnemonic == "ASL_A") return 0x0A;
    if (mnemonic == "TSB_ABS") return 0x0C;
    if (mnemonic == "ORA_ABS") return 0x0D;
    if (mnemonic == "ASL_ABS") return 0x0E;
    if (mnemonic == "BBR0") return 0x0F;
    
    // Row 1
    if (mnemonic == "BPL") return 0x10;
    if (mnemonic == "ORA_ZP_Y") return 0x11;
    if (mnemonic == "ORA_ZP_IND") return 0x12;
    if (mnemonic == "TRB_ZP") return 0x14;
    if (mnemonic == "ORA_ZP_X") return 0x15;
    if (mnemonic == "ASL_ZP_X") return 0x16;
    if (mnemonic == "RMB1_ZP") return 0x17;
    if (mnemonic == "CLC") return 0x18;
    if (mnemonic == "ORA_ABS_Y") return 0x19;
    if (mnemonic == "INC_A") return 0x1A;
    if (mnemonic == "TRB_ABS") return 0x1C;
    if (mnemonic == "ORA_ABS_X") return 0x1D;
    if (mnemonic == "ASL_ABS_X") return 0x1E;
    if (mnemonic == "BBR1") return 0x1F;
    
    // Row 2
    if (mnemonic == "JSR") return 0x20;
    if (mnemonic == "AND_ZP_X") return 0x21;
    if (mnemonic == "BIT_ZP") return 0x24;
    if (mnemonic == "AND_ZP") return 0x25;
    if (mnemonic == "ROL_ZP") return 0x26;
    if (mnemonic == "RMB2_ZP") return 0x27;
    if (mnemonic == "PLP") return 0x28;
    if (mnemonic == "AND_IMM") return 0x29;
    if (mnemonic == "ROL_A") return 0x2A;
    if (mnemonic == "BIT_ABS") return 0x2C;
    if (mnemonic == "AND_ABS") return 0x2D;
    if (mnemonic == "ROL_ABS") return 0x2E;
    if (mnemonic == "BBR2") return 0x2F;
    
    // Row 3
    if (mnemonic == "BMI") return 0x30;
    if (mnemonic == "AND_ZP_Y") return 0x31;
    if (mnemonic == "AND_ZP_IND") return 0x32;
    if (mnemonic == "BIT_ZP_X") return 0x34;
    if (mnemonic == "AND_ZP_X") return 0x35;
    if (mnemonic == "ROL_ZP_X") return 0x36;
    if (mnemonic == "RMB3_ZP") return 0x37;
    if (mnemonic == "SEC") return 0x38;
    if (mnemonic == "AND_ABS_Y") return 0x39;
    if (mnemonic == "DEC_A") return 0x3A;
    if (mnemonic == "BIT_ABS_X") return 0x3C;
    if (mnemonic == "AND_ABS_X") return 0x3D;
    if (mnemonic == "ROL_ABS_X") return 0x3E;
    if (mnemonic == "BBR3") return 0x3F;
    
    // Row 4
    if (mnemonic == "RTI") return 0x40;
    if (mnemonic == "EOR_ZP_X") return 0x41;
    if (mnemonic == "EOR_ZP") return 0x45;
    if (mnemonic == "LSR_ZP") return 0x46;
    if (mnemonic == "RMB4_ZP") return 0x47;
    if (mnemonic == "PHA") return 0x48;
    if (mnemonic == "EOR_IMM") return 0x49;
    if (mnemonic == "LSR_A") return 0x4A;
    if (mnemonic == "JMP_ABS") return 0x4C;
    if (mnemonic == "EOR_ABS") return 0x4D;
    if (mnemonic == "LSR_ABS") return 0x4E;
    if (mnemonic == "BBR4") return 0x4F;
    
    // Row 5
    if (mnemonic == "BVC") return 0x50;
    if (mnemonic == "EOR_ZP_Y") return 0x51;
    if (mnemonic == "EOR_ZP_IND") return 0x52;
    if (mnemonic == "EOR_ZP_X") return 0x55;
    if (mnemonic == "LSR_ZP_X") return 0x56;
    if (mnemonic == "RMB5_ZP") return 0x57;
    if (mnemonic == "CLI") return 0x58;
    if (mnemonic == "EOR_ABS_Y") return 0x59;
    if (mnemonic == "PHY") return 0x5A;
    if (mnemonic == "EOR_ABS_X") return 0x5D;
    if (mnemonic == "LSR_ABS_X") return 0x5E;
    if (mnemonic == "BBR5") return 0x5F;
    
    // Row 6
    if (mnemonic == "RTS") return 0x60;
    if (mnemonic == "ADC_ZP_X") return 0x61;
    if (mnemonic == "STZ_ZP") return 0x64;
    if (mnemonic == "ADC_ZP") return 0x65;
    if (mnemonic == "ROR_ZP") return 0x66;
    if (mnemonic == "RMB6_ZP") return 0x67;
    if (mnemonic == "PLA") return 0x68;
    if (mnemonic == "ADC_IMM") return 0x69;
    if (mnemonic == "ROR_A") return 0x6A;
    if (mnemonic == "JMP_IND") return 0x6C;
    if (mnemonic == "ADC_ABS") return 0x6D;
    if (mnemonic == "ROR_ABS") return 0x6E;
    if (mnemonic == "BBR6") return 0x6F;
    
    // Row 7
    if (mnemonic == "BVS") return 0x70;
    if (mnemonic == "ADC_ZP_Y") return 0x71;
    if (mnemonic == "ADC_ZP_IND") return 0x72;
    if (mnemonic == "STZ_ZP_X") return 0x74;
    if (mnemonic == "ADC_ZP_X") return 0x75;
    if (mnemonic == "ROR_ZP_X") return 0x76;
    if (mnemonic == "RMB7_ZP") return 0x77;
    if (mnemonic == "SEI") return 0x78;
    if (mnemonic == "ADC_ABS_Y") return 0x79;
    if (mnemonic == "PLY") return 0x7A;
    if (mnemonic == "JMP_IND_X") return 0x7C;
    if (mnemonic == "ADC_ABS_X") return 0x7D;
    if (mnemonic == "ROR_ABS_X") return 0x7E;
    if (mnemonic == "BBR7") return 0x7F;
    
    // Row 8
    if (mnemonic == "BRA") return 0x80;
    if (mnemonic == "STA_ZP_X") return 0x81;
    if (mnemonic == "STY_ZP") return 0x84;
    if (mnemonic == "STA_ZP") return 0x85;
    if (mnemonic == "STX_ZP") return 0x86;
    if (mnemonic == "SMB0_ZP") return 0x87;
    if (mnemonic == "DEY") return 0x88;
    if (mnemonic == "BIT_IMM") return 0x89;
    if (mnemonic == "TXA") return 0x8A;
    if (mnemonic == "STY_ABS") return 0x8C;
    if (mnemonic == "STA_ABS") return 0x8D;
    if (mnemonic == "STX_ABS") return 0x8E;
    if (mnemonic == "BBS0") return 0x8F;
    
    // Row 9
    if (mnemonic == "BCC") return 0x90;
    if (mnemonic == "STA_ZP_Y") return 0x91;
    if (mnemonic == "STA_ZP_IND") return 0x92;
    if (mnemonic == "STY_ZP_X") return 0x94;
    if (mnemonic == "STA_ZP_X") return 0x95;
    if (mnemonic == "STX_ZP_Y") return 0x96;
    if (mnemonic == "SMB1_ZP") return 0x97;
    if (mnemonic == "TYA") return 0x98;
    if (mnemonic == "STA_ABS_Y") return 0x99;
    if (mnemonic == "TXS") return 0x9A;
    if (mnemonic == "STZ_ABS") return 0x9C;
    if (mnemonic == "STA_ABS_X") return 0x9D;
    if (mnemonic == "STZ_ABS_X") return 0x9E;
    if (mnemonic == "BBS1") return 0x9F;
    
    // Row A
    if (mnemonic == "LDY_IMM") return 0xA0;
    if (mnemonic == "LDA_ZP_X") return 0xA1;
    if (mnemonic == "LDX_IMM") return 0xA2;
    if (mnemonic == "LDY_ZP") return 0xA4;
    if (mnemonic == "LDA_ZP") return 0xA5;
    if (mnemonic == "LDX_ZP") return 0xA6;
    if (mnemonic == "SMB2_ZP") return 0xA7;
    if (mnemonic == "TAY") return 0xA8;
    if (mnemonic == "LDA_IMM") return 0xA9;
    if (mnemonic == "TAX") return 0xAA;
    if (mnemonic == "LDY_ABS") return 0xAC;
    if (mnemonic == "LDA_ABS") return 0xAD;
    if (mnemonic == "LDX_ABS") return 0xAE;
    if (mnemonic == "BBS2") return 0xAF;
    
    // Row B
    if (mnemonic == "BCS") return 0xB0;
    if (mnemonic == "LDA_ZP_Y") return 0xB1;
    if (mnemonic == "LDA_ZP_IND") return 0xB2;
    if (mnemonic == "LDY_ZP_X") return 0xB4;
    if (mnemonic == "LDA_ZP_X") return 0xB5;
    if (mnemonic == "LDX_ZP_Y") return 0xB6;
    if (mnemonic == "SMB3_ZP") return 0xB7;
    if (mnemonic == "CLV") return 0xB8;
    if (mnemonic == "LDA_ABS_Y") return 0xB9;
    if (mnemonic == "TSX") return 0xBA;
    if (mnemonic == "LDY_ABS_X") return 0xBC;
    if (mnemonic == "LDA_ABS_X") return 0xBD;
    if (mnemonic == "LDX_ABS_Y") return 0xBE;
    if (mnemonic == "BBS3") return 0xBF;
    
    // Row C
    if (mnemonic == "CPY_IMM") return 0xC0;
    if (mnemonic == "CMP_ZP_X") return 0xC1;
    if (mnemonic == "CPY_ZP") return 0xC4;
    if (mnemonic == "CMP_ZP") return 0xC5;
    if (mnemonic == "DEC_ZP") return 0xC6;
    if (mnemonic == "SMB4_ZP") return 0xC7;
    if (mnemonic == "INY") return 0xC8;
    if (mnemonic == "CMP_IMM") return 0xC9;
    if (mnemonic == "DEX") return 0xCA;
    if (mnemonic == "WAI") return 0xCB;
    if (mnemonic == "CPY_ABS") return 0xCC;
    if (mnemonic == "CMP_ABS") return 0xCD;
    if (mnemonic == "DEC_ABS") return 0xCE;
    if (mnemonic == "BBS4") return 0xCF;
    
    // Row D
    if (mnemonic == "BNE") return 0xD0;
    if (mnemonic == "CMP_ZP_Y") return 0xD1;
    if (mnemonic == "CMP_ZP_IND") return 0xD2;
    if (mnemonic == "CMP_ZP_X") return 0xD5;
    if (mnemonic == "DEC_ZP_X") return 0xD6;
    if (mnemonic == "SMB5_ZP") return 0xD7;
    if (mnemonic == "CLD") return 0xD8;
    if (mnemonic == "CMP_ABS_Y") return 0xD9;
    if (mnemonic == "PHX") return 0xDA;
    if (mnemonic == "STP") return 0xDB;
    if (mnemonic == "CMP_ABS_X") return 0xDD;
    if (mnemonic == "DEC_ABS_X") return 0xDE;
    if (mnemonic == "BBS5") return 0xDF;
    
    // Row E
    if (mnemonic == "CPX_IMM") return 0xE0;
    if (mnemonic == "SBC_ZP_X") return 0xE1;
    if (mnemonic == "CPX_ZP") return 0xE4;
    if (mnemonic == "SBC_ZP") return 0xE5;
    if (mnemonic == "INC_ZP") return 0xE6;
    if (mnemonic == "SMB6_ZP") return 0xE7;
    if (mnemonic == "INX") return 0xE8;
    if (mnemonic == "SBC_IMM") return 0xE9;
    if (mnemonic == "NOP") return 0xEA;
    if (mnemonic == "CPX_ABS") return 0xEC;
    if (mnemonic == "SBC_ABS") return 0xED;
    if (mnemonic == "INC_ABS") return 0xEE;
    if (mnemonic == "BBS6") return 0xEF;
    
    // Row F
    if (mnemonic == "BEQ") return 0xF0;
    if (mnemonic == "SBC_ZP_Y") return 0xF1;
    if (mnemonic == "SBC_ZP_IND") return 0xF2;
    if (mnemonic == "SBC_ZP_X") return 0xF5;
    if (mnemonic == "INC_ZP_X") return 0xF6;
    if (mnemonic == "SMB7_ZP") return 0xF7;
    if (mnemonic == "SED") return 0xF8;
    if (mnemonic == "SBC_ABS_Y") return 0xF9;
    if (mnemonic == "PLX") return 0xFA;
    if (mnemonic == "SBC_ABS_X") return 0xFD;
    if (mnemonic == "INC_ABS_X") return 0xFE;
    if (mnemonic == "BBS7") return 0xFF;
    
    return 0xFF;  // Not found
}

// Convert hex opcode to mnemonic string
String hexToOpcode(byte opcode) {
    switch (opcode) {
        // Row 0
        case 0x00: return "BRK";
        case 0x01: return "ORA_ZP_X";
        case 0x04: return "TSB_ZP";
        case 0x05: return "ORA_ZP";
        case 0x06: return "ASL_ZP";
        case 0x07: return "RMB0_ZP";
        case 0x08: return "PHP";
        case 0x09: return "ORA_IMM";
        case 0x0A: return "ASL_A";
        case 0x0C: return "TSB_ABS";
        case 0x0D: return "ORA_ABS";
        case 0x0E: return "ASL_ABS";
        case 0x0F: return "BBR0";
        
        // Row 1
        case 0x10: return "BPL";
        case 0x11: return "ORA_ZP_Y";
        case 0x12: return "ORA_ZP_IND";
        case 0x14: return "TRB_ZP";
        case 0x15: return "ORA_ZP_X";
        case 0x16: return "ASL_ZP_X";
        case 0x17: return "RMB1_ZP";
        case 0x18: return "CLC";
        case 0x19: return "ORA_ABS_Y";
        case 0x1A: return "INC_A";
        case 0x1C: return "TRB_ABS";
        case 0x1D: return "ORA_ABS_X";
        case 0x1E: return "ASL_ABS_X";
        case 0x1F: return "BBR1";
        
        // Row 2
        case 0x20: return "JSR";
        case 0x21: return "AND_ZP_X";
        case 0x24: return "BIT_ZP";
        case 0x25: return "AND_ZP";
        case 0x26: return "ROL_ZP";
        case 0x27: return "RMB2_ZP";
        case 0x28: return "PLP";
        case 0x29: return "AND_IMM";
        case 0x2A: return "ROL_A";
        case 0x2C: return "BIT_ABS";
        case 0x2D: return "AND_ABS";
        case 0x2E: return "ROL_ABS";
        case 0x2F: return "BBR2";
        
        // Row 3
        case 0x30: return "BMI";
        case 0x31: return "AND_ZP_Y";
        case 0x32: return "AND_ZP_IND";
        case 0x34: return "BIT_ZP_X";
        case 0x35: return "AND_ZP_X";
        case 0x36: return "ROL_ZP_X";
        case 0x37: return "RMB3_ZP";
        case 0x38: return "SEC";
        case 0x39: return "AND_ABS_Y";
        case 0x3A: return "DEC_A";
        case 0x3C: return "BIT_ABS_X";
        case 0x3D: return "AND_ABS_X";
        case 0x3E: return "ROL_ABS_X";
        case 0x3F: return "BBR3";
        
        // Row 4
        case 0x40: return "RTI";
        case 0x41: return "EOR_ZP_X";
        case 0x45: return "EOR_ZP";
        case 0x46: return "LSR_ZP";
        case 0x47: return "RMB4_ZP";
        case 0x48: return "PHA";
        case 0x49: return "EOR_IMM";
        case 0x4A: return "LSR_A";
        case 0x4C: return "JMP_ABS";
        case 0x4D: return "EOR_ABS";
        case 0x4E: return "LSR_ABS";
        case 0x4F: return "BBR4";
        
        // Row 5
        case 0x50: return "BVC";
        case 0x51: return "EOR_ZP_Y";
        case 0x52: return "EOR_ZP_IND";
        case 0x55: return "EOR_ZP_X";
        case 0x56: return "LSR_ZP_X";
        case 0x57: return "RMB5_ZP";
        case 0x58: return "CLI";
        case 0x59: return "EOR_ABS_Y";
        case 0x5A: return "PHY";
        case 0x5D: return "EOR_ABS_X";
        case 0x5E: return "LSR_ABS_X";
        case 0x5F: return "BBR5";
        
        // Row 6
        case 0x60: return "RTS";
        case 0x61: return "ADC_ZP_X";
        case 0x64: return "STZ_ZP";
        case 0x65: return "ADC_ZP";
        case 0x66: return "ROR_ZP";
        case 0x67: return "RMB6_ZP";
        case 0x68: return "PLA";
        case 0x69: return "ADC_IMM";
        case 0x6A: return "ROR_A";
        case 0x6C: return "JMP_IND";
        case 0x6D: return "ADC_ABS";
        case 0x6E: return "ROR_ABS";
        case 0x6F: return "BBR6";
        
        // Row 7
        case 0x70: return "BVS";
        case 0x71: return "ADC_ZP_Y";
        case 0x72: return "ADC_ZP_IND";
        case 0x74: return "STZ_ZP_X";
        case 0x75: return "ADC_ZP_X";
        case 0x76: return "ROR_ZP_X";
        case 0x77: return "RMB7_ZP";
        case 0x78: return "SEI";
        case 0x79: return "ADC_ABS_Y";
        case 0x7A: return "PLY";
        case 0x7C: return "JMP_IND_X";
        case 0x7D: return "ADC_ABS_X";
        case 0x7E: return "ROR_ABS_X";
        case 0x7F: return "BBR7";
        
        // Row 8
        case 0x80: return "BRA";
        case 0x81: return "STA_ZP_X";
        case 0x84: return "STY_ZP";
        case 0x85: return "STA_ZP";
        case 0x86: return "STX_ZP";
        case 0x87: return "SMB0_ZP";
        case 0x88: return "DEY";
        case 0x89: return "BIT_IMM";
        case 0x8A: return "TXA";
        case 0x8C: return "STY_ABS";
        case 0x8D: return "STA_ABS";
        case 0x8E: return "STX_ABS";
        case 0x8F: return "BBS0";
        
        // Row 9
        case 0x90: return "BCC";
        case 0x91: return "STA_ZP_Y";
        case 0x92: return "STA_ZP_IND";
        case 0x94: return "STY_ZP_X";
        case 0x95: return "STA_ZP_X";
        case 0x96: return "STX_ZP_Y";
        case 0x97: return "SMB1_ZP";
        case 0x98: return "TYA";
        case 0x99: return "STA_ABS_Y";
        case 0x9A: return "TXS";
        case 0x9C: return "STZ_ABS";
        case 0x9D: return "STA_ABS_X";
        case 0x9E: return "STZ_ABS_X";
        case 0x9F: return "BBS1";
        
        // Row A
        case 0xA0: return "LDY_IMM";
        case 0xA1: return "LDA_ZP_X";
        case 0xA2: return "LDX_IMM";
        case 0xA4: return "LDY_ZP";
        case 0xA5: return "LDA_ZP";
        case 0xA6: return "LDX_ZP";
        case 0xA7: return "SMB2_ZP";
        case 0xA8: return "TAY";
        case 0xA9: return "LDA_IMM";
        case 0xAA: return "TAX";
        case 0xAC: return "LDY_ABS";
        case 0xAD: return "LDA_ABS";
        case 0xAE: return "LDX_ABS";
        case 0xAF: return "BBS2";
        
        // Row B
        case 0xB0: return "BCS";
        case 0xB1: return "LDA_ZP_Y";
        case 0xB2: return "LDA_ZP_IND";
        case 0xB4: return "LDY_ZP_X";
        case 0xB5: return "LDA_ZP_X";
        case 0xB6: return "LDX_ZP_Y";
        case 0xB7: return "SMB3_ZP";
        case 0xB8: return "CLV";
        case 0xB9: return "LDA_ABS_Y";
        case 0xBA: return "TSX";
        case 0xBC: return "LDY_ABS_X";
        case 0xBD: return "LDA_ABS_X";
        case 0xBE: return "LDX_ABS_Y";
        case 0xBF: return "BBS3";
        
        // Row C
        case 0xC0: return "CPY_IMM";
        case 0xC1: return "CMP_ZP_X";
        case 0xC4: return "CPY_ZP";
        case 0xC5: return "CMP_ZP";
        case 0xC6: return "DEC_ZP";
        case 0xC7: return "SMB4_ZP";
        case 0xC8: return "INY";
        case 0xC9: return "CMP_IMM";
        case 0xCA: return "DEX";
        case 0xCB: return "WAI";
        case 0xCC: return "CPY_ABS";
        case 0xCD: return "CMP_ABS";
        case 0xCE: return "DEC_ABS";
        case 0xCF: return "BBS4";
        
        // Row D
        case 0xD0: return "BNE";
        case 0xD1: return "CMP_ZP_Y";
        case 0xD2: return "CMP_ZP_IND";
        case 0xD5: return "CMP_ZP_X";
        case 0xD6: return "DEC_ZP_X";
        case 0xD7: return "SMB5_ZP";
        case 0xD8: return "CLD";
        case 0xD9: return "CMP_ABS_Y";
        case 0xDA: return "PHX";
        case 0xDB: return "STP";
        case 0xDD: return "CMP_ABS_X";
        case 0xDE: return "DEC_ABS_X";
        case 0xDF: return "BBS5";
        
        // Row E
        case 0xE0: return "CPX_IMM";
        case 0xE1: return "SBC_ZP_X";
        case 0xE4: return "CPX_ZP";
        case 0xE5: return "SBC_ZP";
        case 0xE6: return "INC_ZP";
        case 0xE7: return "SMB6_ZP";
        case 0xE8: return "INX";
        case 0xE9: return "SBC_IMM";
        case 0xEA: return "NOP";
        case 0xEC: return "CPX_ABS";
        case 0xED: return "SBC_ABS";
        case 0xEE: return "INC_ABS";
        case 0xEF: return "BBS6";
        
        // Row F
        case 0xF0: return "BEQ";
        case 0xF1: return "SBC_ZP_Y";
        case 0xF2: return "SBC_ZP_IND";
        case 0xF5: return "SBC_ZP_X";
        case 0xF6: return "INC_ZP_X";
        case 0xF7: return "SMB7_ZP";
        case 0xF8: return "SED";
        case 0xF9: return "SBC_ABS_Y";
        case 0xFA: return "PLX";
        case 0xFD: return "SBC_ABS_X";
        case 0xFE: return "INC_ABS_X";
        case 0xFF: return "BBS7";
        
        default: return "UNKNOWN";
    }
}
