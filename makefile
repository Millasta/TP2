# makefile pour TP2 projet 2 C
# MECHERIKI Charles
# MAURICE Valentin

CC=gcc
CFLAGS = -W -Wall -g
EXEC = tp2_a

tp2_a: tp2_a.c
	$(CC) tp2_a.c -o $(EXEC) $(CFLAGS)

valgrind: tp2_a
	valgrind ./tp2_a

valgrind-test: tp2_a
	valgrind ./tp2_a Tests/test.json --memory-leak=full

valgrind-test1: tp2_a
	valgrind ./tp2_a Tests/test1.json --memory-leak=full

valgrind-test1_err: tp2_a
	valgrind ./tp2_a Tests/test1_err.json --memory-leak=full

valgrind-test2: tp2_a
	valgrind ./tp2_a Tests/test2.json --memory-leak=full

valgrind-test2_err: tp2_a
	valgrind ./tp2_a Tests/test2_err.json --memory-leak=full

valgrind-test3: tp2_a
	valgrind ./tp2_a Tests/test3.json --memory-leak=full

valgrind-test3_err: tp2_a
	valgrind ./tp2_a Tests/test3_err.json --memory-leak=full

valgrind-test4: tp2_a
	valgrind ./tp2_a Tests/test4.json --memory-leak=full

valgrind-test4_err: tp2_a
	valgrind ./tp2_a Tests/test4_err.json --memory-leak=full

valgrind-test5: tp2_a
	valgrind ./tp2_a Tests/test5.json --memory-leak=full

valgrind-test5_err: tp2_a
	valgrind ./tp2_a Tests/test5_err.json --memory-leak=full
