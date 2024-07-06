#include<bits/stdc++.h>
using namespace std;

//to get the rd register value
void compute_rd(vector<int> bin, int* rd){

	int pow = 1;
	int rd_val=0;
	for(int i = 7; i <= 11; i++){
		rd_val += bin[i]*pow;
		pow *= 2;
	}
	*rd = rd_val;
}
//to get the immediate value when there is a J type input
void compute_immJ(vector<int> bin, int* immJ){
	int pow = 4096;
	int immJ_val = 0;
	for(int i = 12; i <= 19; i++){
		immJ_val += pow*bin[i];
		pow *= 2;
	}
	immJ_val += 2048*bin[20];
	pow = 2;
	for(int i = 21;i <=30; i++){
		immJ_val += pow*bin[i];
		pow*=2;
	}
	immJ_val += bin[31]*(1048576);
	*immJ=immJ_val;
}
//to get the immediate value in an I type input
void compute_immI(vector<int> bin, int* imm){
	int pow = 1;
	int imm_val = 0;
	for(int i = 20; i <= 30; i++){
		imm_val += bin[i]*pow;
		pow*=2;
	}
	imm_val += -bin[31]*pow;
	*imm = imm_val;
}
//to extract funct3 
void compute_funct3(vector<int> bin, int* funct3){
	int pow = 1;
	int funct3_val = 0;
	for(int i = 12; i <= 14; i++){
		funct3_val += bin[i]*pow;
		pow *= 2;
	}
	*funct3 = funct3_val;
}
//to extract funct7
void compute_funct7(vector<int> bin, int* funct7){
	int pow = 1;
	int funct7_val = 0;
	for(int i = 25; i <= 31; i++){
		funct7_val += bin[i]*pow;
		pow *= 2;
	}
	*funct7 = funct7_val;
}
//to extract rs1 register number
void compute_rs1(vector<int> bin, int* rs1){
	int pow = 1;
	int rs1_val=0;
	for(int i = 15; i <= 19; i++){
		rs1_val += bin[i]*pow;
		pow *= 2;
	}
	*rs1 = rs1_val;
}
//to extract rs2 register number
void compute_rs2(vector<int> bin, int* rs2){
	int pow = 1;
	int rs2_val=0;
	for(int i = 20; i <= 24; i++){
		rs2_val += bin[i]*pow;
		pow *= 2;
	}
	*rs2 = rs2_val;
}
int main(void)
{ 
	
	string user_input[]={"3ea002ef"};
	int len = sizeof(user_input)/sizeof(user_input[0]);
	int n=len;
	/*
	For labels
	big stores value of last label, +1
	tags stores the label value associated if label is present, otherwise 0
	*/
	int tags[len]={0};
	int big = 1;
	int current = -1;//current is the current line(from 0) of input
	int n1 = len;
	while(n1--){
		current++;
		string input="";
		for(int i=0;i<8;i++){
			input += user_input[current][i];
		}

		long long deci_input = 0LL;

		reverse(input.begin(),input.end());//reversing input to easily change into decimal and then binary
		long long mult=1LL;//mult is the corr. 16 power

		for(auto x:input){
			if(x>=97){
				deci_input+=mult*(x-87);
			}
			else{
				deci_input+=mult*(x-'0');
			}
			mult*=16;
		}
		vector<int> binary;//a vector to store binary form, each element will be 0 or 1

		while(deci_input!=0){
			binary.push_back(deci_input%2);
			deci_input/=2;
		}	
		while(binary.size()!=32){
			binary.push_back(0);
		}
		reverse(binary.begin(),binary.end());

		mult = 1LL;
		int opcode=0;
		reverse(binary.begin(),binary.end());
		//opcode will be the first 7 binary values, convert into decimal
		for(int i = 0; i < 7; i++){
			opcode += binary[i]*mult;
			mult *= 2;
		}
		int rd = 0, funct3 = 0, rs1 = 0, rs2 = 0, funct7 = 0;
		//cout<<opcode<<"\n";
		compute_rd(binary, &rd);
		compute_rs1(binary, &rs1);
		compute_rs2(binary, &rs2);
		compute_funct3(binary, &funct3);
		compute_funct7(binary, &funct7);

		string ans="";
		int immI=0;
		compute_immI(binary, &immI);
		/*for immediate value in S type*/

		int immS_1 = rd;
		int immS_2 = funct7;
		int immS = immS_1 + immS_2*32;

		int immB_1 = rd;
		int immB_2 = funct7; 
		int immB = 0;
		
		//cout<<funct3<<"\n";
		int immJ = 0;
		
		reverse(input.begin(),input.end());
		int label_num=0;//to store label number
		int target=0;//to store the line of input that will be jumped to
		compute_immJ(binary, &immJ);
		switch(opcode){
			case 51://R format
				if(funct3 == 0){
					if(funct7 == 32){
						ans+="sub ";
					}
					else{
						ans+="add ";
					}
				}
				else if(funct3 == 1){
					ans+="sll ";
				}
				else if(funct3 == 2){
					ans+="slt ";
				}
				else if(funct3 == 3){
					ans+="sltu ";
				}
				else if(funct3 == 4){
					ans+="xor ";
				}
				else if(funct3 == 5){
					if(funct7 == 32){
						ans+="sra ";
					}
					else{
						ans+="srl ";
					}
				}
				else if(funct3 == 6){
					ans+="or ";
				}
				else{
					ans+="and ";
				}

				ans+="x"+to_string(rd)+", x"+to_string(rs1)+", x"+to_string(rs2);
				//cout <<"x" << rd <<", x" << rs1 << ", x" << rs2 <<"\n";

				break;
			case 19:// I format
				if(funct3 == 0){
					ans+="addi ";
				}
				else if(funct3 == 1){
					ans+="slli ";
				}
				else if(funct3 == 2){
					ans+="slti ";
				}
				else if(funct3 == 3){
					ans+="sltiu ";
				}
				else if(funct3 == 4){
					ans+="xori ";
				}
				else if(funct3 == 5){
					funct7-=binary[25];
					if(funct7 == 0){
						ans+="srli ";
					}
					else{
						immI-=1024;
						ans+="srai ";
					}
				}
				else if(funct3 == 6){
					ans+="ori ";
				}
				else{
					ans+="andi ";
				}
				ans+="x"+to_string(rd)+", x"+to_string(rs1)+", "+to_string(immI);
				break;
			case 3://I format
				if(funct3 == 0){
					ans+="lb ";
				}
				else if(funct3 == 1){
					ans+="lh ";
				}
				else if(funct3 == 2){
					ans+="lw ";
				}
				else if(funct3 == 3){
					ans+="ld ";
				}
				else if(funct3 == 4){
					ans+="lbu ";
				}
				else if(funct3 == 5){
					ans+="lhu ";
				}
				else if(funct3 == 6){
					ans+="lwu ";
				}
				ans+="x"+to_string(rd)+", "+to_string(immI)+"(x"+to_string(rs1)+")";
				break;
			case 103://I format
				ans+="jalr ";
				ans+="x"+to_string(rd)+", "+to_string(immI)+"(x"+to_string(rs1)+")";
				break;
			case 35: //S format
				if(funct3 == 0){
					ans+="sb ";
				}
				else if(funct3 == 1){
					ans+="sh ";
				}
				else if(funct3 == 2){
					ans+="sw ";
				}
				else if(funct3 == 3){
					ans+="sd ";
				}
				if(immS>=2048){
					immS-=4096;
				}
				ans+="x"+to_string(rs2)+", "+to_string(immS)+"(x"+to_string(rs1)+")";
				break;
			case 99://B format
				if(immB_1%2){
					immB_1--;
				}
				if(immB_2&64){
					immB_2-=64;
				}
				immB = immB_1 + immB_2*32 + 4096*binary[31] + 2048*binary[7];
				if(funct3 == 0){
					ans+="beq ";
				}
				else if(funct3 == 1){
					ans+="bne ";
				}
				else if(funct3 == 4){
					ans+="blt ";
				}
				else if(funct3 == 5){
					ans+="bge ";
				}
				else if(funct3 == 6){
					ans+="bltu ";
				}
				else if(funct3 == 7){
					ans+="bgeu ";
				}

				if(immB>4096){
					immB-=8192;
				}
				target = current + immB/4;
				if(target<n){
					if(tags[target]){

						label_num = tags[target];
					}
					else{
						label_num = big;
						big++;
						tags[target] = label_num;
					}
					ans+="x"+to_string(rs1)+", x"+to_string(rs2)+", L"+to_string(label_num);
				}
				else
				ans+="x"+to_string(rs1)+", x"+to_string(rs2)+", "+to_string(immB);
				break;
			case 111://J format
				ans+="jal ";
				target = current + immJ/4;
				if(target<n){
					if(tags[target]){
						label_num = tags[target];
					}
					else{
						label_num = big;
						big++;
						tags[target] = label_num;
					}
					ans+="x"+to_string(rd)+", L"+to_string(label_num);
				}
				else{
					ans+="x"+to_string(rd)+", "+to_string(immJ);
				}
				
				break;
			case 55://U format
				ans+="lui x"+to_string(rd)+", 0x"+input.substr(0,5);
				break;
			default:
				cout<<"Invalid";
			
		}
		//output
		if(tags[current]){
			cout<<"L"<<tags[current]<<": ";
		}
		cout<<ans<<"\n";
	}
}	