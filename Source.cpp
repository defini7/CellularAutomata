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

	int antPosX = 0;
	int antPosY = 0;
	int antDir = 0;

protected:
	bool OnUserCreate() override
	{
		automata = new CellularAutomata(ScreenWidth(), ScreenHeight());
		mode = new CA_Mode_LanctonsAnt(&antPosX, &antPosY, &antDir);

		/*auto set = [&](int x, int y, string v)
		{
			for (int i = 0; i < v.size(); i++)
				automata->state->set(x + i, y, v[i] == '#');
		};*/

		// Gosper's glider gun
		/*set(60, 45, "........................#............");
		set(60, 46, "......................#.#............");
		set(60, 47, "............##......##............##.");
		set(60, 48, "...........#...#....##............##.");
		set(60, 49, "##........#.....#...##...............");
		set(60, 50, "##........#...#.##....#.#............");
		set(60, 51, "..........#.....#.......#............");
		set(60, 52, "...........#...#.....................");
		set(60, 53, "............##.......................");*/

		// Plane
		/*set(60, 45, "###.###");
		set(60, 46, "#.#.#.#");
		set(60, 47, "###.#.#");
		set(60, 48, "..#.#.#");
		set(60, 49, "###.###");*/

		//automata->UpdateState();

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
				ticks = 0.0f;
			}

			ticks += fDeltaTime;
		}

		if (GetKey(def::Key::LEFT).bHeld) delay -= fDeltaTime;
		if (GetKey(def::Key::RIGHT).bHeld) delay += fDeltaTime;

		delay = clamp(delay, 0.0f, 1.0f);

		if (GetMouse(0).bHeld) automata->UpdateBoth(MouseX(), MouseY(), CellularAutomata::Field::State::ON);
		if (GetMouse(1).bHeld) automata->UpdateBoth(MouseX(), MouseY(), CellularAutomata::Field::State::OFF);

		if (GetMouse(2).bHeld)
		{
			//automata->UpdateBoth(MouseX(), MouseY(), CellularAutomata::Field::State::DYING);
			antPosX = MouseX(); antPosY = MouseY();
		}

		if (GetKey(def::Key::LEFT_CONTROL).bPressed)
		{
			for (int i = 0; i < ScreenWidth(); i++)
				for (int j = 0; j < ScreenHeight(); j++)
					automata->UpdateBoth(i, j, CellularAutomata::Field::State::OFF);
		}

		for (int x = 0; x < ScreenWidth(); x++)
			for (int y = 0; y < ScreenHeight(); y++)
			{
				def::Pixel col;

				if (antPosX == x && antPosY == y)
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

				Draw(x, y, col);
			}

		DrawString(2, 2, "Delay = " + to_string(delay), def::YELLOW);

		return true;
	}
};

int main()
{
	srand(clock());

	AppDemo game;

	game.Construct(160, 100, 8, 8);
	game.Run();

	return 0;
}
