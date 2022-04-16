#!/bin/sh

PATH=$PATH:$(pwd)

function luacpp_build()
{
  LuaClassBuilder.py $1 --otype header > $2.hpp
  LuaClassBuilder.py $1 --otype impl > $2.cpp
}
