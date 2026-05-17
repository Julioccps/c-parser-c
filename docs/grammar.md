# Gramática da Linguagem (EBNF)

Este documento descreve a gramática formal do subconjunto de C++ suportado por este analisador.

```ebnf
<programa> ::= <lista_declaracoes>

<lista_declaracoes> ::= <declaracao> <lista_declaracoes> 
                     | <declaracao>

<declaracao> ::= <definicao_funcao> 
              | <declaracao_variavel>

<declaracao_variavel> ::= "int" <identificador> ";"

<definicao_funcao> ::= "void" <identificador> "(" ")" "{" <comandos> "}"

<comandos> ::= <comando> <comandos> 
            | ε

<comando> ::= <atribuicao> 
           | <saida> 
           | <chamada_funcao> 
           | <declaracao_variavel>

<atribuicao> ::= <identificador> "=" <expressao> ";"

<expressao> ::= <termo> <operador_add> <expressao> 
             | <termo>

<termo> ::= <fator> <operador_mult> <termo> 
         | <fator>

<fator> ::= <identificador> 
         | <inteiro> 
         | "(" <expressao> ")"

<operador_add> ::= "+" | "-"

<operador_mult> ::= "*" | "/"

<saida> ::= "std" "::" "cout" "<<" <itens_saida> ";"

<itens_saida> ::= <item_saida> "<<" <itens_saida> 
               | <item_saida>

<item_saida> ::= <identificador> 
              | <inteiro> 
              | <string> 
              | "std" "::" "endl"

<chamada_funcao> ::= <identificador> "(" ")" ";"
```
