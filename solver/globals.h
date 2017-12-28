#pragma once

#include <pthread.h>
#include "helpers.h"

extern pthread_mutex_t FreeThreadsMutex;
extern pthread_mutex_t AnswerMutex;
extern pthread_attr_t ThreadAttributes;

extern int ThreadsNumber;
extern int FreeThreads;
extern bool Answer;
extern bool Strict;
extern bool Debug;

extern SPDI Spdi;
extern SPDIReachTask ReachTask;
