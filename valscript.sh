#!/bin/sh

# Valgrind flags
VALGFLAGS="--leak-check=full --show-leak-kinds=all --track-origins=no --verbose"

# Valgrind log file
VALG_OUT=valgrind_log.out

# executable path
EXEC=bin/renderer

if [ ! -f $EXEC ]
then
    make || (echo "error: failed to make $EXEC"; exit 2);
fi

valgrind $VALGFLAGS --log-file=$VALG_OUT $EXEC $@ || \
    echo "error: valgrind error; please check the log file ($VALG_OUT)"
