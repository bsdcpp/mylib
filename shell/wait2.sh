#!/bin/sh

sleep 257 | sleep 31 &
echo jobs -l %%
wait
