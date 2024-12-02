#include "factory.hpp"

#include <cstdlib>
#include <functional>
#include <iostream>
#include <list>
#include <string>
#include <unordered_map>
#include <vector>

namespace StronglyCoupled
{
    class MusicApp
    {
    public:
        MusicApp() = default;

        void play(const std::string& track_title)
        {
            // creation of product
            SpotifyService music_service("spotify_user", "rjdaslf276%2", 45);

            // usage of product
            std::optional<Track> track = music_service.get_track(track_title);

            if (track)
            {
                std::cout << "Playing track: ";
                for (const auto& note : *track)
                    std::cout << note << ".";
                std::cout << "|\n";
            }
            else
            {
                std::cout << "Track not found!\n";
            }
        }
    };
}

class MusicApp
{
    std::shared_ptr<MusicServiceCreator> music_service_creator_;

public:
    MusicApp(std::shared_ptr<MusicServiceCreator> music_service_creator)
        : music_service_creator_(music_service_creator)
    {
    }

    void play(const std::string& track_title)
    {
        // creation of the object
        std::unique_ptr<MusicService> music_service = music_service_creator_->create_music_service(); // delegation

        // usage of the object
        std::optional<Track> track = music_service->get_track(track_title);

        if (track)
        {
            std::cout << "Playing track: ";
            for (const auto& note : *track)
                std::cout << note << ".";
            std::cout << "|\n";
        }
        else
        {
            std::cout << "Track not found!\n";
        }
    }
};

// static factory
std::unique_ptr<MusicServiceCreator> create_music_service(const std::string& id)
{
    if (id == "Tidal")
        return std::make_unique<TidalServiceCreator>("tidal_user", "KJH8324d&df");
    else if (id == "Spotify")
        return std::make_unique<SpotifyServiceCreator>("spotify_user", "rjdaslf276%2", 45);
    else if (id == "Filesystem")
        return std::make_unique<FsMusicServiceCreator>();

    throw std::runtime_error("Unknown music service id");
}

class MusicServiceFactory
{
    std::unordered_map<std::string, std::shared_ptr<MusicServiceCreator>> music_service_creators_;
public:
    bool register_creator(const std::string& id, std::shared_ptr<MusicServiceCreator> creator)
    {
        return music_service_creators_.emplace(id, creator).second;
    }

    std::shared_ptr<MusicServiceCreator> create_music_service_creator(const std::string& id)
    {
        auto it = music_service_creators_.find(id);
        if (it != music_service_creators_.end())
            return it->second;
        
        throw std::runtime_error("Unknown music service id");
    }
};

int main()
{
    // MusicApp app(std::make_shared<AppleMusicServiceCreator>("apple_user", "KJH8324d&df", 1500));
    // app.play("Would?");

    MusicServiceFactory music_service_factory;
    music_service_factory.register_creator("Tidal", std::make_shared<TidalServiceCreator>("tidal_user", "KJH8324d&df"));
    music_service_factory.register_creator("Spotify", std::make_shared<SpotifyServiceCreator>("spotify_user", "rjdaslf276%2", 45));
    music_service_factory.register_creator("Filesystem", std::make_shared<FsMusicServiceCreator>());
    music_service_factory.register_creator("AppleMusic", std::make_shared<AppleMusicServiceCreator>("apple_user", "KJH8324d&df", 1500));

    std::string id_from_config = "Spotify";
    MusicApp app(music_service_factory.create_music_service_creator(id_from_config));
    app.play("Would?");
}