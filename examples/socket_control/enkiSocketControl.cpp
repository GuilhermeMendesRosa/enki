/*
    Exemplo de Controle via Socket - Enki
    
    Este exemplo demonstra como controlar um robô E-Puck via socket TCP.
    O programa cria uma simulação gráfica onde o robô se move em um plano
    e recebe comandos de movimento de um cliente externo via socket.
    
    Comandos aceitos via socket:
    - "forward X" - move para frente com velocidade X
    - "turn_left X" - vira à esquerda com velocidade X
    - "turn_right X" - vira à direita com velocidade X  
    - "stop" - para o robô
    - "status" - retorna posição e orientação atual
    - "quit" - encerra o programa
*/

#include <viewer/Viewer.h>
#include <enki/PhysicalEngine.h>
#include <enki/robots/e-puck/EPuck.h>
#include <QApplication>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTimer>
#include <QtGui>
#include <iostream>
#include <sstream>
#include <string>

using namespace Enki;
using namespace std;

class SocketControlExample : public ViewerWidget
{
    Q_OBJECT
    
protected:
    EPuck* robot;
    QTcpServer* server;
    QTcpSocket* clientSocket;
    QTimer* statusTimer;
    int stepCounter;
    bool verbose;
    
public:
    SocketControlExample(World *world, QWidget *parent = 0);
    ~SocketControlExample();
    
protected:
    virtual void timerEvent(QTimerEvent * event);
    
private slots:
    void onNewConnection();
    void onDataReceived();
    void onClientDisconnected();
    void sendStatus();
    
private:
    void setupRobot(World* world);
    void setupTcpServer();
    void processCommand(const QString& command);
    void sendResponse(const QString& message);
    void sendRobotStatus();
};

SocketControlExample::SocketControlExample(World *world, QWidget *parent) :
    ViewerWidget(world, parent),
    robot(nullptr),
    server(nullptr),
    clientSocket(nullptr),
    stepCounter(0),
    verbose(true)
{
    // Criar e configurar o robô E-Puck
    setupRobot(world);
    
    // Configurar servidor TCP
    setupTcpServer();
    
    // Timer para enviar status periodicamente
    statusTimer = new QTimer(this);
    connect(statusTimer, &QTimer::timeout, this, &SocketControlExample::sendStatus);
    statusTimer->start(1000); // Enviar status a cada 1 segundo
    
    cout << "=== Controle via Socket - Enki ===" << endl;
    cout << "Servidor TCP rodando na porta 9999" << endl;
    cout << "Conecte um cliente para controlar o robô" << endl;
    cout << "Comandos: forward X, turn_left X, turn_right X, stop, status, quit" << endl;
}

void SocketControlExample::setupRobot(World* world)
{
    // Criar robô E-Puck no centro do mundo
    robot = new EPuck;
    robot->pos = Point(60, 60);     // Posição central
    robot->angle = 0;               // Orientação inicial
    robot->setColor(Color(0.2, 0.7, 0.2)); // Verde
    
    // Inicialmente parado
    robot->leftSpeed = 0.0;
    robot->rightSpeed = 0.0;
    
    world->addObject(robot);
    
    if (verbose) {
        cout << "Robô criado em (" << robot->pos.x << ", " << robot->pos.y << ")" << endl;
    }
}

void SocketControlExample::setupTcpServer()
{
    server = new QTcpServer(this);
    
    // Conectar sinais
    connect(server, &QTcpServer::newConnection, this, &SocketControlExample::onNewConnection);
    
    // Iniciar servidor na porta 9999
    if (!server->listen(QHostAddress::Any, 9999)) {
        cout << "Erro: Não foi possível iniciar o servidor TCP: " 
             << server->errorString().toStdString() << endl;
    } else {
        cout << "Servidor TCP iniciado na porta 9999" << endl;
    }
}

void SocketControlExample::timerEvent(QTimerEvent * event)
{
    stepCounter++;
    
    // Log ocasional da posição
    if (verbose && stepCounter % 500 == 0) {
        cout << "Passo " << stepCounter << " - Posição: (" 
             << robot->pos.x << ", " << robot->pos.y 
             << ") Ângulo: " << robot->angle << endl;
    }
    
    ViewerWidget::timerEvent(event);
}

