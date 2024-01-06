#include <stdlib.h>
#include <pthread.h>

#include "wth.h"

#define ENV_VALID 1
#define ENV_INVALID 0

int wth_get_class(char **new_class)
{
    char *env_class = getenv("WTH_CLASS");
    *new_class = env_class ? env_class : "";
    
    return env_class ? ENV_VALID : ENV_INVALID;
}

int wth_get_title(char **new_title)
{
    char *env_title = getenv("WTH_TITLE");
    *new_title = env_title ? env_title : "";
    
    return env_title ? ENV_VALID : ENV_INVALID;
}

void wth_init_once(struct wth_once *once, init_func_type init_func, void *user_arg)
{
    pthread_mutex_lock(&once->lock);
    if (!once->initialized) {
        init_func(user_arg);
        once->initialized = 1;
    }
    pthread_mutex_unlock(&once->lock);
}
