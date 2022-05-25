#include "detail.h"
/*
 '.'; 0
 '+'; 1
 '-'; 2
 '0'; 3
 '1'; 4
 '^'; 5
 '!'; 6
 'm'; 7
 '#'; 8
 '?'; 9
 'V'; 10
 'Y'; 11
 'W'; 12
 'H'; 13
 'Q'; 14

*/
int wn[80][32] = { 0 };

int CUNTn = 65;
int showpath(int path[85][32])
{
	for (int t = -4; t <= 25; t++)
	{
		cout << "Q" << t << "\t: ";
		for (int b = 31; b >= 0; b--)
		{
			switch (path[t + Qoffset][b]) {
			case 0:	cout << '.'; break;
			case 1:	cout << '+'; break;
			case 2:	cout << '-'; break;
			case 3:	cout << '0'; break;
			case 4:	cout << '1'; break;
			case 5: cout << '^'; break;
			case 6:	cout << '!'; break;
			case 7:	cout << 'm'; break;
			case 8:	cout << '#'; break;
			case 9:	cout << '?'; break;
			case 10:cout << 'V'; break;
			case 11:cout << 'Y'; break;
			case 12:cout << 'W'; break;
			case 13:cout << 'H'; break;
			case 14:cout << 'Q'; break;
			default:
				throw;
			}
		}
		cout << endl;
	}

}
int showpath2n(int path[85][32], int wfilpout[][2], int wflagout[], int w[80][32])
{
	//int w[80][32] = {0};
	for (int i = 0; i < 6; i++)
	{
		if (wfilpout[i][0] >= 0)
			w[wfilpout[i][0]][wfilpout[i][1]] = (wflagout[i]) * 32 + CUNTn;
	}
	for (int t = -4; t <= 25; t++)
	{
		cout << "Q" << t << "\t: ";
		for (int b = 31; b >= 0; b--)
		{
			switch (path[t + Qoffset][b]) {
			case 0:	cout << '.'; break;
			case 1:	cout << '+'; break;
			case 2:	cout << '-'; break;
			case 3:	cout << '0'; break;
			case 4:	cout << '1'; break;
			case 5: cout << '^'; break;
			case 6:	cout << '!'; break;
			case 7:	cout << 'm'; break;
			case 8:	cout << '#'; break;
			case 9:	cout << '?'; break;
			case 10:cout << 'V'; break;
			case 11:cout << 'Y'; break;
			case 12:cout << 'W'; break;
			case 13:cout << 'H'; break;
			case 14:cout << 'Q'; break;
			default:
				throw;
			}
		}




		if (t >= 6)
		{
			cout << "\t" << "w" << t << "\t: ";
			for (int b = 31; b >= 0; b--)
			{
				if (w[t][b] == 0)
				{
					cout << '.';
				}
				else
				{
					printf("%c", w[t][b]);
				}

			}

		}

		cout << endl;
	}

}






