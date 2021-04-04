#pragma once

#ifndef server_observer_h__
#define server_observer_h__

#include <functional>
#include <memory>
#include <list>

namespace rpc {

template <typename... Agrs> class observable;

template <typename... Agrs> class observer {
private:
    std::function<void(Agrs...)> subscribe_;
public:
    friend class observable<Agrs...>;
    observer() = delete;
    observer(std::function<void(Agrs...)> subscribe) : subscribe_{subscribe} {}
};

template <typename... Agrs>
using observer_ptr = std::shared_ptr<observer<Agrs...>>;

template <typename... Agrs>
using observer_weak_ptr = std::weak_ptr<observer<Agrs...>>;

template <typename... Agrs> class observable {
private:
    std::list<observer_weak_ptr<Agrs...>> observer_;

    void remove_observers_if_expired() {
        for (auto it = observer_.begin(); it != observer_.end(); ++it) {
            auto spt = it->lock();
            if (!spt) {
                observer_.erase(it);
            }
        }
    }

public:
    void notify(Agrs... args) {
        remove_observers_if_expired();

        for (auto &it : observer_) {
            if (auto spt = it.lock()) {
                spt->subscribe_(std::forward<Agrs>(args)...);
            }
        }
    }
    void subscribe(observer_ptr<Agrs...> ptr) { observer_.push_back(ptr); }
};

} // namespace rpc

#endif // server_observer_h__
