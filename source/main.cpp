#include <core/application.h>
#include <core/components/cube.h>
#include <core/components/camera.h>
#include <core/x-platform/scene.h>

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

    int bullets;

public:
    void Init()
    {
        bullets = 0;
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
        title = new Text("You have picked up: " + String(bullets) + " / 8 pickups");
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

        title->Update();

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
                    delete title;
                    title = new Text("You have picked up: " + String(bullets) + " / 8 pickups");
                    pickups[i]->Hide();
                }
            }
        }
    }
};

int main(int argc, char **argv)
{
    Application application(argc, argv);
    application.AddScene(new City);

    return application.Exec();
}

