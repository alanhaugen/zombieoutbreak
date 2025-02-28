#include <core/application.h>
#include <core/components/cube.h>
#include <core/components/camera.h>
#include <core/x-platform/scene.h>

int bullets;

class BezierCurve
{
private:
    int d;

public:
    glm::vec3 c[4];

    BezierCurve()
    {
        d = 4;
    }

    // deCasteljau's algorithm for evaluating Bezier curves
    glm::vec3 EvaluateBezier(float t)
    {
        glm::vec3 a[4]; // 4=d+1 for kubisk Bezier for (int i=0; i<4; i++)

        for (int i=0; i < 4; i++)
        {
            a[i] = c[i];
        }

        for (int k = d; k > 0; k--) //for (int k=1; k<=d; k++) {
        {
            for (int i = 0; i < k; i++) //for (int i=0; i<=d=k; i++) a[i] = a[i] * (1=t) + a[i+1] * t;
            {
                a[i] = a[i] * (1 - t) + a[i + 1] * t;
            }
        }

        return a[0];
    }
};


class Enemy : public Cube
{
private:
    bool goRight;
    float tick;
    BezierCurve path;

public:
    Enemy(float x, float y, float z, float length, float width, float height)
        : Cube(x, y, z, length, width, height)
    {
        goRight = true;
        tick = 0.0f;

        path.c[0] = glm::vec3(-6, 0, 0);
        path.c[1] = glm::vec3(0, 4, 0);
        path.c[2] = glm::vec3(3, 4, 0);
        path.c[3] = glm::vec3(6, 0, 0);
    }

    void Update()
    {
        Cube::Update();

        tick += 0.01;

        if (tick > 1.0)
        {
            tick = 0.0;
        }

        *matrix.x = path.EvaluateBezier(tick).x;
        *matrix.y = path.EvaluateBezier(tick).y;
        //*matrix.z = path.EvaluateBezier(tick).z;

        /*if (*matrix.x > 9.0f)
        {
            goRight = false;
        }

        if (*matrix.x < -5.0f)
        {
            goRight = true;
        }

        if (goRight)
        {
            *matrix.x += 0.1;
        }
        else
        {
            *matrix.x -= 0.1;
        }

        *matrix.y += sin(tick) / 10;*/
    }
};

