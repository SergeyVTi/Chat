#pragma once

#include <string>
#include <memory>

typedef unsigned int uint;

class AuthData {
	public:
		explicit AuthData(const char* password, size_t msize_bytes);

		~AuthData() = default;
		void setOnline(bool status){
			online_=status;
		}
		bool getOnline(){
			return online_;
		}
		uint* getPasswordHash() const {
			return password_sha1_hash_;
		}
		uint* sha1(const char* message, size_t msize_bytes);


	protected:
		uint cycle_shift_left(uint val, size_t bit_count);
		uint bring_to_human_view(uint val);

		uint* password_sha1_hash_;
		bool online_ {0};
};

