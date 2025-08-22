/*
    Exemplo Gráfico Básico para Enki
    Este é um exemplo didático simples que demonstra:
    - Como criar e configurar robôs
    - Como adicionar obstáculos
    - Como implementar comportamentos básicos
    - Interface gráfica mínima
*/

#include <viewer/Viewer.h>
#include <enki/PhysicalEngine.h>
#include <enki/robots/e-puck/EPuck.h>
#include <QApplication>
#include <QtGui>
#include <iostream>

using namespace Enki;
using namespace std;

class BasicExample : public ViewerWidget
{
protected:
    EPuck* robot;
    int stepCounter;
    
public:
    BasicExample(World *world, QWidget *parent = 0) :
        ViewerWidget(world, parent),
        stepCounter(0)
    {
        // Criar um robô E-Puck
        robot = new EPuck;
        robot->pos = Point(30, 30);     // Posição inicial
        robot->angle = 0;               // Orientação inicial (0 = para a direita)
        robot->setColor(Color(0.2, 0.7, 0.2)); // Verde
        
        // Configuração inicial: mover para frente
        robot->leftSpeed = 5.0;
        robot->rightSpeed = 5.0;
        
        // Adicionar o robô ao mundo
        world->addObject(robot);
        
        // Criar alguns obstáculos simples
        createSimpleObstacles(world);
        
        cout << "=== Exemplo Básico do Enki ===" << endl;
        cout << "Robô criado em (" << robot->pos.x << ", " << robot->pos.y << ")" << endl;
        cout << "O robô irá se mover e evitar obstáculos automaticamente" << endl;
        cout << "Feche a janela para sair" << endl;
    }
    
    void createSimpleObstacles(World *world)
    {
        // Obstáculo 1: Círculo no centro
        PhysicalObject* centralObstacle = new PhysicalObject;
        centralObstacle->pos = Point(60, 60);
        centralObstacle->setCylindric(8, 8, 6);  // raio_x, raio_y, altura
        centralObstacle->setColor(Color(0.8, 0.2, 0.2)); // Vermelho
        world->addObject(centralObstacle);
        
        // Obstáculo 2: Parede lateral
        PhysicalObject* wall = new PhysicalObject;
        wall->pos = Point(90, 40);
        wall->setCylindric(3, 20, 8);  // Parede vertical
        wall->setColor(Color(0.5, 0.5, 0.8)); // Azul acinzentado
        world->addObject(wall);
        
        // Obstáculo 3: Pequeno obstáculo
        PhysicalObject* smallObstacle = new PhysicalObject;
        smallObstacle->pos = Point(40, 80);
        smallObstacle->setCylindric(4, 4, 3);
        smallObstacle->setColor(Color(0.8, 0.8, 0.2)); // Amarelo
        world->addObject(smallObstacle);
        
        cout << "Criados 3 obstáculos no ambiente" << endl;
    }
    
    virtual void timerEvent(QTimerEvent * event)
    {
        stepCounter++;
        
        // Implementar comportamento simples de evitação de obstáculos
        simpleAvoidanceBehavior();
        
        // Log a cada 200 steps (aproximadamente a cada 2 segundos)
        if (stepCounter % 200 == 0)
        {
            cout << "Passo " << stepCounter << " - Posição: (" 
                 << robot->pos.x << ", " << robot->pos.y << ")" << endl;
        }
        
        ViewerWidget::timerEvent(event);
    }
    
    void simpleAvoidanceBehavior()
    {
        // Usar os sensores frontais para detectar obstáculos
        double frontRight = robot->infraredSensor0.getDist();   // Sensor front-front-right
        double frontLeft = robot->infraredSensor7.getDist();    // Sensor front-front-left
        double rightSide = robot->infraredSensor2.getDist();    // Sensor direito
        double leftSide = robot->infraredSensor5.getDist();     // Sensor esquerdo
        
        // Distância mínima de segurança
        const double safeDistance = 10.0;
        
        // Velocidades padrão (para frente)
        double leftSpeed = 5.0;
        double rightSpeed = 5.0;
        
        // Lógica de evitação simples
        if (frontRight < safeDistance || frontLeft < safeDistance)
        {
            // Obstáculo à frente - escolher direção para virar
            if (leftSide > rightSide)
            {
                // Mais espaço à esquerda - virar à esquerda
                leftSpeed = 2.0;
                rightSpeed = 7.0;
            }
            else
            {
                // Mais espaço à direita - virar à direita
                leftSpeed = 7.0;
                rightSpeed = 2.0;
            }
        }
        else if (rightSide < safeDistance)
        {
            // Obstáculo muito próximo à direita - afastar
            leftSpeed = 3.0;
            rightSpeed = 6.0;
        }
        else if (leftSide < safeDistance)
        {
            // Obstáculo muito próximo à esquerda - afastar
            leftSpeed = 6.0;
            rightSpeed = 3.0;
        }
        
        // Aplicar as velocidades calculadas
        robot->leftSpeed = leftSpeed;
        robot->rightSpeed = rightSpeed;
    }
    
    ~BasicExample()
    {
        cout << "Simulação finalizada após " << stepCounter << " passos." << endl;
    }
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // Criar o mundo da simulação (120x120 unidades, fundo cinza claro)
    World world(120, 120, Color(0.9, 0.9, 0.9));
    
    // Criar o visualizador
    BasicExample viewer(&world);
    viewer.setWindowTitle("Exemplo Básico - Enki Robotics");
    viewer.resize(800, 600);
    viewer.show();
    
    return app.exec();
}
