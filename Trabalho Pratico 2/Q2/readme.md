# Instruções

### Compilar o Programa:

```
gcc sequencialMatriz.c -o sequencial
```

```
gcc paraleloMatriz.c -o paralelo -pthread
```

### Executar o programa:

O valor do programa sequencial é o tamanho da matriz

```
./sequencial 100
```

```
./sequencial 200
```

```
./sequencial 300
```

```
./sequencial 400
```

```
./sequencial 500
```

```
./sequencial 600
```

```
./sequencial 700
```

```
./sequencial 800
```

```
./sequencial 900
```

```
./sequencial 1000
```

O valor do programa paralelo é o tamanho da matriz e o número de threads

```
./paralelo 100 2
```

```
./paralelo 200 2
```

```
./paralelo 300 2
```

```
./paralelo 400 2
```

```
./paralelo 500 2
```

```
./paralelo 600 2
```

```
./paralelo 700 2
```

```
./paralelo 800 2
```

```
./paralelo 900 2
```

```
./paralelo 1000 2
```
