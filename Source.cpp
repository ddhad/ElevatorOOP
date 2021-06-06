#include <SFML/Graphics.hpp>
using namespace std;
const float tx_size = 64;

class Elevator {
public:
	int x, y, lx, rx, ly, ry;
	bool in_motion, direction, door_open, door_inmotion;
	Elevator() {
		x = 5 * tx_size;
		y = 7 * tx_size;
		lx = x + 32;
		ly = y + 64;
		rx = x + 64;
		ry = y + 64;
		in_motion = direction = door_open = door_inmotion = false;
	}
	void go_up(Elevator& e, int& Q, int& L) {
		if (!e.direction) {
			e.y -= tx_size / 4;
			e.ly -= tx_size / 4;
			e.ry -= tx_size / 4;
		}
		if (e.y == tx_size) {
			if (e.direction == 1) {
				Q = 2;
			}
			else {
				e.in_motion = false;
				e.direction = 1;
				L = -2;
				Q = 2;
			}
		}
		else e.in_motion = true;

	}
	void go_down(Elevator& e, int& Q, int& L) {
		if (e.direction) {
			e.y += tx_size / 4;
			e.ly += tx_size / 4;
			e.ry += tx_size / 4;
		}
		if (e.y == tx_size * 7) {
			if (e.direction == 0) {
				Q = 2;
			}
			else {
				e.in_motion = false;
				e.direction = 0;
				L = -2;
				Q = 2;
			}
		}
		else e.in_motion = true;

	}void move_door(Elevator& e, int& Q, int& tim) {
		if (tim > -1) {
			if (tim == 5)
				tim = -1;
			else tim++;
		}
		else if (e.door_open) {
			e.door_inmotion = true;
			e.lx += 4;
			e.rx -= 4;
			if (e.lx == e.x + 32) {
				e.door_open = false;
				Q = -1;

			}
		}
		else {
			e.door_inmotion = true;
			e.lx -= 4;
			e.rx += 4;
			if (e.lx == e.x) {
				e.door_open = true;
				tim = 0;
				Q = 2;

			}
		}
	}
};
class key {
private:
	int K;
	key* next;
public:
	key() { K = -1, next = NULL; }
	void push(int x) {
		key* p = last;
		last = new key;
		last->K = x;
		last->next = NULL;
		if (p != NULL)
			p->next = last;
		else first = last;

	}int pop() {
		int n;
		key* p = NULL;
		if (first) {
			n = first->K;
			p = first;
			first = first->next;
			if (first == NULL)
				last = first;
			delete p;
			return n;
		}
		else return -1;
	}
}*first = NULL, * last = NULL;
int main() {

	int X = 17, Y = 10;
	int x = tx_size * X;
	int y = tx_size * Y;
	sf::RenderWindow window(sf::VideoMode(x, y), "Elevator");
	sf::Texture
		tx_back, tx_up, tx_down, tx_elevator, tx_door, tx_floor, tx_wall,
		tx_left, tx_left_pressed, tx_st, tx_op, tx_st_pressed, tx_op_pressed,
		tx_up_pressed, tx_down_pressed, tx_1, tx_1_pressed,
		tx_2, tx_2_pressed, tx_0, tx_0_pressed;
	tx_back.loadFromFile("images/background64.png"); tx_back.setSmooth(true);
	tx_up.loadFromFile("images/up64.png"); tx_up.setSmooth(true);
	tx_down.loadFromFile("images/down64.png"); tx_down.setSmooth(true);
	tx_elevator.loadFromFile("images/elevator128.png"); tx_elevator.setSmooth(true);
	tx_door.loadFromFile("images/door64x32.png"); tx_door.setSmooth(true);
	tx_floor.loadFromFile("images/floor64.png"); tx_floor.setSmooth(true);
	tx_wall.loadFromFile("images/wall64.png"); tx_wall.setSmooth(true);
	tx_st.loadFromFile("images/st64.png"); tx_st.setSmooth(true);
	tx_st_pressed.loadFromFile("images/st64pressed.png"); tx_st_pressed.setSmooth(true);
	tx_op.loadFromFile("images/op64.png"); tx_op.setSmooth(true);
	tx_op_pressed.loadFromFile("images/op64pressed.png"); tx_op_pressed.setSmooth(true);
	tx_up_pressed.loadFromFile("images/up64pressed.png"); tx_up_pressed.setSmooth(true);
	tx_down_pressed.loadFromFile("images/down64pressed.png"); tx_down_pressed.setSmooth(true);
	tx_1.loadFromFile("images/1.png"); tx_1.setSmooth(true);
	tx_2.loadFromFile("images/2.png"); tx_2.setSmooth(true);
	tx_0.loadFromFile("images/0.png"); tx_0.setSmooth(true);
	tx_1_pressed.loadFromFile("images/1_pressed.png"); tx_1_pressed.setSmooth(true);
	tx_2_pressed.loadFromFile("images/2_pressed.png"); tx_2_pressed.setSmooth(true);
	tx_0_pressed.loadFromFile("images/0_pressed.png"); tx_0_pressed.setSmooth(true);
	tx_left.loadFromFile("images/left64.png"); tx_left.setSmooth(true);
	tx_left_pressed.loadFromFile("images/left64pressed.png"); tx_left_pressed.setSmooth(true);
	sf::Sprite
		sprite_back(tx_back), sprite_up(tx_up),
		sprite_down(tx_down), sprite_elevator(tx_elevator),
		sprite_door_left(tx_door), sprite_door_right(tx_door),
		sprite_floor(tx_floor), sprite_wall(tx_wall),
		sprite_left_pressed(tx_left_pressed), sprite_left(tx_left),
		sprite_st(tx_st), sprite_st_pressed(tx_st_pressed),
		sprite_op(tx_op), sprite_op_pressed(tx_op_pressed),
		sprite_up_pressed(tx_up_pressed), sprite_down_pressed(tx_down_pressed),
		sprite_1(tx_1), sprite_1_pressed(tx_1_pressed),
		sprite_2(tx_2), sprite_2_pressed(tx_2_pressed),
		sprite_0(tx_0), sprite_0_pressed(tx_0_pressed);
	float timer, delay = 0.12;
	bool in, stop = false, buttons[6]; int last_input = 0, queue_size = 0, Q = -1, door_count = -1, but_timer[6];
	for (int i = 0; i < 6; i++) { buttons[i] = false; but_timer[i] = 0; }
	sf::Event event;
	sf::Clock clock;
	Elevator elevator;
	key queue;
	while (window.isOpen()) {
		for (int i = 0; i < 6; i++) {
			if (buttons[i]) {
				but_timer[i]++;
				if (but_timer[i] == 2) {
					buttons[i] = false;
					but_timer[i] = 0;
					if (i == 5) last_input = 9;
				}
			}
		}
		timer = 0;
		in = false;
		while (timer < delay) {
			float time = clock.getElapsedTime().asSeconds();
			clock.restart();
			timer += time;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num0) && !in && last_input != 8) {
				if (stop == false) {
					stop = true;
				}
				else stop = false;
				in = true; buttons[5] = true;
				last_input = 8;
			}
			if (stop == false) {
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && last_input != 1 && !in && last_input != 3 && queue_size < 1) {
					queue.push(1);
					last_input = 1;
					queue_size++;
					in = true;
					buttons[0] = true;
				}if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && !in && last_input != 0 && last_input != 4 && queue_size < 1) {
					queue.push(0);
					last_input = 0;
					queue_size++;
					in = true;
					buttons[1] = true;
				}if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && !in) {
					last_input = 2;
					if (!elevator.in_motion)
						Q = 2;
					in = true;
					buttons[2] = true;
				}if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2) && !in && last_input != 3 && last_input != 1 && queue_size < 1) {
					queue.push(1);
					last_input = 3;
					queue_size++;
					in = true;
					buttons[3] = true;
				}if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1) && !in && last_input != 4 && last_input != 0 && queue_size < 1) {
					queue.push(0);
					last_input = 4;
					queue_size++;
					in = true;
					buttons[4] = true;
				}
			}
			if (window.pollEvent(event)) {
				if (event.type == sf::Event::Closed)
					window.close();
			}
		}

		if (!stop) {
			if (queue_size > 0 && Q == -1) {
				Q = queue.pop();
				queue_size--;
			}
			if (Q == 0) {
				elevator.go_down(elevator, Q, last_input);

			}
			else if (Q == 1) {
				elevator.go_up(elevator, Q, last_input);

			}
			else if (Q == 2) {
				elevator.move_door(elevator, Q, door_count);
			}

		}

		window.clear();
		for (int i = Y; i >= 0; i--) {
			for (int j = X; j >= 0; j--) {
				sprite_back.setPosition(j * tx_size, i * tx_size);
				window.draw(sprite_back);
			}
		}
		for (int i = 0; i < 11; i++) {
			sprite_wall.setPosition(9 * tx_size, i * tx_size);
			window.draw(sprite_wall);
		}
		if (!stop) {
			sprite_st.setPosition(11 * tx_size, 1 * tx_size);
			window.draw(sprite_st);
			sprite_op.setPosition(12 * tx_size, 1 * tx_size);
			window.draw(sprite_op);
		}
		else {
			sprite_st_pressed.setPosition(11 * tx_size, 1 * tx_size);
			window.draw(sprite_st_pressed);
			sprite_op_pressed.setPosition(12 * tx_size, 1 * tx_size);
			window.draw(sprite_op_pressed);
		}
		if (!buttons[5]) {
			sprite_0.setPosition(14 * tx_size, 1 * tx_size);
			window.draw(sprite_0);
		}
		else {
			sprite_0_pressed.setPosition(14 * tx_size, 1 * tx_size);
			window.draw(sprite_0_pressed);

		}if (!buttons[4]) {
			sprite_1.setPosition(14 * tx_size, 5 * tx_size);
			window.draw(sprite_1);
		}
		else {
			sprite_1_pressed.setPosition(14 * tx_size, 5 * tx_size);
			window.draw(sprite_1_pressed);

		}if (!buttons[3]) {
			sprite_2.setPosition(14 * tx_size, 3 * tx_size);
			window.draw(sprite_2);
		}
		else {
			sprite_2_pressed.setPosition(14 * tx_size, 3 * tx_size);
			window.draw(sprite_2_pressed);

		}if (!buttons[2]) {
			sprite_left.setPosition(14 * tx_size, 7 * tx_size);
			window.draw(sprite_left);
		}
		else {
			sprite_left_pressed.setPosition(14 * tx_size, 7 * tx_size);
			window.draw(sprite_left_pressed);
		}
		if (!buttons[0]) {
			sprite_up.setPosition(0, 2 * tx_size);
			window.draw(sprite_up);
		}
		else {
			sprite_up_pressed.setPosition(0, 2 * tx_size);
			window.draw(sprite_up_pressed);
		}
		if (!buttons[1]) {
			sprite_down.setPosition(0, 8 * tx_size);
			window.draw(sprite_down);
		}
		else {
			sprite_down_pressed.setPosition(0, 8 * tx_size);
			window.draw(sprite_down_pressed);
		}

		sprite_elevator.setPosition(elevator.x, elevator.y);
		window.draw(sprite_elevator);
		sprite_door_left.setPosition(elevator.lx, elevator.ly);
		window.draw(sprite_door_left);
		sprite_door_right.setPosition(elevator.rx, elevator.ry);
		window.draw(sprite_door_right);

		for (int i = 1; i < 5; i++) {
			sprite_floor.setPosition(i * tx_size, 8 * tx_size);
			window.draw(sprite_floor);
		}for (int i = 1; i < 5; i++) {
			sprite_floor.setPosition(i * tx_size, 2 * tx_size);
			window.draw(sprite_floor);
		}
		window.display();
	}
	window.close();

}