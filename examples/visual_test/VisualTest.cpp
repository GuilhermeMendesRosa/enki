/*
    Exemplo Gráfico Avançado para Enki
    Este exemplo demonstra:
    - Múltiplos robôs com comportamentos diferentes
    - Sensores infravermelhos
    - Detecção de obstáculos e navegação
    - Interface gráfica interativa
    - Diferentes tipos de objetos no ambiente
*/

#include <viewer/Viewer.h>
#include <enki/PhysicalEngine.h>
#include <enki/robots/e-puck/EPuck.h>
#include <QApplication>
#include <QtGui>
#include <QTimer>
#include <QKeyEvent>
#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <ctime>
#include <cstdlib>
#include <algorithm>

using namespace Enki;
using namespace std;

class AdvancedVisualTest : public ViewerWidget
{
    Q_OBJECT

protected:
    vector<EPuck*> robots;
    QTimer* simulationTimer;
    int frameCount;
    bool paused;
    
public:
    AdvancedVisualTest(World *world, QWidget *parent = 0) :
        ViewerWidget(world, parent),
        frameCount(0),
        paused(false)
    {
        setupEnvironment(world);
        setupRobots(world);
        setupUI();
        
        cout << "=== Teste Visual Avançado do Enki ===" << endl;
        cout << "Controles:" << endl;
        cout << "  ESPAÇO - Pausar/Continuar simulação" << endl;
        cout << "  R - Resetar posições dos robôs" << endl;
        cout << "  ESC - Sair" << endl;
        cout << "======================================" << endl;
    }
    
    void setupEnvironment(World *world)
    {
        // Criar um labirinto simples
        createMaze(world);
        
        // Adicionar obstáculos circulares aleatórios
        for (int i = 0; i < 8; i++)
        {
            PhysicalObject* obstacle = new PhysicalObject;
            double x = 30 + (rand() % 60);
            double y = 30 + (rand() % 60);
            obstacle->pos = Point(x, y);
            obstacle->setCylindric(3 + (rand() % 3), 3 + (rand() % 3), 5 + (rand() % 5));
            obstacle->setColor(Color(0.8, 0.3, 0.1)); // Laranja
            world->addObject(obstacle);
        }
        
        // Adicionar alguns objetos "alvos" em verde
        for (int i = 0; i < 4; i++)
        {
            PhysicalObject* target = new PhysicalObject;
            double x = 20 + (rand() % 80);
            double y = 20 + (rand() % 80);
            target->pos = Point(x, y);
            target->setCylindric(2, 2, 1);
            target->setColor(Color(0.2, 0.8, 0.2)); // Verde brilhante
            world->addObject(target);
        }
    }
    
    void createMaze(World *world)
    {
        // Paredes externas
        vector<pair<Point, Point>> walls = {
            // Bordas
            {Point(10, 10), Point(110, 10)},   // Parede inferior
            {Point(10, 110), Point(110, 110)}, // Parede superior
            {Point(10, 10), Point(10, 110)},   // Parede esquerda
            {Point(110, 10), Point(110, 110)}, // Parede direita
            
            // Paredes internas para criar um labirinto simples
            {Point(30, 30), Point(30, 70)},    // Parede vertical 1
            {Point(70, 50), Point(70, 90)},    // Parede vertical 2
            {Point(50, 30), Point(90, 30)},    // Parede horizontal 1
            {Point(20, 80), Point(60, 80)},    // Parede horizontal 2
        };
        
        for (auto& wall : walls)
        {
            createWall(world, wall.first, wall.second);
        }
    }
    
    void createWall(World *world, Point start, Point end)
    {
        double length = sqrt(pow(end.x - start.x, 2) + pow(end.y - start.y, 2));
        Point center = Point((start.x + end.x) / 2, (start.y + end.y) / 2);
        double angle = atan2(end.y - start.y, end.x - start.x);
        
        PhysicalObject* wall = new PhysicalObject;
        wall->pos = center;
        wall->angle = angle;
        wall->setCylindric(length/2, 1, 8);
        wall->setColor(Color(0.5, 0.5, 0.5)); // Cinza
        world->addObject(wall);
    }
    
