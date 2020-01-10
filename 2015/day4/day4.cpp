#include <iostream>
#include <string>
#include <thread>

// leftrotate function definition
constexpr uint32_t left_rotate(uint32_t x, uint32_t c) {
	return ((x) << (c)) | ((x) >> (32 - (c)));
}

struct md5_result {
	uint32_t h0, h1, h2, h3;
};

// simplified version from https://gist.github.com/creationix/4710780
md5_result md5(std::string const initial_msg) {
	// r specifies the per-round shift amounts
	constexpr uint32_t r[] = {
		7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
		5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20,
		4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
		6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21 };

	// Use binary integer part of the sines of integers (in radians) as constants
	constexpr uint32_t k[] = {
		0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
		0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
		0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
		0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
		0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
		0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
		0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
		0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
		0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
		0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
		0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
		0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
		0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
		0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
		0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
		0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391 };

	int const new_len = ((((initial_msg.size() + 8) / 64) + 1) * 64) - 8;

	// Message (to prepare)
	std::basic_string<uint8_t> msg{ initial_msg.begin(), initial_msg.end() }; // also appends "0" bits (we alloc also 64 extra bytes...)
	msg.resize(new_len + 64);
	msg[initial_msg.size()] = 128; // write the "1" bit

	uint32_t bits_len = 8 * initial_msg.size(); // note, we append the len
	msg[new_len] = bits_len;

	// Process the message in successive 512-bit chunks:
	// for each 512-bit chunk of message:
	md5_result initial = { 0x67452301,0xefcdab89,0x98badcfe,0x10325476 };
	for (int offset = 0; offset < new_len; offset += (512 / 8)) {

		// break chunk into sixteen 32-bit words w[j], 0 ≤ j ≤ 15
		uint32_t* w = (uint32_t*)(msg.data() + offset);

		// Initialize answer value for this chunk:
		uint32_t a = initial.h0;
		uint32_t b = initial.h1;
		uint32_t c = initial.h2;
		uint32_t d = initial.h3;

		// Main loop:
		for (uint32_t i = 0; i < 64; i++) {
			uint32_t f=0, g=0;

			if (i < 16) {
				f = (b & c) | ((~b) & d);
				g = i;
			}
			else if (i < 32) {
				f = (d & b) | ((~d) & c);
				g = (5 * i + 1) % 16;
			}
			else if (i < 48) {
				f = b ^ c ^ d;
				g = (3 * i + 5) % 16;
			}
			else {
				f = c ^ (b | (~d));
				g = (7 * i) % 16;
			}

			uint32_t temp = d;
			d = c;
			c = b;
			b = b + left_rotate((a + f + k[i] + w[g]), r[i]);
			a = temp;
		}

		// Add this chunk's answer to result so far:
		initial.h0 += a;
		initial.h1 += b;
		initial.h2 += c;
		initial.h3 += d;
	}

	return initial;
}

void print_hash(md5_result r) {
	uint8_t* p;
	p = (uint8_t*)&r.h0;
	printf("%2.2x%2.2x%2.2x%2.2x", p[0], p[1], p[2], p[3]);

	p = (uint8_t*)&r.h1;
	printf("%2.2x%2.2x%2.2x%2.2x", p[0], p[1], p[2], p[3]);

	p = (uint8_t*)&r.h2;
	printf("%2.2x%2.2x%2.2x%2.2x", p[0], p[1], p[2], p[3]);

	p = (uint8_t*)&r.h3;
	printf("%2.2x%2.2x%2.2x%2.2x\n", p[0], p[1], p[2], p[3]);
}

int main() {
	std::string const input{ "bgvyzdsv" };

	int answer = 0;
	while (answer >= 0) {
		//md5_result const r = md5(input + std::to_string(answer));
		md5_result const r = md5(input + std::to_string(answer));
		if ((r.h0 & 0x00F0FFFF)==0) {
			std::cout << "Found answer '" << answer << "', with hash ";
			print_hash(r);
			break;
		}

		answer++;
	}

	return 0;
}
