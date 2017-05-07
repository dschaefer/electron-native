#include <node.h>
#include <uv.h>

using namespace v8;

struct Job {
    uv_work_t request;
    Persistent<Promise::Resolver> resolver;
    double arg0, arg1, result;
};

static void Add(const FunctionCallbackInfo<Value> &args) {
    auto isolate = args.GetIsolate();
    HandleScope handleScope(isolate);

    auto job = new Job();
    job->request.data = job;
    job->arg0 = args[0]->NumberValue();
    job->arg1 = args[1]->NumberValue();

    auto resolver = Promise::Resolver::New(isolate);
    job->resolver.Reset(isolate, resolver);

    auto work = [](uv_work_t *req) {
        auto job = static_cast<Job *>(req->data);
        job->result = job->arg0 + job->arg1;
    };

    auto after = [](uv_work_t* req, int status) {
        auto isolate = Isolate::GetCurrent();
        HandleScope handleScope(isolate);

        auto job = static_cast<Job *>(req->data);
        auto result = Number::New(isolate, job->result);
        auto resolver = Local<Promise::Resolver>::New(isolate, job->resolver);
        resolver->Resolve(isolate->GetCurrentContext(), result);
    
        job->resolver.Reset();
        delete job;
    };

    uv_queue_work(uv_default_loop(), &job->request, work, after);

    args.GetReturnValue().Set(resolver->GetPromise());
}

void init(Local<Object> exports) {
    NODE_SET_METHOD(exports, "add", Add);
}

NODE_MODULE(addon, init)
