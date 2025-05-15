#ifndef SCORE_H
#define SCORE_H
#include <QGraphicsTextItem>
class ScoreSystem : public QGraphicsTextItem{
public:
    ScoreSystem(QGraphicsItem * parent = 0);
    void increasescore();
    void decrementscore();
    int getscore();
    void resetscore();
private:
    int score;
};

#endif // SCORE_H
