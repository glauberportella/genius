# Genius

Seção de programação recreativa no feriado das crianças.

Iniciamos baseando na especificação abaixo porém fomos avançando sem olharmuito para ela :)

---

# Especificação - Genius (jogo de memória)

**Tipo:** especificação funcional e lógica  
**Objetivo:** orientar a implementação do jogo Genius em linguagem C, inicialmente no terminal, sem prescrever a solução em código.

## 1. Objetivo do projeto

Implementar um jogo de memória inspirado no Genius/Simon. O computador apresenta uma sequência de sinais; o jogador deve reproduzi-la na mesma ordem. A cada rodada bem-sucedida, um novo sinal é acrescentado à sequência.

A primeira versão deve priorizar a lógica do jogo e manter as regras independentes da forma de entrada e saída, permitindo uma futura interface gráfica.

## 2. Escopo

### Incluído na primeira versão

- Inicialização de uma partida.
- Geração pseudoaleatória de sinais.
- Armazenamento e crescimento da sequência.
- Apresentação da sequência completa a cada rodada.
- Recebimento e validação das respostas do jogador.
- Progressão de rodada e pontuação.
- Encerramento da partida em caso de erro.
- Opção de iniciar uma nova partida.

### Fora do escopo inicial

- Interface gráfica.
- Efeitos sonoros.
- Persistência do recorde em arquivo.
- Multiplayer.
- Níveis de dificuldade configuráveis.
- Animações ou temporização sofisticada.

Esses itens podem ser adicionados em versões futuras.

## 3. Regras do jogo

1. O jogo começa com uma sequência vazia, nível inicial definido e pontuação zerada.
2. No início de cada rodada, o computador gera um sinal e o acrescenta ao final da sequência.
3. O jogo apresenta todos os sinais da sequência, do primeiro ao último.
4. Após a apresentação, o jogo recebe a sequência de respostas do jogador.
5. Cada resposta é comparada com o sinal esperado na posição correspondente.
6. Se uma resposta estiver incorreta, a partida termina.
7. Se todas as respostas da rodada estiverem corretas, a rodada é concluída com sucesso e o jogo avança para a próxima.
8. A sequência anterior é preservada; somente um novo sinal é acrescentado em cada rodada.
9. Ao terminar, o jogo informa o resultado e oferece a opção de jogar novamente ou sair.

## 4. Representação dos sinais

A lógica pode representar os quatro sinais por inteiros:

| Valor | Sinal | Cor sugerida |
|---:|---|---|
| 0 | Sinal A | Verde |
| 1 | Sinal B | Vermelho |
| 2 | Sinal C | Amarelo |
| 3 | Sinal D | Azul |

A associação entre valores e cores é conceitual. A lógica interna não deve depender de códigos de cor ou de detalhes de apresentação.

## 5. Modelo de dados conceitual

A implementação deverá manter informações equivalentes às seguintes:

| Dado | Finalidade |
|---|---|
| Sequência | Armazenar os sinais gerados na ordem em que foram adicionados |
| Comprimento da sequência | Informar quantos sinais válidos existem na sequência |
| Capacidade da sequência | Definir quantos sinais podem ser armazenados com segurança |
| Índice de apresentação | Identificar o sinal que está sendo apresentado |
| Índice de resposta | Identificar a posição que o jogador precisa reproduzir |
| Rodada ou nível | Representar a progressão da partida |
| Pontuação | Registrar o resultado conforme o critério definido |
| Estado do jogo | Identificar a fase atual da partida |
| Recorde (opcional) | Guardar a melhor pontuação, se essa funcionalidade for implementada |

A estrutura concreta dos dados fica a critério da implementação em C.

## 6. Estados do jogo

A partida deve ter estados lógicos distintos. Os nomes abaixo são sugestivos:

- **PRONTO:** aguardando o início da partida.
- **ADICIONANDO_SINAL:** gerando e armazenando o novo sinal da rodada.
- **APRESENTANDO_SEQUENCIA:** mostrando a sequência ao jogador.
- **AGUARDANDO_RESPOSTA:** recebendo as respostas do jogador.
- **RODADA_CONCLUIDA:** todas as respostas da rodada foram corretas.
- **FIM_DE_JOGO:** uma resposta foi incorreta ou a partida terminou por outra condição.
- **SAINDO:** encerrando a execução.

