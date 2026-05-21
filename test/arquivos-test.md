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