# Arquivos de teste
> Guilherme Mayer Chamse Ddine RA140656
> Julio César Carvalho de Paula Souza RA138288

## test_0
- A função velocidade vai receber duas variáveis, vai fazer uma divisão de `s`, `t` e vai retornar `v`.
- Esse arquivo não deve apresentar nenhum erro ao analisar.

## test_1
- A função conversor vai cacular a conversão de Celsius para Kelvin usando função lambda.
- Esse arquivo possui um erro em que a função não tem `x` definido e é chamada para calcular a conversão.

## test_2
- A função energia vai calcular a energia cinética usando uma função lambda que recebe `m` e `v` como parâmetros e retorna a energia cinética.
- Esse arquivo possui um erro em que a função não tem `v` definido no mesmo escopo que a função lambda, o que pode causar um erro de compilação.

## test_3
- A função corrente recebe `R` como parâmetro, vai calcular a corrente eletrica usando uma função lambda que recebe `V` como parâmetro e retorna a corrente.
- Esse arquivo possui um erro em que `e` é definido duas vezes, o que pode causar um erro de compilação devido à redefinição da variável.

## test_4
- O arquivo vai ter uma variável global `x`, vai ter uma função int `func_nr` que cria uma variável dentro `y` , e na função `main` vai criar dentro dela uma variável `x`, sendo um shadowing.
- Esse arquivo vai ter um erro que é a falta de um `return` na função `func_nr`, já que a função foi declarada como `int` e um warning(aviso) que seria o shadowing sendo em `x`, pois a variável `x` é declarado em dois escopos diferentes e não é utilizada.

## test_5
- O arquivo vai ter uma função `func_v` que tem um tipo de retorno `void`, e dentro dela tem uma variável `nao_usada` que não é usada, e na função `main` tem uma variável `x` que recebe a chamada da função `func_v`.
- Esse arquivo vai ter um erro que é a atribuição de um valor de retorno de uma função `void` para uma variável, o que não é permitido em C++. Além disso, a variável `nao_usada` dentro da função `func_v` não é utilizada, o que deve gerar um warning de variável não utilizada.