#include<iostream>
#include<thread>
#include<conio.h>
using namespace std;
using namespace std::chrono_literals;

#define tab "\t"
#define delimiter "\n------------------------------------------\n"

#define MIN_TANK_VOLUME 20
#define MAX_TANK_VOLUME 80

class Tank
{
	const unsigned int VOLUME; // характеристика объекта
	double fuel;               // состояние объекта
public:
	unsigned int get_volume()const
	{
		return VOLUME;
	}
	double get_fuel_level()const
	{
		return fuel;
	}
	void fill(double fuel)
	{
		if (fuel < 0)return;
		if (this->fuel + fuel < VOLUME)this->fuel += fuel;
		else this->fuel = VOLUME;
	}
	Tank(unsigned int volume)
		:VOLUME(volume<MIN_TANK_VOLUME ? MIN_TANK_VOLUME :
			volume>MAX_TANK_VOLUME ? MAX_TANK_VOLUME : volume)
	{
		this->fuel = 0;
		cout << "Tank is ready" << endl;
	}
	~Tank()
	{
		cout << "Tank is over" << endl;
	}
	void info()const
	{
		cout << "Tank volume: " << VOLUME << " liters" << endl;
		cout << "Tank level: " << fuel << " liters" << endl;
	}
};

#define MIN_ENGINE_CONSUMPTION  4
#define MAX_ENGINE_CONSUMPTION 20

class Engine
{
	double consumption;
	double consumption_per_second;
	bool is_started;
public:
	double get_consumption()const
	{
		return consumption;
	}
	double get_consumtion_per_second()const
	{
		return consumption_per_second;
	}
	Engine(double consumption)
		:consumption(consumption<MIN_ENGINE_CONSUMPTION?MIN_ENGINE_CONSUMPTION:
		consumption>MAX_ENGINE_CONSUMPTION?MAX_ENGINE_CONSUMPTION:consumption)
	{
		is_started = false;
		consumption_per_second = consumption * 3e-5;
		cout << "Engine is ready" << endl;
	}
	~Engine()
	{
		cout << "Engine is over" << endl;
	}
	void start()
	{
		is_started = true;
	}
	void stop()
	{
		is_started = false;
	}
	bool started()const
	{
		return is_started;
	}
	void info()const
	{
		cout << "Consuption/100  km: " << consumption << " liters" << endl;
		cout << "Consuption/1sec km: " << consumption_per_second << " liters" << endl;
		cout << "Engine is " << (is_started ? "started" : "stopped") << endl;
	}
};

class Car
{
	Tank tank;
	Engine engine;
	bool driver_inside;
	struct Control
	{
		std::thread panel_thread;
	}threads;
public:
	Car(double consumption, int volume) :engine(consumption), tank(volume)
	{
		driver_inside = false;
		cout << "Your car is to go" << endl;
	}
	~Car()
	{
		cout << "You car is over" << endl;
	}
	void get_in()
	{
		driver_inside = true;
		threads.panel_thread = std::thread(&Car::panel,this);
	}
	void get_out()
	{
		driver_inside = false;
		if (threads.panel_thread.joinable())threads.panel_thread.join();
		system("CLS");
		cout << "You are out of car" << endl;
	}

	void control()
	{
		char key;
		do
		{
			key = _getch();
			switch (key)
			{
			case 13:
				if (driver_inside)get_out();
				else get_in();
				break;
			case 'F':
			case 'f':
				if (driver_inside)
				{
					cout << "Чтобы заправиться выйдите из машины" << endl;
				}
				else
				{
					double fuel;
					cout << "Введите объем топлива: "; cin >> fuel;
					tank.fill(fuel);
				}
				break;
			case 'I':  // Зажигание
			case 'i':
				
			case 27:
				get_out();
				cout << "Good buy;-)" << endl;
				break;
			}
		} while (key != 27);
	}

	void panel()const
	{
		while (driver_inside)
		{
			system("CLS");
			cout << "Engine is " << (engine.started() ? "started" : "stopped") << endl;
			cout << "Fuel level: " << tank.get_fuel_level() << " liters" << endl;
			std::this_thread::sleep_for(500ms);
		}
	}
	void info()const
	{
		engine.info();
		tank.info();

	}
};

//#define TANK_CHECK
//#define ENGINE_CHECK
void main()
{
	setlocale(LC_ALL, "");
#ifdef TANK_CHECK
	Tank tank(3);
	tank.info();
	cout << delimiter << endl;
	tank.fill(10);
	tank.fill(3);
	tank.fill(10);
	tank.info();
#endif // TANK_CHECK

#ifdef ENGINE_CHECK
	Engine engine(8);
	//engine.start();
	engine.info();
#endif // ENGINE_CHECK

	Car bmw(12, 60);
    //bmw.info();
	bmw.control();
}