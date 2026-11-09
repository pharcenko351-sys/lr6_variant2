#pragma once

#include <coroutine>
#include <exception>
#include <utility>

class Task
{
public:
    struct promise_type
    {
        std::exception_ptr error;

        Task get_return_object()
        {
            return Task(std::coroutine_handle<promise_type>::from_promise(*this));
        }
        std::suspend_always initial_suspend() noexcept { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        void return_void() {}
        void unhandled_exception() { error = std::current_exception(); }
    };

    explicit Task(std::coroutine_handle<promise_type> handle) : handle_(handle) {}
    Task(const Task &) = delete;
    Task &operator=(const Task &) = delete;
    Task(Task &&other) noexcept : handle_(std::exchange(other.handle_, nullptr)) {}
    Task &operator=(Task &&other) noexcept
    {
        if (this != &other)
        {
            destroy();
            handle_ = std::exchange(other.handle_, nullptr);
        }
        return *this;
    }
    ~Task() { destroy(); }

    bool done() const { return handle_.done(); }

    void resume()
    {
        handle_.resume();
        if (handle_.promise().error)
            std::rethrow_exception(handle_.promise().error);
    }

private:
    void destroy()
    {
        if (handle_)
            handle_.destroy();
    }

    std::coroutine_handle<promise_type> handle_;
};