int construtapth_new(vector<q53sol_t>  q53sol_ts)
{
	int path[85][32] = { 0 };

	for (int t = -4; t < 40; ++t)
	{
		//cout << "Q" << t << "\t: ";
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
					{
						//cout << (flip ? "-" : "+");
						if (flip)
						{
							//	cout << "-";
							path[Qoffset + t][b] = 2;
						}
						else {
							//	cout << "+";
							path[Qoffset + t][b] = 1;
						}
					}

					else
					{
						//	cout << (flip ? "1" : "0");
						if (flip)
						{
							//	cout << "1";
							path[Qoffset + t][b] = 4;
						}
						else {
							//	cout << "0";
							path[Qoffset + t][b] = 3;
						}
					}

				}
				if (prev)
				{
					//	cout << (!flip ? "^" : "!");
					if (!flip)
					{
						//	cout << "^";
						path[Qoffset + t][b] = 5;
					}
					else {
						//	cout << "!";
						path[Qoffset + t][b] = 6;
					}
				}

				if (prevr)
				{
					//cout << (!flip ? "r" : "R");
					if (!flip)
					{
						//	cout << "r";
					}
					else {
						//cout << "R";
					}
				}
				if (prev2r)
				{
					//	cout << (!flip ? "s" : "S");
					if (!flip)
					{
						//	cout << "s";
					}
					else {
						//	cout << "S";
					}
				}
				if (next)
				{
					//cout << (!flip ? "v" : "Y");
					if (!flip)
					{
						//cout << "v";
						path[Qoffset + t][b] = 10;
					}
					else {
						//cout << "Y";
						path[Qoffset + t][b] = 11;
					}
				}

			}
			else
			{
				//cout << ".";
				path[Qoffset + t][b] = 0;

			}
		}
		//	cout << endl;
	}

	for (int t = -4; t <= 30; t++)
	{
		for (int b = 31; b >= 0; b--)
		{
			//		cout << path[Qoffset + t][b]<<" ";
		}
		//	cout << endl;
	}
	cout << endl;
	showpath(path);
	cout << endl;

	for (int t = 11; t >= 1; t--)
	{
		//	int aa = 0;
			//int t = 7;
		for (int j = 31; j >= 0; j--)
		{
			cout << t << "," << j << endl;
			//	int	j = 6;
			int pathnew[85][32] = { 0 };
			int pathout[85][32] = { 0 };
			int  pro_hold_out[85] = { 0 };
			int max = -5; int wfilpout[6][2] = { 0 }, wflagout[6] = { 0 };
			if (!(Qcondmask[Qoffset + t] & (1 << j)))
			{
				if (!(Qprevmask[Qoffset + t + 1] & (1 << j)))
					if (!(Qnextmask[Qoffset + t - 1] & (1 << j)))
					{
						int wfilp[6][2] = { 0 }, wflag[6] = { 0 };

						for (int flag = 1; flag <= 2; flag++)
						{	/*修正Qt+5和Qt+6步的差分*/
							wfilp[0][0] = t - 1;
							wfilp[0][1] = j;
							wflag[0] = flag - 1;
							wfilp[1][0] = t;
							wfilp[1][1] = j + 5;
							wflag[1] = wflag[0] ^ 1;
							wfilp[5][0] = t + 5 - 1;
							wfilp[5][1] = j - 2;
							wflag[5] = wflag[0] ^ 1;
							/*----------------------*/

							/*===============================step t+4===============================*/
							int temptp2jm2 = 0, temptp1j = 0;
							if (path[Qoffset + t + 2][j - 2] == 5)
							{
								temptp2jm2 = 0;
							}
							else {
								temptp2jm2 = path[Qoffset + t + 2][j - 2];
							}
							temptp1j = path[Qoffset + t + 1][j];

							int index3 = (temptp2jm2 * 15 * 15) + (temptp1j * 15) + flag;

							for (int num3 = 0; num3 < 3; num3++)
							{
								if (table_fout_back[index3][num3] >= 0)
								{
									int fcon = temptp2jm2, scon = temptp1j, tcon = flag;
									int newcon[3] = { 0 };



									newcon[0] = table_back[(fcon * 15 * 15 * 3) + (scon * 15 * 3) + (tcon * 3) + table_fout_back[index3][num3]][0];
									newcon[1] = table_back[(fcon * 15 * 15 * 3) + (scon * 15 * 3) + (tcon * 3) + table_fout_back[index3][num3]][1];
									newcon[2] = table_back[(fcon * 15 * 15 * 3) + (scon * 15 * 3) + (tcon * 3) + table_fout_back[index3][num3]][2];
									pathnew[Qoffset + t + 2][j - 2] = newcon[0];
									pathnew[Qoffset + t + 1][j] = newcon[1];
									pathnew[Qoffset + t][j] = newcon[2];
									pathnew[Qoffset + t + 1][j - 2] = path[Qoffset + t + 1][j - 2];
									if (path[Qoffset + t + 2][j - 2] == 5)
									{
										pathnew[Qoffset + t + 1][j - 2] = newcon[0];
									}
									if (path[Qoffset + t + 2][j - 2] == 6)
									{
										pathnew[Qoffset + t + 1][j - 2] = newcon[0] ^ 1;
									}
									//	cout << table_fout[index][num1] << "  " << (flag) << " " << scon << " " << tcon <<"   "<< table_fout[index][num1] <<"   "<< (flag * 14 * 14 * 3) + (scon * 14 * 3) + (tcon * 3) + table_fout[index][num1] <<endl;

									//	cout<<"pathnew[Qoffset + t ][j]"<< newcon[0] + 2 <<"  "<< newcon[1]<<"   "<<
							//	cout << endl;
							//	showpath(pathnew);
							//	cout << endl;
								//	cout << fcon << "  " << scon << "  " << tcon << "  " << endl;
								//	cout << table_fout[index3][num3] << "  " << newcon[0] << "  " << newcon[1] << "  " << newcon[2] << endl;
									;
									/*----------------修正Qt+4步的差分------------------*/
									if (table_fout_back[index3][num3] > 0)
									{
										wfilp[4][0] = t + 4 - 1;
										wfilp[4][1] = j - 2;
										if (table_fout_back[index3][num3] == 1)
										{
											wflag[4] = 1;
										}
										else {
											wflag[4] = 0;
										}

									}
									else
									{
										wfilp[4][0] = -1;


									}

									/*								cout << endl;
																	showpath(pathnew);
																	cout << endl;
																for (int ii = 0; ii < 6; ii++)
																{
																	if (wfilp[ii][0] > 0)
																	{
																		cout << "w[" << wfilp[ii][0] << "," << wfilp[ii][1] << "]" << "=" << wflag[ii] << "		";
																	}
																}
																*/

#if 1
																/*===============================step t+3===============================*/

									int temptp1jm2 = 0;


									temptp1jm2 = pathnew[Qoffset + t + 1][j - 2];


									int index2 = (temptp1jm2 * 15 * 15) + (pathnew[Qoffset + t][j] * 15) + path[Qoffset + t - 1][j];

									for (int num2 = 0; num2 < 3; num2++)
									{
										if (table_fout_back[index2][num2] >= 0)
										{
											int fcon = temptp1jm2, scon = pathnew[Qoffset + t][j], tcon = path[Qoffset + t - 1][j];
											int newcon[3] = { 0 };



											newcon[0] = table_back[(fcon * 15 * 15 * 3) + (scon * 15 * 3) + (tcon * 3) + table_fout_back[index2][num2]][0];
											newcon[1] = table_back[(fcon * 15 * 15 * 3) + (scon * 15 * 3) + (tcon * 3) + table_fout_back[index2][num2]][1];
											newcon[2] = table_back[(fcon * 15 * 15 * 3) + (scon * 15 * 3) + (tcon * 3) + table_fout_back[index2][num2]][2];
											pathnew[Qoffset + t + 1][j - 2] = newcon[0];
											pathnew[Qoffset + t][j] = newcon[1];
											pathnew[Qoffset + t - 1][j] = newcon[2];


											pathnew[Qoffset + t][j - 2] = path[Qoffset + t][j - 2];
											if (path[Qoffset + t + 1][j - 2] == 5)
											{
												pathnew[Qoffset + t][j - 2] = newcon[0];
											}
											if (path[Qoffset + t + 1][j - 2] == 6)
											{
												pathnew[Qoffset + t][j - 2] = newcon[0] ^ 1;
											}

											//	
													//cout << table_fout[index2][num2] << "  " << fcon << " " << scon << " " << tcon  <<endl;

												//	cout<<"pathnew[Qoffset + t ][j]"<< newcon[0] + 2 <<"  "<< newcon[1]<<"   "<<
											//cout << endl;
											//showpath(pathnew);
											//cout << endl;
											//	cout << fcon << "  " << scon << "  " << tcon << "  " << endl;
											//	cout << table_fout[index3][num3] << "  " << newcon[0] << "  " << newcon[1] << "  " << newcon[2] << endl;
											;
											/*----------------修正Qt+3步的差分------------------*/
											if (table_fout_back[index2][num2] > 0)
											{
												wfilp[3][0] = t + 3 - 1;
												wfilp[3][1] = j - 2;
												if (table_fout_back[index2][num2] == 1)
												{
													wflag[3] = 1;
												}
												else {
													wflag[3] = 0;
												}

											}
											else
											{
												wfilp[3][0] = -1;


											}
											/*
											cout << endl;
											showpath(pathnew);
											cout << endl;
											for (int ii = 0; ii < 6; ii++)
											{
												if (wfilp[ii][0] > 0)
												{
													cout << "w[" << wfilp[ii][0] << "," << wfilp[ii][1] << "]" << "=" << wflag[ii] << "		";
												}
											}
											*/
#if 1

											/*===============================step t+2===============================*/
											int index1 = (pathnew[Qoffset + t][j] * 15 * 15) + (path[Qoffset + t - 1][j + 2] * 15) + path[Qoffset + t - 2][j + 2];

											for (int num1 = 0; num1 < 3; num1++)
											{
												if (table_fout_back[index1][num1] >= 0)
												{
													fcon = pathnew[Qoffset + t][j], scon = path[Qoffset + t - 1][j + 2], tcon = path[Qoffset + t - 2][j + 2];
													int newcon[3] = { 0 };



													newcon[0] = table_back[(fcon * 15 * 15 * 3) + (scon * 15 * 3) + (tcon * 3) + table_fout_back[index1][num1]][0];
													newcon[1] = table_back[(fcon * 15 * 15 * 3) + (scon * 15 * 3) + (tcon * 3) + table_fout_back[index1][num1]][1];
													newcon[2] = table_back[(fcon * 15 * 15 * 3) + (scon * 15 * 3) + (tcon * 3) + table_fout_back[index1][num1]][2];
													pathnew[Qoffset + t][j] = newcon[0];
													pathnew[Qoffset + t - 1][j + 2] = newcon[1];
													pathnew[Qoffset + t - 2][j + 2] = newcon[2];

													//	
														//	cout << table_fout[index][num1] << "  " << (flag) << " " << scon << " " << tcon <<"   "<< table_fout[index][num1] <<"   "<< (flag * 14 * 14 * 3) + (scon * 14 * 3) + (tcon * 3) + table_fout[index][num1] <<endl;

														//	cout<<"pathnew[Qoffset + t ][j]"<< newcon[0] + 2 <<"  "<< newcon[1]<<"   "<<

													//	cout << fcon << "  " << scon << "  " << tcon << "  " << endl;
													//	cout << table_fout[index3][num3] << "  " << newcon[0] << "  " << newcon[1] << "  " << newcon[2] << endl;
													;
													/*----------------修正Qt+2步的差分------------------*/
													if (table_fout_back[index1][num1] > 0)
													{
														wfilp[2][0] = t + 2 - 1;
														wfilp[2][1] = j;
														if (table_fout_back[index1][num1] == 1)
														{
															wflag[2] = 1;
														}
														else {
															wflag[2] = 0;
														}

													}
													else
													{
														wfilp[2][0] = -1;


													}
													
												/*	cout << endl;
													showpath(pathnew);
													cout << endl;
													for (int ii = 0; ii < 6; ii++)
													{
														if (wfilp[ii][0] > 0)
														{
															cout << "w[" << wfilp[ii][0] << "," << wfilp[ii][1] << "]" << "=" << wflag[ii] << "		";
														}
													}*/
													//cout << endl;
													int  pro_hold[85] = { 0 };
													bool a;
													for (int i2 = 0; i2 < q53sol_ts.size(); i2++)
														a = model4(q53sol_ts[i2], wfilp, wflag, pro_hold);
													int temp = 0;



													if (1)
													{
														//cout << "------------------------------" << endl;
														for (int i = -4 + Qoffset; i <= 35; i++)
														{
														//	cout << "pro Q" << i - Qoffset << ":" << pro_hold[i] / (q53sol_ts.size()*1.0) << endl;
															if (pro_hold[i] / (q53sol_ts.size()*1.0) > 0.1)
															{

																temp = i;
																break;
															}
														}
														//	cout << 1<<endl;
															//showpath(pathnew);
															/*保存作用范围更远的辅助路径*/
														if (temp > max)
														{
															max = temp;
															for (int ii3 = 0; ii3 <= 84; ii3++)
															{

																for (int jj3 = 0; jj3 <= 31; jj3++)
																{
																	pathout[ii3][jj3] = pathnew[ii3][jj3];
																}

																pro_hold_out[ii3] = pro_hold[ii3];

															}
															pathout[Qoffset + t][j] = pathout[Qoffset + t][j] + 2;
															for (int ii3 = 0; ii3 < 6; ii3++)
															{
																wfilpout[ii3][0] = wfilp[ii3][0];
																wfilpout[ii3][1] = wfilp[ii3][1];
																wflagout[ii3] = wflag[ii3];
															}

														}
														//	cout << 1 << endl;
														//	cout << endl;
															//cout << fcon << "  " << scon << "  " << tcon << "  " << endl;
															//cout << table_fout[index3][num3] << "  " << newcon[0] << "  " << newcon[1] << "  " << newcon[2] << endl;
														//	showpath(pathnew);

														//	cout << endl << "------------------------------" << endl;
													}






												}



											}




#endif






										}



									}

#endif


								}


							}


						}


					}


				if (max > 16)
				{


					for (int ii = 0; ii < 6; ii++)
					{
						if (wfilpout[ii][0] > 0)
						{
							cout << "w[" << wfilpout[ii][0] << "," << wfilpout[ii][1] << "]" << "=" << wflagout[ii] << "		";
						}
					}
					cout << endl;
					for (int i = -4 + Qoffset; i <= 35; i++)
					{
						cout << "pro Q" << i - Qoffset << ":" << pro_hold_out[i] / (q53sol_ts.size()*1.0) << endl;

					}

					cout << "arrive Q_" << max - Qoffset << endl;;


					for (int ii3 = 0; ii3 <= 84; ii3++)
					{

						for (int jj3 = 0; jj3 <= 31; jj3++)
						{
							if (pathout[ii3][jj3] > 0)
								path[ii3][jj3] = pathout[ii3][jj3];
						}


					}
					int wsn[80][32] = { 0 };
					showpath2n(pathout, wfilpout, wflagout, wsn);
					showpath2n(path, wfilpout, wflagout, wn);

					CUNTn++;
				}


			}

		}

	}
	return 0;
}