### Transições esperadas

1. `PRONTO` → `ADICIONANDO_SINAL`, quando a partida começa.
2. `ADICIONANDO_SINAL` → `APRESENTANDO_SEQUENCIA`, após acrescentar um sinal.
3. `APRESENTANDO_SEQUENCIA` → `AGUARDANDO_RESPOSTA`, quando todos os sinais foram apresentados.
4. `AGUARDANDO_RESPOSTA` → `FIM_DE_JOGO`, quando uma resposta válida não corresponde ao sinal esperado.
5. `AGUARDANDO_RESPOSTA` → `RODADA_CONCLUIDA`, quando toda a sequência foi reproduzida corretamente.
6. `RODADA_CONCLUIDA` → `ADICIONANDO_SINAL`, para iniciar a próxima rodada.
7. `FIM_DE_JOGO` → `PRONTO`, se o jogador escolher reiniciar.
8. `FIM_DE_JOGO` → `SAINDO`, se o jogador escolher sair.

Uma entrada inválida, como um caractere não reconhecido, não precisa encerrar a partida. A implementação pode informar o erro e solicitar novamente a entrada da mesma posição.

## 7. Fluxo principal

1. Inicializar o gerador pseudoaleatório.
2. Preparar os dados da partida.
3. Aguardar o início.
4. Acrescentar um novo sinal à sequência.
5. Apresentar a sequência completa na ordem correta.
6. Reiniciar o índice de resposta para a primeira posição.
7. Enquanto houver respostas esperadas:
   - Ler uma entrada.
   - Validar o formato e o intervalo da entrada.
   - Se a entrada for inválida, solicitar novamente sem avançar o índice.
   - Comparar a entrada com o sinal esperado.
   - Se estiver incorreta, encerrar a rodada e a partida.
   - Se estiver correta, avançar o índice de resposta.
8. Se todas as respostas estiverem corretas:
   - Atualizar o resultado da rodada.
   - Avançar para a próxima rodada.
   - Voltar à etapa de adicionar um sinal.
9. Ao final da partida, mostrar o resultado e oferecer reinício ou saída.

## 8. Algoritmo de validação

Para cada resposta do jogador:

1. Obter o sinal informado.
2. Verificar se ele pertence ao conjunto de sinais válidos.
3. Comparar com o elemento da sequência na posição atual.
4. Se forem diferentes, marcar a partida como encerrada.
5. Se forem iguais, avançar para a próxima posição.
6. Considerar a rodada concluída quando o número de respostas corretas alcançar o comprimento da sequência.

O índice de resposta só deve avançar depois de uma resposta válida e correta.

## 9. Pontuação e progressão

Definir explicitamente o critério de pontuação antes de implementar. Sugestão simples:

- A rodada 1 contém um sinal.
- Cada rodada concluída corretamente aumenta o nível em um.
- A pontuação representa o número de rodadas concluídas com sucesso.

Com esse critério, errar durante uma rodada não concede o ponto daquela rodada. Outra convenção possível é pontuar cada sinal reproduzido corretamente; se escolhida, deve ser aplicada de forma consistente.

## 10. Aleatoriedade

- Cada novo sinal deve pertencer ao conjunto de quatro sinais válidos.
- A geração deve ocorrer uma vez por rodada, ao acrescentar o sinal.
- Não gerar uma sequência inteiramente nova a cada rodada.
- Inicializar o gerador pseudoaleatório uma vez no início da execução, e não a cada sinal.
- Considerar testes reproduzíveis: durante o desenvolvimento, pode ser útil permitir uma semente fixa para facilitar a depuração.

## 11. Capacidade e segurança de memória

Como a sequência cresce a cada rodada, a implementação precisa definir como tratar seu limite:

- **Capacidade fixa:** definir um máximo e encerrar ou informar que o limite foi atingido antes de escrever fora do espaço reservado.
- **Capacidade dinâmica:** ampliar o armazenamento quando necessário e verificar se a alocação foi bem-sucedida.

Não acessar posições fora dos limites, não usar o comprimento da sequência como índice válido e não continuar uma rodada se a inclusão do novo sinal falhar.

## 12. Entrada e saída

Na primeira versão de terminal:

