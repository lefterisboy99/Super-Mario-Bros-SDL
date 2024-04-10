#pragma once
#include <string>
#include <vector>
#include <functional>
#include <tilemap_system.h>

namespace engine {

    class Layer {
    public:
        Layer(const std::string& name = "Name");
        void PushNewTileMapOnLayer(Tilemap *tilemap);
        void ForEach(std::function<void(int,int,int)> cb);
        const Tilemap* GetCurrentItem();
        const Tilemap* GetLastItem();

    private:
        std::vector<Tilemap *> tilemaps;
        std::string LayerName;
        unsigned int currentItem = 0;

    };

}
