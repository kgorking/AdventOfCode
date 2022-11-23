#include <array>
#include <bitset>
#include <cassert>
#include <iostream>
#include <string_view>
#include <variant>
#include <vector>

constexpr std::string_view input{
#include "input.txt"
};

using packets = std::vector<struct packet>;
struct packet {
	uint8_t version;
	uint8_t type_id;

	// Contains a literal or more sub-packets
	std::variant<uint32_t, packets> data;

	bool has_literal() const {
		return data.index() == 0;
	}
	bool has_subpackets() const {
		return data.index() == 1;
	}
	uint32_t get_literal() const {
		assert(data.index() == 0);
		return std::get<0>(data);
	}
	packets const& get_subpackets() const {
		assert(data.index() == 1);
		return std::get<1>(data);
	}
};

struct bitreader_from_hexstream {
	bitreader_from_hexstream(std::string_view stream) : hexstream(stream) {
		total_bits_size = 4 * hexstream.size();
		next_hex();
	}

	bitreader_from_hexstream(bitreader_from_hexstream const& stream, size_t max_bits)
		: hexstream(stream.hexstream), total_bits_read(stream.total_bits_read), total_bits_size(max_bits), current_hex(stream.current_hex),
		  bits_read(stream.bits_read) {}

	bool done() const {
		assert(total_bits_read <= total_bits_size);
		return total_bits_read == total_bits_size;
	}

	bool can_read_packet() const {
		return (total_bits_read + 3 + 3 + 5) <= total_bits_size; // 5 = smallest literal size
		// return (total_bits_read + 3 + 3 + 1 + 11) <= total_bits_size; // 11 = smallest packet size
	}

	template <typename T = uint32_t>
	T read_next(uint8_t n) {
		assert(n > 0);
		assert(sizeof(T) * 8 > n);

		constexpr uint8_t hex_size = 8;

		size_t val = current_hex;
		while (n + bits_read > hex_size) {
			val <<= hex_size - bits_read;
			n -= hex_size - bits_read;

			next_hex();
			val |= current_hex;
		}

		val >>= (hex_size - n);
		current_hex <<= n;

		bits_read += n;
		total_bits_read += n;

		assert(val < std::numeric_limits<T>::max());
		return static_cast<T>(val);
	}

	size_t get_total_bits_read() const {
		return total_bits_read;
	}

	bool read_bool() {
		return read_next(1) > 0;
	}

	unsigned long read_literal_value() {
		bool more = read_bool();
		unsigned long value = read_next(4);

		while (more) {
			value <<= 4;
			more = read_bool();
			value |= read_next(4);
		}

		return value;
	}

	packet read_packet() {
		packet sp;
		sp.version = read_next<uint8_t>(3);
		sp.type_id = read_next<uint8_t>(3);

		if (sp.type_id == 4) {
			sp.data = read_literal_value();
		} else {
			std::vector<packet> subpackets;

			auto const length_type_id = read_next(1);
			if (length_type_id == 0) {
				auto const subpacket_bits = total_bits_read + read_next(15);

				bitreader_from_hexstream substream{*this, subpacket_bits};
				while (substream.can_read_packet()) {
					subpackets.push_back(substream.read_packet());
				}

				*this = bitreader_from_hexstream{substream, total_bits_size};
			} else {
				auto subpacket_count = read_next(11);
				while (subpacket_count-- != 0) {
					subpackets.push_back(read_packet());
				}
			}

			sp.data = subpackets;
		}
		return sp;
	}

private:
	uint8_t hex2dec(char c) {
		if (c >= 'A') {
			return 10 + c - 'A';
		} else {
			return c - '0';
		}
	}

	void next_hex() {
		assert(!hexstream.empty());
		current_hex = hex2dec(hexstream[0]) << 4;
		current_hex |= hex2dec(hexstream[1]);
		hexstream.remove_prefix(2);
		bits_read = 0;
	}

	std::string_view hexstream{};
	size_t total_bits_read = 0;
	size_t total_bits_size = 0;
	uint8_t current_hex = 0;
	uint8_t bits_read = 0;
};

int accumulate_version_numbers(packet const& pack) {
	int subversion = pack.version;

	if (pack.has_subpackets()) {
		for (packet const& subpack : pack.get_subpackets()) {
			subversion += accumulate_version_numbers(subpack);
		}
	}

	return subversion;
}

