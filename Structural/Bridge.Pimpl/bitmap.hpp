#ifndef BITMAP_HPP
#define BITMAP_HPP

#include <algorithm>
#include <iostream>
#include <memory>
#include <vector>

class Bitmap
{
    struct Impl;
    std::unique_ptr<Impl> pimpl_; // Private Implementation (Pimpl) Idiom
public:
    Bitmap(size_t size, char fill_char = '*');
    ~Bitmap();

    Bitmap(const Bitmap&) = delete;
    Bitmap& operator=(const Bitmap&) = delete;

    Bitmap(Bitmap&&) = delete;
    Bitmap& operator=(Bitmap&&) = delete;

    void draw();
};

#endif // BITMAP_HPP
