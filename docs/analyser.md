# Analyser
### O que foi feito e porquê
- [Definições da linguagem](../include/lang_defs.h);
    - Agora o motivo, as linguagens possuem keywords e simbolos definidos, o
    header foi criado para poder guardar essas definições como macros já que
    serão muito utilizados e não serão alterados.
- Lexer, [definição](../include/lexer.h), [implementação](../src/lexer.c);
    - Para o motivo, simples, para poder fazer a analise léxica, é útil ter uma
    maneira de entender e controlar cada parte do código, um contexto é uma boa
    forma de atingir isso e o sistema de Tokens auxilia no parsing do arquivo.
    - O que mais está incluso no lexer? A tokenização do source code para 
    facilitar nos outros tipos de analise(sintática e semântica).
- Parser, [definição](../include/parser.h), [implementação](../src/parser.c);
    - O motivo aqui é organizar os tokens gerados pelo lexer em uma estrutura
    hierárquica chamada AST (Abstract Syntax Tree). Isso permite que o programa
    entenda a "frase" completa do código (como uma função ou atribuição) em vez
    de apenas palavras isoladas.
    - A implementação usa a técnica de descida recursiva, onde cada regra da
    gramática vira uma função, facilitando a detecção de erros sintáticos como
    a falta de ponto e vírgula ou chaves.
- Analisador Semântico e Tabela de Símbolos, [definição](../include/semantic.h), [implementação](../src/semantic.c);
    - O objetivo é garantir que o código faça sentido logicamente. Não basta a
    frase estar gramaticalmente correta, as variáveis usadas precisam ter sido
    declaradas antes e os nomes das funções precisam existir.
    - Para isso, usamos uma Tabela de Símbolos que guarda o nome, tipo e linha
    de cada identificador, permitindo validar o escopo e evitar redeclarações
    inválidas.
- Extensão para Parâmetros e Lambdas;
    - O motivo: Evoluir o suporte da linguagem para permitir abstrações mais complexas, como funções que recebem dados e funções anônimas (lambdas).
    - O que foi feito: Foram adicionados novos símbolos ao Lexer (`,`, `[`, `]`) e suporte a comentários de linha (`//`). O Parser foi expandido para suportar parâmetros e uma nova camada de expressões *postfix*, permitindo que lambdas sejam tratadas como expressões chamáveis. O Analisador Semântico agora gerencia escopos de parâmetros e valida o uso de variáveis dentro de lambdas.
