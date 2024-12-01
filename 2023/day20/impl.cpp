import aoc;

struct modul;
struct module_decl {
	char type;
	std::string_view name;
	std::vector<std::string_view> dest;
};

struct pulse {
	bool high = false;
};

struct modul {
	static inline int pulse_counts[2] = {0, 0};
	std::vector<modul*> dest;
	pulse next_pulse;

	static void reset_pulse_counts() {
		pulse_counts[0] = 0;
		pulse_counts[1] = 0;
	}

	virtual void recv(modul* from, pulse p) {
		pulse_counts[p.high] += 1;
	}

	virtual void send() {
		for (modul* m : dest) {
			m->recv(this, next_pulse);
		}
		for (modul* m : dest) {
			m->send();
		}
	}
};

struct broadcast : modul {
	void recv(modul* m, pulse p) override {
		modul::recv(m, p);
		next_pulse = p;
	}
};

struct flip_flop : modul {
	bool on = false;
	bool ignore = false;

	void recv(modul* m, pulse p) override {
		modul::recv(m, p);
		if (!p.high) {
			on = !on; // flips between on and off
			next_pulse = {!on};
			ignore = false;
		} else {
			ignore = true;
		}
	}
	void send() override {
		if (!ignore)
			modul::send();
	}
};

struct conjunction : modul {
	std::unordered_map<modul*, pulse> last_pulse;

	void recv(modul* from, pulse p) override {
		modul::recv(from, p);
		last_pulse[from] = p;
	}
	void send() override {
		auto const count_highs = std::ranges::count(last_pulse | std::views::values, true, &pulse::high);
		if (count_highs == 0) // all lows
			next_pulse.high = false;
		else if (count_highs == last_pulse.size()) // all highs
			next_pulse.high = false;

		modul::send();
	}
};

modul* build_module_graph(auto const& input) {
	static std::vector<std::unique_ptr<modul>> modules;
	static std::unordered_map<std::string_view, int> map;

	if (modules.size() != input.size()) {
		map.clear();
		modules.clear();

		modules.resize(input.size());

		for (int curr_module = 0; module_decl const& md : input) {
			std::unique_ptr<modul>& m = modules[curr_module];
			map[md.name] = curr_module;

			switch (md.type) {
			case 0: m = std::make_unique<broadcast>(); break;
			case 1: m = std::make_unique<flip_flop>(); break;
			case 2: m = std::make_unique<conjunction>(); break;
			default: throw;
			}

			curr_module += 1;
		}

		for (int curr_module = 0; module_decl const& md : input) {
			std::unique_ptr<modul>& m = modules[map[md.name]];
			for (std::string_view name : md.dest) {
				m->dest.push_back(modules[map[name]].get());
			}
		}
	}

	return modules[map["broadcaster"]].get();
}

constexpr auto part1(auto const& input) {
	modul* broadcaster = build_module_graph(input);
	for (int i = 0; i < 1; i++) {
		broadcaster->recv(nullptr, {});
		broadcaster->send();
	}

	int lows = modul::pulse_counts[0];
	int highs = modul::pulse_counts[1];
	return 1000 * (lows + highs);
}

constexpr auto part2(auto const& input) {
	return 0;
}
