export module aoc:tlssplit;
import std;

export namespace kg {
// Provides a thread-local instance of the type T for each thread that
// accesses it. Data is not preserved when threads die.
// This class locks when a thread is created/destroyed.
// The thread_local T's can be accessed through tls_split::for_each.
// Note: Two tls_split<T> can point to the same data by passing the
//	     the same type to the last argument.
template <typename T, typename XX = void>
class tls_split {
	// This struct manages the instances that access the thread-local data.
	// Its lifetime is marked as thread_local, which means that it can live longer than
	// the tls_split<> instance that spawned it.
	struct thread_data {
		// The destructor triggers when a thread dies and the thread_local
		// instance is destroyed
		~thread_data() noexcept {
			if (owner != nullptr) {
				owner->remove_thread(this);
			}
		}

		// Return a reference to an instances local data
		[[nodiscard]] T& get(tls_split* instance) noexcept {
			// If the owner is null, (re-)initialize the instance.
			// Data may still be present if the thread_local instance is still active
			if (owner == nullptr) {
				data = {};
				owner = instance;
				instance->init_thread(this);
			}
			return data;
		}

		void remove(tls_split* instance) noexcept {
			if (owner == instance) {
				data = {};
				owner = nullptr;
				next = nullptr;
			}
		}

		[[nodiscard]] T* get_data() noexcept {
			return &data;
		}
		[[nodiscard]] T const* get_data() const noexcept {
			return &data;
		}

		void set_next(thread_data* ia) noexcept {
			next = ia;
		}
		[[nodiscard]] thread_data* get_next() noexcept {
			return next;
		}
		[[nodiscard]] thread_data const* get_next() const noexcept {
			return next;
		}

	private:
		T data{};
		tls_split* owner{};
		thread_data* next = nullptr;
	};

private:
	// the head of the threads that access this tls_split instance
	inline static thread_data* head{};

	// Mutex for serializing access for adding/removing thread-local instances
	inline static std::shared_mutex mtx;

protected:
	// Adds a thread_data
	void init_thread(thread_data* t) noexcept {
		std::scoped_lock sl(mtx);
		t->set_next(head);
		head = t;
	}

	// Remove the thread_data
	void remove_thread(thread_data* t) noexcept {
		std::scoped_lock sl(mtx);
		// Remove the thread from the linked list
		if (head == t) {
			head = t->get_next();
		} else {
			auto curr = head;
			while (curr->get_next() != nullptr) {
				if (curr->get_next() == t) {
					curr->set_next(t->get_next());
					return;
				} else {
					curr = curr->get_next();
				}
			}
		}
	}

public:
	tls_split() noexcept = default;
	tls_split(tls_split const&) = delete;
	tls_split(tls_split&&) noexcept = default;
	tls_split& operator=(tls_split const&) = delete;
	tls_split& operator=(tls_split&&) noexcept = default;
	~tls_split() noexcept {
		reset();
	}

	// Get the thread-local instance of T
	T& local() noexcept {
		thread_local thread_data var{};
		return var.get(this);
	}

	// Count the number of threads
	unsigned count() const {
		std::scoped_lock sl(mtx);
		unsigned count = 0;
		for (thread_data* thread = head; thread != nullptr; thread = thread->get_next()) {
			count += 1;
		}
		return count;
	}

	// Perform an action on all each instance of the data. Locks.
	template <class Fn>
	void for_each(Fn&& fn) {
		std::scoped_lock sl(mtx);
		for (thread_data* thread = head; thread != nullptr; thread = thread->get_next()) {
			fn(*thread->get_data());
		}
	}

	// Perform an action on all each instance of the data until
	// predicate return true. Locks
	template <class Fn>
	void for_each_until(Fn&& fn) {
		std::scoped_lock sl(mtx);
		for (thread_data* thread = head; thread != nullptr; thread = thread->get_next()) {
			if(fn(*thread->get_data()))
				return;
		}
	}

	// Perform an action on all each instance of the data until
	// predicate return true.
	template <class Fn>
	void unlocked_for_each_until(Fn&& fn) {
		for (thread_data* thread = head; thread != nullptr; thread = thread->get_next()) {
			if(fn(*thread->get_data()))
				return;
		}
	}

	// Resets all data and threads
	void reset() noexcept {
		std::scoped_lock sl(mtx);
		for (thread_data* instance = head; instance != nullptr;) {
			auto next = instance->get_next();
			instance->remove(this);
			instance = next;
		}

		head = nullptr;
	}
};
}