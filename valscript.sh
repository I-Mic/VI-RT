#!/bin/sh

valgrind --leak-check=full --track-origins=no --verbose --log-file=valgrind_log.out bin/renderer $@
