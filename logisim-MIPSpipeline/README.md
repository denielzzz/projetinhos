# MIPS Pipeline
### Trabalho Final de Arquitetura de Computadores
Equipe:
  - Daniel Lins;
  - Juliana Zambon;
  - Matheus Piovesan.

### Desenvolvimento do MIPS Pipeline

Desenvolvimento de um Pipeline MIPS no Logisim Evolution com um programa de teste destinado a calcular o décimo sexto termo da sequência de Fibonacci.

Como parte integrante deste projeto, foi elaborado um programa de teste específico com o propósito de calcular o décimo sexto termo da sequência de Fibonacci, uma série matemática notável, na qual cada termo é a soma dos dois anteriores. O décimo sexto termo da sequência de Fibonacci é particularmente interessante, pois exige uma quantidade substancial de cálculos iterativos, oferecendo assim uma avaliação abrangente da capacidade do pipeline em lidar com operações complexas de maneira eficiente.
<br>
### Descrição da Arquitetura

A estrutura do processador é delineada por uma série de componentes, cada qual desempenhando um papel importante na execução eficiente de instruções.

O clock sinaliza sua operação na borda de subida, garantindo uma sincronização precisa das operações. Os blocos de registradores intermediários, como IF/ID, ID/EX, EX/MEM e MEM/WB, desempenham um papel central na orquestração ordenada dos dados, armazenando informações durante um ciclo de clock.

Na fase de decodificação (ID), as instruções são analisadas e formatadas, distribuindo-se em bits específicos para os campos funct, shamt, RD, imm, RT, RS e opcode, variando conforme o formato da instrução (R, I ou J).

A unidade de controle (Control) desempenha um papel crucial na distribuição dos sinais de controle, tomando decisões com base nos valores do OPcode e funct. O OPcode é utilizado exclusivamente quando diferente de zero; caso contrário, o funct é empregado para determinar os sinais de controle adequados, promovendo uma adaptação precisa às instruções em execução.

Contamos com um multiplexador que realiza um flush, transformando a instrução em uma bolha. Em situações como de previsão errada de salto ou de load seguido de uso, o multiplexador distribui o valor zero para todos os sinais, indicando que nenhuma operação deve ser executada. Portanto, penalidade de dois ciclos de clock para previsões erradas e de um para loads seguidos de uso. 

Dentro do banco de registradores, dispomos de 32 registradores, sendo notável que o registrador 0 é dedicado à constante 0.<br>
Os registradores RT e RD passam por um multiplexador, uma vez que, dependendo do formato da instrução em execução, o registrador de destino pode ser modificado.

A memória de instrução possui 16 bits de endereço, pois o logisim não suporta 32 bits. <br>
O mesmo para a outra memória, os dados possuem 32 bits, mas somente 16 bits de endereço.

O processador integra uma Unidade de Previsão de Desvio, encarregada de identificar se uma instrução é um Jump ou um Branch. No caso de um Jump, a unidade realiza o cálculo do endereço correspondente, sendo que, devido às particularidades da arquitetura, instruções Jump têm um endereço de apenas 16 bits. O funcionamento desta unidade é orientado por uma máquina de estados de 2 bits, otimizando o processo de previsão de desvio e contribuindo para a eficiência global do processador. <br>

A Unidade de Hazard (Hazard Unit) entra em ação quando ocorre uma situação de load seguido por uma operação de uso. Verifica se RS ou RT são iguais ao RD.

A Unidade de Encaminhamento (Forwarding) analisa se os registradores RS ou RT são idênticos ao registrador no qual ocorrerá a escrita.
