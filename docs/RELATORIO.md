# Relatório Técnico (rascunho)

> Documentação da etapa 2 (20 pts). Versão final em **no máximo 4 páginas**,
> seguindo o modelo de artigos da SBC. Este arquivo é só um roteiro do que
> precisa ser descrito.

## 1. Organização das threads
- Quais threads existem, quantas de cada e o que cada uma faz.
- Como são criadas e finalizadas (`pthread_create` / `pthread_join`).

## 2. Recursos compartilhados
- Mapa (ocupação das células), esteira (buffer limitado), filas das estações,
  contadores/estatísticas, flag de execução.

## 3. Seções críticas identificadas
- Movimentação de robôs (ocupar/liberar célula).
- Inserir / avançar / retirar na esteira.
- Enfileirar / desenfileirar pacotes nas estações.
- Atualização dos contadores globais.

## 4. Mecanismos de sincronização utilizados
- `pthread_mutex_t` por recurso; justificar a granularidade escolhida.
- (Se usar) `pthread_cond_t` para espera sem busy-wait.
- Uso de `volatile sig_atomic_t` para a flag de término.

## 5. Decisões de projeto
- Por que um mutex por recurso (e não um global).
- Estratégia de geração, de escolha de estação/despacho, de movimentação.
- Como o término da simulação é coordenado entre as threads.

## 6. Guia de utilização
- Instruções de compilação e execução em Ubuntu 24.04 (ver `README.md`).
