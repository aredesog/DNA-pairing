# Makefile para o Trabalho Prático 2 - Pareamento de Sequências de DNA
# Disciplina: DCE797 - AEDS 3

# Compilador a ser utilizado
CC = gcc

# Flags de compilação (avisos e debug)
CFLAGS = -Wall -Wextra -g -std=c99

# Nome do executável final
TARGET = dna_aligner

# Arquivos fonte
SRCS = base.c algoritimos.c

# Objetos gerados a partir dos arquivos fonte
OBJS = $(SRCS:.c=.o)

# Regra padrão executada ao rodar apenas 'make'
all: $(TARGET)

# Regra para gerar o executável
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Regra para gerar os arquivos objeto (.o)
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Regra para limpar os arquivos gerados pela compilação ('make clean')
clean:
	rm -f $(OBJS) $(TARGET)

# Regra auxiliar para testar a execução ('make run')
run: $(TARGET)
	./$(TARGET)

# Regra para compilar e rodar
test: clean all run

# Declaração das regras que não são arquivos
.PHONY: all clean run test