- Apresentar instruções claras sobre os sinais disponíveis.
- Mostrar os sinais na ordem gerada.
- Separar visualmente a apresentação da sequência e a entrada do jogador.
- Informar a posição ou o progresso da resposta, se isso não revelar respostas futuras.
- Rejeitar entradas inválidas sem corromper o estado da partida.
- Informar quando a partida termina, a pontuação e as opções de reinício ou saída.

A camada de entrada e saída deve ser separada, sempre que possível, das regras do jogo. Isso facilita substituir o terminal por uma interface gráfica futuramente.

## 13. Temporização e evolução futura

A primeira versão pode apresentar os sinais em texto, sem temporização real. Em uma versão posterior:

- Cada sinal pode ficar visível por um intervalo definido.
- Pode haver uma pausa entre sinais.
- O intervalo pode diminuir conforme o nível aumenta.
- O programa deve impedir respostas durante a apresentação da sequência.
- Se for necessária uma interface responsiva, avaliar um loop de eventos em vez de depender de pausas bloqueantes.

A temporização não deve alterar a ordem nem o conteúdo da sequência.

## 14. Critérios de aceitação

A versão inicial estará funcional quando:

- [ ] Uma partida puder começar com os dados corretamente inicializados.
- [ ] A primeira rodada contiver exatamente um sinal.
- [ ] Cada nova rodada acrescentar exatamente um sinal.
- [ ] A sequência completa for apresentada em sua ordem original a cada rodada.
- [ ] Uma sequência reproduzida corretamente permitir avançar de rodada.
- [ ] Uma resposta válida incorreta encerrar a partida.
- [ ] Uma entrada inválida não avançar indevidamente a posição de resposta.
- [ ] O comprimento e a capacidade da sequência forem respeitados.
- [ ] A pontuação seguir o critério escolhido.
- [ ] O jogador puder reiniciar ou sair após o fim da partida.
- [ ] A lógica principal puder ser testada sem depender de uma interface gráfica.

## 15. Casos de teste sugeridos

| Caso | Preparação/ação | Resultado esperado |
|---|---|---|
| Primeira rodada | Iniciar uma partida | Um sinal é gerado e apresentado |
| Acerto completo | Reproduzir corretamente todos os sinais | Rodada concluída e próxima rodada iniciada |
| Erro na primeira posição | Informar um sinal diferente do esperado | Partida encerrada |
| Erro em posição posterior | Acertar algumas posições e errar a seguinte | Partida encerrada no erro |
| Entrada inválida | Informar um valor fora do conjunto permitido | Entrada rejeitada; posição esperada permanece igual |
| Preservação da sequência | Concluir uma rodada | Sinais anteriores permanecem na mesma ordem |
| Crescimento | Concluir várias rodadas | O comprimento cresce em uma unidade por rodada |
| Limite da sequência | Atingir a capacidade máxima | Nenhuma escrita fora dos limites |
| Reinício | Escolher jogar novamente | Nova partida começa com dados reinicializados |
| Reprodutibilidade | Usar uma semente fixa durante testes | A sequência gerada pode ser reproduzida no ambiente de teste |

## 16. Decisões a tomar antes de codificar

- Qual será o limite máximo da sequência na primeira versão?
- A entrada será feita por números, letras ou nomes dos sinais?
- A pontuação contará rodadas completas ou sinais corretos?
- O jogo terminará automaticamente ao atingir um limite de nível?
- O recorde será apenas mantido durante a execução ou salvo em arquivo?
- A temporização ficará para uma segunda versão?

## 17. Estratégia de implementação recomendada

Implemente por etapas, validando cada uma antes de avançar:

1. Representação da sequência e dos sinais.
2. Inicialização e geração de um sinal.
3. Apresentação da sequência no terminal.
4. Leitura e validação de uma resposta.
5. Verificação de uma rodada completa.
6. Laço de progressão entre rodadas.
7. Pontuação, fim de jogo e reinício.
8. Tratamento de limites e entradas inválidas.
9. Testes de regressão.
10. Eventual interface gráfica e temporização.

**Princípio de projeto:** mantenha as regras do jogo independentes do terminal. A interface deve apresentar sinais e recolher entradas; a lógica deve decidir se a resposta está correta e qual é o próximo estado.
