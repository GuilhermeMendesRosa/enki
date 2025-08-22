# Teste Visual Avançado - Enki

Este exemplo demonstra um teste gráfico avançado usando o simulador de robótica Enki com interface Qt.

## Características

### Ambiente de Simulação
- Labirinto com paredes e obstáculos
- Objetos alvos distribuídos pelo ambiente
- Obstáculos circulares de tamanhos variados
- Interface gráfica interativa

### Robôs com Comportamentos Diferentes
1. **Robô Azul (Explorador)**: Movimento aleatório com detecção básica de obstáculos
2. **Robô Magenta (Seguidor de Parede)**: Segue paredes usando algoritmo de wall-following
3. **Robô Amarelo (Evitador de Obstáculos)**: Navegação sofisticada com evitação de obstáculos

### Controles Interativos
- **ESPAÇO**: Pausar/continuar a simulação
- **R**: Resetar as posições dos robôs
- **ESC**: Sair da aplicação

### Recursos Demonstrados
- Uso de sensores infravermelhos dos robôs E-Puck
- Diferentes algoritmos de navegação robótica
- Interface gráfica responsiva
- Logging de estados dos robôs
- Manipulação de eventos de teclado

## Compilação

```bash
# No diretório raiz do projeto Enki
mkdir -p build
cd build
cmake ..
make visual_test
```

## Execução

```bash
# No diretório build
./examples/visual_test/visual_test
```

## Algoritmos Implementados

### 1. Explorador (Robô Azul)
- Movimento baseado em números aleatórios
- Detecção frontal de obstáculos
- Mudanças de direção aleatórias periódicas

### 2. Seguidor de Parede (Robô Magenta)
- Mantém distância constante da parede direita
- Comportamento de virada quando encontra obstáculos frontais
- Busca por paredes quando não há referência

### 3. Evitador de Obstáculos (Robô Amarelo)
- Usa todos os 8 sensores infravermelhos
- Cálculo de influência baseado na distância
- Ajuste dinâmico das velocidades das rodas

## Estrutura do Código

- `AdvancedVisualTest`: Classe principal que herda de `ViewerWidget`
- `setupEnvironment()`: Criação do labirinto e obstáculos
- `updateRobotBehaviors()`: Lógica de controle dos robôs
- Event handlers para interação com teclado
- Logging e monitoramento de estados

## Extensões Possíveis

1. Adicionar mais tipos de sensores (câmera, ultrassom)
2. Implementar algoritmos de SLAM (Localização e Mapeamento Simultâneos)
3. Comunicação entre robôs
4. Algoritmos de enxame (swarm robotics)
5. Tarefas colaborativas (busca e resgate)
6. Interface para configuração em tempo real

Este exemplo serve como base para desenvolvimento de simulações roboticas mais complexas e pesquisa em algoritmos de navegação autônoma.
