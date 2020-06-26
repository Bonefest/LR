#ifndef RESOURCESMANAGER_H_INCLUDED
#define RESOURCESMANAGER_H_INCLUDED

#include <map>
#include <string>
#include <fstream>

using std::map;
using std::pair;
using std::string;
using std::ifstream;

template <class Resource>
using ResContainer = map<string, pair<Resource*, uint32_t>>;

template <class Derived, class Resource>
class ResourcesManager {
public:

    ~ResourcesManager() {
        freeResources();
    };

    void loadPathsFromFile(const string& fileName) {
        ifstream file(fileName);
        if(file.is_open()) {
            string line, name, path;

            while(std::getline(file, line)) {
                std::size_t spacePos = line.find(' ');
                if(spacePos == std::string::npos) {
                    continue;
                }

                name = line.substr(0, spacePos);
                path = line.substr(spacePos + 1);

                m_paths.emplace(name, path);
            }

            file.close();
        }
    }

    Resource* get(const string& name) {

        auto resIt = m_resources.find(name);
        if(resIt != m_resources.end()) {
            resIt->second.second++;
            return resIt->second.first;
        }

        auto pathIt = m_paths.find(name);
        if(pathIt == m_paths.end()) {
            return nullptr;
        }

        Resource* res = load(pathIt->second);
        if(res == nullptr) {
            return nullptr;
        }

        m_resources.emplace(name, std::make_pair(res, 1));
        return res;
    }

    void release(const string& name) {
        auto resIt = m_resources.find(name);
        if(resIt != m_resources.end()) {
            resIt->second.second--;
            if(resIt->second.second == 0) {
                delete resIt->second.first;
                m_resources.erase(resIt);
            }
        }
    }

    bool isAvailable(const string& name) const {
        return m_resources.find(name) != m_resources.end();
    }

    Resource* load(const string& name) {
        return static_cast<Derived*>(this)->load(name);
    }

private:
    void freeResources() {
        auto resIt = m_resources.begin();
        while(resIt != m_resources.end()) {
            delete resIt->second.first;
            resIt = m_resources.erase(resIt);
        }
    }

    ResContainer<Resource>   m_resources;
    map<string, string>      m_paths;

};


#include <SFML/Graphics.hpp>
#include <iostream>

class TextureManager: public ResourcesManager<TextureManager, sf::Texture> {
public:
    sf::Texture* load(const string& name) {
        sf::Texture* texture = new sf::Texture();
        if(!texture->loadFromFile(name)) {
            delete texture;
            texture = nullptr;

            std::cerr << "[Error] Cannot load texture '" << name << "'!\n";
        }

        return texture;
    }
};

#endif // RESOURCESMANAGER_H_INCLUDED
