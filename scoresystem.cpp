#include "scoresystem.h"
#include <QFont>
#include <QGraphicsScene>
ScoreSystem::ScoreSystem(QGraphicsItem * parent):QGraphicsTextItem(parent)
{
    score = 0;
    setPlainText(QString("Score: ") + QString::number(score));
    setDefaultTextColor(Qt::yellow);
    setFont(QFont("Times",16));
    setPos(0,0);
}


void ScoreSystem::increasescore()
{
    score = score + 30;
    setPlainText(QString("Score: ") + QString::number(score));
}


void ScoreSystem::decrementscore(){
    score--;
}

int ScoreSystem::getscore()
{
    return score;
}

void ScoreSystem::resetscore() {
    score = 0;
    setPlainText(QString("Score: ") + QString::number(score));
}
