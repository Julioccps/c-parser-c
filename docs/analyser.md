# Analyzer
### O que foi feito e porquê
- [Definições da linguagem](../include/lang_defs.h);
    - Agora o motivo, as linguagens possuem keywords e simbolos definidos, o
    header foi criado para poder guardar essas definições como macros já que
    serão muito utilizados e não serão alterados.
- Lexer, [definição](../include/lexer.h), [implementação](../src/lexer.c);
    - Para o motivo, simples, para poder fazer a analize léxica, é útil ter uma
    maneira de entender e controlar cada parte do código, um contexto é uma boa
    forma de atingir isso e o sistema de Tokens auxilia no parsing do arquivo.