void verify() {
	{ // check bit decoder
		// 110100101111111000101000
		bitreader_from_hexstream bithex{"D2FE28"};
		auto const version = bithex.read_next(3);
		assert(version == 0b110);
		auto const type_id = bithex.read_next(3);
		assert(type_id == 0b100);
		auto const val = bithex.read_next(15);
		assert(val == 0b101111111000101);
	}

	{ // check bit decoder
		bitreader_from_hexstream bithex{"38006F45291200"};
		auto const version = bithex.read_next(3);
		assert(version == 1);
		auto const type_id = bithex.read_next(3);
		assert(type_id == 6);
		auto const lengt_id = bithex.read_next(1);
		assert(lengt_id == 0);
		auto const lengt_bits = bithex.read_next(15);
		assert(lengt_bits == 27);
	}

	{ // check bitreader
		bitreader_from_hexstream bithex{"D2FE28"};
		packet const pack = bithex.read_packet();
		assert(0 == pack.data.index());
		assert(2021 == pack.get_literal());
	}

	{ // check bit length type
		bitreader_from_hexstream bithex{"38006F45291200"};
		packet const pack = bithex.read_packet();
		assert(1 == pack.data.index());

		packets const& p = std::get<packets>(pack.data);
		assert(2 == p.size());

		assert(0 == p[0].data.index());
		assert(10 == p[0].get_literal());

		assert(0 == p[1].data.index());
		assert(20 == p[1].get_literal());
	}

	{ // check count length type
		bitreader_from_hexstream bithex{"EE00D40C823060"};
		packet const pack = bithex.read_packet();
		assert(1 == pack.data.index());

		packets const& p = std::get<packets>(pack.data);
		assert(3 == p.size());

		assert(0 == p[0].data.index());
		assert(0 == p[1].data.index());
		assert(0 == p[2].data.index());
		assert(1 == std::get<uint32_t>(p[0].data));
		assert(2 == std::get<uint32_t>(p[1].data));
		assert(3 == std::get<uint32_t>(p[2].data));
	}

	{ // check example 1
		bitreader_from_hexstream bithex{"8A004A801A8002F478"};
		packet pack = bithex.read_packet();

		auto const acc_versions = accumulate_version_numbers(pack);
		assert(16 == acc_versions);

		assert(4 == pack.version);
		assert(1 == pack.data.index());

		packets p = pack.get_subpackets();
		assert(1 == p.size());
		assert(1 == p[0].data.index());

		pack = p[0];
		p = std::get<packets>(pack.data);
		assert(1 == pack.version);
		assert(1 == p.size());
		assert(1 == p[0].data.index());

		pack = p[0];
		p = std::get<packets>(pack.data);
		assert(5 == pack.version);
		assert(1 == p.size());
		assert(0 == p[0].data.index());
	}

	{ // check example 2
		bitreader_from_hexstream bithex{"620080001611562C8802118E34"};
		packet pack = bithex.read_packet();
		auto const acc_versions = accumulate_version_numbers(pack);
		assert(12 == acc_versions);

		assert(3 == pack.version);
		assert(1 == pack.data.index());

		// 2 subpackets
		packets const& p = pack.get_subpackets();
		assert(2 == p.size());

		packets const& p1 = std::get<packets>(p[0].data);
		assert(2 == p1.size());
		assert(0 == p1[0].data.index());
		assert(0 == p1[1].data.index());

		packets const& p2 = std::get<packets>(p[1].data);
		assert(2 == p2.size());
		assert(0 == p2[0].data.index());
		assert(0 == p2[1].data.index());
	}

	{ // check example 3
		bitreader_from_hexstream bithex{"C0015000016115A2E0802F182340"};
		packet pack = bithex.read_packet();
		auto const acc_versions = accumulate_version_numbers(pack);
		assert(23 == acc_versions);

		assert(1 == pack.data.index());

		// 2 subpackets
		packets const& p = std::get<packets>(pack.data);
		assert(2 == p.size());
		assert(1 == p[0].data.index());
		assert(1 == p[1].data.index());

		packets const& p1 = std::get<packets>(p[0].data);
		assert(2 == p1.size());
		assert(0 == p1[0].data.index());
		assert(0 == p1[1].data.index());

		packets const& p2 = std::get<packets>(p[1].data);
		assert(2 == p2.size());
		assert(0 == p2[0].data.index());
		assert(0 == p2[1].data.index());
	}
}

int main() {
	verify();

	// Part 1
	bitreader_from_hexstream bithex{input};
	packet const pack = bithex.read_packet();

	int const result = accumulate_version_numbers(pack);
	std::cout << "Part 1: " << result << '\n';

	// Part 2
}
