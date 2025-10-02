# Sistemas Operacionais

### Trabalho Prático 1

Se estiver no windows, recomendo executar o programa através do WSL.

Maiores detalhes do trabalho no pdf.

### Equipe

* Íris Camille Esteves Guimarães
* Jessica de Figueredo Colares
* Roberta Brasil

### Temática:

1. Leitores e Escritores -> Movimentação bancária.
2. Produtores e Consumidores -> Etapas de um e-comerce.

## Leitores e Escritores

Para compilar o programa:

`gcc main.c bancario.c v1.c v2.c v3.c -o programa -lpthread`

Execute de 3 maneiras diferentes:

2 leitores, 2 escritores

`./programa 2 2`

3 leitores, 1 escritores

`./programa 3 1`

1 leitores, 3 escritores

`./programa 1 3`

## Produtores e Consumidores

Para compilar o programa:

`gcc main.c loja.c v1.c v2.c v3.c -o programa -lpthread -lrt`

Execute de 3 maneiras diferentes:

2 separadores, 2 entregadores

`./programa 2 2`

3 separadores, 1 entregador

`./programa 3 1`

1 separador, 3 entregadores

`./programa 1 3`
