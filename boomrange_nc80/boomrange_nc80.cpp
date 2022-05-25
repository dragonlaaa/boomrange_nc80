// boomrange80.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include "detail.h"
#include <stdlib.h>

int main(int argc, char *argv[])
{
	//   std::cout << "Hello World!\n";

	if (argc > 1)
	{
		int i, j, offset;
		i = atoi(argv[1]);
		//std::cout << i;
		j = atoi(argv[2]);
		offset = atoi(argv[3]);
		//	print2(i, j, offset);
		return 0;
	}
	vector<q53sol_t> q53sols;
	vector<q53sol_t> q53sols_f;

	string a = "H:\\boomrang_nc80\\q53.log";
	cout << a << endl;
	read_new(a, q53sols);
	//sort(q60sols.begin(), q60sols.end());
//	q60sols.erase(unique(q60sols.begin(), q60sols.end()), q60sols.end());
	filtler(q53sols, q53sols_f);


	cout << "size= " << q53sols.size() << endl;
	cout << "size= " << q53sols_f.size() << endl;

	for (int t = -4; t < 30; ++t)
	{
		cout << "Q" << t << "\t: ";
		uint32_t Q1 = Qset1mask[Qoffset + t];
		uint32_t Q2 = Q1 + dQ[Qoffset + t];
		uint32_t XQ = Q1 ^ Q2;
		for (int b = 31; b >= 0; --b)
		{
			if (Qcondmask[Qoffset + t] & (1 << b))
			{
				bool flip = (Qset1mask[Qoffset + t] & (1 << b)) != 0;
				bool prev = (Qprevmask[Qoffset + t] & (1 << b)) != 0;
				bool prevr = (Qprevrmask[Qoffset + t] & (1 << b)) != 0;
				bool prev2r = (Qprev2rmask[Qoffset + t] & (1 << b)) != 0;
				bool next = (Qnextmask[Qoffset + t] & (1 << b)) != 0;
				unsigned cnt = 0;
				if (prev) ++cnt;
				if (prevr) ++cnt;
				if (prev2r) ++cnt;
				if (next) ++cnt;
				if (cnt > 1)
				{
					cout << "cnt>1: " << cnt << " " << prev << prevr << prev2r << next << endl;
				}
				if (cnt == 0)
				{
					if (XQ & (1 << b))
						cout << (flip ? "-" : "+");
					else
						cout << (flip ? "1" : "0");
				}
				if (prev) cout << (!flip ? "^" : "!");
				if (prevr) cout << (!flip ? "r" : "R");
				if (prev2r) cout << (!flip ? "s" : "S");
				if (next) cout << (!flip ? "v" : "Y");
			}
			else
			{
				cout << ".";
			}
		}
		cout << endl;
	}
	//test_boomrange_80( q53sols_f);

	//test_boomrange_80(q60sols_f);
	construtapth_new(q53sols_f);
	return 0;
}

