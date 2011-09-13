#!/bin/sh

sleep 1000&
pid=$!
sleep 5
kill -kill $pid
wait $pid
echo $pid was terminated by a SIG$(kill -l $?) signal.
