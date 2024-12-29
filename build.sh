#! /usr/bin/zsh

export CC=clang
export CXX=clang++

meson setup buildclang
cd buildclang
meson compile
