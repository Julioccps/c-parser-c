# Gramática da Linguagem (EBNF)

Este documento descreve a gramática do subconjunto de C++ suportado por este analisador.

```ebnf
<programa> ::= <lista_declaracoes>

<lista_declaracoes> ::= <declaracao> <lista_declaracoes> 
                     | <declaracao>

<declaracao> ::= <definicao_funcao> 
              | <declaracao_variavel>

<declaracao_variavel> ::= <tipo_variavel> <identificador> <inicializacao> ";"

<tipo_variavel> ::= "int" | "auto"

<inicializacao> ::= "=" <expressao>
                 | ε

<definicao_funcao> ::= <tipo_retorno> <identificador> "(" <lista_parametros> ")" "{" <comandos> "}"

<tipo_retorno> ::= "int" | "void"

<lista_parametros> ::= <parametro> "," <lista_parametros>
                    | <parametro>
                    | ε

<parametro> ::= <tipo_variavel> <identificador>

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

<fator> ::= <postfix>

<postfix> ::= <primario> "(" <lista_argumentos> ")"
           | <primario>

<primario> ::= <identificador> 
            | <inteiro> 
            | <lambda>
            | "(" <expressao> ")"

<lambda> ::= "[" <lista_capturas> "]" "(" <lista_parametros> ")" "{" <comandos> "}"

<lista_capturas> ::= <identificador> "," <lista_capturas>
                  | <identificador>
                  | ε

<operador_add> ::= "+" | "-"

<operador_mult> ::= "*" | "/"

<saida> ::= "std" "::" "cout" "<<" <itens_saida> ";"

<itens_saida> ::= <item_saida> "<<" <itens_saida> 
               | <item_saida>

<item_saida> ::= <identificador> 
              | <inteiro> 
              | <string> 
              | "std" "::" "endl"

<chamada_funcao> ::= <expressao> ";"

<lista_argumentos> ::= <expressao> "," <lista_argumentos>
                    | <expressao>
                    | ε
```
