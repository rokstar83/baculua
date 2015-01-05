AM_CPPFLAGS = -g -Wall -O0 -I../../..//src -I/usr/local/include/
bin_PROGRAMS = monitor_tests
monitor_tests_SOURCES = main.c monitor_tests.c ../../..//src/monitor.c ../../..//src/baculua_error.c \
                        ../../..//src/md5.c
monitor_tests_LDADD = -L/usr/local/lib/ -lcunit -lcunitsexpoutputter -llua