void SocketControlExample::processCommand(const QString& command)
{
    QStringList parts = command.trimmed().split(' ');
    if (parts.isEmpty()) return;
    
    QString cmd = parts[0].toLower();
    
    if (cmd == "forward") {
        double speed = parts.size() > 1 ? parts[1].toDouble() : 5.0;
        robot->leftSpeed = speed;
        robot->rightSpeed = speed;
        sendResponse("OK: Moving forward at speed " + QString::number(speed));
        
    } else if (cmd == "turn_left") {
        double speed = parts.size() > 1 ? parts[1].toDouble() : 3.0;
        robot->leftSpeed = speed * 0.5;
        robot->rightSpeed = speed;
        sendResponse("OK: Turning left");
        
    } else if (cmd == "turn_right") {
        double speed = parts.size() > 1 ? parts[1].toDouble() : 3.0;
        robot->leftSpeed = speed;
        robot->rightSpeed = speed * 0.5;
        sendResponse("OK: Turning right");
        
    } else if (cmd == "stop") {
        robot->leftSpeed = 0.0;
        robot->rightSpeed = 0.0;
        sendResponse("OK: Robot stopped");
        
    } else if (cmd == "status") {
        sendRobotStatus();
        
    } else if (cmd == "quit") {
        sendResponse("OK: Goodbye!");
        QApplication::quit();
        
    } else {
        sendResponse("ERROR: Unknown command. Use: forward, turn_left, turn_right, stop, status, quit");
    }
    
    if (verbose) {
        cout << "Comando executado: " << command.toStdString() << endl;
    }
}

void SocketControlExample::sendResponse(const QString& message)
{
    if (clientSocket && clientSocket->state() == QTcpSocket::ConnectedState) {
        clientSocket->write((message + "\n").toUtf8());
        clientSocket->flush();
    }
}

void SocketControlExample::sendRobotStatus()
{
    QString status = QString("STATUS: pos=(%.2f,%.2f) angle=%.2f left_speed=%.2f right_speed=%.2f")
                    .arg(robot->pos.x)
                    .arg(robot->pos.y)
                    .arg(robot->angle)
                    .arg(robot->leftSpeed)
                    .arg(robot->rightSpeed);
    sendResponse(status);
}

void SocketControlExample::onNewConnection()
{
    clientSocket = server->nextPendingConnection();
    
    connect(clientSocket, &QTcpSocket::readyRead, this, &SocketControlExample::onDataReceived);
    connect(clientSocket, &QTcpSocket::disconnected, this, &SocketControlExample::onClientDisconnected);
    
    cout << "Cliente conectado!" << endl;
    sendResponse("HELLO: Connected to Enki Robot Controller");
    sendResponse("COMMANDS: forward X, turn_left X, turn_right X, stop, status, quit");
}

void SocketControlExample::onDataReceived()
{
    if (!clientSocket) return;
    
    QByteArray data = clientSocket->readAll();
    QString command = QString::fromUtf8(data).trimmed();
    
    if (!command.isEmpty()) {
        cout << "Comando recebido: " << command.toStdString() << endl;
        processCommand(command);
    }
}

void SocketControlExample::onClientDisconnected()
{
    cout << "Cliente desconectado." << endl;
    clientSocket = nullptr;
}

void SocketControlExample::sendStatus()
{
    // Enviar status periodicamente apenas se há cliente conectado
    if (clientSocket && clientSocket->state() == QTcpSocket::ConnectedState) {
        // Enviar apenas se solicitado, para não sobrecarregar
        // sendRobotStatus();
    }
}

SocketControlExample::~SocketControlExample()
{
    if (server) {
        server->close();
    }
    cout << "Simulação finalizada após " << stepCounter << " passos." << endl;
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // Criar o mundo da simulação (120x120 unidades)
    World world(120, 120, Color(0.9, 0.9, 0.9));
    
    // Criar o visualizador com controle via socket
    SocketControlExample viewer(&world);
    viewer.setWindowTitle("Controle via Socket - Enki Robotics");
    viewer.resize(800, 600);
    viewer.show();
    
    cout << "\nFeche a janela ou envie 'quit' via socket para sair." << endl;
    
    return app.exec();
}

#include "enkiSocketControl.moc"
