#!/bin/sh

PARENT_PID="${$}"

# save init's stdout and stderr {{{
INIT_FIFO="/tmp/init.log.fifo"
[ -e "${INIT_FIFO}" ] && rm "${INIT_FIFO}"
mkfifo "${INIT_FIFO}"

{ cat <>"${INIT_FIFO}" | \
      tee /tmp/init.log; } &
# }}}

# attach strace to init {{{
{ nohup \
      strace \
          --follow-forks \
          --quiet \
          --trace='/^exec' \
          --signal='!all' \
          --attach="${PARENT_PID}" 2>&1 \
              | tee "/tmp/init.strace"; } &

sleep 0.2
# }}}

exec >"${INIT_FIFO}" 2>&1
exec /sbin/init "${@}"
