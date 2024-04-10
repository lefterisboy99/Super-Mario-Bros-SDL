#include <layer.h>

namespace engine {
    Layer::Layer(const std::string& name) : LayerName(name) {

    }

    void Layer::PushNewTileMapOnLayer(Tilemap *tilemap) {
        tilemaps.push_back(tilemap);
    }

    void Layer::ForEach(std::function<void(int,int,int)> cb) {
        int index = 0;
        for (Tilemap* t : tilemaps) {
            t->ForEach(cb);
        }
    }

    const Tilemap* Layer::GetCurrentItem() {
        if(currentItem < tilemaps.size()) return tilemaps.at(currentItem++);

        currentItem = 0;
        return tilemaps.at(currentItem++);
    }

    const Tilemap* Layer::GetLastItem() {
        return tilemaps.at(tilemaps.size() - 1);
    }
}