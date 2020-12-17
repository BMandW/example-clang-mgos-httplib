#!/bin/bash

mos build --local &&\
mos flash &&\
sleep 3 &&\
mos put conf9.json &&\
mos console
