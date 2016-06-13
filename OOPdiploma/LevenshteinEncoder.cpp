#include "LevenshteinEncoder.h"

void LevenshteinEncoder::encode(dict d)
{
	ofstream out;
	out.open((path + ".aslv").c_str());

	vector < Code > word_concat;
	for (int i = 0; i < d.size(); i++)
	{
		if (d[i].c.q == -1 || d[i].c.r == -1)
			continue;
		word_concat.push_back(d[i].c);
	}

	// coding of min(i1, i2)
	vector < string > r_s;
	int k = 0;
	for (int i = 0; i < word_concat.size(); i++)
	{
		int q_log = get_log(word_concat[i].q);
		string kemp = "";
		for (int i = 0; i <= q_log; i++)
		{
			kemp += (1 << i) & word_concat[i].r ? "1" : "0";
		}
		reverse(kemp.begin(), kemp.end());
		r_s.push_back(kemp);

		int r = word_concat[i].q;
		word_concat[i].q -= k;
		k = r;
	}

	string long_string = "";
	int q = 0;
	for (int i = 0; i < word_concat.size(); i++)
	{
		string tempor = get_code(word_concat[i].q) + r_s[i] + (word_concat[i].d == true ? "1" : "0");
		long_string = long_string + tempor;
	}

	int l_str_size = long_string.size();
	int cur_p = 0;
	while (cur_p < l_str_size)
	{
		unsigned char c = 0;
		for (int i = 0; i < min(8, l_str_size - cur_p); i++)
		{
			int t = long_string[i + cur_p] == '0' ? 0 : 1;
			c += (t << i);
		}
		cur_p += 8;
		out << c;
	}

	out.close();
}

string LevenshteinEncoder::get_code(int x)
{
	if (x == 0)
		return "0";
	string ones = "1";
	string res = "";
	int m = x;
	while (m > 0)
	{
		string cur = "";
		while (m > 1)
		{
			cur = (m & 1 ? '1' : '0') + cur;
			m /= 2;
		}
		res = cur + res;
		if (cur != "")
		{
			ones += '1';
		}
		m = cur.length();
	}
	return ones + "0" + res;
}

LevenshteinEncoder::LevenshteinEncoder(string file_path) : path(file_path)
{

}
