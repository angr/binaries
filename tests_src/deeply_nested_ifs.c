/* A function whose control flow structures into a tree deeper than CPython's recursion limit allows a recursive
 * walker to descend.
 *
 * Two hundred guarded statements, each guard reading a value the compiler cannot fold away, so the structurer
 * produces a ConditionNode wrapping a SequenceNode per level and the structured tree is about 400 nodes deep along
 * its longest path.
 *
 * gcc -O0 -no-pie -o tests/x86_64/deeply_nested_ifs tests_src/deeply_nested_ifs.c
 *
 * Built with GCC 15.3.0. On a Nix system the compiler wrapper writes its own store path
 * into the interpreter and adds a RUNPATH, which no other object here carries, so that
 * build additionally needs NIX_DONT_SET_RPATH_x86_64_unknown_linux_gnu=1, an unset
 * NIX_LDFLAGS and -Wl,--dynamic-linker=/lib64/ld-linux-x86-64.so.2.
 */

int main(int argc, char **argv)
{
    long x = argc;
    long v = 0;

    (void)argv;

    if (x > 1) { v += 1;
    if (x > 2) { v += 2;
    if (x > 3) { v += 3;
    if (x > 4) { v += 4;
    if (x > 5) { v += 5;
    if (x > 6) { v += 6;
    if (x > 7) { v += 7;
    if (x > 8) { v += 8;
    if (x > 9) { v += 9;
    if (x > 10) { v += 10;
    if (x > 11) { v += 11;
    if (x > 12) { v += 12;
    if (x > 13) { v += 13;
    if (x > 14) { v += 14;
    if (x > 15) { v += 15;
    if (x > 16) { v += 16;
    if (x > 17) { v += 17;
    if (x > 18) { v += 18;
    if (x > 19) { v += 19;
    if (x > 20) { v += 20;
    if (x > 21) { v += 21;
    if (x > 22) { v += 22;
    if (x > 23) { v += 23;
    if (x > 24) { v += 24;
    if (x > 25) { v += 25;
    if (x > 26) { v += 26;
    if (x > 27) { v += 27;
    if (x > 28) { v += 28;
    if (x > 29) { v += 29;
    if (x > 30) { v += 30;
    if (x > 31) { v += 31;
    if (x > 32) { v += 32;
    if (x > 33) { v += 33;
    if (x > 34) { v += 34;
    if (x > 35) { v += 35;
    if (x > 36) { v += 36;
    if (x > 37) { v += 37;
    if (x > 38) { v += 38;
    if (x > 39) { v += 39;
    if (x > 40) { v += 40;
    if (x > 41) { v += 41;
    if (x > 42) { v += 42;
    if (x > 43) { v += 43;
    if (x > 44) { v += 44;
    if (x > 45) { v += 45;
    if (x > 46) { v += 46;
    if (x > 47) { v += 47;
    if (x > 48) { v += 48;
    if (x > 49) { v += 49;
    if (x > 50) { v += 50;
    if (x > 51) { v += 51;
    if (x > 52) { v += 52;
    if (x > 53) { v += 53;
    if (x > 54) { v += 54;
    if (x > 55) { v += 55;
    if (x > 56) { v += 56;
    if (x > 57) { v += 57;
    if (x > 58) { v += 58;
    if (x > 59) { v += 59;
    if (x > 60) { v += 60;
    if (x > 61) { v += 61;
    if (x > 62) { v += 62;
    if (x > 63) { v += 63;
    if (x > 64) { v += 64;
    if (x > 65) { v += 65;
    if (x > 66) { v += 66;
    if (x > 67) { v += 67;
    if (x > 68) { v += 68;
    if (x > 69) { v += 69;
    if (x > 70) { v += 70;
    if (x > 71) { v += 71;
    if (x > 72) { v += 72;
    if (x > 73) { v += 73;
    if (x > 74) { v += 74;
    if (x > 75) { v += 75;
    if (x > 76) { v += 76;
    if (x > 77) { v += 77;
    if (x > 78) { v += 78;
    if (x > 79) { v += 79;
    if (x > 80) { v += 80;
    if (x > 81) { v += 81;
    if (x > 82) { v += 82;
    if (x > 83) { v += 83;
    if (x > 84) { v += 84;
    if (x > 85) { v += 85;
    if (x > 86) { v += 86;
    if (x > 87) { v += 87;
    if (x > 88) { v += 88;
    if (x > 89) { v += 89;
    if (x > 90) { v += 90;
    if (x > 91) { v += 91;
    if (x > 92) { v += 92;
    if (x > 93) { v += 93;
    if (x > 94) { v += 94;
    if (x > 95) { v += 95;
    if (x > 96) { v += 96;
    if (x > 97) { v += 97;
    if (x > 98) { v += 98;
    if (x > 99) { v += 99;
    if (x > 100) { v += 100;
    if (x > 101) { v += 101;
    if (x > 102) { v += 102;
    if (x > 103) { v += 103;
    if (x > 104) { v += 104;
    if (x > 105) { v += 105;
    if (x > 106) { v += 106;
    if (x > 107) { v += 107;
    if (x > 108) { v += 108;
    if (x > 109) { v += 109;
    if (x > 110) { v += 110;
    if (x > 111) { v += 111;
    if (x > 112) { v += 112;
    if (x > 113) { v += 113;
    if (x > 114) { v += 114;
    if (x > 115) { v += 115;
    if (x > 116) { v += 116;
    if (x > 117) { v += 117;
    if (x > 118) { v += 118;
    if (x > 119) { v += 119;
    if (x > 120) { v += 120;
    if (x > 121) { v += 121;
    if (x > 122) { v += 122;
    if (x > 123) { v += 123;
    if (x > 124) { v += 124;
    if (x > 125) { v += 125;
    if (x > 126) { v += 126;
    if (x > 127) { v += 127;
    if (x > 128) { v += 128;
    if (x > 129) { v += 129;
    if (x > 130) { v += 130;
    if (x > 131) { v += 131;
    if (x > 132) { v += 132;
    if (x > 133) { v += 133;
    if (x > 134) { v += 134;
    if (x > 135) { v += 135;
    if (x > 136) { v += 136;
    if (x > 137) { v += 137;
    if (x > 138) { v += 138;
    if (x > 139) { v += 139;
    if (x > 140) { v += 140;
    if (x > 141) { v += 141;
    if (x > 142) { v += 142;
    if (x > 143) { v += 143;
    if (x > 144) { v += 144;
    if (x > 145) { v += 145;
    if (x > 146) { v += 146;
    if (x > 147) { v += 147;
    if (x > 148) { v += 148;
    if (x > 149) { v += 149;
    if (x > 150) { v += 150;
    if (x > 151) { v += 151;
    if (x > 152) { v += 152;
    if (x > 153) { v += 153;
    if (x > 154) { v += 154;
    if (x > 155) { v += 155;
    if (x > 156) { v += 156;
    if (x > 157) { v += 157;
    if (x > 158) { v += 158;
    if (x > 159) { v += 159;
    if (x > 160) { v += 160;
    if (x > 161) { v += 161;
    if (x > 162) { v += 162;
    if (x > 163) { v += 163;
    if (x > 164) { v += 164;
    if (x > 165) { v += 165;
    if (x > 166) { v += 166;
    if (x > 167) { v += 167;
    if (x > 168) { v += 168;
    if (x > 169) { v += 169;
    if (x > 170) { v += 170;
    if (x > 171) { v += 171;
    if (x > 172) { v += 172;
    if (x > 173) { v += 173;
    if (x > 174) { v += 174;
    if (x > 175) { v += 175;
    if (x > 176) { v += 176;
    if (x > 177) { v += 177;
    if (x > 178) { v += 178;
    if (x > 179) { v += 179;
    if (x > 180) { v += 180;
    if (x > 181) { v += 181;
    if (x > 182) { v += 182;
    if (x > 183) { v += 183;
    if (x > 184) { v += 184;
    if (x > 185) { v += 185;
    if (x > 186) { v += 186;
    if (x > 187) { v += 187;
    if (x > 188) { v += 188;
    if (x > 189) { v += 189;
    if (x > 190) { v += 190;
    if (x > 191) { v += 191;
    if (x > 192) { v += 192;
    if (x > 193) { v += 193;
    if (x > 194) { v += 194;
    if (x > 195) { v += 195;
    if (x > 196) { v += 196;
    if (x > 197) { v += 197;
    if (x > 198) { v += 198;
    if (x > 199) { v += 199;
    if (x > 200) { v += 200;
    }}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}

    return (int)v;
}
