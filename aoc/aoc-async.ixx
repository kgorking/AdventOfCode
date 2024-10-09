export module aoc:async;

import :tlssplit;
import std;

struct func_view {
	template <typename Fn>
	explicit func_view(Fn const& x)
		: obj(&x), impl{[](void const* o) {
			  return (*static_cast<Fn const*>(o))();
		  }} {}

	void run() const {
		impl(obj);
	}

private:
	void const* obj;
	void (*impl)(void const*);
};

export namespace kg {

template <typename Fn>
class async_fn {
public:
	async_fn(Fn&& fn, unsigned int num_threads = std::thread::hardware_concurrency()) : fn(std::forward<Fn>(fn)) {
		threads.reserve(num_threads);
		for (unsigned i = 0; i < num_threads; i += 1) {
			threads.emplace_back(&async_fn::run, this);
		}

		while (per_thread_data.count() < num_threads) {
			std::this_thread::yield();
		}
	}

	~async_fn() {
		stop = true;
		per_thread_data.for_each([](tls& tls) {
			tls.flag->wait(true);
			tls.flag->test_and_set();
			tls.flag->notify_one();
		});
	}

	void operator()(auto&& arg) {
		if (data.size() < threads.size()) {
			data.clear();
			per_thread_data.for_each([&](tls& tls) {
				data.push_back(&tls);
			});
		}

		while (true) {
			tls& tls = *data[current_data];
			if (!tls.flag->test()) {
				tls.fv = func_view{[this, arg]() {
					this->fn(arg);
				}};
				tls.flag->test_and_set();
				tls.flag->notify_one();
				break;
			}

			current_data += 1;
			if (current_data == data.size())
				current_data = 0;
		}
	}

private:
	void run() {
		auto& tls = per_thread_data.local();

		std::atomic_flag flag;
		tls.flag = &flag;

		while (true) {
			flag.clear();
			flag.wait(false);

			tls.fv.run();
			if (stop)
				break;
		}

		flag.clear();
		flag.notify_one();
	}

	struct tls {
		std::atomic_flag* flag{};
		func_view fv{[] {}};
	};

	Fn fn;
	int current_data = 0;
	std::vector<tls*> data;
	std::vector<std::jthread> threads;

	static inline kg::tls_split<tls> per_thread_data;
	std::atomic_bool stop = false;
};

// single-producer, multiple-consumer
// Producer returns a std::optional<T> or T*.
// Keeps calling prod() as long as a valid value is returned.
// cons(T) is run on threads with valid values.
template <typename FnCons>
void spmc(auto prod, FnCons&& cons, unsigned int num_threads = std::thread::hardware_concurrency()) {
	async_fn async_cons{std::forward<FnCons>(cons), num_threads};

	auto val = prod();
	while (val) {
		async_cons(*val);
		val = prod();
	}
}

} // namespace kg
