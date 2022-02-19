#include "Enemy.h"

Enemy::Enemy()
{   
}


Enemy::Enemy(float height,float width,float speed,int id)
{
    mainShape = myobject2D::CreateSquare("enemy", glm::vec3(0, 0, 0), 1, glm::vec3(0.1f, 0.2f, 0.75f), true);
    mainSize = 20.f;
    secSize = 10.f;
    offset = 10.f;
    sensingRadius = 80.f;
    avoidAbility = randUtils::randInterval(1.5f, 3.f);
    maxSpeed = randUtils::randInterval(1.f, 1.5f);
    maxForce = randUtils::randInterval(0.5f, 1.f);
    pos = glm::vec2(randUtils::randInterval(mainSize,width-2*mainSize),randUtils::randInterval(mainSize,height-2*mainSize));
    v = speed * glm::normalize(glm::vec2(width / 2, height / 2) - pos);

    acc = glm::vec2(0, 0);
    this->id = id;
}

Enemy::~Enemy()
{
}

Mesh* Enemy::GetMesh()
{
    return mainShape;
}

glm::vec2 Enemy::GetPos()
{
    return pos;
}

glm::vec2 Enemy::GetSecPos(int select)
{
    if (select == 0) {
        return pos + glm::vec2((mainSize - secSize) / 2, (mainSize - secSize) / 2) + glm::vec2(-offset, -offset);
    }
    else {
        pos + glm::vec2((mainSize - secSize) / 2, (mainSize - secSize) / 2) + glm::vec2(offset, -offset);
    }
}

glm::mat3 Enemy::GetMainMatrix()
{
    glm::mat3 m = glm::mat3(1);
    m *= transform2D::Translate(pos.x, pos.y);
    m *= transform2D::Translate(mainSize / 2, mainSize / 2);
    m *= transform2D::Rotate(angle);
    m *= transform2D::Translate(-mainSize/2, -mainSize/2);
    m *= transform2D::Scale(mainSize, mainSize);

    return m;
}

glm::mat3 Enemy::GetSecMatrix(int select)
{
    float sign = (select == 0) ? -1.f : 1.f;
    glm::mat3 m = glm::mat3(1);
    m *= transform2D::Translate(pos.x, pos.y);
    m *= transform2D::Translate(mainSize / 2, mainSize / 2);
    m *= transform2D::Rotate(angle);
    m *= transform2D::Translate(-mainSize / 2, -mainSize / 2);
    m *= transform2D::Translate(sign * offset, -offset);
    m *= transform2D::Translate((mainSize - secSize) / 2, (mainSize - secSize) / 2);
    m *= transform2D::Scale(secSize, secSize);

    return m;
}

glm::vec4 Enemy::GetHitBox()
{
    glm::vec2 pos = GetSecPos(0);
    return glm::vec4(pos, 3 * secSize, (mainSize + 3 * secSize) / 2);
}

glm::vec2 Enemy::GetMainSize()
{
    return glm::vec2(mainSize,mainSize);
}

glm::vec2 Enemy::GetSecSize()
{
    return glm::vec2(secSize,secSize);
}

void Enemy::UpdatePos()
{
    pos = pos + v;
    glm::vec2 prev_v = v;
    v = v + acc;
    acc *= 0.0f;
    

}

bool Enemy::CheckBoundary(float width,float height)
{
    glm::vec2 next_v = v + acc;
    glm::vec2 next_pos = pos + next_v;
    glm::vec2 left = next_pos + glm::vec2((mainSize - secSize) / 2, (mainSize - secSize) / 2) + glm::vec2(-offset, -offset);
    glm::vec2 right = next_pos + glm::vec2((mainSize - secSize) / 2, (mainSize - secSize) / 2) + glm::vec2(offset, -offset);
    if (left.y <= 0 || right.y<=0) {
        pos.y += 10;
        v.y = -v.y;
        return false;
    }
    if (left.x <= 0) {
        pos.x += 10;
        v.x = -v.x;
        return false;
    }
    if (next_pos.y + mainSize >= height) {
        pos.y -= 10;
        v.y = -v.y;
        return false;
    }
    if (right.x + secSize >= width) {
        pos.x -= 10;
        v.x = -v.x;
        return false;
    }
    return true;
}

