#!/bin/sh

_PARENT_PID="${$}"

{ /usr/bin/nohup \
    /bin/strace \
        --follow-forks \
        --quiet \
        --trace='/^exec' \
        --signal='!all' \
        --attach="${_PARENT_PID}" 2>&1 | cat; } &

sleep 0.2

exec /sbin/init "${@}"
