#include <iostream>
#include <array>

#define ALIVE "█"
#define DEAD "░"
#define WIDTH 64
#define HEIGHT 32

#include "main.hpp"

char Byte::getBit(char i) const {
    if (i >= 8 || i < 0) return -1;
    return (this->bits & (1 << i)) >> i;
}

void Byte::setBit(char i, char b) {
    if (i >= 8 || i < 0) return;
    if (b) this->bits |= (1 << i);
    else this->bits &= (-2 << i);
}

std::ostream& operator<<(std::ostream &out, Byte const& b) {
    for (char i = 0; i < 8; i++) {
        if (b.getBit(i)) out << ALIVE;
        else out << DEAD;
    }
    return out;
}

Block::Block() {
    this->bytes = new std::array<Byte, WIDTH * HEIGHT / 8>();
}

Block::~Block() {
    delete this->bytes;
}

Block::Block(const Block& other) {
    this->bytes = new std::array<Byte, WIDTH * HEIGHT / 8>(*other.bytes);
}

char Block::getBit(int x, int y) const {
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) return 0;
    return ((*this->bytes)[(x + y * WIDTH) / 8].getBit(x % 8));
}

void Block::setBit(int x, int y, char b) {
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) return;
    (*this->bytes)[(x + y * WIDTH) / 8].setBit(x % 8, b);
}

char Block::countNeighbors(int x, int y) {
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) return -1;
    char count = 0;
    if (this->getBit(x-1,y-1)) count++;
    if (this->getBit(x-1,y+1)) count++;
    if (this->getBit(x+1,y-1)) count++;
    if (this->getBit(x+1,y+1)) count++;
    if (this->getBit(x,y-1)) count++;
    if (this->getBit(x,y+1)) count++;
    if (this->getBit(x-1,y)) count++;
    if (this->getBit(x+1,y)) count++;
    return count;
}

char Block::lifeCycle(char live, char count) {
    if (live && (count == 2 || count == 3)) return 1;
    if (!live && count == 3) return 1;
    return 0;
}

char Block::step() {
    char changed = 0;
    Block tmp;
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            char count = this->countNeighbors(x,y);
            char live = this->getBit(x,y);
            char live_ = Block::lifeCycle(this->getBit(x,y), count);
            if (live_) tmp.setBit(x,y,1);
            changed |= live != live_;
        }
    }
    delete this->bytes;
    this->bytes = tmp.bytes;
    tmp.bytes = nullptr;
    return changed;
}

std::ostream& operator<<(std::ostream &out, Block const& f) {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++)
            if (f.getBit(x, y)) out << ALIVE;
            else out << DEAD;
        out << std::endl;
    }
    return out;
}

int main(int argc, char *argv[]) {
    Block f;
    f.setBit(2,0,1);
    f.setBit(2,1,1);
    f.setBit(2,2,1);
    f.setBit(1,2,1);
    f.setBit(0,1,1);
    do {
        std::cout << f;
        std::cin.get();
    } while (f.step());
    return 0;
}