bool Enemy::checkProjectiles(std::vector<Projectile>& projectiles)
{
    bool b1, b2, b3;
    int index = 0;
    for (Projectile& p : projectiles) {
        b1 = collision::circleRectCollision(p.GetPos(), p.GetRadius(), this->GetPos(), this->GetMainSize());
        b2 = collision::circleRectCollision(p.GetPos(), p.GetRadius(), this->GetSecPos(0), this->GetSecSize());
        b3 = collision::circleRectCollision(p.GetPos(), p.GetRadius(), this->GetSecPos(1), this->GetSecSize());
        if (b1 || b2 || b3) {
            projectiles.erase(projectiles.begin() + index);
            return true;
        }
        index += 1;
    }
    return false;
}

bool Enemy::checkHitBox(glm::vec3 hitBox)
{
    bool b1, b2, b3;
    b1 = collision::circleRectCollision(glm::vec2(hitBox.x, hitBox.y), hitBox.z, this->GetPos(), this->GetMainSize());
    b2 = collision::circleRectCollision(glm::vec2(hitBox.x, hitBox.y), hitBox.z, this->GetSecPos(0), this->GetSecSize());
    b3 = collision::circleRectCollision(glm::vec2(hitBox.x, hitBox.y), hitBox.z, this->GetSecPos(1), this->GetSecSize());
    return b1 || b2 || b3;
}

void Enemy::SeekPlayer(glm::vec2 pos,std::vector<Enemy> otherEnemies,std::vector<Projectile> projectiles)
{
    glm::vec2 desired = pos - this->pos;
    angle = atan2(desired.y, desired.x) - 3 * PI / 2;
    if (glm::length(desired) > maxSpeed) {
        desired = glm::normalize(desired) * maxSpeed;
    }
    glm::vec2 force = desired - this->v;
    if (glm::length(force) > maxForce) {
        force = glm::normalize(force) * maxForce;
    }
    
    keepDistanceFromOthers(otherEnemies);
    avoidProjectiles(projectiles);
    acc += force;
}

int Enemy::GetID()
{
    return id;
}

void Enemy::keepDistanceFromOthers(std::vector<Enemy> otherEnemies)
{
    glm::vec2 f(0, 0);
    int total = 0;
    for (Enemy& e : otherEnemies) {
        if (e.GetID() != this->id) {
            float d = glm::distance(e.GetPos(), this->GetPos());
            if (d < sensingRadius) {
                glm::vec2 delta = this->GetPos() - e.GetPos();
                f += (1.f / (d * d)) * delta;

                total += 1;
            }
        }
    }
    if (total != 0) {
        f = (1.f / total) * f;
        f = glm::normalize(f) * maxSpeed;

        f = f - this->v;
        if (glm::length(f) > maxForce) {
            f = glm::normalize(f) * maxForce;
        }
        acc += f;
    }
}

void Enemy::avoidProjectiles(std::vector<Projectile> projectiles)
{
    glm::vec2 f(0, 0);
    int total = 0;
    for (Projectile& p : projectiles) {
        float d = glm::distance(p.GetPos(), this->GetPos());
        if (d < sensingRadius) {
            glm::vec2 delta = this->GetPos() - p.GetPos();
            f += (1.f / (d * d)) * delta;
            total += 1;
        }
    }
    if (total != 0) {
        f = (1.f / total) * f;
        f = glm::normalize(f) * avoidAbility *maxSpeed;

        f = f - this->v;
        if (glm::length(f) > maxForce) {
            f = glm::normalize(f) * avoidAbility * maxForce;
        }
        acc += f;
    }
}
