#include "TextureHolder.hpp"
#include <iostream>
#include <cassert>

// Member function that loads a resource to a texture
void TextureHolder::load(Textures::ID id, const std::string& filename) {
	std::unique_ptr<sf::Texture> texture(new sf::Texture());
	if (!texture->loadFromFile(filename)) {
		throw std::runtime_error("The texture " + filename + "couldn't be loaded! :: cerr");
	}

	auto inserted = mTextureMap.insert(std::make_pair(id, std::move(texture)));
	assert (inserted.second);
	return;
}

// Member function that retrieves a texture
sf::Texture& TextureHolder::get(Textures::ID id) {
	auto found = mTextureMap.find(id);
	assert(found != mTextureMap.end());

	return *found->second;
}

// Member function that retrieves a READ-ONLY texture
const sf::Texture& TextureHolder::get(Textures::ID id) const {
	auto found = mTextureMap.find(id);
	assert(found != mTextureMap.end());

	return *found->second;
}