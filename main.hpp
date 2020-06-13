#pragma once

#include <iostream>
#include <array>

class Byte {
    unsigned char bits = 0;
public:
    char getBit(char) const;
    void setBit(char,char);
};
std::ostream& operator<<(std::ostream&,Byte const&);

class Block {
    std::array<Byte, WIDTH * HEIGHT / 8> *bytes;
    char countNeighbors(int,int);
    static char lifeCycle(char,char);
public:
    Block();
    Block(const Block&);
    ~Block();
    void setBit(int,int,char);
    char getBit(int,int) const;
    char step();
};
std::ostream& operator<<(std::ostream &,Block const&);

int main(int,char*[]);
