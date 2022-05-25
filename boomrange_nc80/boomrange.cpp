#include "detail.h"

int test_boomrange_80(vector<q53sol_t> q53sols)
{
	int i, j;

	//for (i = 0; i <= 10; i++)
		i = 6;
	//for (j = 0; j <= 31; j++)
		j = 6;
			if (model1(q53sols[0], i, j))
			{
				cout << "w[" << i << "," << j << "]" << endl;
			}


	for (int t = -4; t < 20; ++t)
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










	return 0;
}
/*
W10: ........ ........ ........ A.......
W11: ........ ........ ...a.... ........
W12: ........ ........ ........ ........
W13: ........ ........ ........ ........
W14: ........ ........ ........ ........
W15: ........ ........ ........ ..a.....
W16: ........ ........ ........ ........

��������������


*/
bool model1(q53sol_t  q53sol, int i, int j)
{
	int offset = 4;
	unsigned int w[80] = { 0 }, w2[80];
	unsigned int Q[85] = { 0 }, Q2[85] = { 0 }, iv[5] = { 0 }, iv2[5] = { 0 };

	for (int i = 49; i <= 53; i++)
	{
		Q[Qoffset + i] = q53sol.Q[i - 49];
	}
	for (int i = 37; i <= 52; i++)
	{
		w[i] = q53sol.m[i - 37];
	}

	sha1_me_generalised(w, 37);


	for (int t = 52; t >= 0; --t)
	{
		sha1_step_bw(t, Q, w);
		//sha1_step_bw(t, Q2, w2);
	}
	bool okay = true;
	for (int i = -4; i <= 0; ++i)
		if (Q[Qoffset + i] != Qset1mask[Qoffset + i])
		{
			okay = false;
			std::cout << "Q53 bad: CV incorrect" << std::endl;
			break;
		}

	okay &= verify(0, 15, 0, Q, w);


	for (int i = -4; i <= 0; ++i)
		Q2[Qoffset + i] = Q[Qoffset + i] + dQ[Qoffset + i];

	for (int i = 0; i <= 52; ++i)
	{
		w2[i] = w[i] ^ DV_DW[i];
		sha1_step(i, Q2, w2);
	}
	for (int i = 49; i <= 53; ++i)
		if (Q[Qoffset + i] != Q2[Qoffset + i])
		{
			okay = false;
			for (int j = -4; j <= 53; ++j)
				std::cout << "dQ" << j << "\t:" << std::hex << (Q2[Qoffset + j] - Q[Qoffset + j]) << "\t" << std::hex << dQ[Qoffset + j] << std::dec << std::endl;
			break;
		}




	for (int t = -4; t <= 20; ++t)
	{
		unsigned int Qval = Qset1mask[Qoffset + t];
		if (t - 1 >= -4)
		{
			Qval ^= (Qprevmask[Qoffset + t] & Q[Qoffset + t - 1]);
			Qval ^= (Qprevrmask[Qoffset + t] & rotate_left(Q[Qoffset + t - 1], 30));
		}
		if (t - 2 >= -4)
		{
			Qval ^= (Qprev2rmask[Qoffset + t] & rotate_left(Q[Qoffset + t - 2], 30));
		}
		Qval ^= (Qnextmask[Qoffset + t] & Q[Qoffset + t + 1]);
		if ((Qval ^ Q[Qoffset + t]) & Qcondmask[Qoffset + t])
		{
			cout << "Q" << t << "\t: cond bad: " << hex << setw(8) << setfill('0') << ((Qval^Q[Qoffset + t])&Qcondmask[Qoffset + t]) << dec << endl;

			//return (t);
			okay = false;
		}
	}

	show_massage_bit(w);


	show_Q_bit(Q);
	{

		w[i] = w[i] ^ (1 << j);
		w[i + 1] = w[i + 1] ^ (1 << ((j + 5) % 32));
		w[i + 5] = w[i + 5] ^ (1 << ((j - 2) % 32));
	}


	//show_massage_bit(w);


	sha1_me_generalised(w, 0);

	
	for (int t = 0; t < 80; ++t)
	{
		w2[t] = w[t] ^ DV_DW[t];


	}




	for (int i = -4; i <= 0; ++i)
		Q2[Qoffset + i] = Q[Qoffset + i] + dQ[Qoffset + i];


	int step = i;



	for (int t = 16; t < 80; ++t)
	{
		sha1_step(t, Q, w);
		sha1_step(t, Q2, w2);
	}
	show_massage_bit(w);
	show_Q_bit(Q);

	okay &= verify2(0, 15, 0, Q, w);

	if (okay)

	{
		for (int t = -4; t <= 20; ++t)
		{
			unsigned int Qval = Qset1mask[Qoffset + t];
			if (t - 1 >= -4)
			{
				Qval ^= (Qprevmask[Qoffset + t] & Q[Qoffset + t - 1]);
				Qval ^= (Qprevrmask[Qoffset + t] & rotate_left(Q[Qoffset + t - 1], 30));
			}
			if (t - 2 >= -4)
			{
				Qval ^= (Qprev2rmask[Qoffset + t] & rotate_left(Q[Qoffset + t - 2], 30));
			}
			Qval ^= (Qnextmask[Qoffset + t] & Q[Qoffset + t + 1]);
			if ((Qval ^ Q[Qoffset + t]) & Qcondmask[Qoffset + t])
			{
				//	cout << "Q" << t << "\t: cond bad: " << hex << setw(8) << setfill('0') << ((Qval^Q[Qoffset + t])&Qcondmask[Qoffset + t]) << dec << endl;
					//	pro_hold[t + offset]++;
						//return (-1);
						//okay = false;
			}
		}
	}



	return okay;
}


