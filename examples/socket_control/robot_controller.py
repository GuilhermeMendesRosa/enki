#!/usr/bin/env python3
"""
Script de Controle do Robô E-Puck via Socket

Este script se conecta ao programa Enki via TCP e permite controlar
o movimento do robô E-Puck através de comandos simples.

Comandos disponíveis:
- forward [velocidade] - move para frente
- turn_left [velocidade] - vira à esquerda  
- turn_right [velocidade] - vira à direita
- stop - para o robô
- status - mostra posição e estado atual
- help - mostra esta ajuda
- quit - encerra conexão
"""

import socket
import time
import threading
import sys

class EnkiRobotController:
    def __init__(self, host='localhost', port=9999):
        self.host = host
        self.port = port
        self.socket = None
        self.connected = False
        self.running = True
        
    def connect(self):
        """Conecta ao servidor Enki"""
        try:
            self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.socket.connect((self.host, self.port))
            self.connected = True
            print(f"✓ Conectado ao Enki em {self.host}:{self.port}")
            
            # Iniciar thread para receber mensagens
            self.receive_thread = threading.Thread(target=self.receive_messages)
            self.receive_thread.daemon = True
            self.receive_thread.start()
            
            return True
            
        except Exception as e:
            print(f"✗ Erro ao conectar: {e}")
            print("Certifique-se de que o programa Enki está rodando!")
            return False
    
    def receive_messages(self):
        """Thread para receber mensagens do servidor"""
        while self.connected and self.running:
            try:
                data = self.socket.recv(1024).decode('utf-8')
                if data:
                    for line in data.strip().split('\n'):
                        if line.strip():
                            print(f"🤖 {line}")
                else:
                    break
            except:
                break
                
        self.connected = False
    
    def send_command(self, command):
        """Envia comando para o robô"""
        if not self.connected:
            print("✗ Não conectado ao servidor!")
            return False
            
        try:
            self.socket.send(f"{command}\n".encode('utf-8'))
            return True
        except Exception as e:
            print(f"✗ Erro ao enviar comando: {e}")
            self.connected = False
            return False
    
    def disconnect(self):
        """Desconecta do servidor"""
        self.running = False
        if self.socket:
            self.socket.close()
        self.connected = False
        print("Desconectado.")
    
    def interactive_mode(self):
        """Modo interativo para controle manual"""
        print("\n" + "="*50)
        print("🤖 CONTROLE INTERATIVO DO E-PUCK")
        print("="*50)
        print("Comandos disponíveis:")
        print("  forward [vel]    - mover para frente (vel padrão: 5)")
        print("  turn_left [vel]  - virar à esquerda (vel padrão: 3)")
        print("  turn_right [vel] - virar à direita (vel padrão: 3)")
        print("  stop             - parar robô")
        print("  status           - mostrar posição atual")
        print("  help             - mostrar esta ajuda")
        print("  quit             - sair")
        print("="*50)
        
        while self.connected and self.running:
            try:
                command = input("\n🎮 Digite um comando: ").strip()
                
                if not command:
                    continue
                    
                if command.lower() in ['quit', 'exit', 'q']:
                    self.send_command("quit")
                    break
                    
                elif command.lower() == 'help':
                    print("\nComandos:")
                    print("  forward [velocidade] - ex: 'forward 10'")
                    print("  turn_left [velocidade] - ex: 'turn_left 5'")
                    print("  turn_right [velocidade] - ex: 'turn_right 5'")
                    print("  stop - parar o robô")
                    print("  status - ver posição atual")
                    continue
                    
                # Enviar comando
                self.send_command(command)
                time.sleep(0.1)  # Pequena pausa para não sobrecarregar
                
            except KeyboardInterrupt:
                print("\n\nInterrompido pelo usuário.")
                break
            except EOFError:
                print("\nFim da entrada.")
                break
    
    def demo_mode(self):
        """Modo demonstração com sequência automática"""
        print("\n🎬 MODO DEMONSTRAÇÃO")
        print("Executando sequência automática...")
        
        demo_commands = [
            ("status", "Verificando posição inicial"),
            ("forward 8", "Movendo para frente por 3 segundos"),
            ("turn_left 4", "Virando à esquerda por 2 segundos"),
            ("forward 6", "Movendo para frente novamente por 3 segundos"),
            ("turn_right 4", "Virando à direita por 2 segundos"),
            ("forward 5", "Movimento final para frente por 2 segundos"),
            ("stop", "Parando o robô"),
            ("status", "Posição final")
        ]
        
        for command, description in demo_commands:
            if not self.connected:
                break
                
            print(f"\n📍 {description}")
            print(f"Comando: {command}")
            
            self.send_command(command)
            
            # Tempo de espera baseado no comando
            if "forward" in command or "turn" in command:
                time.sleep(3)
            else:
                time.sleep(1)
        
        print("\n✓ Demonstração concluída!")

def main():
    controller = EnkiRobotController()
    
    # Verificar argumentos da linha de comando
    if len(sys.argv) > 1 and sys.argv[1] == 'demo':
        demo_mode = True
    else:
        demo_mode = False
    
    print("🚀 Iniciando controlador do robô E-Puck...")
    
    # Tentar conectar
    if not controller.connect():
        return 1
    
    try:
        # Aguardar mensagens de boas-vindas
        time.sleep(0.5)
        
        if demo_mode:
            controller.demo_mode()
        else:
            controller.interactive_mode()
            
    finally:
        controller.disconnect()
    
    return 0

if __name__ == "__main__":
    exit(main())
