#!/bin/sh

i=0
while [ $i -le 5 ]; do
    sleep 5&
    sleep 10&
    sleep 20&
    let "i += 1"
    p1=$!
    wait $p1
    echo Job 1 exited with status $?
done

wait
echo Job exited with status $?
