# Instruções

### Compilar o Programa:

```
gcc sequencial.c -o sequencial
```

```
gcc paralelo.c -o paralelo -pthread
```

### Executar o programa:

O valor do programa sequencial é o tamanho do vetor

```
./sequencial 1000000
```

```
./sequencial 2000000
```

```
./sequencial 3000000
```

```
./sequencial 4000000
```

```
./sequencial 5000000
```

```
./sequencial 6000000
```

```
./sequencial 7000000
```

```
./sequencial 8000000
```

```
./sequencial 9000000
```

```
./sequencial 10000000
```

O primeiro valor do programa paralelo é o tamanho do vetor e o segundo valor é o número de threads

```
./paralelo 1000000 2
```

```
./paralelo 2000000 2
```

```
./paralelo 3000000 2
```

```
./paralelo 4000000 2
```

```
./paralelo 5000000 2
```

```
./paralelo 6000000 2
```

```
./paralelo 7000000 2
```

```
./paralelo 8000000 2
```

```
./paralelo 9000000 2
```

```
./paralelo 10000000 2
```
