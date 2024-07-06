#include <bits/stdc++.h>
using namespace std;
#define MAXSZ 100
void update_cooldown(vector<int>& reg) {
	for (int i = 0; i < 32; i++) {
		reg[i] = max(reg[i] - 1, 0);
	}
}
int main() {
	string s[MAXSZ];//input
	string t;
	map<string, int> mp;//map to map aliases and register name to register values
	if (true) {
		mp["zero"] = 0;
		mp["ra"] = 1;
		mp["sp"] = 2;
		mp["gp"] = 3;
		mp["tp"] = 4;
		mp["t0"] = 5;
		mp["t1"] = 6;
		mp["t2"] = 7;
		mp["s0"] = 8;
		mp["fp"] = 8;
		mp["s1"] = 9;
		mp["a0"] = 10;
		mp["a1"] = 11;
		mp["a2"] = 12;
		mp["a3"] = 13;
		mp["a4"] = 14;
		mp["a5"] = 15;
		mp["a6"] = 16;
		mp["a7"] = 17;
		mp["s2"] = 18;
		mp["s3"] = 19;
		mp["s4"] = 20;
		mp["s5"] = 21;
		mp["s6"] = 22;
		mp["s7"] = 23;
		mp["s8"] = 24;
		mp["s9"] = 25;
		mp["s10"] = 26;
		mp["s11"] = 27;
		mp["t3"] = 28;
		mp["t4"] = 29;
		mp["t5"] = 30;
		mp["t6"] = 31;
	}
	for (int i = 0; i < 32; i++) {
		std::string x_alias = "x" + std::to_string(i);
		mp[x_alias] = i;
	}
	// cout << mp["x31"];
	int curr = 0;

	while (getline(cin, t)) {//taking input
		s[curr] = t;
		curr++;
	}

	//stores how long ago the register was used
	vector<int> register_upd(32, 0);
	vector<int> register_upd_fwd(32, 0);


	int nops[curr + 1] = {0}; //add nops above said lines
	int nops1[curr + 1] = {0};//nops for second case
	for (int i = 0; i < curr; i++) {
		string operation = "";
		int j = 0;

		//extracting parts of the string
		while (s[i][j] != ' ') {
			operation += s[i][j];
			j++;
		}
		string reg1 = "", reg2 = "", reg3 = "";
		j++;
		while (j < s[i].length() && s[i][j] != ' ') {
			reg1 += s[i][j];
			j++;

		}
		reg1.pop_back();
		j++;
		while (j < s[i].length() && s[i][j] != ' ') {
			reg2 += s[i][j];
			j++;
		}
		j++;
		reg2.pop_back();
		while (j < s[i].length() && s[i][j] != ' ') {
			reg3 += s[i][j];
			j++;
		}
		if (reg2.size() >= 4) {
			reg2 = reg2.substr(reg2.length() - 3);
			if (reg2[0] == '(') reg2 = reg2.substr(1);
		}

		//four cases
		if (operation == "lb" || operation == "lh" || operation == "lw" || operation == "ld" || operation == "lbu" || operation == "lhu" || operation == "lwu") {
			if (mp[reg1] == 0) {//check if rd is 0, then no nops required
				update_cooldown(register_upd);
				update_cooldown(register_upd_fwd);
				continue;
			}
			if (register_upd[mp[reg2]] != 0) {
				nops[i] = register_upd[mp[reg2]];
				nops1[i] = register_upd_fwd[mp[reg2]];
				register_upd[mp[reg2]] = 0;
				register_upd_fwd[mp[reg2]] = 0;

			}
			for (int j = 0; j <= nops[i]; j++) {
				update_cooldown(register_upd);//updating the cooldowns based on nops inserted
			}
			for (int j = 0; j <= nops1[i]; j++) {
				update_cooldown(register_upd_fwd);//updating the cooldowns based on nops inserted
			}

			register_upd[mp[reg1]] = 2;// 2 if no fwding
			register_upd_fwd[mp[reg1]] = 1;// 1 if fwding considered
		}
		else if (operation == "lui") {
			update_cooldown(register_upd);
			update_cooldown(register_upd_fwd);
			if (mp[reg1] == 0) continue;
			register_upd[mp[reg1]] = 2;//adding 2 in case of no fwding

		}
		else if (operation == "sd" || operation == "sb" || operation == "sh" || operation == "sw") {
			//s type
			if (register_upd[mp[reg2]] != 0) {//checking if a register needed has been used
				nops[i] = max(register_upd[mp[reg2]], nops[i]);
				nops1[i] = max(register_upd_fwd[mp[reg2]], nops1[i]);
				register_upd[mp[reg2]] = 0;
				register_upd_fwd[mp[reg2]] = 0;
			}
			if (register_upd[mp[reg1]] != 0) {//checking if a register needed has been used
				nops[i] = max(register_upd[mp[reg1]], nops[i]);
				nops1[i] = max(register_upd_fwd[mp[reg1]], nops1[i]);
				register_upd[mp[reg1]] = 0;
				register_upd_fwd[mp[reg1]] = 0;
			}
			for (int j = 0; j <= nops[i]; j++)//updating cooldown
				update_cooldown(register_upd);
			for (int j = 0; j <= nops1[i]; j++) {
				update_cooldown(register_upd_fwd);
			}
			continue;
		}
		else {
			//other types
			//if rd is x0, no need to add nops
			if (mp[reg1] == 0) {
				update_cooldown(register_upd);
				update_cooldown(register_upd_fwd);
				continue;
			}
			if (register_upd[mp[reg2]] != 0) {//checking if rs1 has been used
				nops[i] = max(register_upd[mp[reg2]], nops[i]);
				register_upd[mp[reg2]] = 0;
				nops1[i] = max(register_upd_fwd[mp[reg2]], nops1[i]);
				register_upd_fwd[mp[reg2]] = 0;
			}
			if (register_upd[mp[reg3]] != 0) {//checking if rs2 has been used
				nops[i] = max(register_upd[mp[reg3]], nops[i]);
				register_upd[mp[reg3]] = 0;
				nops1[i] = max(register_upd_fwd[mp[reg3]], nops1[i]);
				register_upd_fwd[mp[reg3]] = 0;

			}
			for (int j = 0; j <= nops[i]; j++)
				update_cooldown(register_upd);
			for (int j = 0; j <= nops1[i]; j++) {
				update_cooldown(register_upd_fwd);
			}
			register_upd[mp[reg1]] = 2;
		}
	}
	//printing case 1
	cout << "case 1: Without data forwarding\n";
	int cycles = 0;
	for (int i = 0; i < curr; i++) {
		while (nops[i]) {
			cycles++;
			cout << "nop\n";
			nops[i]--;
		}
		cycles++;
		cout << s[i] << '\n';
	}
	cout << "Total Cycles: " << cycles + 4;
	cout << '\n';
	//printing case 2
	cout << "case 2: With data forwarding\n";
	cycles = 0;
	for (int i  = 0; i < curr; i++) {
		while (nops1[i]) {
			cout << "nop\n";
			cycles++;
			nops1[i]--;
		}
		cycles++;
		cout << s[i] << "\n";
	}
	cout << "Total Cycles: " << cycles + 4;
}