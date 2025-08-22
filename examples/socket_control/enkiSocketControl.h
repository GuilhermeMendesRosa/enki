#ifndef ENKISOCKETCONTROL_H
#define ENKISOCKETCONTROL_H

#include <viewer/Viewer.h>
#include <enki/PhysicalEngine.h>
#include <enki/robots/e-puck/EPuck.h>
#include <QApplication>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTimer>
#include <QtGui>

using namespace Enki;

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
    
    void setupRobot(World* world);
    void setupTcpServer();
    virtual void timerEvent(QTimerEvent * event);
    void processCommand(const QString& command);
    void sendResponse(const QString& message);
    void sendRobotStatus();
    
public slots:
    void onNewConnection();
    void onDataReceived();
    void onClientDisconnected();
    void sendStatus();
};

#endif // ENKISOCKETCONTROL_H
