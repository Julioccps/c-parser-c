# Definição do Bytecode (Representação Intermediária)

Este documento descreve a gramática e a especificação do bytecode da máquina virtual baseada em pilha utilizado como alvo de compilação do analisador.

## Gramática do Bytecode (Lark / EBNF)

O bytecode gerado é lido e executado pelo interpretador em Python. A sua gramática é definida da seguinte forma:

```lark
program:      definition* function* call* main

function:     "fun" TYPE NAME params statement* return_stmt
params:       (param)*
param:        "param" TYPE NAME
return_stmt:  "ret" TYPE (NAME | SCOPE ":" NAME)?

statement:    definition | attribution | call | function | stack | op | print
definition:   "set" SCOPE ":" NAME TYPE (NUMBER | STRING | SCOPE ":" NAME)
attribution:  "set" SCOPE ":" NAME (NUMBER | STRING | SCOPE ":" NAME)
call:         "sub" NAME "()"

print:        "print" TYPE (SCOPE ":" NAME | NUMBER | STRING)

stack:        PUSH (SCOPE ":" NAME | NUMBER) 
            | POP SCOPE ":" NAME

op:           ADD | SUBT | MUL | DIV | MOD

main:         "fun" "void" "main" statement* "ret" "void"

TYPE:         "int" | "void" | "string" | "auto"
STRING:       /"[^"]*"/
SCOPE:        /\w+/
NAME:         /\w+/
NUMBER:       /\d+/
ADD:          "add"
SUBT:         "subt"
MUL:          "mul"
DIV:          "div"
MOD:          "mod"
PUSH:         "push"
POP:          "pop"
```

## Modelo de Execução e Escopo

1. **Modelo de Pilha**:
   - A máquina virtual utiliza uma pilha para avaliação de expressões aritméticas.
   - Operações matemáticas (como `add`, `subt`, `mul`, `div` e `mod`) removem os dois valores do topo da pilha, realizam a operação e inserem o resultado de volta na pilha.

2. **Resolução de Escopo**:
   - As variáveis são identificadas no formato `ESCOPO:NOME`.
   - Para variáveis globais, o escopo padrão é `global` (ex: `global:x`).
   - Para variáveis locais ou parâmetros de uma função `foo`, o escopo é o nome da função (ex: `foo:v`).

3. **Chamadas de Função**:
   - Argumentos são passados através da pilha (inseridos antes da chamada).
   - A instrução `sub NOME()` invoca a função. Ela remove os argumentos na ordem inversa de inserção e os atribui às variáveis locais de parâmetros da função.
   - O valor retornado pela função via `ret` é colocado na pilha após a finalização da execução.