    void setupRobots(World *world)
    {
        // Criar 3 robôs com comportamentos diferentes
        
        // Robô 1: Explorador (movimento aleatório)
        EPuck* explorer = new EPuck;
        explorer->pos = Point(20, 20);
        explorer->angle = 0;
        explorer->setColor(Color(0.2, 0.2, 0.8)); // Azul
        robots.push_back(explorer);
        world->addObject(explorer);
        
        // Robô 2: Seguidor de parede
        EPuck* wallFollower = new EPuck;
        wallFollower->pos = Point(100, 100);
        wallFollower->angle = M_PI;
        wallFollower->setColor(Color(0.8, 0.2, 0.8)); // Magenta
        robots.push_back(wallFollower);
        world->addObject(wallFollower);
        
        // Robô 3: Evitador de obstáculos
        EPuck* avoider = new EPuck;
        avoider->pos = Point(60, 60);
        avoider->angle = M_PI/2;
        avoider->setColor(Color(0.8, 0.8, 0.2)); // Amarelo
        robots.push_back(avoider);
        world->addObject(avoider);
        
        cout << "Criados " << robots.size() << " robôs com comportamentos diferentes:" << endl;
        cout << "  Azul: Explorador (movimento aleatório)" << endl;
        cout << "  Magenta: Seguidor de parede" << endl;
        cout << "  Amarelo: Evitador de obstáculos" << endl;
    }
    
    void setupUI()
    {
        setWindowTitle("Teste Visual Avançado - Enki Robotics Simulator");
        resize(1000, 800);
        setFocusPolicy(Qt::StrongFocus); // Para receber eventos de teclado
    }
    
    virtual void timerEvent(QTimerEvent * event)
    {
        if (!paused)
        {
            frameCount++;
            updateRobotBehaviors();
            
            // Log periódico
            if (frameCount % 200 == 0)
            {
                logRobotStates();
            }
        }
        
        ViewerWidget::timerEvent(event);
    }
    
    void updateRobotBehaviors()
    {
        for (size_t i = 0; i < robots.size(); i++)
        {
            EPuck* robot = robots[i];
            
            switch (i)
            {
                case 0: // Explorador
                    explorerBehavior(robot);
                    break;
                case 1: // Seguidor de parede
                    wallFollowerBehavior(robot);
                    break;
                case 2: // Evitador
                    avoidanceBehavior(robot);
                    break;
            }
        }
    }
    
    void explorerBehavior(EPuck* robot)
    {
        // Movimento aleatório com detecção básica de obstáculos
        static int randomCounter = 0;
        randomCounter++;
        
        // Sensores frontais
        double frontLeft = robot->infraredSensor7.getDist();   // Sensor front-front-left
        double frontRight = robot->infraredSensor0.getDist();  // Sensor front-front-right
        double front = robot->infraredSensor2.getDist();       // Sensor right (usado como frontal)
        
        if (front < 8.0 || frontLeft < 8.0 || frontRight < 8.0)
        {
            // Obstáculo detectado - virar
            robot->leftSpeed = -2.0 + (rand() % 6);
            robot->rightSpeed = 8.0 + (rand() % 4);
        }
        else if (randomCounter % 100 == 0)
        {
            // Mudança aleatória de direção
            robot->leftSpeed = 3.0 + (rand() % 5);
            robot->rightSpeed = 3.0 + (rand() % 5);
        }
    }
    
