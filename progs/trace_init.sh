#!/bin/sh

_PARENT_PID="${$}"

_LOG_FIFO="/trace_init.fifo"
[ -e "${_LOG_FIFO}" ] && rm "${_LOG_FIFO}"
mkfifo "${_LOG_FIFO}"

# Write from fifo to log file
{ /bin/cat <>"${_LOG_FIFO}" | /bin/tee /trace_init.log; } &

{ /usr/bin/nohup \
    /bin/strace \
        --follow-forks \
        --quiet \
        --trace='/^exec' \
        --signal='!all' \
        --attach="${_PARENT_PID}" 2>&1 >"${_LOG_FIFO}"; } &

sleep 0.2

exec 2>&1 >"${_LOG_FIFO}"
exec /sbin/init "${@}"
