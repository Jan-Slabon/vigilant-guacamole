#!/bin/bash

for file in build/gc_test/*
do
    file_name=$(basename ${file})
    ./"$file" > log/"$file_name"_run.log
done