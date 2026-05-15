# Arquivos de teste
> Guilherme Mayer Chamse Ddine RA140656
> Julio César Carvalho de Paula Souza RA138288

## test_0
- O código vai fazer uma soma de dois números inteiros e imprimir o resultado.
- Esse arquivo não deve apresentar nenhum erro ao analisar.

## test_1
- O código vai fazer uma subtração de dois números inteiros e imprimir o resultado.
- Esse arquivo possui um erro no `int numero1 = 30` e no final da declaração não possui o `;`.
- Mesmo possuindo espaços (`std::cout <<              resultado << std::endl;`), a analise não deve    apresentar erros, já que não é considerado um erro
  segundo a gramática do C++.

## test_2
- O código vai fazer uma multiplicação de dois números inteiros e imprimir o resultado.
- Esse arquivo possui dois , no primeiro é `int resultado  numero1 * numero2;`, não possui `=` depois do resultado e segundo erro é `<< <<`, possui dois bitshift left, sendo que um deles não tem valor para a operação.

## test_3
- O código vai fazer uma divisão de dois números inteiros e imprimir o resultado.
- Esse arquivo possui dois erros, no primeiro é `std:endl;`, não possui `::` que é a resolução se escopo e o segundo é `divisao(;`, não possui `()` para chamada da função.