#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <pthread.h>
#include <unistd.h>

#include "ul_log.h"

#include "http_handle.h"
#include "dict.h"
#include "config.h"

#define URLMAXLEN 255
#define MAXTHREAD 100

const char * USEAGE = 
"Useage : %s -f filename [OPTION]\n"
"OPTION:\n"
"  -n  integer    thread setting\n"
"  -t  integer    set timeout(ms)\n"
"  -s  integer    set maxsize\n"
"  -d             output response content\n"
"  -c  filename   specify configuration file\n"
;

FILE* fp;
static pthread_mutex_t mutex_stdout = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t mutex_file = PTHREAD_MUTEX_INITIALIZER;
static int timeout;
static int maxsize;
static bool need_content;
static ul_logstat_t logstat = {16, 0, 0};
static Dict *dict;

void* crawler(void* arg)
{
    char url[URLMAXLEN];
    Http_handle http_handle;
    int code;
    int length;

    if (0 != ul_openlog_r("crawler_thread", &logstat)) {
        ul_writelog(UL_LOG_WARNING, "ul_openlog_r failed.");
        return NULL;
    }

    http_handle.set_maxsize(maxsize);
    http_handle.set_timeout(timeout);

    ul_writelog(UL_LOG_TRACE, "crawler thread created.");
    
    while (true) {
        pthread_mutex_lock(&mutex_file);
        if ((fgets(url, URLMAXLEN, fp)) == NULL) {
            pthread_mutex_unlock(&mutex_file);
            ul_writelog(UL_LOG_TRACE, "crawler thread ended.");
            ul_closelog_r(0);
            return NULL;
        }
        pthread_mutex_unlock(&mutex_file);

        ul_writelog(UL_LOG_TRACE, "got url.");
        length = strlen(url);
        if (length <= 2) {
            ul_writelog(UL_LOG_WARNING, "url is too short or empty.");
            continue;
        }
        if ('\n' == url[length-1]) {
            url[length-1] = '\0';
        }

        if (dict->query(url)) {
            ul_writelog(UL_LOG_WARNING, "common url, skip :%s", url);
            continue;
        } else {
            if (0 == dict->add(url)) {
                ul_writelog(UL_LOG_TRACE, "added url to dict.");
            } else {
                ul_writelog(UL_LOG_WARNING, "error while add url to dict.");
            }
        }
        
        http_handle.reset(url);
        if (0 != (code = http_handle.doit())) {
            switch (code)
            {
            case -1:
                ul_writelog(UL_LOG_WARNING, "url format not correct.");
                break;
            case -2:
                ul_writelog(UL_LOG_WARNING, "can't get host ip address.");
                break;
            case -3:
                ul_writelog(UL_LOG_WARNING, "can't crawler the url.");
                break;
            default:
                ul_writelog(UL_LOG_WARNING, "unknow error occured.");
                break;
            }
        } else {
            ul_writelog(UL_LOG_TRACE, "crawl success.");
            pthread_mutex_lock(&mutex_stdout);
            printf("URL     : %s\n", url);
            printf("CODE    : %d\n", http_handle.get_code());
            printf("LENTH   : %d\n", http_handle.get_len());
            if (need_content) {
                printf("\nCONTENT : \n%s\n\n", http_handle.get_content());
            }
            printf("<==============================>\n");
            pthread_mutex_unlock(&mutex_stdout);
        }
    }
    ul_writelog(UL_LOG_TRACE, "crawler thread ended.");
    ul_closelog_r(0);
    return NULL;
}

void show_usage(const char *proc_name)
{
    printf(USEAGE, proc_name);
    return;
}

int main(int argc, char *argv[])
{
    int opt;
    int pthread_n = 5;
    char* file = NULL;
    char* config_file = NULL;

    timeout = 5000;
    maxsize = 2048;
    need_content = false;

    if (0 != ul_openlog("./log", "crawler", &logstat, 1024, NULL)) {
        printf("FATAL ERROR: open log error.\n");
        return -1;
    }

    while ((opt = getopt(argc, argv, "c:f:n:t:s:d")) != -1) {
        switch (opt) {
        case 'c': {
            config_file = strdup(optarg);
            Config config(config_file);

            if (!config.isload()) {
                ul_writelog(UL_LOG_FATAL, "configuration file open error.");
                goto fail;
            } else {
                char* str;
                int value;

                if (NULL != (str = config.getstr("urls"))) {
                    if (file) free(file);
                    file = strdup(str);
                }
                if (-1 != (value = config.getint("thread"))) {
                    pthread_n = value;
                }
                if (-1 != (value = config.getint("timeout"))) {
                    timeout = value;
                }
                if (-1 != (value = config.getint("maxsize"))) {
                    maxsize = value;
                }
            }
            break;
        }
        case 'n':
            pthread_n = atoi(optarg);
            break;
        case 'f':
            if (!file) free(file);
            file = strdup(optarg);
            break;
        case 't':
            timeout = atoi(optarg);
            break;
        case 's':
            maxsize = atoi(optarg);
            break;
        case 'd':
            need_content = true;
            break;
        }
    }

    if (NULL == file) {
        show_usage(argv[0]);
        goto fail;
    }

    if ((fp = fopen(file, "r")) == NULL) {
        ul_writelog(UL_LOG_FATAL, "file open error.");
        goto fail;
    }

    // TODO: more settings for dict
    dict = new Dict(1024);

    pthread_t tid[MAXTHREAD];
    for (int i = 0; i < pthread_n; ++i) {
        pthread_t id;
        pthread_create(&id, NULL, crawler, NULL);
        tid[i] = id;
        ul_writelog(UL_LOG_TRACE, "created a new thread.");
    }

    for (int i = 0; i < pthread_n; ++i) {
        pthread_join(tid[i], NULL);
    }

    ul_writelog(UL_LOG_TRACE, "completed, will exit now.");
    fclose(fp);
    ul_closelog(0);
    return 0;

fail:
    ul_closelog(0);
    return -1;
}
