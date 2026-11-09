#pragma once

#include <coroutine>
#include <cstdlib>
#include <optional>

class WaitController
{
public:
    class Awaiter
    {
    public:
        explicit Awaiter(bool mustSuspend) : mustSuspend_(mustSuspend) {}
        bool await_ready() const noexcept { return !mustSuspend_; }
        void await_suspend(std::coroutine_handle<>) const noexcept {}
        void await_resume() const noexcept {}

    private:
        bool mustSuspend_;
    };

    explicit WaitController(int threshold) : threshold_(threshold) {}

    Awaiter wait(int current)
    {
        bool mustSuspend = previous_ && std::abs(current - *previous_) < threshold_;
        previous_ = current;
        return Awaiter(mustSuspend);
    }

private:
    int threshold_;
    std::optional<int> previous_;
};
