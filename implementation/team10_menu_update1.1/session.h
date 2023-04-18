#ifndef SESSION_H
#define SESSION_H


class Session
{
public:
    Session();
    bool update(int HR, int coherence = -1);
    int getTime();
private:
    int elapsedTimeInSeconds;
};

#endif // SESSION_H