    void wallFollowerBehavior(EPuck* robot)
    {
        // Seguir parede do lado direito
        double rightSide = robot->infraredSensor2.getDist();      // Sensor direito
        double front = robot->infraredSensor1.getDist();          // Sensor front-right
        double rightFront = robot->infraredSensor0.getDist();     // Sensor front-front-right
        
        if (front < 10.0)
        {
            // Obstáculo à frente - virar à esquerda
            robot->leftSpeed = 1.0;
            robot->rightSpeed = 6.0;
        }
        else if (rightSide > 12.0)
        {
            // Sem parede à direita - virar à direita
            robot->leftSpeed = 6.0;
            robot->rightSpeed = 1.0;
        }
        else if (rightSide < 6.0)
        {
            // Muito próximo da parede - afastar
            robot->leftSpeed = 5.0;
            robot->rightSpeed = 3.0;
        }
        else
        {
            // Seguir em frente ao longo da parede
            robot->leftSpeed = 4.0;
            robot->rightSpeed = 4.0;
        }
    }
    
    void avoidanceBehavior(EPuck* robot)
    {
        // Comportamento de evitação sofisticado
        double speeds[2] = {4.0, 4.0}; // left, right
        
        // Array dos sensores para fácil acesso
        IRSensor* sensors[8] = {
            &robot->infraredSensor0, &robot->infraredSensor1, &robot->infraredSensor2, &robot->infraredSensor3,
            &robot->infraredSensor4, &robot->infraredSensor5, &robot->infraredSensor6, &robot->infraredSensor7
        };
        
        // Verificar todos os sensores
        for (int i = 0; i < 8; i++)
        {
            double distance = sensors[i]->getDist();
            double influence = max(0.0, (15.0 - distance) / 15.0);
            
            // Influência baseada na posição do sensor
            if (i <= 3) // Sensores esquerdos/frontais
            {
                speeds[1] += influence * 3.0; // Acelerar roda direita
                speeds[0] -= influence * 2.0; // Desacelerar roda esquerda
            }
            else // Sensores direitos/traseiros
            {
                speeds[0] += influence * 3.0; // Acelerar roda esquerda
                speeds[1] -= influence * 2.0; // Desacelerar roda direita
            }
        }
        
        // Limitar velocidades
        robot->leftSpeed = max(-5.0, min(8.0, speeds[0]));
        robot->rightSpeed = max(-5.0, min(8.0, speeds[1]));
    }
    
    void logRobotStates()
    {
        cout << "Frame " << frameCount << " - Estados dos robôs:" << endl;
        for (size_t i = 0; i < robots.size(); i++)
        {
            EPuck* robot = robots[i];
            cout << "  Robô " << i << ": pos(" << fixed << setprecision(1) 
                 << robot->pos.x << ", " << robot->pos.y 
                 << ") ângulo:" << robot->angle * 180.0 / M_PI << "°" << endl;
        }
    }
    
    void resetRobots()
    {
        if (robots.size() >= 3)
        {
            robots[0]->pos = Point(20, 20);
            robots[0]->angle = 0;
            
            robots[1]->pos = Point(100, 100);
            robots[1]->angle = M_PI;
            
            robots[2]->pos = Point(60, 60);
            robots[2]->angle = M_PI/2;
        }
        
        cout << "Posições dos robôs resetadas!" << endl;
    }
    
    virtual void keyPressEvent(QKeyEvent* event)
    {
        switch (event->key())
        {
            case Qt::Key_Space:
                paused = !paused;
                cout << (paused ? "Simulação pausada" : "Simulação retomada") << endl;
                break;
                
            case Qt::Key_R:
                resetRobots();
                break;
                
            case Qt::Key_Escape:
                close();
                break;
                
            default:
                ViewerWidget::keyPressEvent(event);
        }
    }
    
    virtual void sceneCompletedHook()
    {
        // Hook para análise pós-frame (se necessário)
    }
    
    ~AdvancedVisualTest()
    {
        cout << "Simulação finalizada após " << frameCount << " frames." << endl;
    }
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // Seed para números aleatórios
    srand(time(nullptr));
    
    // Criar o mundo da simulação
    World world(120, 120, Color(0.95, 0.95, 0.95));
    
    // Criar o visualizador
    AdvancedVisualTest viewer(&world);
    viewer.show();
    
    return app.exec();
}

// Necessário para o MOC do Qt (Meta-Object Compiler)
#include "VisualTest.moc"
