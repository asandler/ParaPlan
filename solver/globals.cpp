#include "globals.h"

pthread_mutex_t FreeThreadsMutex;
pthread_mutex_t AnswerMutex;
pthread_attr_t ThreadAttributes;

int ThreadsNumber = 1;
int FreeThreads = 0;
bool Answer = false;

SPDI Spdi;
SPDIReachTask ReachTask;
