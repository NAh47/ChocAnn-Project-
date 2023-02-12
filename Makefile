CC = g++
CFLAGS = -g -Wall -Werror
PRODUCTION = main.cpp validate.cpp chocan.cpp serviceReport.cpp terminal.cpp unit_tests.cpp
UNIT = unit.cpp validate.cpp chocan.cpp serviceReport.cpp terminal.cpp unit_tests.cpp





all: production




production:
	$(CC) $(CFLAGS) -o production $(PRODUCTION)


unit:
	$(CC) $(CFLAGS) -o unit $(UNIT)
