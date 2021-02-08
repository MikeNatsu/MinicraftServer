#ifndef MCPLUS_EVENT_HEADER
#define MCPLUS_EVENT_HEADER

#include "MinicraftDef.h"

#include <functional>

namespace mcplus {

    class Event {};

    using EventListener = std::function<void(Event& event)>;

    class PlayerEvent : Event {
        PlayerId player;
    public:
        PlayerEvent(PlayerId player);

        const PlayerId getPlayer() const;
        PlayerId getPlayer();
    };

    class PlayerJoinEvent : PlayerEvent {
    public:
        PlayerJoinEvent(PlayerId player);
    };

    class PlayerQuitEvent : PlayerEvent {
    public:
        PlayerQuitEvent(PlayerId player);
    };

};

#endif // MCPLUS_EVENT_HEADER