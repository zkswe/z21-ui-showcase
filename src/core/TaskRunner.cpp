/*
 * TaskRunner.cpp - 后台任务执行器实现
 */
#include "core/TaskRunner.hpp"

#include <deque>
#include <pthread.h>
#include <unistd.h>

#include <base/log.h>

namespace cmcore {

struct TaskRunner::Impl {
    std::deque<std::function<void()> > q;
    pthread_mutex_t mx;
    pthread_t tid;
    volatile bool quit;
    long long done;
    Impl() : tid(0), quit(false), done(0) {
        pthread_mutex_init(&mx, NULL);
    }
};

TaskRunner::TaskRunner() : mImpl(NULL) {
}

TaskRunner &TaskRunner::instance() {
    static TaskRunner sInst;
    return sInst;
}

void *TaskRunner::trampoline(void *user) {
    ((TaskRunner *) user)->loop();
    return NULL;
}

void TaskRunner::loop() {
    while (!mImpl->quit) {
        std::function<void()> fn;
        pthread_mutex_lock(&mImpl->mx);
        if (!mImpl->q.empty()) {
            fn = mImpl->q.front();
            mImpl->q.pop_front();
        }
        pthread_mutex_unlock(&mImpl->mx);
        if (!fn) {
            usleep(10 * 1000);
            continue;
        }
        fn();
        pthread_mutex_lock(&mImpl->mx);
        ++mImpl->done;
        pthread_mutex_unlock(&mImpl->mx);
    }
}

void TaskRunner::start() {
    if (mImpl != NULL) {
        return;
    }
    Impl *im = new Impl();
    mImpl = im;
    if (pthread_create(&im->tid, NULL, &TaskRunner::trampoline, this) != 0) {
        LOGE_TRACE("TaskRunner 线程创建失败");
        im->tid = 0;
    } else {
        LOGD_TRACE("TaskRunner started");
    }
}

void TaskRunner::post(const std::function<void()> &fn) {
    start();
    pthread_mutex_lock(&mImpl->mx);
    mImpl->q.push_back(fn);
    pthread_mutex_unlock(&mImpl->mx);
}

long long TaskRunner::doneCount() const {
    if (mImpl == NULL) {
        return 0;
    }
    pthread_mutex_lock(&((Impl *) mImpl)->mx);
    const long long d = mImpl->done;
    pthread_mutex_unlock(&((Impl *) mImpl)->mx);
    return d;
}

int TaskRunner::pendingCount() const {
    if (mImpl == NULL) {
        return 0;
    }
    pthread_mutex_lock(&((Impl *) mImpl)->mx);
    const int n = (int) mImpl->q.size();
    pthread_mutex_unlock(&((Impl *) mImpl)->mx);
    return n;
}

void TaskRunner::stop() {
    if (mImpl == NULL) {
        return;
    }
    mImpl->quit = true;
}

} /* namespace cmcore */
