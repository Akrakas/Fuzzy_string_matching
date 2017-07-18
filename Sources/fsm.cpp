#include "fsm.h"

//Calcul le nombre de modifications a appliquer a s1 pour obtenir s2
int LevenshteinDistance(string s1, string s2) {
	int mat[s1.length() +1][s2.length() +1];
	int i, j, cost;
	
	for(int i=0 ; i<s1.length() +1 ; ++i) {
		mat[i][0] = i;
	}
	for(int j=0 ; j<s2.length() +1 ; ++j) {
		mat[0][j] = j;
	}
	
	for(int i=1 ; i<s1.length() +1; ++i) {
		for(int j=1 ; j<s2.length() +1; ++j) {
			if(s1.at(i-1) == s2.at(j-1)) cost = 0;
			else cost = 1;
			mat[i][j] = min(min(mat[i-1][j]+1, mat[i][j-1]+1), mat[i-1][j-1]+cost);
		}
	}
	
	return mat[s1.length()][s2.length()];
}


int FastLevenshteinDistance(string s, string t)
{
	// degenerate cases
	if (s == t) return 0;
	if (s.length() == 0) return t.length();
	if (t.length() == 0) return s.length();

	// create two work vectors of integer distances
	int* v0 = new int[t.length() + 1];
	int* v1 = new int[t.length() + 1];

	// initialize v0 (the previous row of distances)
	// this row is A[0][i]: edit distance for an empty s
	// the distance is just the number of characters to delete from t
	for (int i = 0; i < t.length() + 1; i++)
		v0[i] = i;

	for (int i = 0; i < s.length(); i++)
	{
		// calculate v1 (current row distances) from the previous row v0

		// first element of v1 is A[i+1][0]
		//   edit distance is delete (i+1) chars from s to match empty t
		v1[0] = i + 1;

		// use formula to fill in the rest of the row
		for (int j = 0; j < t.length(); j++)
		{
			int cost = (s[i] == t[j]) ? 0 : 1;
			v1[j + 1] = min(min(v1[j] + 1, v0[j + 1] + 1), v0[j] + cost);
		}

		// copy v1 (current row) to v0 (previous row) for next iteration
		for (int j = 0; j < t.length() + 1; j++)
			v0[j] = v1[j];
	}

	int LD = v1[t.length()];
	delete v0;
	delete v1;
	return LD;
}
