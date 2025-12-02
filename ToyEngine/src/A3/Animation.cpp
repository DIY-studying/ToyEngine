#include "Animation.h"
#include <cmath>
#include <utility>



Animation::Animation()
    :m_sprite((const sf::Texture&)sf::Texture::Texture())
{
}

Animation::Animation(const std::string &name, const sf::Texture &t)
        : Animation(name, t, 1, 0) {}

Animation::Animation(std::string name, const sf::Texture &t, size_t frameCount, size_t speed)
        : m_name(std::move(name)), m_sprite(t), m_frameCount(frameCount), m_currentFrame(0), m_speed(speed) {
    m_size = vec2((float) t.getSize().x / float(frameCount), (float) t.getSize().y);
    m_sprite.setOrigin({ m_size.x / 2.0f, m_size.y / 2.0f });
    m_sprite.setTextureRect(sf::IntRect(sf::Vector2i(std::floor(float(m_currentFrame) * m_size.x), 0 ),sf::Vector2i(m_size.x, m_size.y )));
}

void Animation::update() {
    m_currentFrame++;
    m_sprite.setTextureRect(sf::IntRect(sf::Vector2i(std::floor(float((m_currentFrame/m_speed % m_frameCount)) * m_size.x), 0), sf::Vector2i(m_size.x, m_size.y)));
}

bool Animation::hasEnded() const {
    return (m_currentFrame+1)%(m_speed * m_frameCount )==0;
}

const vec2 &Animation::getSize() const {
    return m_size;
}

const std::string &Animation::getName() const {
    return m_name;
}

sf::Sprite &Animation::getSprite() {
    return m_sprite;
}

