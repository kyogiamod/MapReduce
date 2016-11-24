
#Re escribimos gcc para simplicidad
CC = gcc

CFLAGS = -c

RM = rm -f

###Reglas###

#Crea el archivo ejecutable
mapreduce: mapreduce.o
	$(CC) $^ -o $@

#Crea el archivo de tipo Object
mapreduce.o: mapreduce.c
	$(CC) $(CFLAGS) $^

#Limpia todos los archivos tipo "Object" 
#y los ejecutables llamados mapreduce
clean:
	$(RM) *.o mapreduce

