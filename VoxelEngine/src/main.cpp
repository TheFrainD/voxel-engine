#include <vepch.h>
#include <Config.h>
#include <Core/Game.h>

class VoxelEngine : public Voxel::Game
{
public:
	VoxelEngine()
	{

	}

	~VoxelEngine()
	{

	}

	virtual void Start() override
	{

	}

	virtual void Update(float deltaTime) override
	{

	}

	virtual void Render() override
	{

	}

	virtual void Destroy() override
	{

	}
};

MAIN()
{
	VoxelEngine* engine = new VoxelEngine();
	engine->Run(1280, 720, "VoxelEngine");
	delete engine;
	return 0;
}