#include "detail.h"
using namespace std;
void getline(std::istream& ifs)
{
	char c = 0;
	while (ifs && c != '\n' && c != '\r')
		ifs.get(c);
	while (ifs && (c == '\n' || c == '\r'))
		ifs.get(c);
	if (ifs)
		ifs.putback(c);
}
int read_new(string& a, vector<q53sol_t> & q53sol_ts)
{
	ifstream ifs(a.c_str());
	if (!ifs) {
		cerr << "Error: could not open " << a << "!" << endl;
		return 1;
	}

	q53sol_t  q53sol;
	//cout << "Parsing inputfile:";
	while (ifs) {
		//	cout << endl;
		char c;
		ifs >> c;

		if (!ifs) break;
		if ((c != 'Q') && (c != 'm')) {
			//	cout << "expected 'Q'or 'm' here, going to next line";
			getline(ifs);
			continue;
		}
		if (c == 'Q')
		{
			//cout << c << " ";
			int t;
			if (!(ifs >> t)) break;
			//cout << t << " ";
			if (t < 49 || t > 53) {
				cout << "expected integer t with 52 <= t <= 56 here, going to next line";
				getline(ifs);
				continue;
			}
			while (ifs && c != ':') {
				ifs >> c;
				//cout << c;
			}
			if (c != ':') {
				//cout << "expected ':' here, going to next line";
				getline(ifs);
				continue;
			}
			string out;
			string temp;
			for (int i = 0; i <= 9; i++)
			{
				ifs >> c;
				temp = c;
				out.append(1, c);
			}
			//ifs >> c;
			//cout << out;
			q53sol.Q[t - 49] = hex_3_int(out);
			//	cout << typeid(out).name() << " ";

				//cout << "Q" << t << ": ";
			//cout << hex << setw(8) << setfill('0') << q56sol.Q[t - 52] << dec << endl;

		}
		if (c == 'm')
		{
			//    cout << c << " ";
			int t;
			if (!(ifs >> t)) break;
			//cout << t << " ";
			if (t < 37 || t > 52) {
				cout << "expected integer t with 40 <= t <= 55 here, going to next line";
				getline(ifs);
				continue;
			}
			while (ifs && c != ':') {
				ifs >> c;
				//cout << c;
			}
			if (c != ':') {
				//cout << "expected ':' here, going to next line";
				getline(ifs);
				continue;
			}
			string out;
			string temp;
			for (int i = 0; i <= 9; i++)
			{
				ifs >> c;
				temp = c;
				out.append(1, c);
			}
			//ifs >> c;
			//cout << out;
			q53sol.m[t - 37] = hex_3_int(out);
			//	cout << typeid(out).name() << " ";

			//	cout << "m" << t << ": ";
		//	cout << hex << setw(8) << setfill('0') << q56sol.m[t - 40] << dec << endl;
			if (t == 52)
			{
				q53sol_ts.push_back(q53sol);
			}

		}




		if (!ifs) break;
		/*if (c != '|') {
			cout << "expected '|' here, going to next line";
			getline(ifs);
			continue;
		}*/

		getline(ifs);
	}
	//cout << endl << "Parsed path:" << endl;


	return 0;
}





