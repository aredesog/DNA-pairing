# ESTE ARQUIVO NÃO DEVE SER MODIFICADO
# O TRABALHO SERÁ AVALIADO UTILIZANDO UNICAMENTE ESTE MAKEFILE

# Compilador a ser utilizado
CC = gcc

# Flags de compilação (avisos e debug)
CFLAGS = -Wall -Wextra -g

# Nome do executável final
TARGET = base

# Arquivos fonte
SRCS = base.c algoritmos.c

# Objetos gerados a partir dos arquivos fonte
OBJS = $(SRCS:.c=.o)

# Em Windows usamos PowerShell para remover os artefatos
ifeq ($(OS),Windows_NT)
CLEAN_CMD = powershell -NoProfile -Command "foreach ($$path in @('base.o','algoritmos.o','base','base.exe')) { Remove-Item -Force -ErrorAction SilentlyContinue -LiteralPath $$path }; exit 0"
# Em Linux/Unix mantemos rm -f.
else
CLEAN_CMD = rm -f $(OBJS) $(TARGET)
endif

# Regra padrão executada ao rodar apenas 'make'j
all: $(TARGET)

# Regra para gerar o executável
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Regra para gerar os arquivos objeto (.o)
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Regra para limpar os arquivos gerados pela compilação ('make clean')
clean:
	$(CLEAN_CMD)

# Regra auxiliar para testar a execução lendo o arquivo 'bla' ('make run')
run: $(TARGET)
	./$(TARGET) ../bla

# Declaração das regras que não são arquivos
.PHONY: all clean run
