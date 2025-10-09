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

### **Sistema Bancário - Leitores e Escritores**

#### **Por que é uma escolha ideal:**

#### **1. Representação fiel do problema clássico:**

* **Leitores**: Consultas de saldo (múltiplos podem acessar simultaneamente)
* **Escritores**: Depósitos e saques (acesso exclusivo necessário)
* **Dados compartilhados**: A conta bancária é o recurso crítico

#### **2. Demonstração clara dos conflitos:**

```
typedef struct {
    double saldo;           // Dado frequentemente atualizado
    int ultima_transacao;   // Identificador de transação
    char historico[100];    // Informação complementar
} ContaBancaria;
```

* "Leitura suja": Um leitor pode ver saldo inconsistente durante uma transação
* Condição de corrida: Dois escritores tentando modificar o saldo simultaneamente
* Starvation: Leitores ou escritores podem ficar indefinidamente esperando

#### **3. Contexto do mundo real intuitivo:**

* Todos entendem operações bancárias
* Fácil visualizar os problemas (saldo errado, transações perdidas)
* Prioridades claras: escritores (transações) vs leitores (consultas)

### **Sistema de E-commerce - Produtores e Consumidores**

#### **Por que é uma escolha perfeita:**

#### **1. Modelagem natural do padrão:**

```
const char* etapas_entrega[NUM_ETAPAS] = {
    "em separacao",        // Produtor cria aqui
    "despachado",          // 
    "em transito",         // Processamento em etapas
    "saiu p/ entrega",     // 
    "entregue"             // Consumidor finaliza aqui
};
```

* **Produtores**: Funcionários separando pedidos (criam itens no buffer)
* **Consumidores**: Funcionários processando entregas (consumem itens do buffer)
* **Buffer**: Área de pedidos prontos para processamento

#### **2. Problemas de sincronização evidentes:**

```
typedef struct {
    Pedido pedidos[BUFFER_SIZE];  // Recurso compartilhado
    int inicio, fim, count;       // Controles críticos
} Buffer;
```

* **Buffer cheio**: Produtores devem esperar
* **Buffer vazio**: Consumidores devem esperar
* **Condições de corrida**: Acesso concorrente aos índices do buffer

#### 3. Fluxo sequencial natural:

* As etapas de entrega formam um pipeline natural
* Cada pedido passa por múltiplos estágios
* Ótimo para demonstrar processamento em cadeia

#### **4. Cenário do mundo real relevante:**

* Logística de e-commerce é um problema atual e compreensível
* Mostra a importância do controle de estoque/pedidos
* Facilita entender prioridades e gargalos

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
