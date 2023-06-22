#define DGE_APPLICATION
#include "defGameEngine.h"

#include "CellularAutomata.h"

#include <algorithm>

using namespace std;

class AppDemo : public def::GameEngine
{
public:
	AppDemo()
	{
		SetTitle("Cellular Automata");
	}

	~AppDemo()
	{
		delete automata;
		delete mode;
	}

protected:
	float ticks = 0.0f;
	float delay = 0.0f;

	CellularAutomata* automata;
	CA_Mode* mode;

	def::vi2d antPos;
	int antDir = 0;

	int epoch = 1;

	int scale = 2;

protected:
	bool OnUserCreate() override
	{
		automata = new CellularAutomata(ScreenWidth() / scale, ScreenHeight() / scale);
		mode = new CA_Mode_LangtonsAnt(&antPos.x, &antPos.y, &antDir);

		/*auto set = [&](int x, int y, string v)
		{
			for (int i = 0; i < v.size(); i++)
				automata->state->set(x + i, y, CellularAutomata::Field::State(v[i] == '#'));
		};*/

		// Gosper's glider gun
		/*set(10, 15, "........................#............");
		set(10, 16, "......................#.#............");
		set(10, 17, "............##......##............##.");
		set(10, 18, "...........#...#....##............##.");
		set(10, 19, "##........#.....#...##...............");
		set(10, 20, "##........#...#.##....#.#............");
		set(10, 21, "..........#.....#.......#............");
		set(10, 22, "...........#...#.....................");
		set(10, 23, "............##.......................");*/

		// Plane
		/*set(60, 45, "###.###");
		set(60, 46, "#.#.#.#");
		set(60, 47, "###.#.#");
		set(60, 48, "..#.#.#");
		set(60, 49, "###.###");*/

		//automata->UpdateState(mode);

		antPos = ScreenSize() / scale / 2;

		epoch = 10000;
		automata->SimulateEpochs(mode, epoch);

		return true;
	}

	bool OnUserUpdate(float fDeltaTime) override
	{
		if (GetKey(def::Key::SPACE).bPressed)
			automata->UpdateState(mode);

		if (GetKey(def::Key::LEFT_SHIFT).bHeld)
		{
			if (ticks >= delay)
			{
				automata->UpdateState(mode);
				epoch++;
				ticks = 0.0f;
			}

			ticks += fDeltaTime;
		}

		if (GetKey(def::Key::LEFT).bHeld) delay -= fDeltaTime;
		if (GetKey(def::Key::RIGHT).bHeld) delay += fDeltaTime;

		delay = clamp(delay, 0.0f, 1.0f);

		if (GetMouse(0).bHeld) automata->UpdateBoth(MouseX() / scale, MouseY() / scale, CellularAutomata::Field::State::ON);
		if (GetMouse(1).bHeld) automata->UpdateBoth(MouseX() / scale, MouseY() / scale, CellularAutomata::Field::State::OFF);

		if (GetMouse(2).bHeld)
		{
			//automata->UpdateBoth(MouseX(), MouseY(), CellularAutomata::Field::State::DYING);
			antPos = GetMouse() / scale;
		}

		if (GetKey(def::Key::LEFT_CONTROL).bPressed)
		{
			for (int i = 0; i < ScreenWidth(); i++)
				for (int j = 0; j < ScreenHeight(); j++)
					automata->UpdateBoth(i, j, CellularAutomata::Field::State::OFF);

			epoch = 1;
		}

		for (int x = 0; x < ScreenWidth(); x++)
			for (int y = 0; y < ScreenHeight(); y++)
			{
				def::Pixel col;

				if (antPos == def::vi2d(x, y))
				{
					col = def::RED;
				}
				else
				{
					switch (automata->output->get(x, y))
					{
					case CellularAutomata::Field::State::ON:	col = def::WHITE;	break;
					case CellularAutomata::Field::State::OFF:	col = def::BLACK;	break;
					case CellularAutomata::Field::State::DYING: col = def::BLUE;	break;
					}
				}

				FillRectangle(x * scale, y * scale, scale, scale, col);
			}

		DrawString(2, 2,  "Delay = " + to_string(delay), def::YELLOW);
		DrawString(2, 12, "Epoch = " + to_string(epoch), def::YELLOW);

		return true;
	}
};

int main()
{
	srand(clock());

	AppDemo game;

	game.Construct(256, 240, 4, 4);
	game.Run();

	return 0;
}
