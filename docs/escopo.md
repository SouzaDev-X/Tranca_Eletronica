# Escopo do Projeto — Tranca Eletrônica com Senha (100% Simulada)

## Título
Tranca Eletrônica com Senha utilizando Arduino UNO, Keypad 4x4 e Servo SG90 (simulado no Wokwi).

## Fluxo Básico
1. Usuário digita senha no teclado 4×4.
2. Arduino valida:
   - Se correta: servo gira para 90° por 3 segundos (destrava).
   - Se incorreta: notifica erro e incrementa contador.
   - Após 3 tentativas erradas consecutivas: bloqueio por 10 segundos (não aceita mais entradas durante esse tempo).
3. Feedback via **Serial Monitor** (mensagens claras) — opcional: LEDs virtuais verde/vermelho no Wokwi.

## Regras
- Senha padrão: `1234` (4 dígitos).
- Tecla `*` = limpa entrada; tecla `#` = confirma.
- Bloqueio por 10 segundos após 3 erros.
- Sistema ignora input durante bloqueio.
- Mensagens: “Senha correta”, “Senha incorreta”, “Bloqueado por X s”.

## Critérios de Aceitação
- Senha correta abre o servo por 3 segundos; depois retorna a posição inicial.
- Senha errada mostra erro e faz bloqueio após 3 tentativas.
- Durante bloqueio, entrada é ignorada e serial informa isso.
- Flags visuais (opcional): LED verde aberto, LED vermelho bloqueio.
- Link da simulação Wokwi disponível no repositório para avaliação.
