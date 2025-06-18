#ifndef RGB_HPP
#define RGB_HPP

struct RGB {
    unsigned char r, g, b;
    RGB() : r(0), g(0), b(0) {}
    RGB(unsigned char r, unsigned char g, unsigned char b) : r(r), g(g), b(b) {}
};

#endif