class City : public IScene
{
private:
    Cube* floor;
    Cube* house;
    Cube* door;
    Cube* door2;
    Cube* player;
    Text* title;
    Camera* cam;
    Array<Cube*> pickups;
    Array<Cube*> enemies;
    bool gameOver;

public:
    void Init()
    {
        bullets = 0;
        gameOver = false;
        floor  = new Cube(0, 0, -5, 10, 0.1, 10);
        house  = new Cube(1, 2, -3, 1, 1, 1);
        house->Uniform("colour", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
        door = new Cube(1, 1.0, -3.5, .3f, .5, .1f);
        door2 = new Cube(1, 1.0, -3.5, .3f, .5, .1f);
        door->Uniform("colour", glm::vec4(0.1, 0.0f, 1.0f, 1.0f));
        door2->Uniform("colour", glm::vec4(0.1, 0.0f, 1.0f, 1.0f));
        floor->Uniform("colour", glm::vec4(133 / 255.f, 202 / 255.f, 93 / 255.f, 1.0f));
        player = new Cube(0, 0, -4, 0.1, 0.1, 0.1);
        player->Uniform("colour", glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
        player->tag = "player";
        player->collisionBox->type = "player";
        title = new Text("You have picked up: " + String(bullets) + " / 9 pickups");
        cam = new Camera(glm::vec3(0,0,0), glm::vec3(0,1,0), glm::vec3(0,0.5,-1), 75);

        for (int i = 0; i < 8; i++)
        {
            Cube* pickup = new Cube(5.0f - random.RandomRange(0, 10), 5.0f - random.RandomRange(0, 10), -4, 0.1, 0.1, 0.1);
            pickup->Uniform("colour", glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
            pickup->tag = "pickup";
            pickup->collisionBox->type = "pickup";
            pickups.Add(pickup);
            components.Add(pickup);
        }

        for (int i = 0; i < 2; i++)
        {
            Enemy* enemy = new Enemy(5.0f - random.RandomRange(0, 10), 5.0f - random.RandomRange(0, 10), -4, 0.1, 0.1, 0.1);
            enemy->Uniform("colour", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
            enemy->tag = "pickup";
            enemies.Add(enemy);
            components.Add(enemy);
        }

        components.Add(floor);
        components.Add(player);
        components.Add(cam);
        components.Add(house);
        components.Add(door);
        components.Add(door2);

        door2->matrix.Rotate(M_PI / 2, glm::vec3(0, 0, 1));
    }

    void Update()
    {
        title->Update();

        if (gameOver == true)
        {
            return;
        }

        if (input.Held(input.Key.A))
        {
            *player->matrix.x -= 0.1f;
        }
        if (input.Held(input.Key.D))
        {
            *player->matrix.x += 0.1f;
        }
        if (input.Held(input.Key.W))
        {
            *player->matrix.y += 0.1f;
        }
        if (input.Held(input.Key.S))
        {
            *player->matrix.y -= 0.1f;
        }

        cam->position.x = *player->matrix.x;
        cam->position.y = *player->matrix.y - 2;

        door->matrix.Rotate(0.01, glm::vec3(0, 0, 1));
        door2->matrix.Rotate(0.01, glm::vec3(0, 0, 1));
    }

    void UpdateAfterPhysics()
    {
        for (unsigned int i = 0; i < pickups.Size(); i++)
        {
            if (pickups[i]->isVisible())
            {
                if (physics->Collide(pickups[i]->collisionBox, "player"))
                {
                    bullets++;

                    if (bullets >= 9)
                    {
                        delete title;
                        title = new Text("You Won");
                    }
                    else
                    {
                        delete title;
                        title = new Text("You have picked up: " + String(bullets) + " / 9 pickups");
                    }

                    pickups[i]->Hide();
                }
            }
        }
        for (unsigned int i = 0; i < enemies.Size(); i++)
        {
            if (enemies[i]->isVisible())
            {
                if (physics->Collide(enemies[i]->collisionBox, "player"))
                {
                    delete title;
                    title = new Text("You lost");
                    gameOver = true;
                }
            }
        }

        if (physics->Collide(door->collisionBox, "player"))
        {
            //Application::NextScene();
        }
    }
};

class House : public IScene
{
private:
    Camera* cam;
    Cube* player;
    Text* title;
    Cube* pickup;

public:
    void Init()
    {
        cam = new Camera(glm::vec3(0,0,0), glm::vec3(0,1,0), glm::vec3(0,0.5,-1), 75);
        components.Add(cam);

        player = new Cube(0, 0, -4, 0.1, 0.1, 0.1);
        player->Uniform("colour", glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
        player->tag = "player";
        player->collisionBox->type = "player";
        components.Add(player);

        pickup = new Cube(0,2.5, -4, 0.1, 0.1, 0.1);
        pickup->Uniform("colour", glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
        pickup->tag = "pickup";
        pickup->collisionBox->type = "pickup";
        components.Add(pickup);

        title = new Text("You have picked up: " + String(bullets) + " / 9 pickups");
    }

    void Update()
    {
        if (input.Held(input.Key.A))
        {
            *player->matrix.x -= 0.1f;
        }
        if (input.Held(input.Key.D))
        {
            *player->matrix.x += 0.1f;
        }
        if (input.Held(input.Key.W))
        {
            *player->matrix.y += 0.1f;
        }
        if (input.Held(input.Key.S))
        {
            *player->matrix.y -= 0.1f;
        }

        title->Update();
    }

    void UpdateAfterPhysics()
    {
        if (physics->Collide(pickup->collisionBox, "player") && pickup->isVisible())
        {
            bullets++;

            if (bullets >= 9)
            {
                delete title;
                title = new Text("You Won");
            }
            else
            {
                delete title;
                title = new Text("You have picked up: " + String(bullets) + " / 9 pickups");
            }

            pickup->Hide();
        }
    }
};

class Game : public IScene
{
private:
    IScene* city;
    IScene* house;
    IScene* activeScene;
    bool isInCity;

public:
    void Init()
    {
        city  = new City();
        house = new House();

        city->Init();
        house->Init();

        isInCity = true;
    }

    void Update()
    {
        if (isInCity)
        {
            activeScene = city;
        }
        else
        {
            activeScene = house;
        }

        if (input.Pressed(input.Key.SPACE))
        {
            isInCity = !isInCity;
        }

        // Run the game logic
        activeScene->Update();

        // Update game components
        for (unsigned int i = 0; i < activeScene->components.Size(); i++)
        {
            (*activeScene->components[i])->Update();
        }
    }

    void UpdateAfterPhysics()
    {
        activeScene->UpdateAfterPhysics();

        // Update game components after physics
        for (unsigned int i = 0; i < activeScene->components.Size(); i++)
        {
            (*activeScene->components[i])->UpdateAfterPhysics();
        }
    }
};

int main(int argc, char **argv)
{
    Application application(argc, argv);
    application.AddScene(new Game);

    return application.Exec();
}

