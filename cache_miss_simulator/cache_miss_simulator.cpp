#include<bits/stdc++.h>
using namespace std;
int main() {
	int cache_size;
	int block_size;
	int associativity;
	string repl_policy;
	string write_policy;

	ifstream file("cache.config");

	file >> cache_size >> block_size >> associativity >> repl_policy >> write_policy;
	// cerr<<cache_size<<" "<<block_size<<" "<<associativity<<" "<<repl_policy<<" "<<write_policy<<'\n';
	int index = cache_size / block_size;
	if (associativity == 0) {
		associativity = index;
	}
	index /= associativity;//number of sets

	long long tags[index][associativity];
	queue<int> q[index];
	int recency[index][associativity];
	int valid[index][associativity];
	int dirty[index][associativity];

	for (int i = 0; i < index; i++) {
		for (int j = 0; j < associativity; j++) {
			tags[i][j] = valid[i][j] = recency[i][j] = dirty[i][j] = 0;//initialising values
		}
	}
	int offset = 0;
	for (int i = 0; i < 30; i++) {
		if ((1 << i) == block_size) offset = i;//calculating offset
	}
	file.close();
	ifstream file1("cache.access");
	int time_val = 0;
	srand(time(0));
	while (!file1.eof()) {
		time_val++;
		string input;
		getline(file1, input);
		char type = input[0];//checking if read or write
		// cerr<<input<<'\n';
		input = input.substr(5);
		long long dec_val = 0LL;

		reverse(input.begin(), input.end());
		long long mult = 1LL;

		for (auto x : input) {
			int tmp = x;
			if (tmp >= 65 && tmp < 97) tmp += 32;
			if (tmp < 97)
				dec_val += (tmp - '0') * mult;
			else
				dec_val += (tmp - 'a' + 10) * mult;
			mult *= 16;
		}

		reverse(input.begin(), input.end());
		dec_val = dec_val >> offset;
		int chk = dec_val % index;
		int flag = 0;
		int fnd = 0;

		for (int i = 0; i < associativity; i++) {
			if (valid[chk][i] == 1 && tags[chk][i] == dec_val / index) {
				flag = 1;
				fnd = i;
			}
		}
		if (flag) {
			//we get a hit
			if (repl_policy == "LRU") {
				recency[chk][fnd] = time_val;
			}
			cout << hex << "Address: 0x" << input << ", Set: 0x" << chk << ", Hit, Tag: 0x" << dec_val / index << '\n';

			if (write_policy == "WB" && type == 'W') {
				//need to make dirty bit 1
				dirty[chk][fnd] = 1;
			}

		}
		else {
			//need to add to cache, since we got a miss
			if (write_policy == "WT" && type == 'W') {
				//we directly update the memory, and not the cache, so subsequent calls will be a miss
				cout << hex << "Address: 0x" << input << ", Set: 0x" << chk << ", Miss, Tag: 0x" << dec_val / index << '\n';
				continue;
			}
			int set_val = 0;
			if (repl_policy == "FIFO") {
				if (q[chk].size() != associativity) { //there is space in the cache
					for (int i = 0; i < associativity; i++) {
						if (valid[chk][i] == 0) {
							valid[chk][i] = 1;
							tags[chk][i] = dec_val / index;
							q[chk].push(i);
							if (write_policy == "WB" && type == 'W') {
								//need to make dirty bit 1
								dirty[chk][i] = 1;
							}
							break;
						}
					}
				}
				else {
					//otherwise, there is no space, so we need to replace
					int replace = q[chk].front();
					q[chk].pop();
					tags[chk][replace] = dec_val / index;
					q[chk].push(replace);
					dirty[chk][replace] = 0;
					if (write_policy == "WB" && type == 'W') {
						//if dirty bit is 1, update onto memory
						//need to make dirty bit 1
						dirty[chk][replace] = 1;
					}
				}
			}
			if (repl_policy == "LRU") {
				bool full = 1;
				for (int i = 0; i < associativity; i++) {
					if (valid[chk][i] == 0) {
						valid[chk][i] = 1;
						tags[chk][i] = dec_val / index;
						full = 0;
						recency[chk][i] = time_val;
						if (write_policy == "WB" && type == 'W') {
							//need to make dirty bit 1
							dirty[chk][i] = 1;
						}
						break;
					}
				}
				if (full) {
					//need to replace a block
					int replace = 0;
					for (int i = 0; i < associativity; i++) {
						if (recency[chk][i] < recency[chk][replace]) {
							replace = i;
						}
					}
					tags[chk][replace] = dec_val / index;
					recency[chk][replace] = time_val;
					//if dirty bit is 1, update onto memory
					//make dirty bit 0
					dirty[chk][replace] = 0;
					if (write_policy == "WB" && type == 'W') {
						//need to make dirty bit 1
						dirty[chk][replace] = 1;
					}
				}
			}
			if (repl_policy == "RANDOM") {
				bool full = 1;
				for (int i = 0; i < associativity; i++) {
					if (valid[chk][i] == 0) {
						valid[chk][i] = 1;
						tags[chk][i] = dec_val / index;
						full = 0;
						if (write_policy == "WB" && type == 'W') {
							//need to make dirty bit 1
							dirty[chk][i] = 1;
						}
						break;
					}
				}
				if (full) {
					//need to replace a block
					int replace = rand() % associativity;
					tags[chk][replace] = 1;
					//if dirty bit is 1, update onto memory
					//make dirty bit 0
					dirty[chk][replace] = 0;
					if (write_policy == "WB" && type == 'W') {
						//need to make dirty bit 1
						dirty[chk][replace] = 1;
					}
				}
			}
			cout << hex << "Address: 0x" << input << ", Set: 0x" << chk << ", Miss, Tag: 0x" << dec_val / index << '\n';

		}
	}
}