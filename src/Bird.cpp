
#include "Bird.h"
#include <cmath>
Bird::Bird(b2World& world, const sf::Vector2f& position, const sf::Vector2f& size) : m_dragging{ false }
{
    initPhysicBody(world, position, size);
    initGraphicBody();
}


void Bird::initPhysicBody(b2World& world, const sf::Vector2f& position, const sf::Vector2f& size)
{
    // Create Box2D body definition
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(position.x / SCALE, position.y / SCALE);
    bodyDef.linearDamping = 0.7f;
    bodyDef.userData.pointer = reinterpret_cast<uintptr_t>(this);
    m_body = world.CreateBody(&bodyDef);

    // Create Box2D circle shape
    b2CircleShape shape;
    shape.m_radius = size.x / SCALE;

    // Create Box2D fixture definition
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density = 0.5f;
    fixtureDef.friction = 0.3f;
    fixtureDef.restitution = 0.4f;
    m_body->CreateFixture(&fixtureDef);


}
void Bird::initGraphicBody(const sf::Vector2f& size)
{
    m_bird.setTexture(&GameResources::getInstance().getBirdTexture(0));
    m_bird.setRadius(size.x);
    m_bird.setOrigin(size.x, size.x);
    m_bird.setPosition(sf::Vector2f(m_body->GetPosition().x * SCALE, m_body->GetPosition().y * SCALE));
}

void Bird::objectUpdate()
{
    b2Vec2 position = m_body->GetPosition();
    float angle = m_body->GetAngle();
    m_bird.setPosition(position.x * SCALE, position.y * SCALE);
    m_bird.setRotation(angle * 180.0f / b2_pi);
}   


void Bird::applyForce(const sf::Vector2f& force)
{
    // Finished the drag
    m_dragging = false;
    // Apply impulse force to the Box2D body
    b2Vec2 forceScaled{ force.x / SCALE, force.y / SCALE };
    //temp.Normalize();
    m_body->SetLinearVelocity(forceScaled);
    m_body->ApplyLinearImpulse(forceScaled , m_body->GetWorldCenter() , true);

}


void Bird::drawObject(sf::RenderWindow& window)
{
    //first is updating
    this->objectUpdate();
    window.draw(m_bird);
}

void Bird::handleThrow(const float x, const float y)
{
    if (m_bird.getGlobalBounds().contains(sf::Vector2f(x, y)))
    {
        m_dragging = true;
        dragStartPosition = sf::Vector2f(x, y);
    }
}
void Bird::setRangeVector(const sf::Vector2i& mouseLocation, sf::RenderWindow& w)
{
    sf::VertexArray line(sf::Lines, 2);
    line[0] = sf::Vertex(dragStartPosition, sf::Color::Green);
    line[1] = sf::Vertex(dragEndPosition, sf::Color::Green);
    w.draw(line);
    
    sf::Vector2f mouseLastLocation(mouseLocation.x, mouseLocation.y);
    float distance = std::sqrt(std::pow(dragStartPosition.x - mouseLastLocation.x, 2) + std::pow(dragStartPosition.y - mouseLastLocation.y, 2));

    if (distance > MAX_DISTANCE) {
        sf::Vector2f direction = mouseLastLocation - dragStartPosition;
        direction /= distance;
        dragEndPosition = dragStartPosition + direction * MAX_DISTANCE;
        m_bird.setPosition(dragEndPosition);
        m_body->SetTransform(b2Vec2(dragEndPosition.x / SCALE, dragEndPosition.y / SCALE), (dragStartPosition.y - dragEndPosition.y) / SCALE);
    }
    else {
        m_bird.setPosition(mouseLocation.x, mouseLocation.y);
        m_body->SetTransform(b2Vec2(mouseLocation.x / SCALE, mouseLocation.y / SCALE), (dragStartPosition.y - mouseLocation.y) / SCALE);
        dragEndPosition.x = mouseLocation.x;
        dragEndPosition.y = mouseLocation.y;
    }

}
sf::Vector2f Bird::calculateThrow()
{
    return sf::Vector2f(dragStartPosition - dragEndPosition) * 1.3f;
}

void Bird::setPosition(const sf::Vector2f& pos) {

    b2Vec2 temp{ pos.x / SCALE , pos.y / SCALE };
    m_body->SetTransform(temp, 0.f);
    m_bird.setPosition(pos);
}

void Bird::handleEvent(sf::Event& event,const sf::Vector2f & mouse) 
{
    switch (event.type) {

    case sf::Event::MouseButtonPressed:
        if (event.mouseButton.button == sf::Mouse::Left) {
            this->handleThrow(mouse.x, mouse.y);
            break;

        }

    case sf::Event::MouseButtonReleased:
        if (event.mouseButton.button == sf::Mouse::Left && isDragged()) {
            sf::Vector2f force = this->calculateThrow();
            this->applyForce(force);
        }

        break;


    }
}

sf::Vector2f Bird::getPosition() const
{
    return m_bird.getPosition();
}




//################################################
/* OLDER SETRANGE

void Bird::setRangeVector(const sf::Vector2i& mouseLocation, sf::RenderWindow& w)
{
    sf::VertexArray line(sf::Lines, 2);
    line[0] = sf::Vertex(dragStartPosition, sf::Color::Green);
    line[1] = sf::Vertex(dragEndPosition, sf::Color::Green);
    w.draw(line);
    m_bird.setPosition(mouseLocation.x, mouseLocation.y);
    m_body->SetTransform(b2Vec2(mouseLocation.x / SCALE, mouseLocation.y / SCALE),  (dragStartPosition.y-mouseLocation.y) / SCALE);
    dragEndPosition.x = mouseLocation.x;
    dragEndPosition.y = mouseLocation.y;
}
*/