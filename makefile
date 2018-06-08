CC = gcc
CFLAGS = -Wall -g -std=c99
LDFLAGS = -g
EXES = CLI calc lcount
CLIOBJECTS = CLI.o tokenizer.o piping.o history.o ttymode.o

default:    $(EXES)

CLI:	$(CLIOBJECTS)
	$(CC) $(LDFLAGS) -o CLI $(CLIOBJECTS)

calc: 	calc.o
	$(CC) $(LDFLAGS) -o calc calc.o

lcount: 	lcount.o
	$(CC) $(LDFLAGS) -o lcount lcount.o

%.o:	%.c
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -f *.o $(EXES)