int show_massage_bit(unsigned int w[80])
{
	int m_bit[32] = { 0 };
	unsigned int  temp;

	for (int i = 0; i <= 79; i++)
	{
		cout << "m" << i << "\t: ";
		temp = w[i];
		for (int j = 0; j < 32; j++)
		{
			m_bit[31 - j] = temp % 2;
			temp = temp / 2;
		}
		for (int j = 0; j < 32; j++)
		{
			if (m_bit[j] == 0)
				cout << "0";
			else {
				cout << 1;
			}
		}
		cout << endl;
	}
	return 0;
}
int show_Q_bit(unsigned int w[80])
{

	int m_bit[32] = { 0 };
	unsigned int  temp;

	for (int i = 0; i < 85; i++)
	{
		cout << "Q" << i - 4 << "\t: ";
		temp = w[i];
		for (int j = 0; j < 32; j++)
		{
			m_bit[31 - j] = temp % 2;
			temp = temp / 2;
		}
		for (int j = 0; j < 32; j++)
		{
			if (m_bit[j] == 0)
				cout << "0";
			else {
				cout << 1;
			}
		}
		cout << endl;
	}
	return 0;
}


bool model4(q53sol_t  q53sol, int wfilp[][2], int wflag[6], int pro_hold[85])
{
	int offset = 4;
	unsigned int w[80] = { 0 }, w2[80];
	unsigned int Q[85] = { 0 }, Q2[85] = { 0 }, iv[5] = { 0 }, iv2[5] = { 0 };

	for (int i = 49; i <= 53; i++)
	{
		Q[Qoffset + i] = q53sol.Q[i - 49];
	}
	for (int i = 37; i <= 52; i++)
	{
		w[i] = q53sol.m[i - 37];
	}

	sha1_me_generalised(w, 37);


	for (int t = 52; t >= 0; --t)
	{
		sha1_step_bw(t, Q, w);
		//sha1_step_bw(t, Q2, w2);
	}
	bool okay = true;
	for (int i = -4; i <= 0; ++i)
		if (Q[Qoffset + i] != Qset1mask[Qoffset + i])
		{
			okay = false;
			std::cout << "Q53 bad: CV incorrect" << std::endl;
			break;
		}

	okay &= verify(0, 15, 0, Q, w);


	for (int i = -4; i <= 0; ++i)
		Q2[Qoffset + i] = Q[Qoffset + i] + dQ[Qoffset + i];

	for (int i = 0; i <= 52; ++i)
	{
		w2[i] = w[i] ^ DV_DW[i];
		sha1_step(i, Q2, w2);
	}
	for (int i = 49; i <= 53; ++i)
		if (Q[Qoffset + i] != Q2[Qoffset + i])
		{
			okay = false;
			for (int j = -4; j <= 53; ++j)
				std::cout << "dQ" << j << "\t:" << std::hex << (Q2[Qoffset + j] - Q[Qoffset + j]) << "\t" << std::hex << dQ[Qoffset + j] << std::dec << std::endl;
			break;
		}




	for (int t = -4; t <= 20; ++t)
	{
		unsigned int Qval = Qset1mask[Qoffset + t];
		if (t - 1 >= -4)
		{
			Qval ^= (Qprevmask[Qoffset + t] & Q[Qoffset + t - 1]);
			Qval ^= (Qprevrmask[Qoffset + t] & rotate_left(Q[Qoffset + t - 1], 30));
		}
		if (t - 2 >= -4)
		{
			Qval ^= (Qprev2rmask[Qoffset + t] & rotate_left(Q[Qoffset + t - 2], 30));
		}
		Qval ^= (Qnextmask[Qoffset + t] & Q[Qoffset + t + 1]);
		if ((Qval ^ Q[Qoffset + t]) & Qcondmask[Qoffset + t])
		{
			cout << "Q" << t << "\t: cond bad: " << hex << setw(8) << setfill('0') << ((Qval^Q[Qoffset + t])&Qcondmask[Qoffset + t]) << dec << endl;

			//return (t);
			okay = false;
		}
	}
	//show_massage_bit(w);
	//show_Q_bit(Q);
	
	int i;
	int j;
	for (int step = 0; step <= 5; step++)
	{
		if (wfilp[step][0] >= 0)
		{
			i = wfilp[step][0];
			j = wfilp[step][1];
			w[i] = w[i] ^ (1 << j);
		}

	}




	sha1_me_generalised(w, 0);
	for (int t = 0; t < 80; ++t)
	{
		w2[t] = w[t] ^ DV_DW[t];


	}




	for (int i = -4; i <= 0; ++i)
		Q2[Qoffset + i] = Q[Qoffset + i] + dQ[Qoffset + i];


	int step = i;



	for (int t = 16; t < 80; ++t)
	{
		sha1_step(t, Q, w);
		sha1_step(t, Q2, w2);
	}

	//show_massage_bit(w);
	//show_Q_bit(Q);
	okay &= verify2(0, 15, 0, Q, w);

	if (okay)
	{
#if 0
		for (int t = -4; t <= 30; ++t)
		{
			unsigned int Qval = Qset1mask[Qoffset + t];
			if (t - 1 >= -4)
			{
				Qval ^= (Qprevmask[Qoffset + t] & Q[Qoffset + t - 1]);
				Qval ^= (Qprevrmask[Qoffset + t] & rotate_left(Q[Qoffset + t - 1], 30));
			}
			if (t - 2 >= -4)
			{
				Qval ^= (Qprev2rmask[Qoffset + t] & rotate_left(Q[Qoffset + t - 2], 30));
			}
			Qval ^= (Qnextmask[Qoffset + t] & Q[Qoffset + t + 1]);
			if ((Qval ^ Q[Qoffset + t]) & Qcondmask[Qoffset + t])
			{
				//	cout << "Q" << t << "\t: cond bad: " << hex << setw(8) << setfill('0') << ((Qval^Q[Qoffset + t])&Qcondmask[Qoffset + t]) << dec << endl;
						pro_hold[t + offset]++;
						//return (-1);
						//okay = false;
			}
		}
#endif

		for (int t = - 4; t <=30; ++t)
		{
			if (0 != (Qcondmask[Qoffset + t] & (
				Q[Qoffset + t]
				^ Qset1mask[Qoffset + t]
				^ (Qprevmask[Qoffset + t] & Q[Qoffset + t - 1])
				^ (Qprevrmask[Qoffset + t] & rotate_left(Q[Qoffset + t - 1], 30))
				^ (Qprev2rmask[Qoffset + t] & rotate_left(Q[Qoffset + t - 2], 30))
				^ (Qnextmask[Qoffset + t] & Q[Qoffset + t + 1])
				)))
			{


					pro_hold[t + offset]++;
	
		

				//std::cerr << "Q_" << t << " does not satisfy conditions!" << std::endl;
				okay = false;
			}
		}













	}

	return okay;
}