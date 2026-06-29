#!/usr/bin/env sh
unset DEBUGINFOD_URLS
export DEBUGINFOD_URLS=
exec /usr/bin/gdb -iex "set debuginfod enabled off" "$@"
