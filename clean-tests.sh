#!/bin/sh

for group in generic standalone
do
    for extension in diff exp log out mem php sh
    do
        rm -f tests/$group/*.$extension
    done
done
