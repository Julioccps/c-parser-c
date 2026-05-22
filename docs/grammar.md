# Gramática da Linguagem (EBNF)

Este documento descreve a gramática formal do subconjunto de C++ suportado por este analisador.

```ebnf
<programa> ::= <lista_declaracoes>

<lista_declaracoes> ::= <declaracao> <lista_declaracoes> 
                     | <declaracao>

<declaracao> ::= <definicao_funcao> 
              | <declaracao_variavel>

<declaracao_variavel> ::= "int" <identificador> ";"

<definicao_funcao> ::= <tipo> <identificador> "(" <lista_parametros> ")" "{" <comandos> "}"

<tipo> ::= "int" | "void"

<lista_parametros> ::= <parametro> "," <lista_parametros>
                    | <parametro>
                    | ε

<parametro> ::= <tipo> <identificador>

<comandos> ::= <comando> <comandos> 
            | ε

<comando> ::= <atribuicao> 
           | <saida> 
           | <chamada_funcao> 
           | <declaracao_variavel>
           | <retorno>

<retorno> ::= "return" <expressao> ";"
           | "return" ";"

<atribuicao> ::= <identificador> "=" <expressao> ";"

<expressao> ::= <termo> <operador_add> <expressao> 
             | <termo>

<termo> ::= <fator> <operador_mult> <termo> 
         | <fator>

<fator> ::= <identificador> 
         | <inteiro> 
         | <lambda>
         | "(" <expressao> ")"

<lambda> ::= "[" "]" "(" <lista_parametros> ")" "{" <comandos> "}"

<operador_add> ::= "+" | "-"

<operador_mult> ::= "*" | "/"

<saida> ::= "std" "::" "cout" "<<" <itens_saida> ";"

<itens_saida> ::= <item_saida> "<<" <itens_saida> 
               | <item_saida>

<item_saida> ::= <identificador> 
              | <inteiro> 
              | <string> 
              | "std" "::" "endl"

<chamada_funcao> ::= <identificador> "(" <lista_argumentos> ")" ";"

<lista_argumentos> ::= <expressao> "," <lista_argumentos>
                    | <expressao>
                    | ε
```
