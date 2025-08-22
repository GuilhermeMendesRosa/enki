/*
    Projeto de teste simples para Enki
    Robô que se move para frente
    Baseado no playground oficial do Enki
*/

#include <viewer/Viewer.h>
#include <enki/PhysicalEngine.h>
#include <enki/robots/e-puck/EPuck.h>
#include <QApplication>
#include <QtGui>
#include <iostream>

using namespace Enki;
using namespace std;

class SimpleRobotTest : public ViewerWidget
{
protected:
    EPuck* robot;
    
public:
    SimpleRobotTest(World *world, QWidget *parent = 0) :
        ViewerWidget(world, parent)
    {
        // Criar um robô E-Puck simples
        robot = new EPuck;
        robot->pos = Point(60, 60);  // Posição inicial no centro
        robot->angle = 0;            // Orientação inicial (0 = para direita)
        
        // Configurar velocidades para movimento para frente
        robot->leftSpeed = 5.0;      // Velocidade da roda esquerda
        robot->rightSpeed = 5.0;     // Velocidade da roda direita
        
        // Definir cor do robô (opcional)
        robot->setColor(Color(0.2, 0.8, 0.2)); // Verde
        
        // Adicionar o robô ao mundo
        world->addObject(robot);
        
        // Adicionar alguns obstáculos para tornar mais interessante
        addObstacles(world);
        
        cout << "Robô criado na posição (" << robot->pos.x << ", " << robot->pos.y << ")" << endl;
        cout << "Velocidades: esquerda=" << robot->leftSpeed << ", direita=" << robot->rightSpeed << endl;
    }
    
    void addObstacles(World *world)
    {
        // Adicionar algumas paredes/obstáculos
        for (int i = 0; i < 5; i++)
        {
            PhysicalObject* obstacle = new PhysicalObject;
            obstacle->pos = Point(20 + i * 20, 20);
            obstacle->setCylindric(3, 3, 5);  // raio, raio, altura
            obstacle->setColor(Color(0.8, 0.2, 0.2)); // Vermelho
            world->addObject(obstacle);
        }
        
        // Adicionar uma parede lateral
        PhysicalObject* wall = new PhysicalObject;
        wall->pos = Point(100, 60);
        wall->setCylindric(2, 15, 8);  // Parede vertical
        wall->setColor(Color(0.5, 0.5, 0.5)); // Cinza
        world->addObject(wall);
    }
    
    virtual void timerEvent(QTimerEvent * event)
    {
        // Aqui você pode adicionar lógica de controle mais complexa
        // Por exemplo, detecção de obstáculos e mudança de direção
        
        // Verificar sensores infravermelhos (se necessário)
        // double frontSensor = robot->infraredSensor2.getRayDist(2); // Sensor frontal
        
        // Exemplo de lógica simples: se detectar obstáculo, virar
        // if (frontSensor < 10.0) {
        //     robot->leftSpeed = 2.0;
        //     robot->rightSpeed = 8.0;  // Virar à direita
        // } else {
        //     robot->leftSpeed = 5.0;
        //     robot->rightSpeed = 5.0;  // Seguir em frente
        // }
        
        // Imprimir posição atual do robô (opcional)
        static int counter = 0;
        if (++counter % 100 == 0) { // A cada 100 frames
            cout << "Posição do robô: (" << robot->pos.x << ", " << robot->pos.y 
                 << ") Ângulo: " << robot->angle << endl;
        }
        
        ViewerWidget::timerEvent(event);
    }
    
    virtual void sceneCompletedHook()
    {
        // Hook chamado após cada frame de simulação
        // Pode ser usado para logging ou análise
    }
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // Criar o mundo da simulação
    // Parâmetros: largura, altura, cor de fundo
    World world(120, 120, Color(0.9, 0.9, 0.9));
    
    // Criar o visualizador com nosso teste
    SimpleRobotTest viewer(&world);
    
    // Configurar a janela
    viewer.setWindowTitle("Teste Simples - Robô Movendo para Frente");
    viewer.resize(800, 600);
    viewer.show();
    
    cout << "=== Teste Simples do Enki ===" << endl;
    cout << "Robô configurado para se mover para frente" << endl;
    cout << "Pressione Ctrl+C para sair" << endl;
    
    return app.exec();
}