int filtler(vector<q53sol_t> & q53sols, vector<q53sol_t> & q53sols_f)
{
	for (int jj = 0; jj < q53sols.size(); jj++)
	{
		//cout << jj << endl;
		//	int jj = 0;
		int offset = 4;
		unsigned int w[80] = { 0 }, w2[80];
		unsigned int Q[85] = { 0 }, Q2[85] = { 0 }, iv[5] = { 0 }, iv2[5] = { 0 };

		for (int i = 49; i <= 53; i++)
		{
			Q[Qoffset + i] = q53sols[jj].Q[i - 49];
		}
		for (int i = 37; i <= 52; i++)
		{
			w[i] = q53sols[jj].m[i - 37];
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






		if(okay)
		{
			//show_Q_bit(Q);

			int same = 0;
			for (int cout = 0; cout < q53sols_f.size(); cout++)
			{
				int flag = 0;
				for (int ii = 37; ii <= 52; ii++)
				{
					if (q53sols_f[cout].m[ii - 37] != q53sols[jj].m[ii - 37])
					{
						flag = 1;

					}
				}
				for (int ii = 49; ii <= 53; ii++)
				{
					if (q53sols_f[cout].Q[ii - 49] != q53sols[jj].Q[ii - 49])
					{
						flag = 1;

					}
				}
				if (flag == 0)
				{
					same = 1;
				}
			}
			if (!same)
				q53sols_f.push_back(q53sols[jj]);
		}
		//cout<<"1"<<endl;
	}


	return 0;
}



bool verify(int firststep, int laststep, int lastQ, const uint32_t* mQ1, const uint32_t* mm1)
{

	bool ok = true;
	for (int t = firststep - 4; t <= laststep + 1 && t <= lastQ; ++t)
	{
		if (0 != (Qcondmask[Qoffset + t] & (
			mQ1[Qoffset + t]
			^ Qset1mask[Qoffset + t]
			^ (Qprevmask[Qoffset + t] & mQ1[Qoffset + t - 1])
			^ (Qprevrmask[Qoffset + t] & rotate_left(mQ1[Qoffset + t - 1], 30))
			^ (Qprev2rmask[Qoffset + t] & rotate_left(mQ1[Qoffset + t - 2], 30))
			^ (Qnextmask[Qoffset + t] & mQ1[Qoffset + t + 1])
			)))
		{
			std::cerr << "Q_" << t << " does not satisfy conditions!" << std::endl;
			ok = false; 
		}
	}

	// [1200] verify message bitrelations
#if 0
	for (unsigned r = 0; r < msgbitrels_size; ++r)
	{
		bool okay = true;
		const uint32_t* mbrr = msgbitrels[r];

		uint32_t w = mbrr[16];
		for (unsigned t = mainblockoffset; t < mainblockoffset + 16; ++t)
		{
			uint32_t mbrmask = mbrr[t - mainblockoffset];
			if ((t < firststep | t > laststep) && mbrmask != 0)
			{
				okay = false;
				break;
			}
			w ^= mm1[t] & mbrmask;
		}
		if (okay && 0 != (hw(w) & 1))
		{

			std::cerr << "bitrelation " << r << " is not satisfied!" << std::endl;

			ok = false;
			//			return false;
		}
	}
#endif


	for (unsigned r = 0; r < msgbitrels80_size; ++r)
	{
		bool okay = true;
		const uint32_t* mbrr = msgbitrels80[r];

		uint32_t w = mbrr[80];
		for (unsigned t = mainblockoffset; t < mainblockoffset + 80; ++t)
		{
			uint32_t mbrmask = mbrr[t - mainblockoffset];

			w ^= mm1[t] & mbrmask;
		}
		if (okay && 0 != (hw(w) & 1))
		{

			//std::cerr << "bitrelation " << r << " is not satisfied!" << std::endl;

			ok = false;
			//return false;
		}
	}



	// [1300] verify step computations
	for (int t = firststep; t <= laststep; ++t)
	{
		uint32_t f;
		if (t >= 0 && t < 20) f = sha1_f1(mQ1[Qoffset + t - 1], rotate_left(mQ1[Qoffset + t - 2], 30), rotate_left(mQ1[Qoffset + t - 3], 30));
		if (t >= 20 && t < 40) f = sha1_f2(mQ1[Qoffset + t - 1], rotate_left(mQ1[Qoffset + t - 2], 30), rotate_left(mQ1[Qoffset + t - 3], 30));
		if (t >= 40 && t < 60) f = sha1_f3(mQ1[Qoffset + t - 1], rotate_left(mQ1[Qoffset + t - 2], 30), rotate_left(mQ1[Qoffset + t - 3], 30));
		if (t >= 60 && t < 80) f = sha1_f4(mQ1[Qoffset + t - 1], rotate_left(mQ1[Qoffset + t - 2], 30), rotate_left(mQ1[Qoffset + t - 3], 30));
		uint32_t Qtp1 = rotate_left(mQ1[Qoffset + t], 5) + f + rotate_left(mQ1[Qoffset + t - 4], 30) + mm1[t] + sha1_ac[t / 20];
		if (Qtp1 != mQ1[Qoffset + t + 1])
		{

			std::cerr << "step " << t << " is incorrect!" << std::endl;

			ok = false;//return false;
		}
	}
	return ok;//true;

}

uint32_t hex_3_int(string out)
{
	string s = out;
	uint32_t i = 0, sum;

	int count = s.length();
	//cout << count<<endl;
	sum = 0;
	for (i = count - 1; i >= 2; i--)//从十六进制个位开始，每位都转换成十进制  
	{
		//	cout <<s[i]<<endl;
		if (s[i] >= '0'&&s[i] <= '9')//数字字符的转换  
		{
			sum += (s[i] - 48)*pow(16, count - i - 1);
		}
		else if (s[i] >= 'A'&&s[i] <= 'F')//字母字符的转换  
		{
			sum += (s[i] - 55)*pow(16, count - i - 1);
		}
		else if (s[i] >= 'a'&&s[i] <= 'f')
		{
			sum += (s[i] - 87)*pow(16, count - i - 1);
		}
		//	cout << sum<<endl;
	}
	//cout <<endl<< sum<<endl;
	return sum;
}


bool verify2(int firststep, int laststep, int lastQ, const uint32_t* mQ1, const uint32_t* mm1)
{

	bool ok = true;
	for (int t = firststep - 4; t <= laststep + 1 && t <= lastQ; ++t)
	{
		if (0 != (Qcondmask[Qoffset + t] & (
			mQ1[Qoffset + t]
			^ Qset1mask[Qoffset + t]
			^ (Qprevmask[Qoffset + t] & mQ1[Qoffset + t - 1])
			^ (Qprevrmask[Qoffset + t] & rotate_left(mQ1[Qoffset + t - 1], 30))
			^ (Qprev2rmask[Qoffset + t] & rotate_left(mQ1[Qoffset + t - 2], 30))
			^ (Qnextmask[Qoffset + t] & mQ1[Qoffset + t + 1])
			)))
		{
			//std::cerr << "Q_" << t << " does not satisfy conditions!" << std::endl;
			ok = false;
		}
	}

	// [1200] verify message bitrelations

	for (unsigned r = 0; r < msgbitrels80_size; ++r)
	{
		bool okay = true;
		const uint32_t* mbrr = msgbitrels80[r];

		uint32_t w = mbrr[80];
		for (unsigned t = mainblockoffset; t < mainblockoffset + 80; ++t)
		{
			uint32_t mbrmask = mbrr[t - mainblockoffset];

			w ^= mm1[t] & mbrmask;
		}
		if (okay && 0 != (hw(w) & 1))
		{

			//std::cerr << "bitrelation " << r << " is not satisfied!" << std::endl;

			ok = false;
			//return false;
		}
	}



	return ok;//true;

}