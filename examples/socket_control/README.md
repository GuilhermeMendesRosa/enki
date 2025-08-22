# Exemplo de Controle via Socket - Enki

Este exemplo demonstra como controlar um robô E-Puck na simulação Enki através de comandos enviados via socket TCP. O sistema é composto por dois programas:

1. **enkiSocketControl** - Programa C++ que executa a simulação gráfica
2. **robot_controller.py** - Script Python para enviar comandos de controle

## Funcionalidades

- ✅ Visualização gráfica em tempo real do robô E-Puck
- ✅ Controle remoto via socket TCP (porta 9999)
- ✅ Comandos simples de movimento (frente, virar, parar)
- ✅ Modo interativo e modo demonstração
- ✅ Feedback de status em tempo real

## Como Compilar

1. Certifique-se de estar no diretório raiz do Enki:
```bash
cd /Users/guilherme.mendesrosa/code/enki
```

2. Configure e compile o projeto:
```bash
mkdir -p build
cd build
cmake ..
make
```

3. Compile especificamente o exemplo:
```bash
make enkiSocketControl
```

## Como Usar

### Passo 1: Executar a Simulação

No diretório `build`, execute:
```bash
./examples/socket_control/enkiSocketControl
```

Isso abrirá uma janela gráfica mostrando:
- Um robô E-Puck verde no centro
- Ambiente de simulação 120x120 unidades
- Servidor TCP aguardando conexões na porta 9999

### Passo 2: Conectar o Controlador Python

Em outro terminal, execute o script Python:

**Modo Interativo:**
```bash
cd examples/socket_control
python3 robot_controller.py
```

**Modo Demonstração:**
```bash
python3 robot_controller.py demo
```

## Comandos Disponíveis

| Comando | Descrição | Exemplo |
|---------|-----------|---------|
| `forward [velocidade]` | Move para frente | `forward 10` |
| `turn_left [velocidade]` | Vira à esquerda | `turn_left 5` |
| `turn_right [velocidade]` | Vira à direita | `turn_right 5` |
| `stop` | Para o robô | `stop` |
| `status` | Mostra posição atual | `status` |
| `quit` | Encerra conexão | `quit` |

**Nota:** Se não especificar velocidade, valores padrão serão usados.

## Exemplo de Sessão

```
🤖 CONTROLE INTERATIVO DO E-PUCK
🎮 Digite um comando: status
🤖 STATUS: pos=(60.00,60.00) angle=0.00 left_speed=0.00 right_speed=0.00

🎮 Digite um comando: forward 8
🤖 OK: Moving forward at speed 8

🎮 Digite um comando: turn_left 3
🤖 OK: Turning left

🎮 Digite um comando: stop
🤖 OK: Robot stopped
```

## Arquitetura do Sistema

```
┌─────────────────────┐    TCP Socket    ┌──────────────────────┐
│  robot_controller.py │ ◄──────────────► │  enkiSocketControl   │
│  (Cliente Python)   │    Port 9999     │  (Servidor C++/Qt)   │
└─────────────────────┘                  └──────────────────────┘
                                                    │
                                                    ▼
                                         ┌──────────────────────┐
                                         │   Enki Physics       │
                                         │   Engine + Viewer    │
                                         └──────────────────────┘
```

## Troubleshooting

**Erro "Connection refused":**
- Certifique-se de que o programa enkiSocketControl está rodando
- Verifique se a porta 9999 está livre

**Erro de compilação:**
- Verifique se Qt5 está instalado: `brew install qt5` (macOS)
- Verifique se o caminho do Qt está correto no CMAKE

**Robô não se move:**
- Envie comando `status` para verificar se os comandos estão chegando
- Verifique se não há erro de sintaxe nos comandos

## Personalização

Para modificar o comportamento:

1. **Alterar porta:** Modifique a porta 9999 em ambos os arquivos
2. **Adicionar sensores:** Use os sensores IR do E-Puck no código C++
3. **Novos comandos:** Adicione casos no método `processCommand()`
4. **Obstáculos:** Adicione objetos físicos no mundo da simulação

## Dependências

- **C++:** Enki, Qt5 (Core, Widgets, OpenGL, Xml, Network)
- **Python:** Socket (biblioteca padrão)

Este exemplo serve como base para projetos mais complexos de robótica e pode ser estendido com sensores, múltiplos robôs, algoritmos de navegação, etc.
