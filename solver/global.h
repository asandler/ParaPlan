#pragma once

#include <pthread.h>

extern pthread_mutex_t FreeThreadsMutex;
extern pthread_mutex_t AnswerMutex;
extern pthread_attr_t ThreadAttributes;

extern int ThreadsNumber;
extern int FreeThreads;
extern bool Answer;
